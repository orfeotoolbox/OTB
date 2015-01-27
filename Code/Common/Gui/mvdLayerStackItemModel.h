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
#ifndef __mvdLayerStackItemModel_h
#define __mvdLayerStackItemModel_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtCore>

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
class StackedLayerModel;

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class LayerStackItemModel
 *
 * \brief WIP.
 */
class Monteverdi2_EXPORT LayerStackItemModel :
    public QAbstractItemModel
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public types.
public:

  enum Columns
  {
    COLUMN_NONE = -1,
    //
    COLUMN_INDEX = 0,
    COLUMN_NAME,
    /*
    COLUMN_I,
    COLUMN_J,
    COLUMN_R,
    COLUMN_G,
    COLUMN_B,
    COLUMN_X,
    COLUMN_Y,
    COLUMN_EPSG,
    */
    //
    COLUMN_COUNT,
  };

  /*
  enum ItemRole
  {
    ITEM_ROLE_INDEX = Qt::UserRole + 1
    ITEM_ROLE_NAME,
    ITEM_ROLE_I,
    ITEM_ROLE_J,
    ITEM_ROLE_R,
    ITEM_ROLE_G,
    ITEM_ROLE_B,
    ITEM_ROLE_X,
    ITEM_ROLE_Y,
    ITEM_ROLE_EPSG,
    //
    ITEM_ROLE_NONE,
    //
    ITEM_ROLE_COUNT = ITEM_ROLE_NONE - Qt::UserRole,
  };
  */

//
// Public methods.
public:

  /** \brief Constructor. */
  LayerStackItemModel( QObject* parent =NULL );

  /** \brief Destructor. */
  virtual ~LayerStackItemModel();

  /**
   */
  void SetStack( StackedLayerModel * );

  //
  // QAbstractItemModel overloads.

  /**
   * \see http://qt-project.org/doc/qt-4.8/qabstractitemmodel.html#columnCount
   */
  virtual int columnCount( const QModelIndex & parent = QModelIndex() ) const;

  /**
   * \see http://qt-project.org/doc/qt-4.8/qabstractitemmodel.html#data
   */
  virtual
    QVariant
    data( const QModelIndex & index, int role = Qt::DisplayRole ) const;

  /**
   * \see http://qt-project.org/doc/qt-4.8/qabstractitemmodel.html#flags
   */
  virtual Qt::ItemFlags flags( const QModelIndex & index ) const;

  /**
   * \see http://qt-project.org/doc/qt-4.8/qabstractitemmodel.html#hasChildren
   */
  virtual bool hasChildren( const QModelIndex & parent = QModelIndex() ) const;

  /**
   * \see http://qt-project.org/doc/qt-4.8/qabstractitemmodel.html#headerData
   */
  virtual QVariant headerData( int section,
                               Qt::Orientation orientation,
                               int role = Qt::DisplayRole ) const;
  /**
   * \see http://qt-project.org/doc/qt-4.8/qabstractitemmodel.html#index
   */
  virtual
    QModelIndex
    index( int row,
           int column,
           const QModelIndex & parent = QModelIndex() ) const;

  /**
   * \see http://qt-project.org/doc/qt-4.8/qabstractitemmodel.html#insertRows
   */
  virtual
    bool
    insertRows( int row,
                int count,
                const QModelIndex & parent = QModelIndex() );
  /**
   * \see http://qt-project.org/doc/qt-4.8/qabstractitemmodel.html#parent
   */
  virtual QModelIndex parent( const QModelIndex & index ) const;

  /**
   * \see http://qt-project.org/doc/qt-4.8/qabstractitemmodel.html#removeRows
   */
  virtual
    bool
    removeRows( int row,
                int count,
                const QModelIndex & parent = QModelIndex() );

  /**
   * \see http://qt-project.org/doc/qt-4.8/qabstractitemmodel.html#rowCount
   */
  virtual int rowCount( const QModelIndex & parent = QModelIndex() ) const;

  /**
   * \see http://qt-project.org/doc/qt-4.8/qabstractitemmodel.html#setData
   */
  virtual
    bool
    setData( const QModelIndex & index,
             const QVariant & value,
             int role = Qt::EditRole );

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
  /**
   */
  StackedLayerModel * m_StackedLayerModel;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:
  /**
   */
  // void OnContentAboutToBeChanged();
  // void OnContentChanged();
  // void OnModelAboutToBeReset();
  // void OnModelReset();
  void OnLayerAdded( unsigned int );
  void OnLayerDeleted( unsigned int );
};

} // end namespace 'mvd'.

/*****************************************************************************/
/* INLINE SECTION                                                            */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)

namespace mvd
{
} // end namespace 'mvd'

#endif // __mvdLayerStackItemModel_h
