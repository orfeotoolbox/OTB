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

#include <cstdlib>

#include <iostream>
#include <iomanip>
#include <cmath>


#include "ossim/base/ossimCommon.h"
#include "ossim/init/ossimInit.h"
#include "otb/HermiteInterpolator.h"
#include "otb/PlatformPosition.h"
#include "otb/Ephemeris.h"
#include "otb/JSDDateTime.h"

#include "itkTimeProbe.h"
#include "projection/ossimProjection.h"
#include "projection/ossimProjectionFactoryRegistry.h"
#include "ossim/ossimPluginProjectionFactory.h"
#include "ossim/base/ossimKeywordlist.h"


int ossimpluginsHermiteInterpolationPlateformPositionTest(int argc, char * argv[])
{
  ossimInit::instance()->initialize(argc, argv);

  if(argc!=2)
  {
       std::cout << "<input filename>" << std::endl;
       return EXIT_FAILURE;
  }

  char * filename = argv[1];

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
      std::cout<<"OSSIM Instanciate projection FAILED ! ";
      return EXIT_FAILURE;
    }
  }


  ossimKeywordlist geom;
  std::cout << "Read ossim Keywordlist...";

  bool hasMetaData = false;

  hasMetaData = projection->saveState(geom);

  if (!hasMetaData)
  {
    std::cout << "Bad metadata parsing " << std::endl;
    return EXIT_FAILURE;
  }


  // Get plateform position count :
  const char* platform_positions_count_str = geom.find("platform_positions_count");
  unsigned int platform_positions_count = atoi(platform_positions_count_str);
  std::cout << "plateform_positions_count :" << platform_positions_count << std::endl;

  ossimplugins::PlatformPosition platform_position ;
  hasMetaData = platform_position.loadState(geom);

  if (!hasMetaData)
  {
    std::cout << "Bad metadata parsing " << std::endl;
    return EXIT_FAILURE;
  }

  for(int i = 0 ; i < platform_position.getNbrData() ; ++i)
    {
      ossimplugins::Ephemeris * ephemeris = platform_position.getData(i);
      double * position = ephemeris->get_position();
      double * velocity = ephemeris->get_speed();
      ossimplugins::JSDDateTime date = ephemeris->get_date();

      ossimplugins::Ephemeris * interpolateEphemeris = platform_position.Interpolate(date);

      double * interpolatePosition = interpolateEphemeris->get_position();
      double * interpolateVelocity = interpolateEphemeris->get_speed();
      ossimplugins::JSDDateTime interpolateDate = interpolateEphemeris->get_date();

      double diffPosition = 0.0;
      double diffVelocity = 0.0;

      for(unsigned int j = 0 ; j <3 ; ++j)
        {
          diffPosition += (interpolatePosition[j]-position[j]) * (interpolatePosition[j]-position[j]);
          diffVelocity += (interpolateVelocity[j]-velocity[j]) * (interpolateVelocity[j]-velocity[j]);
        }
      std::cout << "Diff position ["<< i<<"] =" << diffPosition << std::endl;
      std::cout << "Diff velocity ["<< i<<"] =" << diffVelocity << std::endl;
    }

  return EXIT_SUCCESS;
}
