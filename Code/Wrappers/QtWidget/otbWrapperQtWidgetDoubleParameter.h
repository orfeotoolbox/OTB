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
#ifndef __otbWrapperQtWidgetDoubleParameter_h
#define __otbWrapperQtWidgetDoubleParameter_h

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
class QtWidgetDoubleParameter : public QtWidgetParameterBase
{
  Q_OBJECT
public:
  QtWidgetDoubleParameter(DoubleParameter*, QtWidgetModel*);
  virtual ~QtWidgetDoubleParameter();

protected slots:
  void SetValue( double value );

private:
  QtWidgetDoubleParameter(const QtWidgetDoubleParameter&); //purposely not implemented
  void operator=(const QtWidgetDoubleParameter&); //purposely not implemented

  virtual void DoCreateWidget();

  virtual void DoUpdateGUI();

  QHBoxLayout *           m_QHBoxLayout;
  QDoubleSpinBox *        m_QDoubleSpinBox;

  DoubleParameter::Pointer m_DoubleParam;
};


}
}

#endif
