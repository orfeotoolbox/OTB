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
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "otbWrapperEmptyParameter.h"
#include "otbWrapperQtWidgetParameterBase.h"
#endif //tag=QT4-boost-compatibility

namespace otb
{
namespace Wrapper
{

/** \class QtWidgetEmptyParameter
 * \brief
 *
 * \ingroup OTBQtWidget
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
