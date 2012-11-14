/*=========================================================================

  Program:   Monteverdi2
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved .
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "mvdApplication.h"

//
// System includes (sorted by alphabetic order)
#include <iostream>

//
// Qt includes (sorted by alphabetic order)
#include <QApplication>

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)

//
// Class pre-declaration.
//
// Class implementation.
namespace mvd
{

Application
::Application( int& argc, char** argv ) :
  QApplication( argc, argv )
{
  QObject::connect(
    this, SIGNAL( aboutToQuit() ),
    this, SLOT( OnAboutToQuit() )
    );
}

Application
::~Application()
{
}

//
// SLOTS
//

void
Application
::OnAboutToQuit()
{
  qDebug( ">DEBUG< mvd::Application::OnAboutToQuit()" );
  qWarning( ">WARNING< mvd::Application::OnAboutToQuit()" );
  std::cout << ">info< mvd::Application::OnAboutToQuit()" << std::endl;
}

} // end namespace 'mvd'
