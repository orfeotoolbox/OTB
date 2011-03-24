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

#include "otbDEMConvert.h"

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "otbStreamingMinMaxVectorImageFilter.h"

// OSSIM include
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimStdOutProgress.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/imaging/ossimJpegWriter.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimImageSource.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/imaging/ossimImageWriterFactoryRegistry.h>
#include <ossim/imaging/ossimImageWriterFactory.h>
#include <ossim/imaging/ossimImageFileWriter.h>
//#include <ossim/imaging/ossimCacheTileSource.h>
//#include <ossim/imaging/ossimBandSelector.h>
//#include <ossim/imaging/ossimCibCadrgTileSource.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimScalarTypeLut.h>
#include <ossim/imaging/ossimMetadataFileWriter.h>
//#include <ossim/base/ossimSource.h>
#include <ossim/imaging/ossimGeomFileWriter.h>

namespace otb
{


template<typename TPixelType>
int generic_convert_to_ras(otb::ApplicationOptionsResult* parseResult, ossimFilename rasFilename)
{
  typedef otb::VectorImage<TPixelType>              ImageType;
  typedef otb::ImageFileReader<ImageType>          ReaderType;
  typedef otb::StreamingImageFileWriter<ImageType> WriterType;
 

  typename ReaderType::Pointer reader = ReaderType::New();
  typename WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(parseResult->GetInputImage().c_str());
  writer->SetFileName(rasFilename); 

  writer->SetInput(reader->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}


int DEMConvert::Describe(ApplicationDescriptor* descriptor)
{
  descriptor->SetName("DEMConvertApplication");
  descriptor->SetDescription("Convert a DEM file into a general raster (.ras, .geom and .omd)");
  descriptor->AddInputImage();
  descriptor->AddOption("OutputPath", "The filename (or path filename) of the output. It generates a Output.geom, Output.omd and Output.ras file.","out", 1, true, ApplicationDescriptor::String);
  
  return EXIT_SUCCESS;
}


/* The main is simple : read image using OTB and write it as a tif.
* Read this tif using OSSIM and convert it as a general raster file (.ras, .geom and . omd)
* first write the image as a tif (supported by OSSIM) allows to not only be abble to work
* with OSSIM supported format but any more.
*/

int DEMConvert::Execute(otb::ApplicationOptionsResult* parseResult)
{
  // Load input and output filename
  const char * input_file = parseResult->GetInputImage().c_str();

  ossimFilename rasFilename(parseResult->GetParameterString("OutputPath"));
  rasFilename += ".ras";
  
  // Search for the input 
  typedef otb::VectorImage<double, 2> InputImageType;
  typedef otb::ImageFileReader<InputImageType> ReaderType;
  typedef otb::StreamingMinMaxVectorImageFilter<InputImageType> MinMaxCalculatorType;
 
  ReaderType::Pointer reader=ReaderType::New();
  reader->SetFileName(input_file);
  reader->UpdateOutputInformation();

  // Generate the tif image using OTB
  // We keep the same pixel type
  std::string componentTypeInfo(reader->GetImageIO()->GetComponentTypeInfo().name());
  if( componentTypeInfo == typeid(unsigned char).name())
    {
      generic_convert_to_ras<unsigned char>(parseResult, rasFilename);
    }
  else if( componentTypeInfo == typeid(char).name())
    {
      generic_convert_to_ras<char>(parseResult, rasFilename);
    }
  else if( componentTypeInfo == typeid(unsigned short).name())
    {
      generic_convert_to_ras<unsigned short>(parseResult, rasFilename);
    }
  else if( componentTypeInfo == typeid(short).name())
    {
      generic_convert_to_ras<short>(parseResult, rasFilename);
    }
  else if( componentTypeInfo == typeid(unsigned int).name())
    {
      generic_convert_to_ras<unsigned int>(parseResult, rasFilename);
    }
  else if( componentTypeInfo == typeid(int).name())
    {
      generic_convert_to_ras<int>(parseResult, rasFilename);
    }
  else if( componentTypeInfo == typeid(unsigned long).name())
    {
      generic_convert_to_ras<unsigned long>(parseResult, rasFilename);
    }
  else if( componentTypeInfo == typeid(long).name())
    {
      generic_convert_to_ras<long>(parseResult, rasFilename);
    }
  else if( componentTypeInfo == typeid(float).name())
    {
      generic_convert_to_ras<float>(parseResult, rasFilename);
    }
  else if( componentTypeInfo == typeid(double).name())
    {
      generic_convert_to_ras<double>(parseResult, rasFilename);
    }
  else
    {
      itkExceptionMacro("This appication doesn't support image pixel type " << componentTypeInfo);
      return EXIT_FAILURE;
    }


  // Keyword list to initialize image writers with.
  ossimKeywordlist kwl;
  const char* PREFIX = "imagewriter.";
  // Define the output file type
  const ossimString output_type("general_raster_bil");
  kwl.add(PREFIX, ossimKeywordNames::TYPE_KW, output_type.c_str(), true);
    
  // Get an image handler for the input file.
  ossimRefPtr<ossimImageHandler> ih = ossimImageHandlerRegistry::instance()->open(rasFilename);
  
  // Initialize the 
  if (ih->getErrorStatus() == ossimErrorCodes::OSSIM_ERROR)
    {
      itkExceptionMacro("Error reading image:  " << input_file << "Exiting application..."); 
      return EXIT_FAILURE;
    }
  
  ih->initialize();
  
  ossimRefPtr<ossimImageSource> source = ih.get();
  
  ossimString interleaveType = "bil";
  ossimString scalar = ossimScalarTypeLut::instance()->getEntryString(ih->getOutputScalarType());
  
  // Compute image min/max
  MinMaxCalculatorType::Pointer minMax = MinMaxCalculatorType::New();
  minMax->SetInput(reader->GetOutput());
  minMax->Update();
  
/*
  std::vector<double> minVect;
  std::vector<double> maxVect;
  for (unsigned int i=0; i<reader->GetOutput()->GetNumberOfComponentsPerPixel(); ++i)
    {
      minVect.push_back( static_cast<double>(ih->getMinPixelValue(i) ) );
      maxVect.push_back( static_cast<double>(ih->getMaxPixelValue(i) ) );
    }
  */
  
  /***************************************************************************
   ************************** WRITE OMD file
   ****************************************************************************/
  // Make a header file name from the image file.
   ossimFilename headerFile(rasFilename);
   headerFile.setExtension(".omd"); // ossim meta data

   std::ofstream os;
   os.open(headerFile.c_str(), ios::out);
   if (!os)
   {
      itkExceptionMacro(" Error: Could not open:  " << headerFile);
      return EXIT_FAILURE;
   }
   
   os << "// *** ossim meta data general raster header file ***\n"
      << ossimKeywordNames::IMAGE_FILE_KW << ":  "
      << rasFilename.file().c_str() << "\n"
      << ossimKeywordNames::IMAGE_TYPE_KW << ":  "
      << output_type << "\n"
      << ossimKeywordNames::INTERLEAVE_TYPE_KW << ":  "
      << interleaveType.c_str() << "\n"
      << ossimKeywordNames::NUMBER_BANDS_KW << ":  "
      << reader->GetOutput()->GetNumberOfComponentsPerPixel() << "\n"
      << ossimKeywordNames::NUMBER_LINES_KW << ":  "
      << ih->getNumberOfLines() << "\n"//(theAreaOfInterest.lr().y - theAreaOfInterest.ul().y + 1) << "\n"
      << ossimKeywordNames::NUMBER_SAMPLES_KW << ":  "
      << ih->getNumberOfSamples() << "\n"//(theAreaOfInterest.lr().x - theAreaOfInterest.ul().x + 1) << "\n"
      << ossimKeywordNames::SCALAR_TYPE_KW << ":  "
      << scalar.c_str() << "\n"
      << ossimKeywordNames::BYTE_ORDER_KW <<": "
      << ((ossimEndian().getSystemEndianType()==OSSIM_BIG_ENDIAN)?"big_endian":"little_endian")
      << "\n"
      << std::endl;

   // Output the null/min/max for each band.
   os << "\n// NOTE:  Bands are one based, band1 is the first band."
      << std::endl;

   for (ossim_uint32 i=0; i<reader->GetOutput()->GetNumberOfComponentsPerPixel(); ++i)
   {
      ossimString prefix = ossimKeywordNames::BAND_KW +
                           ossimString::toString(i+1) + ".";
      
      ossimString null_pix = ossimString::toString(ih->getNullPixelValue(i));
      ossimString  min_pix   = ossimString::toString(minMax->GetMinimum()[i]);
      ossimString max_pix  = ossimString::toString(minMax->GetMaximum()[i]);
      
      os << prefix.c_str() << ossimKeywordNames::NULL_VALUE_KW << ":  "
         << null_pix.c_str() << "\n"
         << prefix << ossimKeywordNames::MIN_VALUE_KW << ":  "
         << min_pix.c_str() << "\n"
         << prefix << ossimKeywordNames::MAX_VALUE_KW << ":  "
         << max_pix.c_str() << std::endl;
   }
   
   os.close();


   /***************************************************************************
   ************************** WRITE GEOM file
   ***************************************************************************/
   // Make the file name.
   ossimFilename geomFile( rasFilename );
   geomFile.setExtension(ossimString("geom"));
     
   // Make the writer.
   ossimRefPtr<ossimMetadataFileWriter> geoWriter = new ossimGeomFileWriter();

   // Set things up.
   geoWriter->connectMyInputTo(0, source.get());
   geoWriter->setFilename(geomFile);
   geoWriter->initialize();
   geoWriter->setPixelType(OSSIM_PIXEL_IS_POINT);
   source.get()->initialize();
   geoWriter->setAreaOfInterest( ih->getBoundingRect() );

   // Write it to disk.
   try
     {
       geoWriter->execute();
     }
   catch(std::exception& e)
     {
       itkExceptionMacro("Error occurs writing geom file...");
       return EXIT_FAILURE;
     }

   return EXIT_SUCCESS;
}



} // namespace otb
