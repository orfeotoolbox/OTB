/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef mvdSearchableTreeWidget_h
#define mvdSearchableTreeWidget_h

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
// #include "mvdTypes.h"


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
class SearchableTreeWidget;
};


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class SearchableTreeWidget
 *
 * \ingroup OTBMonteverdiGUI
 *
 * \brief Database content browser. Presents:
 * - Cached datasets.
 */
class OTBMonteverdiGUI_EXPORT SearchableTreeWidget : public QWidget
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

  //
  // Public types.
public:
  //
  // Public methods.
public:
  /** \brief Constructor. */
  SearchableTreeWidget(QWidget* p = NULL, Qt::WindowFlags flags = 0);

  /** \brief Destructor. */
  ~SearchableTreeWidget() override;

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

  //
  // Public SLOTS.
public Q_SLOTS:


  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

  //
  // Signals.
Q_SIGNALS:

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

  //
  // Protected methods.
protected:
  //
  // Protected attributes.
protected:
  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

  //
  // Private types.
private:
  /**
   * \brief List of dataset item names displayed under the 'dataset'
   * root item.
   */
  QList<QTreeWidgetItem*> TreeWidgetItemList;

  //
  // Private methods.
private:
  /**
   * \brief Setup UI sub-items.
   */
  void SetupUI();

  //
  // Private attributes.
private:
  /**
   * \brief uic generated.
   */
  Ui::SearchableTreeWidget* m_UI;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

  //
  // Slots.
private Q_SLOTS:

  /**
   * \brief Slot called when the current item of the QWidgetTree has
   * changed.
   *
   * \param current The newly selected DatasetTreeWidgetItem.
   * \param previous The previously selected QTreeWidgetItem.
   */
  /*
  void on_databaseTreeWidget_currentItemChanged( QTreeWidgetItem* current,
             QTreeWidgetItem* previous );
  */

  /**
   */
  // void on_searchLineEdit_textChanged( const QString& text );
};

} // end namespace 'mvd'

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{

} // end namespace 'mvd'

#endif // mvdSearchableTreeWidget_h
