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
#ifndef __mvdTreeWidgetItem_h
#define __mvdTreeWidgetItem_h

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

//
// Internal classes pre-declaration.

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class TreeWidgetItem
 *
 * \brief 
 */
class Monteverdi2_EXPORT TreeWidgetItem :
    public QTreeWidgetItem // not a QObject
{

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public types.
public:
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

//
// Public methods.
public:

  /**
   * \brief Constructor.
   */
  TreeWidgetItem( QTreeWidgetItem* parent,
                  const QString& text,
                  const QVariant& id /*=QVariant()*/,
                  const QStringList& columns = QStringList(),
                  TreeWidgetItem::ItemType type =TreeWidgetItem::ITEM_TYPE_LEAF );

  /**
   * \brief Destructor.
   */
  virtual ~TreeWidgetItem();

  /**
   */
  inline TreeWidgetItem::ItemType GetType() const;

  /**
   */
  inline QVariant GetId() const;

  /**
   */
  inline QString GetText() const;

  /**
   * \brief Get the datasetID 
   */
  inline QString GetHash() const;

  //
  // QTreeWidgetItem overloads.

  /**
   * \see http://qt-project.org/doc/qt-4.8/qtreewidgetitem.html#clone
   */
  virtual QTreeWidgetItem* clone();

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
  inline void SetId( const QVariant& id );

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
/* INLINE SECTION                                                            */

namespace mvd
{

/*******************************************************************************/
inline
TreeWidgetItem::ItemType
TreeWidgetItem
::GetType() const
{
  return
    type() < QTreeWidgetItem::UserType
    ? TreeWidgetItem::ITEM_TYPE_NONE
    : TreeWidgetItem::ItemType( type() );
}

/*******************************************************************************/
inline
QVariant
TreeWidgetItem
::GetId() const
{
  return data( 1, TreeWidgetItem::ITEM_ROLE_ID );
}

/*******************************************************************************/
inline
void
TreeWidgetItem
::SetId( const QVariant& id )
{
  setData( 1, TreeWidgetItem::ITEM_ROLE_ID, id );
  setText( 1, id.toString() );
}

/*******************************************************************************/
inline
QString
TreeWidgetItem
::GetText() const
{
  return text( 0 );
}

/*******************************************************************************/
inline
QString
TreeWidgetItem
::GetHash() const
{
  return text( 2 );
}

} // end namespace 'mvd'

#endif // __mvdTreeWidgetItem_h
