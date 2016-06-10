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
#ifndef __otbWrapperQtWidgetComplexInputImageParameter_h
#define __otbWrapperQtWidgetComplexInputImageParameter_h

#include <QtGui>
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "otbWrapperComplexInputImageParameter.h"
#include "otbWrapperQtWidgetParameterBase.h"
#endif //tag=QT4-boost-compatibility


namespace otb
{
namespace Wrapper
{

/** \class QtWidgetComplexInputImageParameter
 * \brief
 *
 * \ingroup OTBQtWidget
 */
class ITK_ABI_EXPORT QtWidgetComplexInputImageParameter : public QtWidgetParameterBase
{
  Q_OBJECT
public:
  QtWidgetComplexInputImageParameter(ComplexInputImageParameter*, QtWidgetModel*);
  ~QtWidgetComplexInputImageParameter() ITK_OVERRIDE;

protected slots:
  bool SetFileName( const QString& value );
  void SelectFile();

private:
  QtWidgetComplexInputImageParameter(const QtWidgetComplexInputImageParameter&); //purposely not implemented
  void operator=(const QtWidgetComplexInputImageParameter&); //purposely not implemented

  void DoCreateWidget() ITK_OVERRIDE;

  void DoUpdateGUI() ITK_OVERRIDE;


  ComplexInputImageParameter::Pointer m_ComplexInputImageParam;

  QHBoxLayout * m_HLayout;
  QLineEdit*    m_Input;
  QPushButton * m_Button;
};


}
}

#endif
