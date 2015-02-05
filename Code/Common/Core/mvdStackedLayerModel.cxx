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
  m_Current( StackedLayerModel::NIL_INDEX ),
  m_Reference( StackedLayerModel::NIL_INDEX )
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

  emit ContentAboutToBeChanged();

  m_LayerModels.insert( LayerModelMap::value_type( key, model ) );
  m_Keys.push_back( key );

  emit LayerAdded( m_Keys.size() - 1 );
  emit ContentChanged();

  return key;
}

/*****************************************************************************/
void
StackedLayerModel
::Clear()
{
  bool emitSignal1 = !IsEmpty() && m_Current<GetCount();
  bool emitSignal2 = !IsEmpty() && m_Reference<GetCount();


  if( emitSignal1 )
    {
    emit CurrentAboutToBeChanged( StackedLayerModel::NIL_INDEX );
    emit AboutToChangeSelectedLayerModel( KeyType() );
    emit ContentAboutToBeReset();
    }

  if( emitSignal2 )
    emit ReferenceAboutToBeChanged( StackedLayerModel::NIL_INDEX );


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


  if( emitSignal1 )
    {
    emit CurrentChanged( StackedLayerModel::NIL_INDEX );
    emit SelectedLayerModelChanged( KeyType() );
    emit ContentReset();
    }

  if( emitSignal2 )
    emit ReferenceChanged( StackedLayerModel::NIL_INDEX );
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

  emit ContentAboutToBeChanged();

  //
  // Emit about to change current item.
  bool emitCurrentChanged = m_Current<GetCount() && index<=m_Current;

  if( emitCurrentChanged )
    {
    KeyType key( GetKey( m_Current + 1 ) );

    emit CurrentAboutToBeChanged(
      key.empty()
      ? StackedLayerModel::NIL_INDEX
      : m_Current + 1
    );

    emit AboutToChangeSelectedLayerModel( key );
    }

  //
  // Emit about to change reference item.
  bool emitReferenceChanged = m_Reference<GetCount() && index<=m_Reference;

  if( emitReferenceChanged )
    emit ReferenceAboutToBeChanged(
      GetKey( m_Reference + 1 ).empty()
      ? StackedLayerModel::NIL_INDEX
      : m_Reference + 1
    );

  //
  // Remove item.
  if( it->second->parent()==this )
    {
    delete it->second;
    it->second = NULL;
    }

  m_LayerModels.erase( it );

  m_Keys.erase( m_Keys.begin() + index );

  emit LayerDeleted( index );
  emit ContentChanged();

  //
  // Emit selected item changed.
  if( emitCurrentChanged )
    {
    if( m_Current>=GetCount() )
      m_Current = StackedLayerModel::NIL_INDEX;

    emit CurrentChanged( m_Current );
    emit SelectedLayerModelChanged( GetKey( m_Current ) );
    }

  //
  // Emit selected item changed.
  if( emitReferenceChanged )
    {
    if( m_Reference>=GetCount() )
      m_Reference = StackedLayerModel::NIL_INDEX;

    emit ReferenceChanged( m_Reference );
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

  emit CurrentAboutToBeChanged( next );
  {
  m_Current = next;
  }
  emit CurrentChanged( next );

  SetReference(
    m_Reference==next
    ? index
    : ( m_Reference==index
        ? next
        : m_Reference )
  );
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

  emit CurrentAboutToBeChanged( prev );
  {
  m_Current = prev;
  }
  emit CurrentChanged( prev );

  SetReference(
    m_Reference==prev
    ? index
    : ( m_Reference==index
        ? prev
        : m_Reference )
  );
}

/*****************************************************************************/
void
StackedLayerModel
::RotateLayerUp( SizeType index )
{
  assert( GetCount()>1 );
  assert( index<GetCount() );

  KeyType currentKey( GetKey( m_Current ) );
  KeyType referenceKey( GetKey( m_Reference ) );

  emit OrderAboutToBeChanged();
  {
  std::rotate( m_Keys.begin(), m_Keys.begin() + index, m_Keys.end() );
  }
  emit OrderChanged();

  if( !currentKey.empty() )
    {
    SizeType current = FindKey( currentKey );

    assert( current!=StackedLayerModel::NIL_INDEX );

    emit CurrentAboutToBeChanged( current );
    {
      m_Current = current;
    }
    emit CurrentChanged( m_Current );
    }

  if( !referenceKey.empty() )
    {
    SizeType reference = FindKey( referenceKey );

    assert( reference!=StackedLayerModel::NIL_INDEX );

    emit ReferenceAboutToBeChanged( reference );
    {
      m_Reference = reference;
    }
    emit ReferenceChanged( m_Reference );
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

  KeyType currentKey( GetKey( m_Current ) );
  KeyType referenceKey( GetKey( m_Reference ) );

  emit OrderAboutToBeChanged();
  {
  std::rotate( m_Keys.rbegin(), m_Keys.rbegin() + index, m_Keys.rend()  );
  }
  emit OrderChanged();

  if( !currentKey.empty() )
    {
    SizeType current = FindKey( currentKey );

    assert( current!=StackedLayerModel::NIL_INDEX );

    emit CurrentAboutToBeChanged( current );
    {
    m_Current = current;
    }
    emit CurrentChanged( current );
    }

  if( !referenceKey.empty() )
    {
    SizeType reference = FindKey( referenceKey );

    assert( reference!=StackedLayerModel::NIL_INDEX );

    emit ReferenceAboutToBeChanged( reference );
    {
    m_Reference = reference;
    }
    emit ReferenceChanged( reference );
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

/*****************************************************************************/
void
StackedLayerModel
::SetReference( const KeyType & key )
{
  if( key==GetKey( m_Reference ) )
    return;

  if( key==StackedLayerModel::NIL_KEY )
    {
    SetReference( StackedLayerModel::NIL_INDEX );

    return;
    }

  for( SizeType i=0; i<m_Keys.size(); ++i )
    if( m_Keys[ i ]==key )
      {
      SetReference( i );
      return;
      }
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
