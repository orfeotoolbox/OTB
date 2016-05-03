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

#include "ossimSentinel1SarSensorModel.h"


int main(int argc, char * argv[])
{
  std::cout.precision(9);
  
  if(argc != 3)
    return EXIT_FAILURE;
  
  std::string annotationXml = argv[2];
  bool inverse = atoi(argv[1]);

  
  ossimplugins::ossimSentinel1SarSensorModel * sensor = new ossimplugins::ossimSentinel1SarSensorModel();

  sensor->readAnnotationFile(annotationXml);

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
