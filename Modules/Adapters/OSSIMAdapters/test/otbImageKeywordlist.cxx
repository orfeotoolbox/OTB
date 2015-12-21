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


#include <iostream>
#include <iomanip>
#include <fstream>
#include "otbMacro.h"
#include "otbImageKeywordlist.h"

#include "ossim/base/ossimKeywordlist.h"


#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include "ossim/projection/ossimProjection.h"
#include "ossim/projection/ossimProjectionFactoryRegistry.h"
#include "ossim/base/ossimFilename.h"
#pragma GCC diagnostic pop
#else
#include "ossim/projection/ossimProjection.h"
#include "ossim/projection/ossimProjectionFactoryRegistry.h"
#include "ossim/base/ossimFilename.h"
#endif



#include "ossim/ossimPluginProjectionFactory.h"

int otbImageKeywordlist(int argc, char* argv[])
{
  if (argc != 4)
    {
    std::cout << argv[0] << " <input filename> <output filename> <output filename2>" << std::endl;
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

  bool             hasMetaData = false;
  ossimKeywordlist geom_kwl, geom_kwl2, geom_kwl3;

  /** Don't use FactoryRegistry because of its default factory that can conflict
   * with plugins factor (cf. TSX .tif image read as QB)*/
  // test ossim plugin factory
  ossimProjection * projection = ossimplugins::ossimPluginProjectionFactory::instance()->createProjection(
    ossimFilename(filename), 0);

  // if ossim plugins factory failed, then test ossim factory
  if (!projection)
    {
    projection = ossimProjectionFactoryRegistry::instance()->createProjection(ossimFilename(filename), 0);
    if (!projection)
      {
      itkGenericExceptionMacro(<< "OSSIM Instanciate projection FAILED ! ");
      return EXIT_FAILURE;
      }
    }

  hasMetaData = projection->saveState(geom_kwl);
  if (!hasMetaData)
    {
    std::cerr << "ERROR: projection saveState return with a false flag !" << std::endl;
    return EXIT_FAILURE;
    }
  otb::ImageKeywordlist otb_kwl;
  otb_kwl.SetKeywordlist(geom_kwl);

  otb_kwl.convertToOSSIMKeywordlist(geom_kwl2);

  hasMetaData = projection->loadState(geom_kwl2);
  if (!hasMetaData)
    {
    std::cerr << "ERROR: projection loadState return with a false flag !" << std::endl;
    return EXIT_FAILURE;
    }
  hasMetaData = projection->saveState(geom_kwl3);
  if (!hasMetaData)
    {
    std::cerr << "ERROR: projection saveState return with a false flag !" << std::endl;
    return EXIT_FAILURE;
    }

  otb::ImageKeywordlist otb_kwl2;
  otb_kwl2.SetKeywordlist(geom_kwl3);

  file << "*** KEYWORD LIST ***\n";
  file << otb_kwl;
  file2 << "*** KEYWORD LIST ***\n";
  file2 << otb_kwl2;

  /** Search keyword in meter_per_pixel to truncate precision */
  ossimString s;
  double      valueX, valueY;

  otb_kwl.convertToOSSIMKeywordlist(geom_kwl2);

  file << std::fixed << std::setprecision(5);
  file2 << std::fixed << std::setprecision(5);

  s = geom_kwl2.find("meters_per_pixel_x");
  if (s != "")
    {
    valueX = s.toDouble();
    file << "truncate_meter_per_pixel_x " << valueX << std::endl;
    }
  s = geom_kwl2.find("meters_per_pixel_y");
  if (s != "")
    {
    valueY = s.toDouble();
    file << "truncate_meter_per_pixel_y " << valueY << std::endl;
    }

  otb_kwl2.convertToOSSIMKeywordlist(geom_kwl3);
  s = geom_kwl3.find("meters_per_pixel_x");
  if (s != "")
    {
    valueX = s.toDouble();
    file2 << "truncate_meter_per_pixel_x " << valueX << std::endl;
    }
  s = geom_kwl3.find("meters_per_pixel_y");
  if (s != "")
    {
    valueY = s.toDouble();
    file2 << "truncate_meter_per_pixel_y " << valueY << std::endl;
    }

  file.close();
  file2.close();

  return EXIT_SUCCESS;
}
