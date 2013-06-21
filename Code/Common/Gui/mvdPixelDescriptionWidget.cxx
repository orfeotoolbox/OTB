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
#include "Gui/mvdPixelDescriptionWidget.h"
#include "Gui/ui_mvdPixelDescriptionWidget.h"


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

//
// Monteverdi includes (sorted by alphabetic order)
#include "Core/mvdAlgorithm.h"

namespace mvd
{
/*
  TRANSLATOR mvd::PixelDescriptionWidget

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
PixelDescriptionWidget
::PixelDescriptionWidget( QWidget* parent, Qt::WindowFlags flags  ):
  QWidget( parent, flags ),
  m_UI( new mvd::Ui::PixelDescriptionWidget() )
{
  m_UI->setupUi( this );

  SetupUI();
}

/*******************************************************************************/
PixelDescriptionWidget
::~PixelDescriptionWidget()
{
}

/*******************************************************************************/
void
PixelDescriptionWidget
::SetupUI()
{
  //
  // Cartographic coordiantes
  m_CartographicRootItem = new QTreeWidgetItem( GetDescriptionTree() ); 
  m_CartographicRootItem->setText(0, tr("Cartographic"));
  m_CartographicRootItem->setExpanded(true);

  m_CartographicItem = new QTreeWidgetItem( m_CartographicRootItem );
  m_CartographicItem->setText(0, tr("Coordinates"));

  //
  // Geographic coordiantes
  m_GeographicRootItem = new QTreeWidgetItem( GetDescriptionTree() );
  m_GeographicRootItem->setText(0, tr("Geographic"));
  m_GeographicRootItem->setExpanded(true);

  //m_GeographicItem = new QTreeWidgetItem( m_GeographicRootItem );  
  //m_GeographicItem->setText(0, tr("Coordinates"));

  //
  // Child items will be created + updated in a dedicated slot
  m_PixelValueRootItem = new QTreeWidgetItem( GetDescriptionTree() ); 
  m_PixelValueRootItem->setText(0, tr("Pixel Values"));
  m_PixelValueRootItem->setExpanded(true);
}

/*******************************************************************************/
QTreeWidget *
PixelDescriptionWidget
::GetDescriptionTree()
{
  return m_UI->m_DescriptionTree;
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void
PixelDescriptionWidget
::OnCurrentPhysicalUpdated(const QString & currentPhysical)
{
  m_CartographicItem->setText(1, currentPhysical);
}

/*******************************************************************************/
void
PixelDescriptionWidget
::OnCurrentGeographicUpdated(const QStringList&/*const QString &*/ currentGeo)
{
  // remove the previous QTreeWidgetItem  of m_GeographicRootItem
  while( m_GeographicRootItem->childCount()>0 )
    {
    // Remove QTreeWidgetItem
    QTreeWidgetItem* child = m_GeographicRootItem->takeChild( 0 );

    // Delete it from memory.
    delete child;
    child = NULL;
    }

  if (!currentGeo.empty())
    {
  // fill with the new values
  QTreeWidgetItem * iGeoLongItem = new QTreeWidgetItem( m_GeographicRootItem );
  iGeoLongItem->setText(0,QString( tr("Long") ));
  iGeoLongItem->setText(1, QString("%1").arg(currentGeo[0] ) );

  QTreeWidgetItem * iGeoLatItem = new QTreeWidgetItem( m_GeographicRootItem );
  iGeoLatItem->setText(0,QString( tr("Lat") ));
  iGeoLatItem->setText(1, QString("%1").arg(currentGeo[1] ) );

  QTreeWidgetItem * iGeoElevationItem = new QTreeWidgetItem( m_GeographicRootItem );
  iGeoElevationItem->setText(0,QString( tr("Elevation") ));
  iGeoElevationItem->setText(1, QString("%1").arg(currentGeo[2] ) );
    }
}                       
/*******************************************************************************/
void
PixelDescriptionWidget
::OnCurrentPixelValueUpdated(const VectorImageType::PixelType & currentPixel, 
                             const QStringList& bandNames)
{
  //
  // remove the previous QTreeWidgetItem  of m_PixelValueRootItem
  while( m_PixelValueRootItem->childCount()>0 )
    {
    // Remove QTreeWidgetItem
    QTreeWidgetItem* child = m_PixelValueRootItem->takeChild( 0 );

    // Delete it from memory.
    delete child;
    child = NULL;
    }

  // fill with the new values
  for (unsigned int idx = 0; idx < currentPixel.GetSize(); idx++)
    {
    QTreeWidgetItem * iBandItem = new QTreeWidgetItem( m_PixelValueRootItem );

    // figure out if a band name is available, if not use Band idx
    if( !bandNames[ idx ].isEmpty() &&
	static_cast< unsigned int >( bandNames.size() )==currentPixel.GetSize() )
      {
      iBandItem->setText(0, bandNames[ idx ] );
      }
    else
      {
      iBandItem->setText(0, QString( tr("Band") )+ QString(" %1").arg(idx) );
      }
    // set the value
    iBandItem->setText(1, QString("%1").arg(currentPixel.GetElement( idx )) );
    }
}

/*******************************************************************************/

} // end namespace 'mvd'
