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

#include "otbDEMConvertAdapter.h"

#include "itkMacro.h"

// OSSIM include
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#pragma GCC diagnostic ignored "-Wshadow"
#include "ossim/base/ossimKeywordNames.h"
#include "ossim/base/ossimStdOutProgress.h"
#include "ossim/base/ossimFilename.h"
#include "ossim/base/ossimKeywordlist.h"
#include "ossim/imaging/ossimJpegWriter.h"
#include "ossim/imaging/ossimImageHandler.h"
#include "ossim/imaging/ossimImageSource.h"
#include "ossim/imaging/ossimImageHandlerRegistry.h"
#include "ossim/imaging/ossimImageWriterFactoryRegistry.h"
#include "ossim/imaging/ossimImageWriterFactory.h"
#include "ossim/imaging/ossimImageFileWriter.h"
#include "ossim/imaging/ossimCacheTileSource.h"
#include "ossim/imaging/ossimBandSelector.h"
#include "ossim/imaging/ossimCibCadrgTileSource.h"
#pragma GCC diagnostic pop
#else
#include "ossim/base/ossimKeywordNames.h"
#include "ossim/base/ossimStdOutProgress.h"
#include "ossim/base/ossimFilename.h"
#include "ossim/base/ossimKeywordlist.h"
#include "ossim/imaging/ossimJpegWriter.h"
#include "ossim/imaging/ossimImageHandler.h"
#include "ossim/imaging/ossimImageSource.h"
#include "ossim/imaging/ossimImageHandlerRegistry.h"
#include "ossim/imaging/ossimImageWriterFactoryRegistry.h"
#include "ossim/imaging/ossimImageWriterFactory.h"
#include "ossim/imaging/ossimImageFileWriter.h"
#include "ossim/imaging/ossimCacheTileSource.h"
#include "ossim/imaging/ossimBandSelector.h"
#include "ossim/imaging/ossimCibCadrgTileSource.h"
#endif

namespace otb
{

DEMConvertAdapter::DEMConvertAdapter()
{}

DEMConvertAdapter::~DEMConvertAdapter()
{}

int DEMConvertAdapter::Convert(std::string tempFilename, std::string output)
{
  // Keyword list to initialize image writers with.
  ossimKeywordlist kwl;
  const char* PREFIX = "imagewriter.";
  // Define the output file type
  std::string output_type("general_raster_bsq_envi");
  kwl.add(PREFIX, ossimKeywordNames::TYPE_KW, output_type.c_str(), true);

  // Get an image handler for the input file.
  ossimRefPtr<ossimImageHandler> ih = ossimImageHandlerRegistry::instance()->open(ossimString(tempFilename));

  // Initialize the
  if (ih->getErrorStatus() == ossimErrorCodes::OSSIM_ERROR)
    {
    itkExceptionMacro("Error reading image:  " << tempFilename  << "Exiting application...");
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
  writer->open(ossimFilename(output));

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
      std::cerr << "std::exception  thrown:" << std::endl;
      std::cerr << e.what() <<  std::endl;
      itkExceptionMacro("Error occurs writing the output image...");
      return EXIT_FAILURE;
      }
    }
  else
    {
    itkExceptionMacro("Error detected in the image writer...");
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}

} // namespace otb
