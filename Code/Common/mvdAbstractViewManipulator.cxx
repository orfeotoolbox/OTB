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
#include "mvdAbstractViewManipulator.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)

namespace mvd
{
/*
  TRANSLATOR mvd::AbstractViewManipulator

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
AbstractViewManipulator
::AbstractViewManipulator( QObject* parent ) :
  QObject( parent ),
  m_NavigationContext(),
  m_MouseContext(),
  m_IsotropicZoom( 1.0 )
{
  m_ViewportOrigin.Fill(0);
  m_Spacing.Fill(1.);
}

/*******************************************************************************/
AbstractViewManipulator
::~AbstractViewManipulator()
{
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

/*******************************************************************************/
void
AbstractViewManipulator
::OnViewportOriginChanged(const IndexType& origin)
{
  m_ViewportOrigin = origin;
}

/*******************************************************************************/
void
AbstractViewManipulator
::SetSpacing(const SpacingType& spacing)
{
  m_Spacing = spacing;
}

/*******************************************************************************/
void
AbstractViewManipulator
::SetOrigin(const PointType& origin)
{
  m_Origin = origin;
}
} // end namespace 'mvd'
