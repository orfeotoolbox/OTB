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
#ifndef __mvdPixelDescriptionWidget_h
#define __mvdPixelDescriptionWidget_h

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
class PixelDescriptionWidget;
};


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class PixelDescriptionWidget
 *
 * \brief Widget to show current pixel under cursor description 
 */
class Monteverdi2_EXPORT PixelDescriptionWidget :
    public QWidget
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:

  /** \brief Constructor. */
  PixelDescriptionWidget( QWidget* parent =NULL, Qt::WindowFlags flags =0 );

  /** \brief Destructor. */
  virtual ~PixelDescriptionWidget();

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

#endif // __mvdPixelDescriptionWidget_h
