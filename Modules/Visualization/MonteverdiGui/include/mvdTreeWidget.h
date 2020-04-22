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

#ifndef mvdTreeWidget_h
#define mvdTreeWidget_h

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


#define USE_CUSTOM_MIME_DATA 1


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
QMimeData* EncodeMimeData(QMimeData* mimeData, const QList<QTreeWidgetItem*>& items);

/**
 */
int DecodeMimeData(QList<QTreeWidgetItem*>& items, const QMimeData* mimeData);

//
// Internal classes pre-declaration.
namespace Ui
{
// class TreeWidget;
};


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class TreeWidget
 *
 * \ingroup OTBMonteverdiGUI
 *
 * \brief Widget template skeleton to copy-paste when adding a new
 * widget class.
 */
class OTBMonteverdiGUI_EXPORT TreeWidget : public QTreeWidget
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
  typedef QList<QTreeWidgetItem*> QTreeWidgetItemList;

  //
  // Public methods.
public:
  /** \brief Constructor. */
  TreeWidget(QWidget* p = NULL);

  /** \brief Destructor. */
  ~TreeWidget() override;

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

  //
  // Public SLOTS.
public Q_SLOTS:

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

  //
  // Signals.
Q_SIGNALS:

  /**
   */
  void ItemMoved(QTreeWidgetItem* item, QTreeWidgetItem* target);

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

  //
  // Protected methods.
protected:
  //
  // QTreeWidget overloads.

  QStringList mimeTypes() const override;

  QMimeData* mimeData(const QList<QTreeWidgetItem*> items) const override;

  void dragEnterEvent(QDragEnterEvent* event) override;
  void dragMoveEvent(QDragMoveEvent* event) override;
  void dragLeaveEvent(QDragLeaveEvent* event) override;
  void dropEvent(QDropEvent* event) override;

  Qt::DropActions supportedDropActions() const override;
  void startDrag(Qt::DropActions supportedActions) override;

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
private Q_SLOTS:
};

} // end namespace 'mvd'


/*****************************************************************************/
/* GLOBAL FUNCTIONS DECLARATION                                              */

#if USE_CUSTOM_MIME_DATA

//
// Declare Qt tree-widget item pointer types so they can be wrapped by
// QVariant.
Q_DECLARE_METATYPE(QTreeWidgetItem*);


#define TREE_WIDGET_ITEM_USE_STREAM_OPERATORS 1

#if TREE_WIDGET_ITEM_USE_STREAM_OPERATORS

/**
 */
QDataStream& operator<<(QDataStream& out, QTreeWidgetItem const* item);

/**
 */
QDataStream& operator>>(QDataStream& in, QTreeWidgetItem*& item);

#endif // !DATASTREAM_USE_TEMPLATE_OPERATORS

#endif // USE_CUSTOM_MIME_DATA

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{
} // end namespace 'mvd'

#endif // mvdTreeWidget_h
