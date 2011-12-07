/*=========================================================================

 Program:   ORFEO Toolbox
 Language:  C++
 Date:      $Date$
 Version:   $Revision$


 Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
 See OTBCopyright.txt for details.


 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 =========================================================================*/
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"


#include <fstream>

// Include differents method for color mapping
#include "otbChangeLabelImageFilter.h"
#include "itkLabelToRGBImageFilter.h"
#include "itkScalarToRGBColormapImageFilter.h"
#include "otbReliefColormapFunctor.h"

namespace otb
{
namespace Wrapper
{

class ColorMapping: public Application
{
public:
/** Standard class typedefs. */
  typedef ColorMapping      Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(ColorMapping, otb::Application);

  typedef FloatImageType::PixelType   PixelType;
  typedef UInt16ImageType             LabelImageType;
  typedef LabelImageType::PixelType   LabelType;
  typedef UInt8VectorImageType        VectorImageType;
  typedef VectorImageType::PixelType  VectorPixelType;
  typedef UInt8RGBImageType           RGBImageType;
  typedef RGBImageType::PixelType     RGBPixelType;

  // Manual label LUT
  typedef otb::ChangeLabelImageFilter
  <LabelImageType, VectorImageType>    ChangeLabelFilterType;

  // Segmentation contrast maximisation LUT
  typedef itk::LabelToRGBImageFilter
  <LabelImageType, RGBImageType>       LabelToRGBFilterType;

  // Continuous LUT mapping
  typedef itk::ScalarToRGBColormapImageFilter
  <FloatImageType, RGBImageType>      ColorMapFilterType;
  typedef otb::Functor::ReliefColormapFunctor
  <PixelType, RGBPixelType>           ReliefColorMapFunctorType;

private:
  void DoInit()
  {
    SetName("ColorMapping");
    SetDescription("Maps an input grayscale image into 8-bits RGB using look-up tables.");

    SetDocName("Color Mapping");
    SetDocLongDescription("This application allows to map an input grayscale into a 8-bits RGB image using three different methods.\n" "-The custom method allows to apply a custom look-up table to a labeled image. The look-up table is loaded from a text file where each line describes an entry. The typical use of this method is to colorise a classification map.\n" "-The continuous method allows to map a range of values in a scalar input image into a colored image using continuous look-up table, in order to enhance image interpretation. Several look-up tables can been chosen with different color ranges.\n" "-The segmentation method is dedicated to segmentation labeled outputs where each segment correspond to a unique label. It computes an optimal look-up table such that color difference between adjacent segmented regions is maximised.");
    SetDocLimitations("The segmentation method does not support streaming, and thus large images.");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("ImageSVMClassifier application");
    AddDocTag(Tags::Learning);

    // Build lut map

    m_LutMap["Red"]=ColorMapFilterType::Red;
    m_LutMap["Green"]=ColorMapFilterType::Green;
    m_LutMap["Blue"]=ColorMapFilterType::Blue;
    m_LutMap["Grey"]=ColorMapFilterType::Grey;
    m_LutMap["Hot"]=ColorMapFilterType::Hot;
    m_LutMap["Cool"]=ColorMapFilterType::Cool;
    m_LutMap["Spring"]=ColorMapFilterType::Spring;
    m_LutMap["Summer"]=ColorMapFilterType::Summer;
    m_LutMap["Autumn"]=ColorMapFilterType::Autumn;
    m_LutMap["Winter"]=ColorMapFilterType::Winter;
    m_LutMap["Copper"]=ColorMapFilterType::Copper;
    m_LutMap["Jet"]=ColorMapFilterType::Jet;
    m_LutMap["HSV"]=ColorMapFilterType::HSV;
    m_LutMap["OverUnder"]=ColorMapFilterType::OverUnder;

    AddParameter(ParameterType_InputImage, "in", "Input Image");
    SetParameterDescription("in", "Input image filename");
    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription("out","Output image filename");
    AddParameter(ParameterType_RAM, "ram", "Available RAM");
    SetDefaultParameterInt("ram", 256);
    MandatoryOff("ram");


    AddParameter(ParameterType_Choice, "method", "Color mapping method");
    SetParameterDescription("method","Selection of color mapping methods and their parameters.");

    // Custom LUT
    AddChoice("method.custom","Color mapping with custom labeled look-up table");
    SetParameterDescription("method.custom","Apply a user-defined look-up table to a labeled image. Look-up table is loaded from a text file.");
    AddParameter(ParameterType_Filename, "method.custom.lut", "Look-up table file");
    SetParameterDescription("method.custom.lut",  "An ASCII file containing the look-up table\n"
                            "with one color per line\n"
                            "(for instance the line '1 255 0 0' means that all pixels with label 1 will be replaced by RGB color 255 0 0)\n"
                            "Lines beginning with a # are ignored");

    // Continuous LUT
    AddChoice("method.continuous","Color mapping with continuous look-up table");
    SetParameterDescription("method.continuous","Apply a continuous look-up table to a range of input values.");
    AddParameter(ParameterType_Choice,"method.continuous.lut","Look-up tables");
    SetParameterDescription("method.continuous.lut","Available look-up tables.");

    AddChoice("method.continuous.lut.red","Red");
    AddChoice("method.continuous.lut.green","Green");
    AddChoice("method.continuous.lut.blue","Blue");
    AddChoice("method.continuous.lut.grey","Grey");
    AddChoice("method.continuous.lut.hot","Hot");
    AddChoice("method.continuous.lut.cool","Cool");
    AddChoice("method.continuous.lut.spring","Spring");
    AddChoice("method.continuous.lut.summer","Summer");
    AddChoice("method.continuous.lut.autumn","Autumn");
    AddChoice("method.continuous.lut.winter","Winter");
    AddChoice("method.continuous.lut.copper","Copper");
    AddChoice("method.continuous.lut.jet","Jet");
    AddChoice("method.continuous.lut.hsv","HSV");
    AddChoice("method.continuous.lut.overunder","OverUnder");
    AddChoice("method.continuous.lut.relief","Relief");

    AddParameter(ParameterType_Float,"method.continuous.min","Mapping range lower value");
    SetParameterDescription("method.continuous.min","Set the lower input value of the mapping range.");
    SetParameterFloat("method.continuous.min", 0.);
    
    AddParameter(ParameterType_Float,"method.continuous.max","Mapping range higher value");
    SetParameterDescription("method.continuous.max","Set the higher input value of the mapping range.");
    SetParameterFloat("method.continuous.max", 255.);

    // Segmentation LUT
    AddChoice("method.segmentation","Color mapping with a look-up table optimised for segmentation");
    SetParameterDescription("method.segmentation","Compute an optimal look-up table such that neighbouring labels in a segmentation are mapped to highly contrasted colors.");
    AddParameter(ParameterType_Int,"method.segmentation.background", "Background label");
    SetParameterDescription("method.segmentation.background","Value of the background label");
    SetParameterInt("method.segmentation.background", 0);
    SetMinimumParameterIntValue("method.segmentation.background", 0);
    SetMaximumParameterIntValue("method.segmentation.background", 255);

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "ROI_QB_MUL_1_SVN_CLASS_MULTI.png");
    SetDocExampleParameterValue("method", "custom");
    SetDocExampleParameterValue("method.custom.lut", "ColorTable.txt");
    SetDocExampleParameterValue("out", "clLabeledFancyImageQB123_1.tif");
 }

  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute()
  {
    if(GetParameterInt("method")==0)
      {
      m_CustomMapper = ChangeLabelFilterType::New();
      m_CustomMapper->SetInput(GetParameterUInt16Image("in"));
      m_CustomMapper->SetNumberOfComponentsPerPixel(3);
      
      ReadLutFromFile();
      
      SetParameterOutputImage("out", m_CustomMapper->GetOutput());
      }
    else if(GetParameterInt("method")==1)
      {
      m_ContinuousColorMapper = ColorMapFilterType::New();

      m_ContinuousColorMapper->SetInput(GetParameterFloatImage("in"));

      // Disable automatic scaling
      m_ContinuousColorMapper->UseInputImageExtremaForScalingOff();

      // Set the lut
      std::string lut = GetParameterString("method.continuous.lut");

      otbAppLogINFO("LUT: "<<lut<<std::endl);
      if(lut == "Relief")
        {
        ReliefColorMapFunctorType::Pointer reliefFunctor = ReliefColorMapFunctorType::New();
        m_ContinuousColorMapper->SetColormap(reliefFunctor);
        }
      else
        {
        m_ContinuousColorMapper->SetColormap((ColorMapFilterType::ColormapEnumType)m_LutMap[lut]);
        }

      m_ContinuousColorMapper->GetColormap()->SetMinimumInputValue(GetParameterFloat("method.continuous.min"));
      m_ContinuousColorMapper->GetColormap()->SetMaximumInputValue(GetParameterFloat("method.continuous.max"));

      SetParameterOutputImage("out", m_ContinuousColorMapper->GetOutput());
      }
    else if(GetParameterInt("method")==2)
      {
      m_SegmentationColorMapper = LabelToRGBFilterType::New();
      m_SegmentationColorMapper->SetInput(GetParameterUInt16Image("in"));
      m_SegmentationColorMapper->SetBackgroundValue(GetParameterInt("method.segmentation.background"));
      SetParameterOutputImage("out", m_SegmentationColorMapper->GetOutput());
      }
  }

