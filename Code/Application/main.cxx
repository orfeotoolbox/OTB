/*=========================================================================

  Program:   Monteverdi2
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

//#include "ConfigureMonteverdi.h"

//
// System includes (sorted by alphabetic order)
#include <cstdlib>

//
// Qt includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdApplication.h"
#include "mvdMainWindow.h"

//
// Main functions definitions.
//

//
// MAIN
//
int
main( int argc, char* argv[] )
{
  mvd::Application application( argc, argv );
  mvd::MainWindow main_window;

  main_window.show();

  return application.exec();
}

//
// Main function implementation.
//
