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
#ifndef otbWrapperQtWidgetRAMParameter_h
#define otbWrapperQtWidgetRAMParameter_h

#include <QtGui>
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "otbWrapperRAMParameter.h"
#include "otbWrapperQtWidgetParameterBase.h"
#endif //tag=QT4-boost-compatibility


namespace otb
{
namespace Wrapper
{

/** \class QtWidgetRAMParameter
 * \brief
 *
 * \ingroup OTBQtWidget
 */
class ITK_ABI_EXPORT QtWidgetRAMParameter : public QtWidgetParameterBase
{
  Q_OBJECT
public:
  QtWidgetRAMParameter(RAMParameter*, QtWidgetModel*);
  ~QtWidgetRAMParameter() ITK_OVERRIDE;

protected slots:
  void SetValue( int value );

private:
  QtWidgetRAMParameter(const QtWidgetRAMParameter&); //purposely not implemented
  void operator=(const QtWidgetRAMParameter&); //purposely not implemented

  void DoCreateWidget() ITK_OVERRIDE;

  void DoUpdateGUI() ITK_OVERRIDE;

  QHBoxLayout *         m_QHBoxLayout;
  QSpinBox *            m_QSpinBox;

  RAMParameter::Pointer m_RAMParam;
};


}
}

#endif
