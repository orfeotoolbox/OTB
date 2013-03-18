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
//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

// Monteverdi includes (sorted by alphabetic order)
#include "ApplicationsWrapper/mvdApplicationsBrowser.h"

int mvdApplicationsBrowserTest(int argc, char* argv[])
{ 
  //
  // instanciate applications browser
  mvd::ApplicationsBrowser *  browser = new mvd::ApplicationsBrowser();

  //
  // is search path set 
  if (argc > 1)
    {
    browser->SetAutoLoadPath(argv[1]);
    }

  //
  // print available applications or exit with failure
  if ( browser->GetAvailableApplications().size() == 0 )
    return EXIT_FAILURE;

  // return ok
  return EXIT_SUCCESS;
}
