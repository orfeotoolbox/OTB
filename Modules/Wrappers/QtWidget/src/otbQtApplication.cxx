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
#include "otbQtApplication.h"
#include <QObject>
#include <iostream>
namespace otb
{
namespace Wrapper
{
QtApplication::QtApplication(int& argc, char** argv)
  : QApplication(argc, argv)
{

}

bool
QtApplication::notify(QObject *object, QEvent* event)
  {
    try
      {
      return QApplication::notify(object, event);
      }
    catch (std::exception &ex)
      {
      QString message = QString("<center><font color=\"#FF0000\">");
      message.append(ex.what());
      message.append("</font></center>");
      emit UnhandledException(message);
      }
    return false;
  }

} //end of namespace Wrapper
} //end of namespace otb
