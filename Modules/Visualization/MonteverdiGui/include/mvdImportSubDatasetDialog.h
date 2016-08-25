/*=========================================================================

  Program:   Monteverdi
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __mvdImportSubDatasetDialog_h
#define __mvdImportSubDatasetDialog_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

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
#include "mvdTypes.h"


/*****************************************************************************/
/* PRE-DECLARATION SECTION                                                   */

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
class ImportSubDatasetDialog;
};


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class ImportSubDatasetDialog
 *
 * \ingroup MonteverdiGui
 *
 * \brief Widget template skeleton to copy-paste when adding a new
 * widget class.
 */
class Monteverdi_EXPORT ImportSubDatasetDialog :
    public QDialog
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:

//
// Public methods.
public:

  /** \brief Constructor. */
  ImportSubDatasetDialog( const QString & filename,
		     QWidget* p = NULL,
		     Qt::WindowFlags flags = 0 );

  /** \brief Destructor. */
  virtual ~ImportSubDatasetDialog();

  /**
   */
  void GetSubDatasets( IntVector & ) const;

  /**
   */
  bool HasSubDatasets() const;

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

//
// Public SLOTS.
public slots:

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Protected attributes.
protected:

//
// Private methods.
private:

  /**
   */
  void Setup( const QString & filename );

  /**
   */
  void SetSubDatasetsCheckState( Qt::CheckState );

//
// Private types.
private:
  enum DatasetColumns
  {
    DATASET_COLUMN_INDEX = 0,
    DATASET_COLUMN_DESC,
    DATASET_COLUMN_NAME,
    //
    DATASET_COLUMN_COUNT
  };

//
// Private attributes.
private:
  /**
   */
  static char const * const DATASET_COLUMN_NAMES[ DATASET_COLUMN_COUNT ];

  /**
   * \brief uic generated.
   */
  Ui::ImportSubDatasetDialog * m_UI;

  /**
   */
  bool m_HasSubDatasets : 1;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:
  /**
   */
  void on_allButton_clicked();
  /**
   */
  void on_invertButton_clicked();
  /**
   */
  void on_noneButton_clicked();
};

} // end namespace 'mvd'

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{
} // end namespace 'mvd'

#endif // __mvdImportSubDatasetDialog_h
