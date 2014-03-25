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
#ifndef __mvdDatabaseTreeWidget_h
#define __mvdDatabaseTreeWidget_h

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
#include "Gui/mvdTreeWidget.h"

/*****************************************************************************/
/* PRE-DECLARATION SECTION                                                   */

#define BYPASS_MOUSE_EVENTS 1

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
//class DatabaseTreeWidget;
};


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class DatabaseTreeWidget
 *
 * \brief Widget template skeleton to copy-paste when adding a new
 * widget class.
 */
class Monteverdi2_EXPORT DatabaseTreeWidget :
    public TreeWidget
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:

  /** \brief Constructor. */
  DatabaseTreeWidget( QWidget* parent =NULL );

  /** \brief Destructor. */
  virtual ~DatabaseTreeWidget();

  //
  //
  void mouseMoveEvent( QMouseEvent * event );
  void mousePressEvent( QMouseEvent * event );
  void dropEvent(QDropEvent *event);
  void keyPressEvent( QKeyEvent * event );

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

//
// Public SLOTS.
public slots:

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/  

//
// Signals.
signals:
  void DatasetToDeleteSelected( const QString & id );
  void DatasetRenamed(const QString &, const QString &);

  void AddItemRequested( QTreeWidgetItem* parent );
  void DeleteItemRequested( QTreeWidgetItem* item );
  void ItemTextChanged( QTreeWidgetItem* item, int column );
 
  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

    //
    // QTreeWidget overloads.

    virtual bool dropMimeData( QTreeWidgetItem* parent,
                               int index,
                               const QMimeData* data,
                               Qt::DropAction action );

//
// Protected attributes.
protected:

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Private methods.
private:
    /**
     */
    static QAction* AddAction( QMenu* menu,
                               const QString& text,
                               QObject* receiver,
                               const char* method,
                               Qt::ConnectionType type =Qt::AutoConnection );
    /**
     */
    /*
    static
      QAction* AddMappedAction( QMenu* menu,
                                const QString& text,
                                int data,
                                QObject* receiver,
                                const char* method,
                                Qt::ConnectionType type
      =Qt::AutoConnection );
    */
    /**
     */
    static
      QAction* AddMappedAction( QMenu* menu,
                                const QString& text,
                                const QString& data,
                                QObject* receiver,
                                const char* method,
                                Qt::ConnectionType type =Qt::AutoConnection );
    /**
     */
    /*
    static
      QAction* AddMappedAction( QMenu* menu,
                                const QString& text,
                                QObject* data,
                                QObject* receiver,
                                const char* method,
                                Qt::ConnectionType type =Qt::AutoConnection );
    */
    /**
     */
    static QSignalMapper* AddMappedAction( QMenu* menu, const QString& text );

    /**
     */
    void EditItem( int column =-1 );

//
// Private attributes.
private:
    class EditionContext;

    // QPoint m_StartDragPosition;
    // QString m_DatasetFilename; 
    // QPoint m_ContextualMenuClickedPosition;

    // QTreeWidgetItem* m_ItemToEdit;
    // bool m_EditionActive;
    // QString m_PreviousItemText;
    // Qt::ItemFlags m_DefaultItemFlags;

    EditionContext* m_EditionContext;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:
    // void OnSelectedDatasetFilenameChanged(const QString& filename);
    // void OnDeleteTriggered(const QString & id);

    void OnCustomContextMenuRequested( const QPoint& pos );
    void OnItemChanged( QTreeWidgetItem* item , int column);

    void OnAddItemTriggered();
    void OnDeleteItemTriggered();
    void OnRenameItemTriggered();
};

} // end namespace 'mvd'

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{
} // end namespace 'mvd'

#endif // __mvdDatabaseTreeWidget_h
