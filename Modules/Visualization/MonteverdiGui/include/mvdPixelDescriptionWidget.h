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

#ifndef mvdPixelDescriptionWidget_h
#define mvdPixelDescriptionWidget_h

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
namespace Ui
{
class PixelDescriptionWidget;
};


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class PixelDescriptionWidget
 *
 * \ingroup OTBMonteverdiGUI
 *
 * \brief Widget to show current pixel under cursor description
 */
class OTBMonteverdiGUI_EXPORT PixelDescriptionWidget :
    public QWidget
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:

  /** \brief Constructor. */
  PixelDescriptionWidget( QWidget* p =NULL, Qt::WindowFlags flags =0 );

  /** \brief Destructor. */
  ~PixelDescriptionWidget() override;

  /** Get TreeWidget */
  QTreeWidget * GetDescriptionTree();

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

//
// Public SLOTS.
public slots:

  void OnCurrentPhysicalUpdated(const QStringList&);

  void OnCurrentGeographicUpdated(const QStringList& /*currentGeo const QString &*/);

  void OnCurrentPixelValueUpdated(const VectorImageType::PixelType &,
                                  const QStringList& bandNames);

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

  void SetupUI();

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Protected attributes.
protected:

//
// Private methods.
private:

//
// Private attributes.
private:
  /**
   * \brief uic generated.
   */
  Ui::PixelDescriptionWidget* m_UI;

  // /**
  //  * \brief map storing applications and its tags
  //  */
  // ApplicationsTagContainer m_AppTags;

  // /**
  //  * \brief map storing applications docName and appName association
  //  */
  // ApplicationDocNameToNameMap m_AppsDocNameToNameMap;

  /**
   * \brief text to search in the widget tree
   */
  QString                  m_SearchText;

  /**
   * \brief Root items
   */
  QTreeWidgetItem*    m_CartographicRootItem;
  QTreeWidgetItem*    m_GeographicRootItem;
  QTreeWidgetItem*    m_PixelValueRootItem;

   /**
   * \brief child items
   */
  QTreeWidgetItem*    m_CartographicItem;
  QTreeWidgetItem*    m_GeographicItem;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:
};

/*****************************************************************************/
/* INLINE SECTION                                                            */

} // end namespace 'mvd'

#endif // mvdPixelDescriptionWidget_h
