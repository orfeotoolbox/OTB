//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL-2
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

#include "ossimTerraSarXSarSensorModel.h"


int main(int argc, char * argv[])
{
  std::cout.precision(9);
  
  if(argc != 4)
    return EXIT_FAILURE;
  
  std::string annotationXml = argv[2];
  std::string geoXml = argv[3];
  bool inverse = atoi(argv[1]);

  ossimplugins::ossimTerraSarXSarSensorModel * sensor = new ossimplugins::ossimTerraSarXSarSensorModel();

  sensor->readAnnotationFile(annotationXml, geoXml);

  bool validate(false);

  if(inverse)
    {
    validate = sensor->autovalidateInverseModelFromGCPs();
    }
  else
    {
    validate = sensor->autovalidateForwardModelFromGCPs();
    }

  delete sensor;

  if(validate)
    return EXIT_SUCCESS;
  
  return EXIT_FAILURE;
}
