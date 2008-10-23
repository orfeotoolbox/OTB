/*!
 *
 * OVERVIEW:
 * 
 * Will reproject the input image to a utm projection.
 * 
 * PURPOSE:
 *
 * Teach you how to create a renderer and set the view and connect it up to an input
 * 
 */

// iostream is used for general output
//
#include <iostream>
#include <iterator>

#include "base/ossimFilename.h"
#include "base/ossimString.h"
#include "imaging/ossimImageHandlerRegistry.h"
#include "imaging/ossimImageHandler.h"
#include "imaging/ossimImageFileWriter.h"
#include "imaging/ossimImageWriterFactoryRegistry.h"

// this is an ossim ground point and has a lat, lon, and datum
// associated with it.
#include "base/ossimGpt.h"

// Base class for accessing projection parameters.
#include "projection/ossimProjection.h"

// the projection used in the reprojection
#include "projection/ossimUtmProjection.h"

// used to instantiate a projector
#include "projection/ossimProjectionFactoryRegistry.h"


// The heart of the OSSIM resampling process for reprojecting imagery
#include "imaging/ossimImageRenderer.h"

// this is the most important class and is called as the first line of all applications.
// without this alll the important factories are not created.
#include "init/ossimInit.h"

#include "itkMacro.h"

using namespace std;

void usage();
void printOutputTypes();
ossimProjection* newUtmView(const ossimGpt& centerGround,
                            const ossimDpt& metersPerPixel);

int ossimIntegrationTest(int argc, char* argv[])
{
  try 
    {

   ossimInit::instance()->initialize(argc, argv);

   if(argc!=4)
   {
      usage();
      itkGenericExceptionMacro( << "Usage error !" );
      return EXIT_FAILURE;
   }
   else
   {
      // try to open up the passed in image
      //
      ossimImageHandler *handler   = ossimImageHandlerRegistry::instance()->open(ossimFilename(argv[2]));

      // try to create a writer for the output image type.
      //
      ossimImageFileWriter* writer = ossimImageWriterFactoryRegistry::instance()->createWriter(ossimString(argv[1]));


      if(!handler)
      {
         itkGenericExceptionMacro( << "Unable to open input image: " << std::string(argv[2]) );
         return EXIT_FAILURE;
      }
      if(!writer)
      {
         delete handler;
         itkGenericExceptionMacro( << "Unable to create writer of type: " << std::string(argv[1]) );
         return EXIT_FAILURE;
      }

      ossimKeywordlist geom;
      handler->getImageGeometry(geom);

      // grab a projection if it exists
      //
      ossimProjection* inputProjection = ossimProjectionFactoryRegistry::instance()->createProjection(geom);

      if (!inputProjection)
      {
         delete handler;
         delete writer;
         itkGenericExceptionMacro( << "the input image has no input projection and can't be reprojected\n" );
         return EXIT_FAILURE;
      }

      // --------------------- SETUP The Resampleing process -------------
      //
      // the renderer is the resampler that fits into the chain.
      // we will set it to a UTM view. We first get the inputs geometry and
      // then get the scale from its projector.
      //
      // now lets set up the renderer
      ossimImageRenderer* renderer = new ossimImageRenderer;

      //
      // get the center ground by running the center pixel
      // through the handler's projection object
      ossimIrect bounds = handler->getBoundingRect();
      ossimGpt centerGround;
      inputProjection->lineSampleToWorld(bounds.midPoint(), centerGround);

      // now set the view to a new UTM view.
      // I pass true in to tell the renderer that it owns the
      // projection and will be responsible for deleting
      //
      renderer->setView(newUtmView(centerGround,
                                   inputProjection->getMetersPerPixel()),
                        true);

      // connect the renderer to the handler
      renderer->connectMyInputTo(handler);
      
      // specify the output file name
      writer->setFilename(ossimFilename(argv[3]));

      // now connect the writer to the renderer
      writer->connectMyInputTo(0, renderer);

      // execute the reprojection process.
      writer->execute();

      delete writer;
      delete handler;
   }

    }
  catch( itk::ExceptionObject & err ) 
    { 
    std::cout << "Exception itk::ExceptionObject levee !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
    } 
  catch (...) 
    {
    std::cerr << "Unknowed exception thrown" << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS; 
}


void usage()
{
   cout << "repoject <output_type> <input filename> <output filename>" << endl
        << "where output types are: " << endl;
   printOutputTypes();
}

void printOutputTypes()
{
    std::vector<ossimString> outputType;
   
    ossimImageWriterFactoryRegistry::instance()->getImageTypeList(outputType);
    std::copy(outputType.begin(),
              outputType.end(),
              std::ostream_iterator<ossimString>(std::cout, "\n"));
}

ossimProjection* newUtmView(const ossimGpt& centerGround,
                            const ossimDpt& metersPerPixel)
{
   ossimUtmProjection* utm = new ossimUtmProjection;

   // we will make it a square pixel in meters
   //double averageGsd = (metersPerPixel.x + metersPerPixel.y)*.5;
   utm->setZone(centerGround);
   utm->setMetersPerPixel(ossimDpt(metersPerPixel));

   
   return utm;
}
