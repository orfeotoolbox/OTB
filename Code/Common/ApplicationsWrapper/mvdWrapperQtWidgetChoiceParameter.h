/*=========================================================================

  Program:   Monteverdi2
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi2 is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __mvdWrapperQtWidgetChoiceParameter_h
#define __mvdWrapperQtWidgetChoiceParameter_h

/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtCore>

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)
#include "otbWrapperChoiceParameter.h"
#include "otbWrapperParameterGroup.h"
#include "otbWrapperQtWidgetParameterBase.h"

//
// Monteverdi includes (sorted by alphabetic order)

/*****************************************************************************/
/* PRE-DECLARATION SECTION                                                   */

//
// External classes pre-declaration.
namespace
{
}

namespace mvd
{
namespace Wrapper
{

/** \class
 * \brief
 */
class QtWidgetChoiceParameter : public otb::Wrapper::QtWidgetParameterBase
{
   /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:
   /** \brief Constructor. */
  QtWidgetChoiceParameter(otb::Wrapper::ChoiceParameter*, otb::Wrapper::QtWidgetModel*);

  /** \brief Destructor. */
  virtual ~QtWidgetChoiceParameter();

protected slots:
  void SetValue( int value );

private:
  QtWidgetChoiceParameter(const QtWidgetChoiceParameter&); //purposely not implemented
  void operator=(const QtWidgetChoiceParameter&); //purposely not implemented

  virtual void DoCreateWidget();

  virtual void DoUpdateGUI();

  otb::Wrapper::ChoiceParameter::Pointer m_ChoiceParam;

  QHBoxLayout*    m_MainHLayout;

  QComboBox*      m_ComboBox;
  QStackedWidget* m_StackWidget;

  QVBoxLayout*    m_VLayout;
  QGroupBox*      m_VLayoutGroup;

  typedef std::vector<otb::Wrapper::QtWidgetParameterBase*> WidgetListType;
  typedef WidgetListType::iterator WidgetListIteratorType;
  WidgetListType m_WidgetList;
};

}
}

#endif
