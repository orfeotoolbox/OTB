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

#ifndef __otbQtApplication_h
#define __otbQtApplication_h

#include <QtGui>
#include <QString>
#include "itkMacro.h"
namespace otb
{
namespace Wrapper
{
class ITK_ABI_EXPORT QtApplication : public QApplication
{
  Q_OBJECT

public:
  QtApplication(int& argc, char** argv);

  bool notify(QObject *object, QEvent* event);

signals:

  void UnhandledException(QString message);
};

} //end of namespace Wrapper
} //end of namepsace otb

#endif
