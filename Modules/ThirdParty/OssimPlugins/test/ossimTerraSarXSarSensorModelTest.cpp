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
// #include "ossimPluginProjectionFactory.h"
#include <iostream>
#include <memory>
#include <map>
#include <string>


using namespace ossimplugins;
enum Inverse { fwd, inv, undef};

int main(int argc, char * argv[])
{
   std::clog.precision(20);

   string annotationXml;
   string geoXml;
   Inverse inverse = undef;
   bool verbose = false;

   for (int i=1; i!=argc ; ++i) {
      std::string const a(argv[i]);
      if (a == "-v" || a == "-verbose") {
         verbose = true;
      } else if (inverse == undef)  {
         inverse = atoi(argv[i]) == 1 ? inv : fwd;
      } else if (annotationXml.empty())  {
         annotationXml = a;
      } else if (geoXml.empty())  {
         geoXml = a;
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
   if (!verbose) {
      std::clog.setstate(std::ios_base::badbit);
   }

   try {
#if 0
      std::auto_ptr<ossimProjection> projection
         (ossimPluginProjectionFactory::instance()->createProjection(annotationXml, 42));
#else
      std::auto_ptr<ossimTerraSarXSarSensorModel> projection
         (new ossimplugins::ossimTerraSarXSarSensorModel());
      projection->readAnnotationFile(annotationXml, geoXml);
#endif

      if (!projection.get()) {
         throw std::runtime_error("Cannot read annotation file ("+annotationXml+"). Cannot create a projection from it.");
      }

      ossimTerraSarXSarSensorModel * sensor = dynamic_cast<ossimTerraSarXSarSensorModel*>(projection.get());
      if (!sensor) {
         throw std::runtime_error(
               "Unlike Expectations, the annotation file ("+annotationXml+") is not a Terra SAR Annotation File");
      }
#if 1
      if (verbose) {
         sensor->activateVerboseMode();
      }
#else
      if (verbose)
      {
         sensor->print(std::clog);
         sensor->activateVerboseMode();
      }

      ossimKeywordlist kwl;
      sensor->saveState(kwl, "S1.");
      sensor->loadState(kwl, "S1.");
      if (verbose) {
         sensor->print(std::clog);
      }
#endif

      const bool validate = inverse == inv
         ? sensor->autovalidateInverseModelFromGCPs(1,1.1,500,0.0000000001);
         : sensor->autovalidateForwardModelFromGCPs()
         ;

      std::cout << "Validate: " << validate << "\n";
      return validate ? EXIT_SUCCESS : EXIT_FAILURE;
   } catch (std::exception const& e) {
      std::cerr << "Error: " << e.what() << "\n";
   }
   return EXIT_FAILURE;

  return EXIT_FAILURE;
}
