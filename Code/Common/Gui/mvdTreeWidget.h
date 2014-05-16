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
#ifndef __mvdTreeWidget_h
#define __mvdTreeWidget_h

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

//
// External classes pre-declaration.
namespace
{
}

namespace mvd
{

/*****************************************************************************/
/* FUNCTIONS DEFINITION SECTION                                              */

/**
 */
QMimeData*
EncodeMimeData( QMimeData* mimeData, const QList< QTreeWidgetItem* >& items );

/**
 */
int
DecodeMimeData( QList< QTreeWidgetItem* >& items, const QMimeData* mimeData );

//
// Internal classes pre-declaration.
namespace Ui
{
//class TreeWidget;
};


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class TreeWidget
 *
 * \brief Widget template skeleton to copy-paste when adding a new
 * widget class.
 */
class Monteverdi2_EXPORT TreeWidget :
    public QTreeWidget
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public types and constants.
public:
  /**
   */
  static const char* ITEM_MIME_TYPE;

  /**
   */
  typedef QList< QTreeWidgetItem* > QTreeWidgetItemList;

//
// Public methods.
public:

  /** \brief Constructor. */
  TreeWidget( QWidget* parent =NULL );

  /** \brief Destructor. */
  virtual ~TreeWidget();

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

//
// Public SLOTS.
public slots:

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/  

//
// Signals.
signals:

  /**
   */
  void ItemMoved( QTreeWidgetItem * item, QTreeWidgetItem * target );

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

    //
    // QTreeWidget overloads.

    virtual QStringList mimeTypes() const;

    virtual QMimeData* mimeData( const QList< QTreeWidgetItem* > items ) const;

    virtual void dragEnterEvent( QDragEnterEvent* event );
    virtual void dragMoveEvent( QDragMoveEvent* event );
    virtual void dragLeaveEvent( QDragLeaveEvent* event );
    virtual void dropEvent( QDropEvent* event );

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

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:
};

} // end namespace 'mvd'


/*****************************************************************************/
/* GLOBAL FUNCTIONS DECLARATION                                              */

//
// Declare Qt tree-widget item pointer types so they can be wrapped by
// QVariant.
Q_DECLARE_METATYPE( QTreeWidgetItem* );


#define TREE_WIDGET_ITEM_USE_STREAM_OPERATORS 1

#if TREE_WIDGET_ITEM_USE_STREAM_OPERATORS

/**
 */
QDataStream&
operator << ( QDataStream& out, QTreeWidgetItem const * item );

/**
 */
QDataStream&
operator >>( QDataStream& in, QTreeWidgetItem * & item );

#endif // !DATASTREAM_USE_TEMPLATE_OPERATORS

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{
} // end namespace 'mvd'

#endif // __mvdTreeWidget_h
