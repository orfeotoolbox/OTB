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
#ifndef otbWrapperQtWidgetStringParameter_h
#define otbWrapperQtWidgetStringParameter_h

#include <QtGui>
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "otbWrapperStringParameter.h"
#include "otbWrapperQtWidgetParameterBase.h"
#endif //tag=QT4-boost-compatibility

namespace otb
{
namespace Wrapper
{

/** \class QtWidgetStringParameter
 * \brief
 *
 * \ingroup OTBQtWidget
 */
class ITK_ABI_EXPORT QtWidgetStringParameter : public QtWidgetParameterBase
{
  Q_OBJECT
public:
  QtWidgetStringParameter(StringParameter*, QtWidgetModel*);
  ~QtWidgetStringParameter() ITK_OVERRIDE;

protected slots:
  void SetValue( const QString& value );

private:
  QtWidgetStringParameter(const QtWidgetStringParameter&); //purposely not implemented
  void operator=(const QtWidgetStringParameter&); //purposely not implemented

  void DoCreateWidget() ITK_OVERRIDE;

  void DoUpdateGUI() ITK_OVERRIDE;

  StringParameter::Pointer m_StringParam;
  QHBoxLayout *            m_HLayout;
  QLineEdit*               m_Input;
};


}
}

#endif
