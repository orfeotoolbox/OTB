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
#ifndef otbWrapperQtWidgetParameterBase_h
#define otbWrapperQtWidgetParameterBase_h

#include <QtGui>
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "otbWrapperParameter.h"
#include "otbWrapperQtWidgetModel.h"
#endif //tag=QT4-boost-compatibility
#include "OTBQtWidgetExport.h"

namespace otb
{
namespace Wrapper
{

/** \class QtWidgetParameterBase
 * \brief
 *
 * \ingroup OTBQtWidget
 */
class OTBQtWidget_EXPORT QtWidgetParameterBase : public QWidget
{
  Q_OBJECT
public:
  QtWidgetParameterBase(Parameter *, QtWidgetModel*);
  ~QtWidgetParameterBase() ITK_OVERRIDE;

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
