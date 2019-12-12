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

#ifndef mvdLayerStackItemModel_h
#define mvdLayerStackItemModel_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"


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
class AbstractImageModel;
class AbstractLayerModel;
class StackedLayerModel;

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class LayerStackItemModel
 *
 * \ingroup OTBMonteverdiGUI
 *
 * \brief WIP.
 */
class OTBMonteverdiGUI_EXPORT LayerStackItemModel : public QAbstractItemModel
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
    COLUMN_PROJ = 0,
    COLUMN_RESOLUTION,
    COLUMN_NAME,
    COLUMN_EFFECT,
    COLUMN_I,
    COLUMN_J,
    COLUMN_R,
    COLUMN_G,
    COLUMN_B,
    COLUMN_X,
    COLUMN_Y,
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
  LayerStackItemModel(QObject* p = NULL);

  /** \brief Destructor. */
  ~LayerStackItemModel() override;

  /**
   */
  static const AbstractLayerModel* GetLayer(const QModelIndex&);

  /**
   */
  void SetStack(StackedLayerModel*);

  //
  // QAbstractItemModel overloads.

  /**
   * \see http://qt-project.org/doc/qt-4.8/qabstractitemmodel.html#columnCount
   */
  int columnCount(const QModelIndex& p = QModelIndex()) const override;

  /**
   * \see http://qt-project.org/doc/qt-4.8/qabstractitemmodel.html#data
   */
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

  /**
   * \see http://qt-project.org/doc/qt-4.8/qabstractitemmodel.html#dropMimeData
   */
  bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& p) override;

  /**
   * \see http://qt-project.org/doc/qt-4.8/qabstractitemmodel.html#flags
   */
  Qt::ItemFlags flags(const QModelIndex& index) const override;

  /**
   * \see http://qt-project.org/doc/qt-4.8/qabstractitemmodel.html#hasChildren
   */
  bool hasChildren(const QModelIndex& p = QModelIndex()) const override;

  /**
   * \see http://qt-project.org/doc/qt-4.8/qabstractitemmodel.html#headerData
   */
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

  /**
   * \see http://qt-project.org/doc/qt-4.8/qabstractitemmodel.html#index
   */
  QModelIndex index(int row, int column, const QModelIndex& p = QModelIndex()) const override;

  /**
   * \see http://qt-project.org/doc/qt-4.8/qabstractitemmodel.html#insertRows
   */
  bool insertRows(int row, int count, const QModelIndex& p = QModelIndex()) override;

  /**
   * \see http://doc.qt.io/qt-4.8/qabstractitemmodel.html#mimeData
   */
  QMimeData* mimeData(const QModelIndexList& indexes) const override;

  /**
   * \see http://doc.qt.io/qt-4.8/qabstractitemmodel.html#mimeTypes
   */
  QStringList mimeTypes() const override;

  /**
   * \see http://qt-project.org/doc/qt-4.8/qabstractitemmodel.html#parent
   */
  QModelIndex parent(const QModelIndex& index) const override;

  /**
   * \see http://qt-project.org/doc/qt-4.8/qabstractitemmodel.html#removeRows
   */
  bool removeRows(int row, int count, const QModelIndex& p = QModelIndex()) override;

  /**
   * \see http://qt-project.org/doc/qt-4.8/qabstractitemmodel.html#rowCount
   */
  int rowCount(const QModelIndex& p = QModelIndex()) const override;

  /**
   * \see http://qt-project.org/doc/qt-4.8/qabstractitemmodel.html#setData
   */
  bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

  /**
   * \see http://qt-project.org/doc/qt-4.8/qabstractitemmodel.html#supportedDropActions
   */
  Qt::DropActions supportedDropActions() const override;

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

  //
  // Public SLOTS.
public slots:

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

  //
  // Signals.
signals:

  // Bugfix for layer deletion
  void LayerDeletingModel(unsigned int index);

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
  /**
   */
  void Connect(AbstractLayerModel*);

  /**
   */
  void Disconnect(AbstractLayerModel*);


  //
  // Private attributes.
private:
  /**
   */
  StackedLayerModel* m_StackedLayerModel;

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
  void OnLayerAboutToBeDeleted(size_t);
  void OnLayerAdded(size_t);
  void OnLayerDeleted(size_t);
  void OnLayerVisibilityChanged(AbstractLayerModel*, bool);
  void OnReferenceChanged(size_t);
  void OnPixelInfoChanged(const QPoint&, const PointType&, const PixelInfo::Vector&);
  void OnImageSettingsUpdated(AbstractImageModel*);
  void OnResolutionsChanged(const PixelInfo::Vector&);
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

#endif // mvdLayerStackItemModel_h
