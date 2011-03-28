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
#include "otbWrapperParameter.h"
#include "otbWrapperNumericalParameter.h"

namespace otb
{
namespace Wrapper
{

/** \class
 * \brief
 */
class QtWidgetIntParameter : public QWidget
{
  Q_OBJECT
public:
  QtWidgetIntParameter(IntParameter*);
  virtual ~QtWidgetIntParameter();

private:
  QtWidgetIntParameter(const QtWidgetIntParameter&); //purposely not implemented
  void operator=(const QtWidgetIntParameter&); //purposely not implemented

  IntParameter::Pointer m_IntParam;
};


}
}

#endif
