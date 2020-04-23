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

#ifndef mvdTreeWidgetItem_h
#define mvdTreeWidgetItem_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtWidgets>

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)
#include "OTBMonteverdiGUIExport.h"
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
class TreeWidgetItem;

//
// Functions declarations.

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class TreeWidgetItem
 *
 * \ingroup OTBMonteverdiGUI
 *
 * \brief
 */
class OTBMonteverdiGUI_EXPORT TreeWidgetItem : public QTreeWidgetItem // not a QObject
{

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

  //
  // Public types.
public:
  enum ColumnIndex
  {
    COLUMN_INDEX_TEXT = 0,
    COLUMN_INDEX_ID   = 1,
    COLUMN_INDEX_HASH = 2,
  };

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
  TreeWidgetItem(QTreeWidgetItem* p = NULL, const QString& text = QString(), const QVariant& id = QVariant(), const QStringList& columns = QStringList(),
                 TreeWidgetItem::ItemType type = TreeWidgetItem::ITEM_TYPE_LEAF);

  /**
   * \brief Destructor.
   */
  ~TreeWidgetItem() override;

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
  QTreeWidgetItem* clone() const override;

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
  static const int m_MetaTypeId;
  /**
   */
  static const int m_PointerMetaTypeId;
  /**
   */
  static const int m_ConstPointerMetaTypeId;

  /**
   */
  inline void SetId(const QVariant& id);

  //
  // Private attributes.
private:
  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

  //
  // Slots.
private slots:
};

} // end namespace 'mvd'

/*
//
// Declare TreeWidgetItem meta-type so it can be used for drag-and-drop
// operations.
Q_DECLARE_METATYPE( mvd::TreeWidgetItem );
*/

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{

/*****************************************************************************/
inline TreeWidgetItem::ItemType TreeWidgetItem::GetType() const
{
  return type() < QTreeWidgetItem::UserType ? TreeWidgetItem::ITEM_TYPE_NONE : TreeWidgetItem::ItemType(type());
}

/*****************************************************************************/
inline QVariant TreeWidgetItem::GetId() const
{
  return data(COLUMN_INDEX_ID, TreeWidgetItem::ITEM_ROLE_ID);
}

/*****************************************************************************/
inline void TreeWidgetItem::SetId(const QVariant& id)
{
  setData(COLUMN_INDEX_ID, TreeWidgetItem::ITEM_ROLE_ID, id);
  setText(COLUMN_INDEX_ID, id.toString());
}

/*****************************************************************************/
inline QString TreeWidgetItem::GetText() const
{
  return text(COLUMN_INDEX_TEXT);
}

/*****************************************************************************/
inline QString TreeWidgetItem::GetHash() const
{
  return text(COLUMN_INDEX_HASH);
}

} // end namespace 'mvd'

#endif // mvdTreeWidgetItem_h
