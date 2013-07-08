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
#include "Gui/mvdDatasetCreationProgressDialog.h"
#include "Gui/ui_mvdDatasetCreationProgressDialog.h"

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
  TRANSLATOR mvd::DatasetCreationProgressDialog

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/

/*******************************************************************************/
DatasetCreationProgressDialog
::DatasetCreationProgressDialog( QWidget* parent ) :
  QDialog( parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint ),
  m_UI( new mvd::Ui::DatasetCreationProgressDialog() )
{
  m_UI->setupUi( this );
}

/*******************************************************************************/
DatasetCreationProgressDialog
::~DatasetCreationProgressDialog()
{
  delete m_UI;
  m_UI = NULL;
}


/*****************************************************************************/
QString
DatasetCreationProgressDialog
::GetImage( ) const
{
  return m_UI->imageLabel->text();
}

/*****************************************************************************/
void
DatasetCreationProgressDialog
::SetImage(QString value)
{
  m_UI->imageLabel->setText(value);
}


/*****************************************************************************/
QString
DatasetCreationProgressDialog
::GetDataset( ) const
{
  return m_UI->datasetLabel->text();
}

/*****************************************************************************/
void
DatasetCreationProgressDialog
::SetDataset(QString value)
{
  m_UI->datasetLabel->setText(value);
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

/*******************************************************************************/

} // end namespace 'mvd'
