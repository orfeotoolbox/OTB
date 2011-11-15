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

#include "otbChangeLabelImageFilter.h"


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

 typedef otb::ChangeLabelImageFilter<LabelImageType, VectorImageType> ChangeLabelFilterType;

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
    AddParameter(ParameterType_Filename, "ct", "Color table");
    SetParameterDescription("ct",  "An ASCII file containing the color table\n"
                                   "with one color per line\n"
                                   "(for instance the line '1 255 0 0' means that all pixels with label 1 will be replaced by RGB color 255 0 0)\n"
                                   "Lines beginning with a # are ignored");
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


