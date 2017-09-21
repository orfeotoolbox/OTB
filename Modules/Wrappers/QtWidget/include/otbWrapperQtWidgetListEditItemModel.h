/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbListEditItemModel_h
#define otbListEditItemModel_h

//
// Configuration include.
//// Included at first position before any other ones.
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "otbMacro.h"
#endif //tag=QT4-boost-compatibility

#include "OTBQtWidgetExport.h"

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

namespace otb
{

namespace Wrapper
{

//
// Internal classes pre-declaration.
class StringListInterface;


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class ListEditItemModel
 *
 * \ingroup OTBMonteverdiCore
 *
 * \brief WIP.
 */
class OTBQtWidget_EXPORT ListEditItemModel :
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
    COLUMN_NAME = 0,
    // COLUMN_BROWSE = 1,
    //
    COLUMN_COUNT,
  };

  enum UserRole
  {
    USER_ROLE_NONE = Qt::UserRole,
    USER_ROLE_DIRECTION,
    USER_ROLE_FILTER,
  };

//
// Public methods.
public:

  /** \brief Constructor. */
  ListEditItemModel( QObject * p = nullptr );

  /** \brief Destructor. */
  ~ListEditItemModel() ITK_OVERRIDE;

  /** */
  void SetStringList( StringListInterface * );

  //
  // QAbstractItemModel overloads.

  /**
   * \see http://qt-project.org/doc/qt-4.8/qabstractitemmodel.html#columnCount
   */
  int columnCount( const QModelIndex & p = QModelIndex() ) const ITK_OVERRIDE;

  /**
   * \see http://qt-project.org/doc/qt-4.8/qabstractitemmodel.html#data
   */
  QVariant
    data( const QModelIndex & index,
	  int role = Qt::DisplayRole ) const ITK_OVERRIDE;

  /**
   * \see http://qt-project.org/doc/qt-4.8/qabstractitemmodel.html#flags
   */
  Qt::ItemFlags flags( const QModelIndex & index ) const ITK_OVERRIDE;

  /**
   * \see http://qt-project.org/doc/qt-4.8/qabstractitemmodel.html#hasChildren
   */
  bool hasChildren( const QModelIndex & p = QModelIndex() ) const ITK_OVERRIDE;

  /**
   * \see http://qt-project.org/doc/qt-4.8/qabstractitemmodel.html#headerData
   */
  QVariant headerData( int section,
                               Qt::Orientation orientation,
                               int role = Qt::DisplayRole ) const ITK_OVERRIDE;

  /**
   * \see http://qt-project.org/doc/qt-4.8/qabstractitemmodel.html#index
   */
  QModelIndex
    index( int row,
           int column,
           const QModelIndex & p = QModelIndex() ) const ITK_OVERRIDE;

  /**
   * \see http://doc.qt.io/qt-4.8/qabstractitemmodel.html#insertRow
   */
  bool
    insertRow( int row, const QModelIndex & parent = QModelIndex() );

  /**
   * \see http://qt-project.org/doc/qt-4.8/qabstractitemmodel.html#insertRows
   */
  bool
    insertRows( int row,
                int count,
                const QModelIndex & p = QModelIndex() ) ITK_OVERRIDE;

  /**
   * \see http://qt-project.org/doc/qt-4.8/qabstractitemmodel.html#parent
   */
  QModelIndex parent( const QModelIndex & index ) const ITK_OVERRIDE;

  /**
   * \see http://qt-project.org/doc/qt-4.8/qabstractitemmodel.html#removeRows
   */
  bool
    removeRows( int row,
                int count,
                const QModelIndex & p = QModelIndex() ) ITK_OVERRIDE;

  /**
   * \see http://qt-project.org/doc/qt-4.8/qabstractitemmodel.html#rowCount
   */
  int rowCount( const QModelIndex & p = QModelIndex() ) const ITK_OVERRIDE;

  /**
   * \see http://qt-project.org/doc/qt-4.8/qabstractitemmodel.html#setData
   */
  bool
    setData( const QModelIndex & index,
             const QVariant & value,
             int role = Qt::EditRole ) ITK_OVERRIDE;

  /** */
  virtual bool Swap( int, int );

  /** */
  virtual bool IsInput() const;

  /** */
  virtual QString GetFilter() const;

  /** */
  virtual bool IsBrowsable() const;

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
  /** */
  StringListInterface * m_StringList;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:
};

} // end namespace 'Wrapper'.

} // end namespace 'otb'.

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

namespace otb
{
} // end namespace 'otb'

#endif // otbListEditItemModel_h
