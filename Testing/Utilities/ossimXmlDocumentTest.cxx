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

#include <iostream>
#include <fstream>
#include <cstdlib>

#include "ossim/base/ossimXmlDocument.h"
#include "itkMacro.h"
// this is the most important class and is called as the first line of all applications.
// without this alll the important factories are not created.
#include "ossim/init/ossimInit.h"

int ossimXmlDocumentTest(int itkNotUsed(argc), char* argv[])
{
   ossimXmlDocument* xdoc = new ossimXmlDocument();
   bool result = xdoc->openFile(argv[1]);
   delete xdoc;

   if(!result)
   {
        return EXIT_FAILURE;
   }
   else
   {
        return EXIT_SUCCESS;
   }

}
