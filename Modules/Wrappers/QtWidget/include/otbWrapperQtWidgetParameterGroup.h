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
#ifndef __otbWrapperQtWidgetParameterGroup_h
#define __otbWrapperQtWidgetParameterGroup_h

#include <QtGui>
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "otbWrapperParameterGroup.h"
#include "otbWrapperRadiusParameter.h"
#include "otbWrapperRAMParameter.h"
#include "otbWrapperQtWidgetParameterBase.h"
#include "otbWrapperInputProcessXMLParameter.h"
#include "otbWrapperOutputProcessXMLParameter.h"
#endif //tag=QT4-boost-compatibility

namespace otb
{
namespace Wrapper
{

/** \class QtWidgetParameterGroup
 * \brief
 *
 * \ingroup OTBQtWidget
 */
class ITK_ABI_EXPORT QtWidgetParameterGroup : public QtWidgetParameterBase
{
  Q_OBJECT
public:
  QtWidgetParameterGroup(ParameterGroup::Pointer, QtWidgetModel*);
  virtual ~QtWidgetParameterGroup();

public slots:
  virtual void SetActivationState( bool value );

private:
  QtWidgetParameterGroup(const QtWidgetParameterGroup&); //purposely not implemented
  void operator=(const QtWidgetParameterGroup&); //purposely not implemented

  void DoCreateWidget();

  virtual void DoUpdateGUI();

  virtual void ProcessChild(Parameter * currentNode, bool status);

  ParameterGroup::Pointer m_ParamList;

  typedef std::vector<QtWidgetParameterBase*> WidgetListType;
  typedef WidgetListType::iterator WidgetListIteratorType;
  WidgetListType m_WidgetList;

};


}
}

#endif
