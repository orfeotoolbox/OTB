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
#ifndef __mvdDatasetCreationProgressDialog_h
#define __mvdDatasetCreationProgressDialog_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtGui>

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdGui.h"
#include "ui_mvdDatasetCreationProgressDialog.h"

//
// External classes pre-declaration.
namespace
{
}

namespace mvd
{
//
// Internal classes pre-declaration.
namespace Ui
{
class DatasetCreationProgressDialog;
}

/** \class DatasetCreationProgressDialog
 *
 */
class Monteverdi2_EXPORT DatasetCreationProgressDialog :
    public QDialog
{
  Q_OBJECT;

  Q_PROPERTY(QString image
        READ GetImage
        WRITE SetImage);

  Q_PROPERTY(QString dataset
        READ GetDataset
        WRITE SetDataset);

//
// Public methods.
public:

  /** Constructor */
  DatasetCreationProgressDialog( QWidget* parent = NULL );

  /** Destructor */
  virtual ~DatasetCreationProgressDialog();
  
  /** */
  inline QString GetImage() const;
  /** */
  inline void SetImage( QString imageName );

  /** */
  inline QString GetDataset() const;
  /** */
  inline void SetDataset( QString datasetName );

//
// SIGNALS.
signals:

//
// Protected methods.
protected:

//
// Protected attributes.
protected:

//
// Private methods.
private:


//
// Private attributes.
private:
  /**
   */
  Ui::DatasetCreationProgressDialog* m_UI;

//
// SLOTS.
private slots:

};


/*****************************************************************************/
inline
QString
DatasetCreationProgressDialog
::GetImage( ) const
{
  return m_UI->imageLabel->text();
}

/*****************************************************************************/
inline
void
DatasetCreationProgressDialog
::SetImage(QString value)
{
  m_UI->imageLabel->setText(value);
}


/*****************************************************************************/
inline
QString
DatasetCreationProgressDialog
::GetDataset( ) const
{
  return m_UI->datasetLabel->text();
}

/*****************************************************************************/
inline
void
DatasetCreationProgressDialog
::SetDataset(QString value)
{
  m_UI->datasetLabel->setText(value);
}

} // end namespace 'mvd'

#endif // __mvdAboutDialog_h
