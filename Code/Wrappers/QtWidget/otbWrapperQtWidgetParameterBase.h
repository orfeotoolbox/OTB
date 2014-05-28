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
class ITK_ABI_EXPORT QtWidgetParameterBase : public QWidget
{
  Q_OBJECT
public:
  QtWidgetParameterBase(Parameter *, QtWidgetModel*);
  virtual ~QtWidgetParameterBase();

  void CreateWidget();

public slots:
  void UpdateGUI();
  virtual void SetActivationState( bool value );
  void Reset();

protected slots:
  void ParameterChanged(const QString& key);

signals:
  void ParameterActiveStatus(bool value);

protected:
  QtWidgetModel* GetModel();


private:
  QtWidgetParameterBase(const QtWidgetParameterBase&); //purposely not implemented
  void operator=(const QtWidgetParameterBase&); //purposely not implemented

  virtual void DoUpdateGUI() = 0;

  virtual void DoCreateWidget() = 0;

  QtWidgetModel* m_Model;

  Parameter*      m_Param;
};


}
}

#endif
