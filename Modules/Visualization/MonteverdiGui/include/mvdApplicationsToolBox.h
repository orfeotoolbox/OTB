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

#ifndef mvdApplicationsToolBox_h
#define mvdApplicationsToolBox_h

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


#define USE_ALGORITHM_ROOT_NODE 0


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
class ApplicationsToolBox;
};

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class ApplicationsToolBox
 *
 * \ingroup OTBMonteverdiGUI
 *
 * \brief Widget template skeleton to copy-paste when adding a new
 * widget class.
 */
class OTBMonteverdiGUI_EXPORT ApplicationsToolBox :
    public QWidget
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:

  /** \brief Constructor. */
  ApplicationsToolBox( QWidget* p =NULL, Qt::WindowFlags flags =0 );

  /** \brief Destructor. */
  ~ApplicationsToolBox() override;

  /** Get TreeWidget */
  // QTreeWidget * GetAlgorithmsTree();

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

//
// Public SLOTS.
public slots:
  void OnAvailableApplicationsTagsChanged( const ApplicationsTagContainer& appsTags,
                                           const ApplicationDocNameToNameMap& docNameToNameMap);
  void OnSearchBoxChanged( const QString & search );
  void OnAlgorithmTreeDoubleClick( QTreeWidgetItem * item , int column );

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:
  void ApplicationToLaunchSelected(const QString &, const QString &);

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
   * \brief fill the widget tree using the tags map
   */
  void FillTreeUsingTags();

  /**
   * \brief Launch the appName algorithm
   */
  void LaunchApplication( const QString& appName );

  /**
   * \brief Helper method to check if searchText matches any of the
   * algorithms string contained in the map
   */
  bool IsSearchTextMatchAnyAlgorithm( const QString & tagName,
				      const QString & search );

  /**
   * \brief Helper method to get the DocName of an application from an
   * application name
   */
  QString GetApplicationDocNameByApplicationName( const QString & appName );


//
// Private attributes.
private:
  /**
   * \brief uic generated.
   */
  Ui::ApplicationsToolBox* m_UI;

  /**
   * \brief map storing applications and its tags
   */
  ApplicationsTagContainer m_AppTags;

  /**
   * \brief map storing applications docName and appName association
   */
  ApplicationDocNameToNameMap m_AppsDocNameToNameMap;

  /**
   * \brief text to search in the widget tree
   */
  QString m_SearchText;


  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:
};

/*****************************************************************************/
/* INLINE SECTION                                                            */

} // end namespace 'mvd'

#endif // mvdApplicationsToolBox_h
