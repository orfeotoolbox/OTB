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

enum Inverse { fwd, inv, undef};

int main(int argc, char * argv[])
{
  std::cout.precision(20);
  
   string annotationXml;
   Inverse inverse = undef;
   bool verbose;

   for (int i=1; i!=argc ; ++i) {
      std::string const a(argv[i]);
      if (a == "-v" || a == "-verbose") {
         verbose = true;
      } else if (inverse == undef)  {
         inverse = atoi(argv[i]) == 1 ? inv : fwd;
      } else if (annotationXml.empty())  {
         annotationXml = a;
      } else {
         std::cerr << "Too many arguments\n"
            << argv[0] << "[-v|--verbose] <inverse> <annotationXml>\n";
         return EXIT_FAILURE;
      }
   }

   if(annotationXml.empty())
   {
      std::cerr << "Not enough arguments\n"
         << argv[0] << "[-v|--verbose] <inverse> <annotationXml>\n";
      return EXIT_FAILURE;
   }

  ossimplugins::ossimSentinel1SarSensorModel * sensor = new ossimplugins::ossimSentinel1SarSensorModel();

  sensor->readAnnotationFile(annotationXml);
  if (verbose) {
     sensor->print(std::cout);
  }

  bool validate(false);

  if(inverse == inv)
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
