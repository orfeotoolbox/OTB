/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#include "otbWrapperQtWidgetListEditItemModel.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

#include "otbWrapperStringListInterface.h"


namespace otb
{

namespace Wrapper
{

/*
  TRANSLATOR otb::Wapper::ListEditItemModel

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */

namespace
{

const char* const HEADERS[ListEditItemModel::COLUMN_COUNT] = {
    QT_TRANSLATE_NOOP("otb::Wrapper::ListEditItemModel", "Name"),
    // QT_TRANSLATE_NOOP( "otb::Wrapper::ListEditItemModel", "Browse" ),
};

} // end of anonymous namespace.


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
ListEditItemModel::ListEditItemModel(StringListInterface* sli, QObject* p) : QAbstractItemModel(p), m_StringList(sli)
{
  assert(sli != nullptr);
}

/*******************************************************************************/
ListEditItemModel::~ListEditItemModel()
{
}

/*****************************************************************************/
/* QAbstractItemModel overloads                                              */
/*****************************************************************************/
int ListEditItemModel::columnCount(const QModelIndex&) const
{
  // qDebug() << this << "::columnCount(" << parent << ")";

  return COLUMN_COUNT;
}

/*****************************************************************************/
QVariant ListEditItemModel::data(const QModelIndex& idx, int role) const
{
  // qDebug() << this << "::data(" << idx << "," << role << ")";

  // Get layer.
  assert(m_StringList != NULL);

  assert(idx.isValid());
  assert(!idx.parent().isValid());
  assert(idx.internalPointer() != NULL);

  const StringListInterface* stringList = static_cast<const StringListInterface*>(idx.internalPointer());

  assert(stringList != nullptr);

  // Return data given role.
  switch (role)
  {
  case Qt::CheckStateRole:
#if 0
      if( idx.column()!=COLUMN_NAME )
        return QVariant();
      else
	{
	assert( idx.row() >= 0 );
	return stringList->IsActive( idx.row() );
	}
#endif
    break;

  case Qt::EditRole:
  case Qt::DisplayRole:
    switch (idx.column())
    {
    case COLUMN_NAME:
    {
      assert(idx.row() >= 0);

      std::string filename(stringList->GetNthFileName(idx.row()));

      // qDebug() << "Filename:" << QString( "%1" ).arg( filename.c_str() );

      return filename.empty() ? (role == Qt::EditRole ? QString() : "EMPTY") : QFile::decodeName(filename.c_str());
    }
    break;

    default:
      break;
    }
    break;

  case Qt::FontRole:
    break;

  case Qt::ToolTipRole:
    switch (idx.column())
    {
    case COLUMN_NAME:
      assert(idx.row() >= 0);
      return QString::fromStdString(stringList->GetToolTip(idx.row()));
      break;
    }
    break;

  case USER_ROLE_DIRECTION:
    assert(idx.row() >= 0);
    return stringList->GetDirection(idx.row());
    break;

  case USER_ROLE_FILTER:
    assert(idx.row() >= 0);
    return QString::fromStdString(stringList->GetFilenameFilter(idx.row()));
    break;

  default:
    break;
  }

  return QVariant();
}

/*****************************************************************************/
Qt::ItemFlags ListEditItemModel::flags(const QModelIndex& idx) const
{
  if (!idx.isValid())
    return QAbstractItemModel::flags(idx);

  Qt::ItemFlags iflags = QAbstractItemModel::flags(idx)
      // | Qt::ItemIsDragEnabled
      // | Qt::ItemIsDropEnabled
      ;

  if (idx.column() == COLUMN_NAME)
    iflags |= Qt::ItemIsEditable
        // | Qt::ItemIsUserCheckable
        // | Qt::ItemIsDragEnabled
        ;

  return iflags;
}

/*****************************************************************************/
bool ListEditItemModel::hasChildren(const QModelIndex& idx) const
{
  return !idx.isValid();
}

/*****************************************************************************/
QVariant ListEditItemModel::headerData(int section, Qt::Orientation /**orientation*/, int role) const
{
  // qDebug()
  //   << this << "::headerData("
  //   << section << "," << orientation << "," << role
  //   << ")";

  // assert( orientation==Qt::Horizontal );

  switch (role)
  {
  case Qt::DisplayRole:
    assert(section >= 0 && section < COLUMN_COUNT);
    return tr(HEADERS[section]);
    break;

  default:
    break;
  }

  return QVariant();
}

/*****************************************************************************/
QModelIndex ListEditItemModel::index(int row, int column, const QModelIndex& p) const
{
  // qDebug()
  //   << this << "::index(" << row << "," << column << "," << parent << ")";

  if (m_StringList == nullptr)
    return QModelIndex();

  // qDebug()
  //   << "index:" << row << "," << column << "," << m_StringList->At( row );

  assert(row >= 0 && column >= 0);

#if 0
  AbstractLayerModel * layer = m_StringList->At( row );

  if( layer==NULL || p.isValid() )
    return QModelIndex();
#endif

  return createIndex(row, column, p.isValid() ? NULL : m_StringList);
}

/*****************************************************************************/
bool ListEditItemModel::insertRow(int row, const QModelIndex& idxParent)
{
  return insertRows(row, 1, idxParent);
}

/*****************************************************************************/
bool ListEditItemModel::insertRows(int row, int count, const QModelIndex& idxParent)
{
  // qDebug() << this << "::insertRows(" << row << "," << count << "," << idxParent << ")";

  assert(m_StringList != nullptr);

  beginInsertRows(idxParent, row, count);
  {
    for (int r = row; r < row + count; ++r)
      m_StringList->Insert("", r);
  }
  endInsertRows();

  return true;
}

/*****************************************************************************/
QModelIndex ListEditItemModel::parent(const QModelIndex&) const
{
  // qDebug() << this << "::parent(" << index << ")";

  return QModelIndex();
}

/*****************************************************************************/
bool ListEditItemModel::removeRows(int row, int count, const QModelIndex& p)
{
  assert(!p.isValid());
  assert(count >= 1);

  if (p.isValid() || count < 1)
    return false;

  assert(m_StringList != nullptr);

  beginRemoveRows(p, row, row + count - 1);
  {
    m_StringList->Erase(row, count);
  }
  endRemoveRows();

  return true;
}

/*****************************************************************************/
int ListEditItemModel::rowCount(const QModelIndex& p) const
{
  // qDebug() << this << "::rowCount(" << p << ")";

  // qDebug() << "row-count:" <<
  //   ( ( m_StringList==NULL || p.isValid() )
  //     ? 0
  //     : m_StringList->GetCount()
  //   );

  return (m_StringList == nullptr || p.isValid()) ? 0 : m_StringList->Size();
}

/*****************************************************************************/
bool ListEditItemModel::setData(const QModelIndex& idx, const QVariant& value, int role)
{
  // qDebug()
  //   << this << "::setData(" << idx << "," << value << "," << role
  //   << ");";

  assert(!idx.parent().isValid());
  assert(idx.row() >= 0);
  assert(idx.internalPointer() != nullptr);

  StringListInterface* stringList = static_cast<StringListInterface*>(idx.internalPointer());

  switch (idx.column())
  {
  case COLUMN_NAME:
    switch (role)
    {
    case Qt::EditRole:
      stringList->SetNthFileName(idx.row(), QFile::encodeName(value.toString()).data());
      Q_EMIT dataChanged(idx, idx);
      return true;
      break;

    case Qt::CheckStateRole:
      break;

    case USER_ROLE_DIRECTION:
      break;

    default:
      break;
    }
    break;

  default:
    break;
  }

  return false;
}

/*******************************************************************************/
bool ListEditItemModel::Swap(int row1, int row2)
{
  assert(m_StringList != nullptr);

  assert(row1 >= 0);
  assert(static_cast<unsigned int>(row1) < m_StringList->Size());

  assert(row2 >= 0);
  assert(static_cast<unsigned int>(row2) < m_StringList->Size());

  assert(row1 != row2);

  Q_EMIT layoutAboutToBeChanged();

  m_StringList->Swap(row1, row2);

  Q_EMIT layoutChanged();

  return true;
}

/*******************************************************************************/
bool ListEditItemModel::IsInput() const
{
  assert(m_StringList != nullptr);

  return m_StringList->GetDirection() == Role_Input;
}

/*******************************************************************************/
bool ListEditItemModel::IsBrowsable() const
{
  assert(m_StringList != nullptr);

  return m_StringList->IsFilename();
}

/*******************************************************************************/
QString ListEditItemModel::GetFilter() const
{
  assert(m_StringList != nullptr);

  return QString::fromStdString(m_StringList->GetFilenameFilter());
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'Wrapper'.

} // end namespace 'otb'
