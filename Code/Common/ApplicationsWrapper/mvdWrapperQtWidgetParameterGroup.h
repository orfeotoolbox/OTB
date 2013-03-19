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
#ifndef __mvdWrapperQtWidgetParameterGroup_h
#define __mvdWrapperQtWidgetParameterGroup_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"

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
#include "otbWrapperParameterGroup.h"
#include "otbWrapperNumericalParameter.h"
#include "otbWrapperRadiusParameter.h"
#include "otbWrapperRAMParameter.h"
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


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class ApplicationLauncher
 *
 * \brief WIP.
 */

class QtWidgetParameterGroup : public otb::Wrapper::QtWidgetParameterBase
{
    /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:

  /** \brief Constructor. */
  QtWidgetParameterGroup(otb::Wrapper::ParameterGroup::Pointer, otb::Wrapper::QtWidgetModel*);

  /** \brief Destructor. */
  virtual ~QtWidgetParameterGroup();

public slots:
  virtual void SetActivationState( bool value );

private:
  QtWidgetParameterGroup(const QtWidgetParameterGroup&); //purposely not implemented
  void operator=(const QtWidgetParameterGroup&); //purposely not implemented

  void DoCreateWidget();

  virtual void DoUpdateGUI();

  virtual void ProcessChild(otb::Wrapper::Parameter * currentNode, bool status);

  otb::Wrapper::ParameterGroup::Pointer m_ParamList;

  typedef std::vector<otb::Wrapper::QtWidgetParameterBase*> WidgetListType;
  typedef WidgetListType::iterator WidgetListIteratorType;
  WidgetListType m_WidgetList;

};


}
}

#endif
