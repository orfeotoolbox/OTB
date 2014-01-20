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
#ifndef __otbWrapperQtWidgetInputVectorDataParameter_h
#define __otbWrapperQtWidgetInputVectorDataParameter_h

#include <QtGui>
#include "otbWrapperInputVectorDataParameter.h"
#include "otbWrapperQtWidgetParameterBase.h"


namespace otb
{
namespace Wrapper
{

/** \class
 * \brief
 */
class QtWidgetInputVectorDataParameter : public QtWidgetParameterBase
{
  Q_OBJECT
public:
  QtWidgetInputVectorDataParameter(InputVectorDataParameter*, QtWidgetModel*);
  virtual ~QtWidgetInputVectorDataParameter();

  inline const QLineEdit* GetInput() const;
  inline QLineEdit* GetInput();

protected slots:
  bool SetFileName( const QString& value );
  void SelectFile();

private:
  QtWidgetInputVectorDataParameter(const QtWidgetInputVectorDataParameter&); //purposely not implemented
  void operator=(const QtWidgetInputVectorDataParameter&); //purposely not implemented

  virtual void DoCreateWidget();

  virtual void DoUpdateGUI();


  InputVectorDataParameter::Pointer m_InputVectorDataParam;

  QHBoxLayout * m_HLayout;
  QLineEdit*    m_Input;
  QPushButton * m_Button;
};


inline
const QLineEdit*
QtWidgetInputVectorDataParameter
::GetInput() const
{
  return m_Input;
}

inline
QLineEdit*
QtWidgetInputVectorDataParameter
::GetInput()
{
  return m_Input;
}

} // Wrapper

} // otb

#endif
