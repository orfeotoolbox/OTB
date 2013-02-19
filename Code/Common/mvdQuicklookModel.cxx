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
#include "mvdQuicklookModel.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)
#include "itksys/SystemTools.hxx"

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdAlgorithm.h"


namespace mvd
{
/*
  TRANSLATOR mvd::QuicklookModel

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
QuicklookModel
::QuicklookModel( QObject* parent ) :
  VectorImageModel( parent )
{
}

/*******************************************************************************/
QuicklookModel
::~QuicklookModel()
{
}

/*******************************************************************************/
void
QuicklookModel
::virtual_BuildModel()
{
 //
  // Step #1: Perform pre-process of AbstractModel::BuildModel()
  // pattern.

  //
  // Step #2: Perform standard AbstractModel::BuildModel()
  // pattern. Call parent virtual method.

  // Empty step #2 in case of Quicklook model.

  //
  // Step #3: Post-process of the BuildModel() pattern.

  SetFilename(
    qobject_cast< VectorImageModel* >( parent() )->GetFilename(),
    50, 50
  );

  // Initialize RgbaImageModel.
  InitializeRgbaPipeline();
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
