/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef mvdImportSubDatasetDialog_h
#define mvdImportSubDatasetDialog_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtWidgets>

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)
#include "OTBMonteverdiGUIExport.h"
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
 * \ingroup OTBMonteverdiGUI
 *
 * \brief Widget template skeleton to copy-paste when adding a new
 * widget class.
 */
class OTBMonteverdiGUI_EXPORT ImportSubDatasetDialog :
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
  ~ImportSubDatasetDialog() override;

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

#endif // mvdImportSubDatasetDialog_h
