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
#include "otbWrapperParameter.h"
#include "otbWrapperOutputImageParameter.h"

#include "otbMultiToMonoChannelExtractROI.h"

namespace otb
{

namespace Wrapper
{

class  SplitImage : public Application
{

public:
  /** Standard class typedefs. */
  typedef SplitImage                    Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(SplitImage, Application);

  /** Filters typedef */
  typedef otb::MultiToMonoChannelExtractROI<FloatVectorImageType::InternalPixelType,
                                            FloatVectorImageType::InternalPixelType> FilterType;

private:
  void DoInit()
  {
    SetName("SplitImage");
    SetDescription("Split a N multiband image into N images");

    SetDocName("Split Image");
    SetDocLongDescription("This application splits a N multiband image into N images. The output images filename will be generated from the output one. Thus if the input image has 2 channels, and the user has set ad output outimage.tif, the generated images will be outimage_0.tif and outimage_1.tif");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Manip);

    AddParameter(ParameterType_InputImage, "in", "Input Image");
    SetParameterDescription("in","Input image filename.");

    AddParameter(ParameterType_Filename, "out", "Output Image");
    SetParameterDescription("out",
                            "Will be used to get the prefix and the extension of the output images to write");

    AddParameter(ParameterType_RAM, "ram", "Available RAM");
    SetDefaultParameterInt("ram", 256);
    MandatoryOff("ram");

  // Doc example parameter settings
    SetDocExampleParameterValue("in", "VegetationIndex.hd");
    SetDocExampleParameterValue("out", "splittedImage.tif");
  }

  void DoUpdateParameters()
  {
    // Nothing to do here for the parameters : all are independent
  }

  void DoExecute()
  {
    // Get the input image
    FloatVectorImageType::Pointer inImage = GetParameterImage("in");

    // Get the path/fileWithoutextension/extension of the output images filename
    std::string path, fname, ext;
    std::string ofname = GetParameterString("out");
    
    // Get the extension and the prefix of the filename
    path  = itksys::SystemTools::GetFilenamePath(ofname);
    fname = itksys::SystemTools::GetFilenameWithoutExtension(ofname);
    ext   = itksys::SystemTools::GetFilenameExtension(ofname);

    // Set the extract filter input image
    m_Filter = FilterType::New();
    m_Filter->SetInput(inImage);

    for (unsigned int i = 0; i < inImage->GetNumberOfComponentsPerPixel(); ++i)
      {
      // Set the channel to extract
      m_Filter->SetChannel(i+1);
      
      // build the current output filename
      std::ostringstream oss;
      oss <<path<<"/"<<fname<<"_"<<i<<ext;
      
      // Create an output parameter to write the current output image
      OutputImageParameter::Pointer paramOut = OutputImageParameter::New();
        
      // writer label
      std::ostringstream osswriter;
      osswriter<< "writer (Channel : "<< i<<")";

      // Set the filename of the current output image
      paramOut->SetFileName(oss.str());
      paramOut->SetValue(m_Filter->GetOutput());
      // Add the current level to be written
      paramOut->InitializeWriters();
      AddProcess(paramOut->GetWriter(), osswriter.str());
      paramOut->Write();
      }

    // Disable the output Image parameter to avoid writing
    // the last image (Application::ExecuteAndWriteOutput method)
    GetParameterByKey("out")->SetActive(false);
  }

  FilterType::Pointer        m_Filter;
};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::SplitImage)
 
