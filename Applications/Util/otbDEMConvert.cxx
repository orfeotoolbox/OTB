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

#include "otbDEMConvertAdapter.h"

#include "itkImageIOBase.h"
#include "itkImageIOFactory.h"

namespace otb
{
namespace Wrapper
{

class DEMConvert : public Application
{
public:
  /** Standard class typedefs. */
  typedef DEMConvert                    Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(DEMConvert, otb::Application);

private:
  DEMConvert()
  {
    SetName("DEMConvert");
    SetDescription("Convert a DEM file into a general raster ");

    SetDocName("DEM Convert Application");
    SetDocLongDescription("This application convert a DEM file into a general raster (.ras, .geom and .omd). To be used, those files have to be in a stand alone directory");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");
    SetDocCLExample("otbApplicationLauncherCommandLine DEMConvert ${OTB-BIN}/bin"
      " --in ${OTB-DATA}/Input/QB_MUL_ROI_1000_100.tif --out ${TEMP}/outputDEM.any");
    AddDocTag("Image manipulation");
  }

  virtual ~DEMConvert()
  {
  }

void DoCreateParameters()
  {
    AddParameter(ParameterType_InputImage,  "in",  "Input Image");
    SetParameterDescription("in", "Input image to filter.");

    AddParameter(ParameterType_Filename,    "out", "Output prefix");
    std::ostringstream oss;
    oss << "will be used to get the prefix of the images to write. ";
    oss << "The files - Output.geom, Output.omd and Output.ras - will be generated";
    SetParameterDescription("out", oss.str());
    SetParameterRole("out", Role_Output);
}

  void DoUpdateParameters()
  {
    // nothing to update
  }
  
/* The main is simple : read image using OTB and write it as a tif.
*  Read this tif using OSSIM and convert it as a general raster file
*  (.ras, .geom and . omd) 
*/

void DoExecute()
{
  // Load input image
  FloatVectorImageType::Pointer inImage = GetParameterImage("in");
  
  // Set temporary tif filename (for ossim)
  std::string ofname = GetParameterString("out");;
  std::string path   = itksys::SystemTools::GetFilenamePath(ofname);
  std::string fname  = itksys::SystemTools::GetFilenameWithoutExtension(ofname);
  std::string tempFilename = path+"/"+fname+"_DEMConvert.tif";
  std::string tempFilenameGeom = path+"/"+fname+"_DEMConvert.geom";
  
  // Generate the tif image using OTB while keeping the same  pixel
  // type then the input image 
  // Initialize an outputParameter and set its output pixeltype
  OutputImageParameter::Pointer paramOut = OutputImageParameter::New();
  std::ostringstream osswriter;
  osswriter<< "writing temporary tif file";
  
  // Set the filename of the current output image
  paramOut->SetFileName(tempFilename);
  paramOut->SetValue(inImage);

  // Set the output pixel type
  itk::ImageIOBase::Pointer  imageIO = ImageIOFactory::CreateImageIO( GetParameterString("in").c_str(), 
                                                                      ImageIOFactory::ReadMode);
  std::string componentTypeInfo(imageIO->GetComponentTypeInfo().name());
  if( componentTypeInfo == typeid(unsigned char).name())
    {
    paramOut->SetPixelType(ImagePixelType_uint8);
    }
  else if( componentTypeInfo == typeid(char).name())
    {
    paramOut->SetPixelType(ImagePixelType_int8);
    }
  else if( componentTypeInfo == typeid(unsigned short).name())
    {
    paramOut->SetPixelType(ImagePixelType_uint16);
    }
  else if( componentTypeInfo == typeid(short).name())
    {
    paramOut->SetPixelType(ImagePixelType_int16);
    }
  else if( componentTypeInfo == typeid(unsigned int).name())
    {
    paramOut->SetPixelType(ImagePixelType_uint32);
    }
  else if( componentTypeInfo == typeid(int).name())
    {
    paramOut->SetPixelType(ImagePixelType_int32);
    }
  else if( componentTypeInfo == typeid(float).name())
    {
    paramOut->SetPixelType(ImagePixelType_float);
    }
  else if( componentTypeInfo == typeid(double).name())
    {
    paramOut->SetPixelType(ImagePixelType_double);
    }
  else
    {
    itkExceptionMacro("This application doesn't support image pixel type " << componentTypeInfo);
    }

  // Add the tempfilename to be written
  paramOut->InitializeWriters();
  AddProcess(paramOut->GetWriter(), osswriter.str());
  paramOut->Write();

  // Set the output ras file
  std::string output = path+"/"+fname+".ras";

  DEMConvertAdapter::Pointer DEMConverter = DEMConvertAdapter::New();
  DEMConverter->Convert(tempFilename, output);

  // remove the temprorary tif file
  if( !itksys::SystemTools::RemoveFile(tempFilename.c_str()) )
    {
    itkExceptionMacro("Problem while removing the file " << tempFilename);
    }
  
  // remove the geom file if any
  if( itksys::SystemTools::FileExists(tempFilenameGeom.c_str()) 
      && !itksys::SystemTools::RemoveFile(tempFilenameGeom.c_str()))
    {
    itkExceptionMacro("Problem while removing the Geom file " << tempFilenameGeom);
    }
  }

};

} // namespace otb
}
OTB_APPLICATION_EXPORT(otb::Wrapper::DEMConvert)


