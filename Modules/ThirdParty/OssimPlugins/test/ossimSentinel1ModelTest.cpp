/*
 * Copyright (C) 2005-2019 by Centre National d'Etudes Spatiales (CNES)
 *
 * This file is licensed under MIT license:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#if defined(__GNUC__) || defined(__clang__)
# pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wunused-parameter"
#   pragma GCC diagnostic ignored "-Woverloaded-virtual"
#   pragma GCC diagnostic ignored "-Wshadow"
#include "ossimSentinel1Model.h"
#include "ossimPluginProjectionFactory.h"
# pragma GCC diagnostic pop
#else
#include "ossimSentinel1Model.h"
#include "ossimPluginProjectionFactory.h"
#endif

#include <iostream>
#include <memory>
#include <map>
#include <string>

using namespace ossimplugins;
enum Inverse { fwd, inv, undef};

int main(int argc, char * argv[])
{
   std::clog.precision(20);

   std::string annotationXml;
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
         sensor->print(std::clog);
      }

      ossimKeywordlist kwl;
      sensor->saveState(kwl, "S1.");
      sensor->loadState(kwl, "S1.");
      if (verbose) {
         sensor->print(std::clog);
      }

      
      bool validate = inverse == inv
         ? sensor->autovalidateInverseModelFromGCPs()
         : sensor->autovalidateForwardModelFromGCPs()
         ;
      
      std::vector<std::pair<unsigned long, unsigned long> > deburstLines;
      std::pair<unsigned long, unsigned long> deburstSamples;

      std::cout<<"Trying to deburst data ..."<<std::endl;
      bool deburstOk = sensor->deburst(deburstLines, deburstSamples);
      std::cout<<"Deburst succeed: "<<(deburstOk?"yes":"no")<<std::endl;

      if(deburstOk)
        {

        std::cout<<"Validation with deburst model"<<std::endl;
        
        validate = validate && (inverse == inv
                                ? sensor->autovalidateInverseModelFromGCPs()
                                : sensor->autovalidateForwardModelFromGCPs());
        }

      std::cout << "Validate: " << validate << "\n";
      return validate ? EXIT_SUCCESS : EXIT_FAILURE;
   } catch (std::exception const& e) {
      std::cerr << "Error: " << e.what() << "\n";
   }
   return EXIT_FAILURE;
}
