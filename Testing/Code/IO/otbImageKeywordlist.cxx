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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <iostream>
#include <iomanip>
#include <fstream>
#include "otbMacro.h"
#include "otbImageKeywordlist.h"

#include "base/ossimKeywordlist.h"
#include "projection/ossimProjection.h"
#include "projection/ossimProjectionFactoryRegistry.h"
#include "ossim/ossimPluginProjectionFactory.h"


int otbImageKeywordlist( int argc, char* argv[] )
{
  if (argc!=4)
  {
    std::cout << argv[0] <<" <input filename> <output filename> <output filename2>"
    << std::endl;

    return EXIT_FAILURE;
  }

  char * filename = argv[1];
  char * outFilename = argv[2];
  char * outFilename2 = argv[3];

  std::ofstream file;
  file.open(outFilename);
  file << std::setprecision(15);
  std::ofstream file2;
  file2.open(outFilename2);
  file2 << std::setprecision(15);

  bool hasMetaData = false;
  ossimKeywordlist geom_kwl, geom_kwl2, geom_kwl3;

  ossimProjectionFactoryRegistry::instance()->registerFactory(ossimplugins::ossimPluginProjectionFactory::instance());
    ossimProjection * projection = ossimProjectionFactoryRegistry::instance()
    ->createProjection(ossimFilename(filename),0);
    if (!projection)
    {
      otbMsgDevMacro( <<"OSSIM Instanciate projection FAILED ! ");
      return EXIT_FAILURE;
    }

    hasMetaData = projection->saveState(geom_kwl);
std::cout << "geom_kwl:"<<geom_kwl<<std::endl;
    otb::ImageKeywordlist otb_kwl;
    otb_kwl.SetKeywordlist( geom_kwl );

    otb_kwl.convertToOSSIMKeywordlist( geom_kwl2 ); 
std::cout << "geom_kwl2:"<<geom_kwl2<<std::endl;

    hasMetaData = projection->loadState(geom_kwl2);
    hasMetaData = projection->saveState(geom_kwl3);
    otb::ImageKeywordlist otb_kwl2;
    otb_kwl2.SetKeywordlist( geom_kwl3 );
std::cout << "geom_kwl3:"<<geom_kwl3<<std::endl;

  file << "*** KEYWORD LIST ***\n";
  file << otb_kwl;
  file2 << "*** KEYWORD LIST ***\n";
  file2 << otb_kwl2;

  file.close();
  file2.close();


  return EXIT_SUCCESS;
}

