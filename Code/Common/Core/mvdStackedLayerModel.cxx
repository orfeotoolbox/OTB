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
#include "mvdStackedLayerModel.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)
// #include <stdexcept>
#include <sstream>

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "Core/mvdAbstractLayerModel.h"
#include "Core/mvdAlgorithm.h"

namespace mvd
{
/*
  TRANSLATOR mvd::StackedLayerModel

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */

namespace
{
} // end of anonymous namespace.


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */

StackedLayerModel::SizeType
StackedLayerModel::m_LayerCount = 0;

const StackedLayerModel::KeyType
StackedLayerModel::NIL_KEY;

const StackedLayerModel::SizeType
StackedLayerModel::NIL_INDEX = -1;

/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
StackedLayerModel
::StackedLayerModel( QObject* parent ) :
  AbstractModel( parent ),
  m_LayerModels(),
  m_Keys(),
  m_Current( StackedLayerModel::NIL_INDEX )
{
}

/*******************************************************************************/
StackedLayerModel
::~StackedLayerModel()
{
}

/*****************************************************************************/
std::string
StackedLayerModel
::Add( AbstractLayerModel * model )
{
  std::string key( StackedLayerModel::GenerateKey( model ) );
  assert( !key.empty() );

  if( key.empty() )
    {
    throw
      std::runtime_error(
        ToStdString(
          tr( "Failed to generate string key for '%1'." )
          .arg( model->metaObject()->className() )
        )
      );
    }

  m_LayerModels.insert( LayerModelMap::value_type( key, model ) );
  m_Keys.push_back( key );

  emit StackContentChanged();

  return key;
}

/*****************************************************************************/
void
StackedLayerModel
::Clear()
{
  for( LayerModelMap::iterator it( m_LayerModels.begin() );
       it!=m_LayerModels.end();
       ++it )
    if( it->second!=NULL && it->second->parent()==this )
      {
      delete it->second;
      it->second = NULL;
      }

  m_LayerModels.clear();
  m_Keys.clear();
  m_Current = StackedLayerModel::NIL_INDEX;
}

/*******************************************************************************/
std::string
StackedLayerModel
::GenerateKey( AbstractLayerModel * layerModel )
{
  std::ostringstream oss;

  oss << "Layer #" << m_LayerCount++;

#ifdef _DEBUG 
  oss << " (" << layerModel->metaObject()->className()
      << "@" << std::hex << layerModel << ")";
#endif

  return oss.str();
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
