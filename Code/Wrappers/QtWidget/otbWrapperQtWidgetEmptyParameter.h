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
#ifndef __otbWrapperQtWidgetEmptyParameter_h
#define __otbWrapperQtWidgetEmptyParameter_h

#include <QtGui>
#include "otbWrapperEmptyParameter.h"
#include "otbWrapperQtWidgetParameterBase.h"

namespace otb
{
namespace Wrapper
{

/** \class QtWidgetEmptyParameter
 * \brief
 */
class ITK_ABI_EXPORT QtWidgetEmptyParameter : public QtWidgetParameterBase
{
  Q_OBJECT

public:
  QtWidgetEmptyParameter(EmptyParameter*, QtWidgetModel*);
  virtual ~QtWidgetEmptyParameter();

private:
  QtWidgetEmptyParameter(const QtWidgetEmptyParameter&); //purposely not implemented
  void operator=(const QtWidgetEmptyParameter&); //purposely not implemented

  virtual void DoCreateWidget();

  virtual void DoUpdateGUI();
};


}
}

#endif
