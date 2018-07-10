/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "otbDEMConvertAdapter.h"

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
  void DoInit() override
  {
    SetName("DEMConvert");
    SetDescription("Converts a geo-referenced DEM image into a general raster file compatible with OTB DEM handling.");

    SetDocName("DEM Conversion");
    SetDocLongDescription("In order to be understood by the Orfeo ToolBox and the underlying OSSIM library, a geo-referenced Digital Elevation Model image can be converted into a general raster image, which consists in 3 files with the following extensions: .ras, .geom and .omd. Once converted, you have to place these files in a separate directory, and you can then use this directory to set the \"DEM Directory\" parameter of a DEM based OTB application or filter.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Manip);

    AddParameter(ParameterType_InputImage,  "in",  "Input geo-referenced DEM");
    SetParameterDescription("in", "Input geo-referenced DEM to convert to general raster format.");

    AddParameter(ParameterType_OutputFilename,    "out", "Prefix of the output files");
    std::ostringstream oss;
    oss << "will be used to get the prefix (name withtout extensions) of the files to write. ";
    oss << "Three files - prefix.geom, prefix.omd and prefix.ras - will be generated.";
    SetParameterDescription("out", oss.str());
    //MandatoryOn("out");

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "QB_Toulouse_Ortho_Elev.tif");
    SetDocExampleParameterValue("out", "outputDEM");

    SetOfficialDocLink();
}

void DoUpdateParameters() override
{
  // nothing to update
}

/* The main is simple : read image using OTB and write it as a tif.
*  Read this tif using OSSIM and convert it as a general raster file
*  (.ras, .geom and . omd)
*/

void DoExecute() override
{
  // Load input image
  FloatVectorImageType::Pointer inImage = GetParameterImage("in");

  // Set temporary tif filename (for ossim)
  std::string ofname = GetParameterString("out");
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
  ImageIOBase::Pointer  imageIO = ImageIOFactory::CreateImageIO( GetParameterString("in").c_str(),
                                                                      ImageIOFactory::ReadMode);
  std::string componentTypeInfo(imageIO->GetComponentTypeInfo().name());
  if( componentTypeInfo == typeid(unsigned char).name())
    {
    paramOut->SetPixelType(ImagePixelType_uint8);
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
  if( !itksys::SystemTools::RemoveFile(tempFilename) )
    {
    itkExceptionMacro("Problem while removing the file " << tempFilename);
    }

  // remove the geom file if any
  if( itksys::SystemTools::FileExists(tempFilenameGeom)
      && !itksys::SystemTools::RemoveFile(tempFilenameGeom))
    {
    itkExceptionMacro("Problem while removing the Geom file " << tempFilenameGeom);
    }
  }

};

} // namespace otb
}
OTB_APPLICATION_EXPORT(otb::Wrapper::DEMConvert)


