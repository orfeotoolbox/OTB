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

class LabeledImageColorMapping: public Application
{
public:
/** Standard class typedefs. */
  typedef LabeledImageColorMapping      Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(LabeledImageColorMapping, otb::Application);

  typedef FloatImageType::PixelType   PixelType;
  typedef UInt16ImageType             LabelImageType;
  typedef LabelImageType::PixelType   LabelType;
  typedef UInt8VectorImageType        VectorImageType;
  typedef VectorImageType::PixelType  VectorPixelType;
  typedef UInt8RGBImageType           RGBImageType;
  typedef RGBImageType::PixelType     RGBPixelType;

  // Manual label LUT
  typedef otb::ChangeLabelImageFilter
  <LabelImageType,VectorImageType>    ChangeLabelFilterType;

  // Segmentation contrast maximisation LUT
  typedef itk::LabelToRGBImageFilter
  <LabelImageType,RGBImageType>       LabelToRGBFilterType;

  // Continuous LUT mapping
  typedef itk::ScalarToRGBColormapImageFilter
  <FloatImageType, RGBImageType>      ColorMapFilterType;
  typedef otb::Functor::ReliefColormapFunctor
  <PixelType, RGBPixelType>           ReliefColorMapFunctorType;


private:
  LabeledImageColorMapping()
  {
    SetName("LabeledImageColorMapping");
    SetDescription("Replace labels of a classification map with user defined 8-bits RGB colors.");

    SetDocName("Labeled Image Color Mapping Application");
    SetDocLongDescription("Create and save a classification map label image (given by ImageSVMClassifier Application for example) with user defined 8-bits RGB colors LUT. Unknown labels are mapped to black by default.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");
    SetDocCLExample("otbApplicationLauncherCommandLine LabeledImageColorMapping   --in ${OTBAPP_BASELINE}/clLabeledImageQB123_1.tif --ct ${OTB-Data}/Input/Classification/ColorTable.txt --out clLabeledFancyImageQB123_1.tif");
    AddDocTag(Tags::Learning);


  }

  virtual ~LabeledImageColorMapping()
  {
  }

  void DoCreateParameters()
  {
    AddParameter(ParameterType_InputImage, "in", "Input Image");
    SetParameterDescription("in", "Input image filename.");
    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription("out","Output image filename.");
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
    // AddChoice("method.continuous.lut.green","Green");
    // AddChoice("method.continuous.lut.blue","Blue");
    // AddChoice("method.continuous.lut.grey","Grey");
    // AddChoice("method.continuous.lut.hot","Hot");
    // AddChoice("method.continuous.lut.cool","Cool");
    // AddChoice("method.continuous.lut.spring","Spring");
    // AddChoice("method.continuous.lut.summer","Summer");
    // AddChoice("method.continuous.lut.autumn","Autumn");
    // AddChoice("method.continuous.lut.winter","Winter");
    // AddChoice("method.continuous.lut.copper","Copper");
    // AddChoice("method.continuous.lut.jet","Jet");
    // AddChoice("method.continuous.lut.overunder","OverUnder");
    // AddChoice("method.continuous.lut.relief","Relief");

    AddParameter(ParameterType_Float,"method.continuous.min","Mapping range lower value");
    SetParameterDescription("method.continuous.min","Set the lower input value of the mapping range.");
    
    AddParameter(ParameterType_Float,"method.continuous.max","Mapping range higher value");
    SetParameterDescription("method.continuous.max","Set the higher input value of the mapping range.");
    
    // Segmentation LUT
    AddChoice("method.segmentation","Color mapping with a look-up table optimised for segmentation");
    SetParameterDescription("method.segmentation","Compute an optimal look-up table such that neighbouring labels in a segmentation are mapped to highly contrasted colors.");
    AddParameter(ParameterType_Int,"method.segmentation.background", "Background label");
    SetParameterDescription("method.segmentation.background","Value of the background label");
    SetParameterInt("method.segmentation.background",0);

  }

  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute()
  {

    m_LabeledImage = GetParameterUInt16Image("in");

    m_Mapper = ChangeLabelFilterType::New();
    m_Mapper->SetInput(m_LabeledImage);
    m_Mapper->SetNumberOfComponentsPerPixel(3);

    std::ifstream ifs;

    ifs.open(GetParameterString("ct").c_str());

    if (!ifs)
      {
      itkExceptionMacro("Can not read file " << GetParameterString("ct") << std::endl);
      }

    otbAppLogINFO("Parsing color map file " << GetParameterString("ct") << "." << std::endl);

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
        m_Mapper->SetChange(clabel, color);
        }
      }
    ifs.close();

    /***/
    SetParameterOutputImage<VectorImageType> ("out", m_Mapper->GetOutput());

  }

  ChangeLabelFilterType::Pointer m_Mapper;
  LabelImageType::Pointer m_LabeledImage;

};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::LabeledImageColorMapping)


