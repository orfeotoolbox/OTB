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
#ifndef __otbWrapperQtWidgetRAMParameter_h
#define __otbWrapperQtWidgetRAMParameter_h

#include <QtGui>
#include "otbWrapperRAMParameter.h"
#include "otbWrapperQtWidgetParameterBase.h"


namespace otb
{
namespace Wrapper
{

/** \class QtWidgetRAMParameter
 * \brief
 */
class ITK_ABI_EXPORT QtWidgetRAMParameter : public QtWidgetParameterBase
{
  Q_OBJECT
public:
  QtWidgetRAMParameter(RAMParameter*, QtWidgetModel*);
  virtual ~QtWidgetRAMParameter();

protected slots:
  void SetValue( int value );

private:
  QtWidgetRAMParameter(const QtWidgetRAMParameter&); //purposely not implemented
  void operator=(const QtWidgetRAMParameter&); //purposely not implemented

  virtual void DoCreateWidget();

  virtual void DoUpdateGUI();

  QHBoxLayout *         m_QHBoxLayout;
  QSpinBox *            m_QSpinBox;

  RAMParameter::Pointer m_RAMParam;
};


}
}

#endif
