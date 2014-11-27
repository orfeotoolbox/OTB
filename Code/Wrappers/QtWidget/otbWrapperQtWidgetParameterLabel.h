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
#ifndef __otbWrapperQtWidgetParameterLabel_h
#define __otbWrapperQtWidgetParameterLabel_h

#include <QtGui>
#include "otbWrapperParameter.h"

namespace otb
{
namespace Wrapper
{

/** \class QtWidgetParameterLabel
 * \brief
 */
class ITK_ABI_EXPORT QtWidgetParameterLabel : public QWidget
{
  Q_OBJECT
public:
  QtWidgetParameterLabel(Parameter*);
  virtual ~QtWidgetParameterLabel();

private:
  QtWidgetParameterLabel(const QtWidgetParameterLabel&); //purposely not implemented
  void operator=(const QtWidgetParameterLabel&); //purposely not implemented
};


}
}

#endif
