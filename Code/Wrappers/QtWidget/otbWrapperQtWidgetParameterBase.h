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
#ifndef __otbWrapperQtWidgetParameterBase_h
#define __otbWrapperQtWidgetParameterBase_h

#include <QtGui>
#include "otbWrapperParameter.h"
#include "otbWrapperQtWidgetModel.h"

namespace otb
{
namespace Wrapper
{

/** \class
 * \brief
 */
class QtWidgetParameterBase : public QWidget
{
  Q_OBJECT
public:
  QtWidgetParameterBase(QtWidgetModel*);
  virtual ~QtWidgetParameterBase();

protected slots:
  void ParameterChanged(const QString& key);

protected:
  QtWidgetModel* GetModel();

private:
  QtWidgetParameterBase(const QtWidgetParameterBase&); //purposely not implemented
  void operator=(const QtWidgetParameterBase&); //purposely not implemented

  QtWidgetModel* m_Model;
};


}
}

#endif
