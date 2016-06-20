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

#define private public
#include "ossimSentinel1Model.h"
#include "ossimPluginProjectionFactory.h"
#include <iostream>
#include <memory>
#include <map>
#include <string>

using namespace ossimplugins;
enum Inverse { forward, inv, undef};

int main(int argc, char * argv[])
{
   std::cout.precision(9);

   string annotationXml;
   Inverse inverse = undef;
   bool verbose;

   for (int i=1; i!=argc ; ++i) {
      std::string const a(argv[i]);
      if (a == "-v" || a == "-verbose") {
         verbose = true;
      } else if (inverse == undef)  {
         inverse = atoi(argv[i]) == 1 ? inv : forward;
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

   try {
      std::auto_ptr<ossimProjection> projection
         (ossimPluginProjectionFactory::instance()->createProjection(annotationXml, 42));
      if (!projection.get()) {
         throw std::runtime_error("Cannot read annotation file ("+annotationXml+"). Cannot create a projection from it.");
      }

      ossimSentinel1Model * sensor = dynamic_cast<ossimSentinel1Model*>(projection.get());
      if (!sensor) {
         throw std::runtime_error(
               "Unlike Expectations, the annotation file ("+annotationXml+") is not a Sentinel Annotation File");
      }

      if (verbose)
      {
         std::map<std::string, std::string> const& kwl = sensor->theProductKwl.getMap();
         for (std::map<std::string, std::string>::const_iterator b = kwl.begin(), e = kwl.end()
               ; b != e
               ; ++b
             )
         {
            std::cout << "kwl["<<b->first<<"] -> " << b->second << "\n";
         }
      }

      ossimKeywordlist kwl;
      sensor->saveState(kwl, "S1.");
      sensor->loadState(kwl, "S1.");
      if (verbose) {
         sensor->print(std::cout);
      }

      const bool validate = inverse == inv
         ? sensor->autovalidateInverseModelFromGCPs()
         : sensor->autovalidateForwardModelFromGCPs()
         ;

      std::cout << "Validate: " << validate << "\n";
      return validate ? EXIT_SUCCESS : EXIT_FAILURE;
   } catch (std::exception const& e) {
      std::cerr << "Error: " << e.what() << "\n";
   }
   return EXIT_FAILURE;
}