  void ReadLutFromFile()
  {
    std::ifstream ifs;

    ifs.open(GetParameterString("method.custom.lut").c_str());

    if (!ifs)
      {
      itkExceptionMacro("Can not read file " << GetParameterString("method.custom.lut") << std::endl);
      }

    otbAppLogINFO("Parsing color map file " << GetParameterString("method.custom.lut") << "." << std::endl);

    while (!ifs.eof())
      {
      std::string line;
      std::getline(ifs, line);

      // Avoid commented lines or too short ones
      if (!line.empty() && line[0] != '#')
        {
        // retrieve the label
        std::string::size_type pos = line.find_first_of(" ", 0);
        LabelType clabel = atoi(line.substr(0, pos).c_str());
        ++pos;
        // Retrieve the color
        VectorPixelType color(3);
        color.Fill(0);
        for (unsigned int i = 0; i < 3; ++i)
          {
          std::string::size_type nextpos = line.find_first_of(" ", pos);
          int value = atoi(line.substr(pos, nextpos).c_str());
          if (value < 0 || value > 255)
            otbAppLogWARNING("WARNING: color value outside 8-bits range (<0 or >255). Value will be clamped." << std::endl);
          color[i] = static_cast<PixelType> (value);
          pos = nextpos + 1;
          nextpos = line.find_first_of(" ", pos);
          }
        otbAppLogINFO("Adding color mapping " << clabel << " -> [" << (int) color[0] << " " << (int) color[1] << " "<< (int) color[2] << " ]" << std::endl);
        m_CustomMapper->SetChange(clabel, color);
        }
      }
    ifs.close();
  }

  ChangeLabelFilterType::Pointer m_CustomMapper;
  ColorMapFilterType::Pointer    m_ContinuousColorMapper;
  LabelToRGBFilterType::Pointer  m_SegmentationColorMapper;
  std::map<std::string, unsigned int> m_LutMap;
};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::ColorMapping)


