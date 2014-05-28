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
#ifndef __otbWrapperQtWidgetFloatParameter_h
#define __otbWrapperQtWidgetFloatParameter_h

#include <QtGui>
#include "otbWrapperNumericalParameter.h"
#include "otbWrapperQtWidgetParameterBase.h"

namespace otb
{
namespace Wrapper
{

/** \class
 * \brief
 */
class ITK_ABI_EXPORT QtWidgetFloatParameter : public QtWidgetParameterBase
{
  Q_OBJECT
public:
  QtWidgetFloatParameter(FloatParameter*, QtWidgetModel*);
  virtual ~QtWidgetFloatParameter();

protected slots:
  void SetValue( double value );

private:
  QtWidgetFloatParameter(const QtWidgetFloatParameter&); //purposely not implemented
  void operator=(const QtWidgetFloatParameter&); //purposely not implemented

  virtual void DoCreateWidget();

  virtual void DoUpdateGUI();

  QHBoxLayout *           m_QHBoxLayout;
  QDoubleSpinBox *        m_QDoubleSpinBox;

  FloatParameter::Pointer m_FloatParam;
};


}
}

#endif
