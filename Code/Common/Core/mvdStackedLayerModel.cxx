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
#include <algorithm>
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

  emit LayerAdded( m_Keys.size() - 1 );

  return key;
}

/*****************************************************************************/
void
StackedLayerModel
::Clear()
{
  bool emitSignal = !IsEmpty() && m_Current<GetCount();


  if( emitSignal )
    {
    emit SelectionAboutToBeChanged( StackedLayerModel::NIL_INDEX );
    emit AboutToChangeSelectedLayerModel( KeyType() );
    emit ContentAboutToBeReset();
    }


  for( LayerModelMap::iterator it( m_LayerModels.begin() );
       it!=m_LayerModels.end();
       ++it )
    {
    assert( it->second!=NULL );

    if( it->second->parent()==this )
      {
      delete it->second;
      it->second = NULL;
      }
    }

  m_LayerModels.clear();
  m_Keys.clear();

  m_Current = StackedLayerModel::NIL_INDEX;


  if( emitSignal )
    {
    emit SelectionChanged( StackedLayerModel::NIL_INDEX );
    emit SelectedLayerModelChanged( KeyType() );
    emit ContentReset();
    }
}

/*****************************************************************************/
bool
StackedLayerModel
::Contains( const AbstractLayerModel * layerModel ) const
{
  for( LayerModelMap::const_iterator it( m_LayerModels.begin() );
       it!=m_LayerModels.end();
       ++it )
    if( it->second==layerModel )
      return true;

  return false;
}

/*****************************************************************************/
void
StackedLayerModel
::Delete( SizeType index )
{
  // qDebug() << this << "::Delete(" << index << ")";


  //
  // Check content.
  if( IsEmpty() )
    return;

  if( index>=GetCount() )
    return;

  assert( !m_Keys[ index ].empty() );

  //
  // Find item.
  LayerModelMap::iterator it(
    m_LayerModels.find( m_Keys[ index ] )
  );

  assert( it!=m_LayerModels.end() );

  //
  // Emit about to change selected item.
  bool emitSelectionChanged = m_Current<GetCount() && index<=m_Current;

  if( emitSelectionChanged )
    {
    KeyType key( GetKey( m_Current + 1 ) );

    emit SelectionAboutToBeChanged(
      key.empty()
      ? StackedLayerModel::NIL_INDEX
      : m_Current + 1
    );

    emit AboutToChangeSelectedLayerModel( key );
    }

  //
  // Remove item.
  if( it->second->parent()==this )
    {
    delete it->second;
    it->second = NULL;
    }

  m_LayerModels.erase( it );

  m_Keys.erase( m_Keys.begin() + index );


  //
  // Emit selected item changed.
  if( emitSelectionChanged )
    {
    if( m_Current>=GetCount() )
      m_Current = StackedLayerModel::NIL_INDEX;

    emit LayerDeleted( index );
    emit SelectionChanged( m_Current );
    emit SelectedLayerModelChanged( GetKey( m_Current ) );
    }
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

/*****************************************************************************/
void
StackedLayerModel
::LowerLayer( SizeType index )
{
  assert( GetCount()>1 );
  assert( index<GetCount() );

  SizeType next = Next( index );

  emit OrderAboutToBeChanged();
  {
  std::swap(
    *( m_Keys.begin() + index ),
    *( m_Keys.begin() + next )
  );
  }
  emit OrderChanged();

  emit SelectionAboutToBeChanged( next );
  {
  m_Current = next;
  }
  emit SelectionChanged( next );

  // qDebug() << "current:" << index;
}

/*****************************************************************************/
void
StackedLayerModel
::RaiseLayer( SizeType index )
{
  assert( GetCount()>1 );
  assert( index<GetCount() );

  SizeType prev = Prev( index );

  emit OrderAboutToBeChanged();
  {
  std::swap(
    *( m_Keys.begin() + index ),
    *( m_Keys.begin() + prev )
  );
  }
  emit OrderChanged();

  emit SelectionAboutToBeChanged( prev );
  {
  m_Current = prev;
  }
  emit SelectionChanged( prev );

  // qDebug() << "current:" << index;
}

/*****************************************************************************/
void
StackedLayerModel
::RotateLayerUp( SizeType index )
{
  assert( GetCount()>1 );
  assert( index<GetCount() );

  KeyType key( GetKey( m_Current ) );

  emit OrderAboutToBeChanged();
  {
  std::rotate( m_Keys.begin(), m_Keys.begin() + index, m_Keys.end() );
  }
  emit OrderChanged();

  if( !key.empty() )
    {
    SizeType current = FindKey( key );

    assert( current!=StackedLayerModel::NIL_INDEX );

    emit SelectionAboutToBeChanged( current );
    {
      m_Current = current;
    }
    emit SelectionChanged( m_Current );
    }

  // qDebug() << "current:" << index;
}

/*****************************************************************************/
void
StackedLayerModel
::RotateLayerDown( SizeType index )
{
  if( GetCount()<2 )
    return;

  if( index>=GetCount() )
    return;

  KeyType key( GetKey( m_Current ) );

  emit OrderAboutToBeChanged();
  {
  std::rotate( m_Keys.rbegin(), m_Keys.rbegin() + index, m_Keys.rend()  );
  }
  emit OrderChanged();

  if( !key.empty() )
    {
    SizeType current = FindKey( key );

    assert( current!=StackedLayerModel::NIL_INDEX );

    emit SelectionAboutToBeChanged( current );
    {
    m_Current = current;
    }
    emit SelectionChanged( current );
    }

  // qDebug() << "current:" << index;
}

/*****************************************************************************/
void
StackedLayerModel
::SetCurrent( AbstractLayerModel * layerModel )
{
  for( LayerModelMap::const_iterator it( m_LayerModels.begin() );
       it!=m_LayerModels.end();
       ++it )
    if( it->second==layerModel )
      {
      SetCurrent( it->first );
      return;
      }

  SetCurrent( StackedLayerModel::NIL_INDEX );
}

/*****************************************************************************/
void
StackedLayerModel
::SetCurrent( const KeyType & key )
{
  if( key==GetCurrentKey() )
    return;

  if( key==StackedLayerModel::NIL_KEY )
    {
    SetCurrent( StackedLayerModel::NIL_INDEX );

    return;
    }

  for( SizeType i=0; i<m_Keys.size(); ++i )
    if( m_Keys[ i ]==key )
      {
      SetCurrent( i );
      return;
      }
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
