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
#include <ossim/imaging/ossimCacheTileSource.h>
#include <ossim/imaging/ossimBandSelector.h>
#include <ossim/imaging/ossimCibCadrgTileSource.h>

namespace otb
{


template<typename TPixelType>
int generic_convert_to_tif(otb::ApplicationOptionsResult* parseResult, ossimFilename tempFilename)
{
  typedef otb::VectorImage<TPixelType>              ImageType;
  typedef otb::ImageFileReader<ImageType>          ReaderType;
  typedef otb::StreamingImageFileWriter<ImageType> WriterType;

  typename ReaderType::Pointer reader = ReaderType::New();
  typename WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(parseResult->GetInputImage().c_str());
  writer->SetFileName(tempFilename); 

  writer->SetInput(reader->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}


int DEMConvert::Describe(ApplicationDescriptor* descriptor)
{
  descriptor->SetName("DEMConvertApplication");
  descriptor->SetDescription("Convert a DEM file into a general raster (.ras, .geom and .omd). To be used, those files have to be in a stand alone directory");
  descriptor->AddInputImage();
  descriptor->AddOption("OutputPath", "The filename (or path filename) of the output. It generates a Output.geom, Output.omd and Output.ras file.","out", 1, true,ApplicationDescriptor::String);
  descriptor->AddOption("KeepTif", "Keep the temporary generate tif file.","ktif", 0, false,ApplicationDescriptor::Boolean);
  
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

  ossimFilename tempFilename(parseResult->GetParameterString("OutputPath"));
  tempFilename += "_DEMConvert.tif";
  
  // Search for the input 
  typedef otb::VectorImage<double, 2> InputImageType;
  typedef otb::ImageFileReader<InputImageType> ReaderType;
  ReaderType::Pointer reader=ReaderType::New();
  reader->SetFileName(input_file);
  reader->UpdateOutputInformation();

  // Generate the tif image using OTB
  // We keep the same pixel type
  std::string componentTypeInfo(reader->GetImageIO()->GetComponentTypeInfo().name());
  if( componentTypeInfo == typeid(unsigned char).name())
    {
      generic_convert_to_tif<unsigned char>(parseResult, tempFilename);
    }
  else if( componentTypeInfo == typeid(char).name())
    {
      generic_convert_to_tif<char>(parseResult, tempFilename);
    }
  else if( componentTypeInfo == typeid(unsigned short).name())
    {
      generic_convert_to_tif<unsigned short>(parseResult, tempFilename);
    }
  else if( componentTypeInfo == typeid(short).name())
    {
      generic_convert_to_tif<short>(parseResult, tempFilename);
    }
  else if( componentTypeInfo == typeid(unsigned int).name())
    {
      generic_convert_to_tif<unsigned int>(parseResult, tempFilename);
    }
  else if( componentTypeInfo == typeid(int).name())
    {
      generic_convert_to_tif<int>(parseResult, tempFilename);
    }
  else if( componentTypeInfo == typeid(unsigned long).name())
    {
      generic_convert_to_tif<unsigned long>(parseResult, tempFilename);
    }
  else if( componentTypeInfo == typeid(long).name())
    {
      generic_convert_to_tif<long>(parseResult, tempFilename);
    }
  else if( componentTypeInfo == typeid(float).name())
    {
      generic_convert_to_tif<float>(parseResult, tempFilename);
    }
  else if( componentTypeInfo == typeid(double).name())
    {
      generic_convert_to_tif<double>(parseResult, tempFilename);
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
   const ossimString output_type("general_raster_bsq_envi");
   kwl.add(PREFIX, ossimKeywordNames::TYPE_KW, output_type.c_str(), true);
    
   // Get an image handler for the input file.
   ossimRefPtr<ossimImageHandler> ih = ossimImageHandlerRegistry::instance()->open(tempFilename);
   
   // Initialize the 
   if (ih->getErrorStatus() == ossimErrorCodes::OSSIM_ERROR)
   {
     itkExceptionMacro("Error reading image:  " << input_file << "Exiting application..."); 
     return EXIT_FAILURE;
   }
   
   ih->initialize();
   
   ossimRefPtr<ossimImageSource> source = ih.get();
   ossimRefPtr<ossimBandSelector> bs = 0;
   
   
   // Get the image rectangle for the rrLevel selected.
   ossimIrect output_rect;
   output_rect = source->getBoundingRect(0);

   ossimRefPtr<ossimImageFileWriter> writer =
     ossimImageWriterFactoryRegistry::instance()->createWriter(kwl, PREFIX);

   writer->connectMyInputTo(0, source.get());
   writer->open(ossimFilename(parseResult->GetParameterString("OutputPath"))+".ras");
   
   // Add a listener to get percent complete.
   ossimStdOutProgress prog(0, true);
   writer->addListener(&prog);

   if (writer->getErrorStatus() == ossimErrorCodes::OSSIM_OK)
   {
      if( (ih->getOutputScalarType() != OSSIM_UCHAR) &&
          (PTR_CAST(ossimJpegWriter, writer.get()) ) )
      {
         writer->setScaleToEightBitFlag(true);
      }

      ossimRefPtr<ossimCacheTileSource> cache = new ossimCacheTileSource;
      ossimIpt tileWidthHeight(ih->getImageTileWidth(),
                               ih->getImageTileHeight());
      // only use the cache if its stripped
      if(static_cast<ossim_uint32>(tileWidthHeight.x) ==
         ih->getBoundingRect().width())
      {
         cache->connectMyInputTo(0, source.get());
         cache->setTileSize(tileWidthHeight);
         writer->connectMyInputTo(0, cache.get());
      }
      else
      {
         writer->connectMyInputTo(0, source.get());
      }
      writer->initialize();
      writer->setAreaOfInterest(output_rect); // Set the output rectangle.
      
      try
      {
         writer->execute();
      }
      catch(std::exception& e)
      {
        itkExceptionMacro("Error occurs writing the ouput image...");
        return EXIT_FAILURE;
      }
      
   }
   else
     {
       itkExceptionMacro("Error detected in the image writer...");
       return EXIT_FAILURE;
     }



   if ( parseResult->IsOptionPresent("KeepTif") == false)
     {
       bool resRemove = tempFilename.remove();
       if( resRemove == false )
         {
           std::cout<<"Enable to erase the output temporary file "<<tempFilename<<"."<<std::endl;
         }
     }

   return EXIT_SUCCESS;
}


} // namespace otb
