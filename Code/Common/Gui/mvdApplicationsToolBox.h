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
#ifndef __mvdApplicationsToolBox_h
#define __mvdApplicationsToolBox_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtGui>

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "Core/mvdTypes.h"

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
 * \brief Widget template skeleton to copy-paste when adding a new
 * widget class.
 */
class Monteverdi2_EXPORT ApplicationsToolBox :
    public QWidget
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:

  /** \brief Constructor. */
  ApplicationsToolBox( QWidget* parent =NULL, Qt::WindowFlags flags =0 );

  /** \brief Destructor. */
  virtual ~ApplicationsToolBox();

  /** Get TreeWidget */
  QTreeWidget * GetAlgorithmsTree();

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
  void ApplicationToLaunchSelected(const QString &);

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
  bool IsSearchTextMatchAnyAlgorithm( const QString & tagName );

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
  QString                  m_SearchText;  

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:
};

/*****************************************************************************/
/* INLINE SECTION                                                            */

} // end namespace 'mvd'

#endif // __mvdApplicationsToolBox_h
