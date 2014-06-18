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
#ifndef __mvdDatabaseBrowserWidgetTest_h
#define __mvdDatabaseBrowserWidgetTest_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"

//
// This file is included if and only if ENABLE_TREE_WIDGET_TEST is
// true but its definition is not included here (see
// mvdDatabaseBrowserController.h).
#ifndef ENABLE_TREE_WIDGET_TEST
#  define ENABLE_TREE_WIDGET_TEST 1
#endif // ENABLE_TREE_WIDGET_TEST
//
// Put this line before #include directives for conditional file
// includes.
#define DISABLE_CUSTOM_TW_ITEM ((defined( ENABLE_TREE_WIDGET_TEST ) && 1) || 0)

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
#include "Core/mvdTypes.h"
#if !DISABLE_CUSTOM_TW_ITEM
#  include "Gui/mvdTreeWidgetItem.h"
#endif // !DISABLE_CUSTOM_TW_ITEM

/*****************************************************************************/
/* PRE-DECLARATION SECTION                                                   */

//
// External classes pre-declaration.
namespace
{
}

namespace mvd
{

// class DatabaseTreeWidget;
//
// Internal classes pre-declaration.
namespace Ui
{
class DatabaseBrowserWidgetTest;
};


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class DatabaseBrowserWidgetTest
 *
 * \brief Database content browser. Presents:
 * - Cached datasets.
 */
class Monteverdi2_EXPORT DatabaseBrowserWidgetTest :
    public QWidget
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public types.
public:
  /**
   */
  typedef QList< QTreeWidgetItem* > QTreeWidgetItemList;

#if DISABLE_CUSTOM_TW_ITEM

  enum ColumnIndex
  {
    COLUMN_INDEX_TEXT = 0,
    COLUMN_INDEX_ID = 1,
    COLUMN_INDEX_HASH = 2,
  };

  enum ItemType
  {
    ITEM_TYPE_NONE = QTreeWidgetItem::UserType,
    ITEM_TYPE_NODE = QTreeWidgetItem::UserType + 1,
    ITEM_TYPE_LEAF = QTreeWidgetItem::UserType + 2,
  };

  enum ItemRole
  {
    ITEM_ROLE_ID = Qt::UserRole + 1,
  };

#endif // DISABLE_CUSTOM_TW_ITEM

//
// Public methods.
public:

  /** \brief Constructor. */
  DatabaseBrowserWidgetTest( QWidget* parent =NULL, Qt::WindowFlags flags =0 );

  /** \brief Destructor. */
  virtual ~DatabaseBrowserWidgetTest();

  /**
   */
  const QTreeWidgetItem* GetRootItem() const;

  /**
   */
  QTreeWidgetItem* GetRootItem();

  /**
   */
  QTreeWidgetItem* InsertNodeItem( QTreeWidgetItem* parent,
                                   const QString& text,
                                   const QVariant& id,
                                   const QStringList& columns =QStringList() );

  /**
   */
  QTreeWidgetItem* InsertLeafItem( QTreeWidgetItem* parent,
                                   const QString& text,
                                   const QVariant& id,
                                   const QStringList& columns =QStringList() );

  /**
   * \brief Select the current dataset item of the displayed list.
   *
   * \param name of dataset item to select.
   */
  void SetCurrentDataset( const QString& name );

  /**
   */
  // TODO: Remove access to child widget which should be hidden by encapsulation when CheckDatasetConsistency() has been fixed.
  // DatabaseTreeWidget* GetDatabaseTreeWidget();

  /**
   */
  void InstallTreeEventFilter( QObject* eventFilter );

  /**
   */
  QTreeWidgetItemList GetItems() const;

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

//
// Public SLOTS.
public slots:


  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:

  /**
   * \brief Slot emitted when the currently selected dataset has changed.
   *
   * \param currentHash Hash string (unique name) of newly selected
   * dataset.
   * \param previousHash Hash string (unique name) of the previously
   * selected dataset.
   */
  void CurrentDatasetChanged( const QString& currentHash,
			      const QString& previousHash =QString() );

  /**
   */
  void ItemMoved( QTreeWidgetItem * item, QTreeWidgetItem * target );

  /**
   */
  void AddItemRequested( QTreeWidgetItem* parent );

  /**
   */
  void DeleteItemRequested( QTreeWidgetItem* item );

  /**
   */
  void ItemTextChanged( QTreeWidgetItem* item, int column );
 
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
  QList< QTreeWidgetItem* > TreeWidgetItemList;

//
// Private methods.
private:
  /**
   * \brief Setup UI sub-items.
   */
  void SetupUI();

  /**
   */
  QTreeWidgetItem* InsertItem( QTreeWidgetItem* parent,
#if DISABLE_CUSTOM_TW_ITEM
                               ItemType,
#else // DISABLE_CUSTOM_TW_ITEM
                               TreeWidgetItem::ItemType type,
#endif // DISABLE_CUSTOM_TW_ITEM
                               const QString& text,
                               const QVariant& id,
                               const QStringList& columns =QStringList() );

//
// Private attributes.
private:
  /**
   * \brief uic generated.
   */
  Ui::DatabaseBrowserWidgetTest* m_UI;

  /**
   */
  QPoint m_StartDragPosition;

  /**
   * \brief text to search in the widget tree
   */
  QString m_SearchText;  

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:

  /**
   * \brief Slot called when the current item of the QWidgetTree has
   * changed.
   *
   * \param current The newly selected DatasetTreeWidgetItem.
   * \param previous The previously selected QTreeWidgetItem.
   */
  void on_databaseTreeWidget_currentItemChanged( QTreeWidgetItem* current,
						 QTreeWidgetItem* previous );

  /**
   */
  void on_m_SearchLine_textChanged( const QString& search );

};

} // end namespace 'mvd'

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{

} // end namespace 'mvd'

#endif // __mvdDatabaseBrowserWidgetTest_h
