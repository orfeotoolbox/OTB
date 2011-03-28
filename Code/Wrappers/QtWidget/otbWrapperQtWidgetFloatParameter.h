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
#include "otbWrapperParameter.h"
#include "otbWrapperNumericalParameter.h"

namespace otb
{
namespace Wrapper
{

/** \class
 * \brief
 */
class QtWidgetFloatParameter : public QWidget
{
  Q_OBJECT
public:
  QtWidgetFloatParameter(FloatParameter*);
  virtual ~QtWidgetFloatParameter();

private:
  QtWidgetFloatParameter(const QtWidgetFloatParameter&); //purposely not implemented
  void operator=(const QtWidgetFloatParameter&); //purposely not implemented

};


}
}

#endif
