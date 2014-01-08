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


/*****************************************************************************/
/* PRE-DECLARATION SECTION                                                   */

#define BYPASS_MOUSE_EVENTS 1
#define BYPASS_DRAG_AND_DROP_EVENTS 1

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
    public QTreeWidget
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

  /**
   */
  inline const QTreeWidgetItem* GetRootItem() const;

  /**
   */
  QTreeWidgetItem* GetRootItem();

  //
  //
  void mouseMoveEvent( QMouseEvent * event );
  void mousePressEvent( QMouseEvent * event );
  void dragEnterEvent(QDragEnterEvent *event);
  void dragMoveEvent(QDragMoveEvent *event);
  void dropEvent(QDropEvent *event);
  void keyPressEvent( QKeyEvent * event );

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

//
// Public SLOTS.
public slots:
  
  void OnSelectedDatasetFilenameChanged(const QString& filename);
  void OnDeleteTriggered(const QString & id);
  void OnRenameTriggered();
  void OnCustomContextMenuRequested(const QPoint& pos);
  void OnItemChanged( QTreeWidgetItem* item , int column);

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/  

//
// Signals.
signals:

  void DatasetToDeleteSelected( const QString & id );

  void DatasetRenamed(const QString &, const QString &);

  void ImageDropped(const QString &);

 
  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:
    
    void InitializeContextualMenu();

    //
    // QTreeWidget overloads.
    virtual QStringList mimeTypes () const;

    virtual QMimeData* mimeData( const QList< QTreeWidgetItem* > items ) const;

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
//
// Private attributes.
private:

  QPoint   m_StartDragPosition;
  QString  m_DatasetFilename; 
  QPoint   m_ContextualMenuClickedPosition;

  QTreeWidgetItem * m_ItemToEdit;

  bool              m_EditionActive;
  QString           m_PreviousItemText;
  Qt::ItemFlags     m_DefaultItemFlags;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:
};

} // end namespace 'mvd'

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{
} // end namespace 'mvd'

#endif // __mvdDatabaseTreeWidget_h
