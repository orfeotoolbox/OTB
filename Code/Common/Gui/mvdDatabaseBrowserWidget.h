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
#ifndef __mvdDatabaseBrowserWidget_h
#define __mvdDatabaseBrowserWidget_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"


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


/*****************************************************************************/
/* PRE-DECLARATION SECTION                                                   */

//
// External classes pre-declaration.
namespace
{
}

namespace mvd
{

class DatabaseTreeWidget;
class DatasetTreeWidgetItem;
//
// Internal classes pre-declaration.
namespace Ui
{
class DatabaseBrowserWidget;
};


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class DatabaseBrowserWidget
 *
 * \brief Database content browser. Presents:
 * - Cached datasets.
 */
class Monteverdi2_EXPORT DatabaseBrowserWidget :
    public QWidget
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:

  /** \brief Constructor. */
  DatabaseBrowserWidget( QWidget* parent =NULL, Qt::WindowFlags flags =0 );

  /** \brief Destructor. */
  virtual ~DatabaseBrowserWidget();

  /**
   * \brief Assign the displayed dataset list (under the 'Datasets'
   * root item).
   *
   * \param list List of dataset names.
   */
  void SetDatasetList( const StringPairListType/*QStringList*/& list );

  /**
   * \brief Select the current dataset item of the displayed list.
   *
   * \param name of dataset item to select.
   */
  void SetCurrentDataset( const QString& name );

  /**
   */
  DatabaseTreeWidget* GetDatabaseTreeWidget();

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

//
// Public SLOTS.
public slots:

  void OnCurrentSelectedItemDeleted();

  void OnSearchBoxChanged( const QString & search );


  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:

  /**
   * \brief Slot emitted when the current (selected) dataset has
   * changed.
   *
   * \param text Name of new current (selected) dataset.
   */
  void CurrentDatasetChanged( const QString& text );

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:
  
  /** Fill the tree using the dataset list */
  void FillTree();
  
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

//
// Private attributes.
private:
  /**
   * \brief uic generated.
   */
  Ui::DatabaseBrowserWidget* m_UI;

  /**
   * \brief Root item of displayed datasets. It is named 'datasets'.
   */
  QTreeWidgetItem* m_DatasetRootItem;

  /**
   * \brief Root item of displayed datasets. It is named 'datasets'.
   */
  StringPairListType  m_DatasetList;

  /**
   */
  QPoint           m_StartDragPosition;

  /**
   * \brief text to search in the widget tree
   */
  QString                  m_SearchText;  

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
};

} // end namespace 'mvd'

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{

} // end namespace 'mvd'

#endif // __mvdDatabaseBrowserWidget_h
