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
#include <fstream>

#include "ossim/base/ossimFilename.h"
#include "ossim/base/ossimString.h"
#include "ossim/imaging/ossimImageHandlerRegistry.h"
#include "ossim/imaging/ossimImageHandler.h"

// this is the most important class and is called as the first line of all applications.
// without this alll the important factories are not created.
#include "ossim/init/ossimInit.h"


int ossimKeywordlistTest(int argc, char* argv[])
{
   ossimInit::instance()->initialize(argc, argv);

   if(argc!=3)
   {
        std::cout << "<input filename> <output ascii file>" << std::endl;
        return EXIT_FAILURE;
   }
   else
   {
      // try to open up the passed in image
      //
        ossimImageHandler *handler   = ossimImageHandlerRegistry::instance()->open(ossimFilename(argv[1]));

        if(!handler)
        {
                cout << "Unable to open input image: "<< argv[1] << endl;
                return EXIT_FAILURE;
        }

        ossimKeywordlist geom;
        handler->getImageGeometry()->getProjection()->saveState(geom);
        ofstream file;
        file.open(argv[2]);
        file << " keywordlist:"<<std::endl<<geom<<std::endl;
        file.close();

        delete handler;

   }

   return EXIT_SUCCESS;
}



