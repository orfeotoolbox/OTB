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
#include "otbWrapperQtWidgetParameterBase.h"

namespace otb
{
namespace Wrapper
{

QtWidgetParameterBase::QtWidgetParameterBase(QtWidgetModel* m)
  : m_Model(m)
{
}

QtWidgetParameterBase::~QtWidgetParameterBase()
{
}

void QtWidgetParameterBase::ParameterChanged(const QString& key)
{

}

QtWidgetModel* QtWidgetParameterBase::GetModel()
{
  return m_Model;
}

}

}
