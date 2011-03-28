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
#ifndef __otbWrapperQtEmptyWidgetFactory_h
#define __otbWrapperQtEmptyWidgetFactory_h

#include <QtGui>
#include "otbWrapperParameter.h"
#include "otbWrapperEmptyParameter.h"

namespace otb
{
namespace Wrapper
{

/** \class
 * \brief
 */
class QtWidgetEmptyParameter : public QWidget
{
  Q_OBJECT

public:
  QtWidgetEmptyParameter(EmptyParameter*);
  virtual ~QtWidgetEmptyParameter();

private:
  QtWidgetEmptyParameter(const QtWidgetEmptyParameter&); //purposely not implemented
  void operator=(const QtWidgetEmptyParameter&); //purposely not implemented

};


}
}

#endif
