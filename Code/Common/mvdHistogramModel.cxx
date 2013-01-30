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
#include "mvdHistogramModel.h"


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
#include "mvdVectorImageModel.h"

namespace mvd
{
/*
  TRANSLATOR mvd::HistogramModel

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
HistogramModel
::HistogramModel( QObject* parent ) :
  AbstractModel( parent ),
  m_Histograms(),
  m_MinPixel(),
  m_MaxPixel()
{
}

/*******************************************************************************/
HistogramModel
::~HistogramModel()
{
}

/*******************************************************************************/
void
HistogramModel
::virtual_BuildModel()
{
  // template_BuildModel_I< VectorImageModel::SourceImageType >();
  template_BuildModel_M< VectorImageModel >();

  // template_BuildModel< otb::Image< FixedArray< double, 4 >, 2 > >();
  // template_BuildModel< otb::Image< itk::FixedArray< float, 4 >, 2 > >();

  /*
  template_BuildModel< otb::VectorImage< float, 2 > >();
  template_BuildModel< otb::Image< float, 2 > >();
  */
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

/*******************************************************************************/

} // end namespace 'mvd'
