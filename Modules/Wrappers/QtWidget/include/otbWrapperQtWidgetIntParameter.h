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
#ifndef __otbWrapperQtWidgetIntParameter_h
#define __otbWrapperQtWidgetIntParameter_h

#include <QtGui>
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "otbWrapperNumericalParameter.h"
#include "otbWrapperQtWidgetParameterBase.h"
#endif //tag=QT4-boost-compatibility


namespace otb
{
namespace Wrapper
{

/** \class QtWidgetIntParameter
 * \brief
 *
 * \ingroup OTBQtWidget
 */
class ITK_ABI_EXPORT QtWidgetIntParameter : public QtWidgetParameterBase
{
  Q_OBJECT
public:
  QtWidgetIntParameter(IntParameter*, QtWidgetModel*);
  virtual ~QtWidgetIntParameter();

protected slots:
  void SetValue( int value );

private:
  QtWidgetIntParameter(const QtWidgetIntParameter&); //purposely not implemented
  void operator=(const QtWidgetIntParameter&); //purposely not implemented

  virtual void DoCreateWidget();

  virtual void DoUpdateGUI();

  QHBoxLayout *         m_QHBoxLayout;
  QSpinBox *            m_QSpinBox;

  IntParameter::Pointer m_IntParam;
};


}
}

#endif
