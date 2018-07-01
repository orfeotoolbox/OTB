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

#ifndef otbWrapperQtWidgetListEditWidget_h
#define otbWrapperQtWidgetListEditWidget_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "otbConfigure.h"


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
#include "OTBQtWidgetExport.h"


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
class ListEditItemModel;
class StringListInterface;

namespace Ui
{
class ListEditWidget;
};


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class ListEditWidget
 *
 * \ingroup OTBQtWidget
 *
 * \brief
 */
class OTBQtWidget_EXPORT ListEditWidget :
    public QWidget
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:

  /** \brief Constructor. */
  ListEditWidget( StringListInterface *,
		  QWidget * p =NULL,
		  Qt::WindowFlags flags =0 );

  /** \brief Destructor. */
  virtual ~ListEditWidget();

  /**
   */
  const ListEditItemModel * GetItemModel() const;

  /**
   */
  ListEditItemModel * GetItemModel();

#if 0

  /** */
  void SetBrowseEnabled( bool );

  /** */
  bool IsBrowseEnabled() const;
#endif

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

//
// Public SLOTS.
public slots:
  void OnFilenameDropped(const QString &);
  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:
  /** */
  void Updated();

  void ValueChanged();
  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Protected attributes.
protected:

//
// Private types.
private:
  enum SwapSelection
  {
    LEFT = -1,
    NONE = 0,
    RIGHT = +1,
  };

//
// Private methods.
private:
  void Swap( int, int, SwapSelection = NONE );

  QStringList browseFilenames( bool multi = false, const QString & filename = QString());

  QString browseFilename( const QModelIndex & );

//
// Private attributes.
private:
  /**
   * \brief uic generated.
   */
  Ui::ListEditWidget * m_UI;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:
  void on_addButton_clicked();
  void on_browseButton_clicked();
  void on_downButton_clicked();
  void on_removeButton_clicked();
  void on_removeAllButton_clicked();
  void on_upButton_clicked();

  void OnDataChanged( const QModelIndex &, const QModelIndex & );
  void OnModelReset();
  void OnRowsInserted( const QModelIndex &, int, int );
  void OnRowsRemoved( const QModelIndex &, int, int );
  void OnSelectionChanged( const QItemSelection &, const QItemSelection & );
};

} // end namespace 'Wrapper'

} // end namespace 'otb'

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace Wrapper
{
} // end namespace 'Wrapper'

#endif // otbWrappersQtWidgetListEditWidget_h
