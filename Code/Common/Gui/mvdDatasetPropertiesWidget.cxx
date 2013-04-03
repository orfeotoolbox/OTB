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
#include "Gui/mvdDatasetPropertiesWidget.h"
#include "Gui/ui_mvdDatasetPropertiesWidget.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)
#include <cassert>

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)


namespace mvd
{

/*
  TRANSLATOR mvd::DatasetPropertiesWidget

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
DatasetPropertiesWidget
::DatasetPropertiesWidget( QWidget* parent, Qt::WindowFlags flags  ):
  QWidget( parent, flags ),
  m_UI( new mvd::Ui::DatasetPropertiesWidget() )
{
  m_UI->setupUi( this );

  SetupUI();
}

/*******************************************************************************/
DatasetPropertiesWidget
::~DatasetPropertiesWidget()
{
}


/*******************************************************************************/
void
DatasetPropertiesWidget
::SetupUI()
{
}

/*******************************************************************************/
QTreeWidget *
DatasetPropertiesWidget
::GetPropertiesTree()
{
  return m_UI->m_DatasetPropertiesTree;
}

/*******************************************************************************/
void
DatasetPropertiesWidget
::FillDatasetProperties(const PropertiesContainer& props  )
{
  //
  // clear the properties tree
  GetPropertiesTree()->clear();

  //
  // iterate on the properties and set as main item the key/ second
  // item the value of the key
  if ( props.size() > 0 )
    {
    PropertiesContainer::const_iterator itCat = props.begin();

    while( itCat != props.end() )
      {
      //
      // current category of properties
      QString  qcurrentCategory( (*itCat).first.c_str() );

      // step #1 ->  category label as main item in the tree
      QTreeWidgetItem * masterItem = new QTreeWidgetItem( GetPropertiesTree() );
      masterItem->setText(0,  qcurrentCategory );
      masterItem->setExpanded( true );
      
      // step # 2 -> iterate on the current category properties
      PropertiesVector::const_iterator itProp = (*itCat).second.begin();
      while( itProp != (*itCat).second.end() )
        {        
        // property description
        QString  qcurrentPropDesc( (*itProp).first.c_str() );              

        // property value
        QString  qcurrentPropValue( (*itProp).second.c_str() );            

        //
        // fill the curent QTreeWidgetItem columns
        QTreeWidgetItem * propItem = new QTreeWidgetItem( masterItem );
        propItem->setText(0,  qcurrentPropDesc );
        propItem->setText(1,  qcurrentPropValue );
        
        ++itProp;
        }

      // iterate
      ++itCat;
      }
    }
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
