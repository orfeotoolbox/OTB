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

#include "itkMacro.h"
#include "ossim/elevation/ossimElevManager.h"

// this is the most important class and is called as the first line of all applications.
// without this alll the important factories are not created.
#include "ossim/init/ossimInit.h"

int ossimElevManagerTest(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{
   ossimElevManager * ElevManager(NULL);
   ElevManager=ossimElevManager::instance();

   if(ElevManager==NULL)
   {
        return EXIT_FAILURE;
   }
   else
   {
        return EXIT_SUCCESS;
   }

}
