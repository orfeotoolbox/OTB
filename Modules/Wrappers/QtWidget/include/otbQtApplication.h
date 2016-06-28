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

#ifndef otbQtApplication_h
#define otbQtApplication_h

#include <QtGui>
#include <QString>
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "itkMacro.h"
#endif //tag=QT4-boost-compatibility
namespace otb
{
namespace Wrapper
{
class ITK_ABI_EXPORT QtApplication : public QApplication
{
  Q_OBJECT

public:
  QtApplication(int& argc, char** argv);

  bool notify(QObject *object, QEvent* event) ITK_OVERRIDE;

signals:

  void UnhandledException(QString message);
};

} //end of namespace Wrapper
} //end of namepsace otb

#endif
