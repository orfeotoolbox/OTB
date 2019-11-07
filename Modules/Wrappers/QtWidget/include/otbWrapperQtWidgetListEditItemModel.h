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

#ifndef otbListEditItemModel_h
#define otbListEditItemModel_h

#include "otbMacro.h"
#include "OTBQtWidgetExport.h"
#include <QtCore>

namespace otb
{

namespace Wrapper
{

// Internal classes forward declarations
class StringListInterface;

/**
 * \class ListEditItemModel
 *
 * \ingroup OTBQtWidget
 *
 * \brief WIP.
 */
class OTBQtWidget_EXPORT ListEditItemModel : public QAbstractItemModel
{

  Q_OBJECT;

  // Public types
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

  // Public methods
public:
  /** \brief Constructor. */
  ListEditItemModel(StringListInterface*, QObject* p = nullptr);

  /** \brief Destructor. */
  ~ListEditItemModel() override;

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
   * \see http://doc.qt.io/qt-4.8/qabstractitemmodel.html#insertRow
   */
  bool insertRow(int row, const QModelIndex& parent = QModelIndex());

  /**
   * \see http://qt-project.org/doc/qt-4.8/qabstractitemmodel.html#insertRows
   */
  bool insertRows(int row, int count, const QModelIndex& p = QModelIndex()) override;

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

  virtual bool Swap(int, int);

  virtual bool IsInput() const;

  virtual QString GetFilter() const;

  virtual bool IsBrowsable() const;

  // Private attributes
private:
  StringListInterface* m_StringList;
};

} // end namespace 'Wrapper'.

} // end namespace 'otb'.

#endif // otbListEditItemModel_h
