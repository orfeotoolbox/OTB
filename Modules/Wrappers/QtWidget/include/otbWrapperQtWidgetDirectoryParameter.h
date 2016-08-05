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
#ifndef otbWrapperQtWidgetDirectoryParameter_h
#define otbWrapperQtWidgetDirectoryParameter_h

#include <QtGui>
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "otbWrapperDirectoryParameter.h"
#include "otbWrapperQtWidgetParameterBase.h"
#endif //tag=QT4-boost-compatibility


namespace otb
{
namespace Wrapper
{

/** \class QtWidgetDirectoryParameter
 * \brief
 *
 * \ingroup OTBQtWidget
 */
class ITK_ABI_EXPORT QtWidgetDirectoryParameter : public QtWidgetParameterBase
{
  Q_OBJECT
public:
  QtWidgetDirectoryParameter(DirectoryParameter*, QtWidgetModel*);
  ~QtWidgetDirectoryParameter() ITK_OVERRIDE;

protected slots:
  void SetFileName( const QString& value );
  void SelectFile();

private:
  QtWidgetDirectoryParameter(const QtWidgetDirectoryParameter&); //purposely not implemented
  void operator=(const QtWidgetDirectoryParameter&); //purposely not implemented

  void DoCreateWidget() ITK_OVERRIDE;

  void DoUpdateGUI() ITK_OVERRIDE;


  DirectoryParameter::Pointer m_DirectoryParam;

  QHBoxLayout * m_HLayout;
  QLineEdit*    m_Input;
  QPushButton * m_Button;
};


}
}

#endif
