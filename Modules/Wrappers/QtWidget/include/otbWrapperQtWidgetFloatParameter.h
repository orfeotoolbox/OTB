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
#ifndef otbWrapperQtWidgetFloatParameter_h
#define otbWrapperQtWidgetFloatParameter_h

#include <QtGui>
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "otbWrapperNumericalParameter.h"
#include "otbWrapperQtWidgetParameterBase.h"
#endif //tag=QT4-boost-compatibility

namespace otb
{
namespace Wrapper
{

/** \class QtWidgetFloatParameter
 * \brief
 *
 * \ingroup OTBQtWidget
 */
class OTBQtWidget_EXPORT QtWidgetFloatParameter : public QtWidgetParameterBase
{
  Q_OBJECT
public:
  QtWidgetFloatParameter(FloatParameter*, QtWidgetModel*);
  ~QtWidgetFloatParameter() ITK_OVERRIDE;

protected slots:
  void SetValue( double value );

private:
  QtWidgetFloatParameter(const QtWidgetFloatParameter&); //purposely not implemented
  void operator=(const QtWidgetFloatParameter&); //purposely not implemented

  void DoCreateWidget() ITK_OVERRIDE;

  void DoUpdateGUI() ITK_OVERRIDE;

  QHBoxLayout *           m_QHBoxLayout;
  QDoubleSpinBox *        m_QDoubleSpinBox;

  FloatParameter::Pointer m_FloatParam;
};


}
}

#endif
