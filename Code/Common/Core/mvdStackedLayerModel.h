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
#ifndef __mvdStackedLayerModel_h
#define __mvdStackedLayerModel_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtCore>

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdAbstractModel.h"

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
class AbstractLayerModel;

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class StackedLayerModel
 *
 * \brief WIP.
 */
class Monteverdi2_EXPORT StackedLayerModel :
    public AbstractModel
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

//
// Private types
private:
  // typedef std::vector< AbstractLayerModel * > LayerModelVector;
  typedef std::map< std::string, AbstractLayerModel * > LayerModelMap;

  // typedef std::pair< AbstractLayerModel *, void * > LayerModelPair;
  // typedef std::map< std::string, LayerDataPair > LayerModelPairMap;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public types.
public:
  typedef std::vector< LayerModelMap::key_type > KeyVector;

  typedef LayerModelMap::size_type SizeType;
  typedef LayerModelMap::key_type KeyType;
  typedef LayerModelMap::const_iterator ConstIterator;

//
// Public methods.
public:

  /** \brief Constructor. */
  StackedLayerModel( QObject* parent =NULL );

  /** \brief Destructor. */
  virtual ~StackedLayerModel();

  /*
  inline const AbstractLayerModel * operator[]( SizeType ) const;
  inline AbstractLayerModel * operator[]( SizeType );
  */

  KeyType Add( AbstractLayerModel * );

  inline ConstIterator Begin() const;

  void Clear();

  inline bool Contains( const KeyType & key ) const;
  bool Contains( const AbstractLayerModel * ) const;

  inline SizeType GetCount() const;

  inline const KeyVector & GetKeys() const;

  inline ConstIterator End() const;

  /*
  inline const AbstractLayerModel * Front() const;
  inline AbstractLayerModel * Front();
  */

  template< typename T >
    T * GetCurrent() const;

  template< typename T >
    T * GetCurrent();

  AbstractLayerModel * GetCurrent() const;
  AbstractLayerModel * GetCurrent();

  inline const KeyType & GetCurrentKey() const;

  inline bool IsEmpty() const;

  void SetCurrent( const KeyType & );
  void SetCurrent( AbstractLayerModel * );

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

//
// Public SLOTS.
public slots:
  
  inline void SelectPrevious();
  inline void SelectNext();
  inline void RotateDown();
  inline void RotateUp();
  inline void DeleteCurrent();

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:
  void AboutToChangeSelectedLayerModel( const StackedLayerModel::KeyType & );
  void SelectedLayerModelChanged( const StackedLayerModel::KeyType & );

  /*
  void AboutToChangeSelectedLayerModel( const AbstractLayerModel * );
  void SelectedLayerModelChanged( AbstractLayerModel * );
  */

  void StackOrderChanged();

  void StackContentChanged();

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

//
// Protected attributes.
protected:

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Private methods.
private:
  static KeyType GenerateKey( AbstractLayerModel * );

  void Delete( SizeType );

  inline const KeyType & GetKey( SizeType ) const;

  inline SizeType Next( SizeType );
  inline SizeType Prev( SizeType );

  inline void SetCurrent( SizeType );
  inline void Swap( SizeType, SizeType );

//
// Private attributes.
private:
  static const KeyType NIL_KEY;
  static const SizeType NIL_INDEX;

  static SizeType m_LayerCount;

  LayerModelMap m_LayerModels;
  KeyVector m_Keys;
  SizeType m_Current;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:
};

} // end namespace 'mvd'.

/*****************************************************************************/
/* INLINE SECTION                                                            */

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

namespace mvd
{

/*****************************************************************************/
/*
inline
const AbstractLayerModel *
StackedLayerModel
::operator[]( SizeType i ) const
{
  return m_LayerModels[ i ];
}
*/

/*****************************************************************************/
/*
inline
AbstractLayerModel *
StackedLayerModel
::operator[]( SizeType i )
{
  return m_LayerModels[ i ];
}
*/

/*****************************************************************************/
inline
StackedLayerModel::ConstIterator
StackedLayerModel
::Begin() const
{
  return m_LayerModels.begin();
}

/*****************************************************************************/
inline
bool
StackedLayerModel
::Contains( const KeyType & key ) const
{
  return m_LayerModels.find( key )!=m_LayerModels.end();
}

/*****************************************************************************/
inline
void
StackedLayerModel
::DeleteCurrent()
{
  qDebug() << this << "::DeleteCurrent()";

  Delete( m_Current );
}

/*****************************************************************************/
inline
StackedLayerModel::SizeType
StackedLayerModel
::GetCount() const
{
  return m_LayerModels.size();
}

/*****************************************************************************/
inline
const StackedLayerModel::KeyVector &
StackedLayerModel
::GetKeys() const
{
  return m_Keys;
}

/*****************************************************************************/
inline
StackedLayerModel::ConstIterator
StackedLayerModel
::End() const
{
  return m_LayerModels.end();
}

/*****************************************************************************/
/*
inline
AbstractLayerModel *
StackedLayerModel
::Front()
{
  return m_LayerModels.front();
}
*/

/*****************************************************************************/
/*
inline
const AbstractLayerModel *
StackedLayerModel
::Front() const
{
  return m_LayerModels.front();
}
*/

/*****************************************************************************/
template< typename T >
inline
T *
StackedLayerModel
::GetCurrent()
{
  return dynamic_cast< T * >( GetCurrent() );
}

/*****************************************************************************/
template< typename T >
inline
T *
StackedLayerModel
::GetCurrent() const
{
  return dynamic_cast< T * >( GetCurrent() );
}

/*****************************************************************************/
inline
AbstractLayerModel *
StackedLayerModel
::GetCurrent()
{
  if( m_Current>=GetCount() )
    return NULL;

  LayerModelMap::const_iterator it(
    m_LayerModels.find( m_Keys[ m_Current ] )
  );

  if( it==m_LayerModels.end() )
    return NULL;

  return it->second;
}

/*****************************************************************************/
inline
AbstractLayerModel *
StackedLayerModel
::GetCurrent() const
{
  return const_cast< StackedLayerModel * >( this )->GetCurrent();
}

/*****************************************************************************/
inline
const StackedLayerModel::KeyType &
StackedLayerModel
::GetCurrentKey() const
{
  return GetKey( m_Current );
}

/*****************************************************************************/
inline
const StackedLayerModel::KeyType &
StackedLayerModel
::GetKey( SizeType index ) const
{
  if( index>=GetCount() )
    return StackedLayerModel::NIL_KEY;

  return m_Keys[ index ];
}

/*****************************************************************************/
inline
bool
StackedLayerModel
::IsEmpty() const
{
  return m_LayerModels.empty();
}

/*****************************************************************************/
inline
StackedLayerModel::SizeType
StackedLayerModel
::Next( SizeType index )
{
  return
    index>=GetCount()
    ? 0
    : ( index + 1 ) % GetCount();
}

/*****************************************************************************/
inline
StackedLayerModel::SizeType
StackedLayerModel
::Prev( SizeType index )
{
  return
    index==StackedLayerModel::NIL_INDEX || index<1
    ? GetCount() - 1
    : index - 1;
}

/*****************************************************************************/
inline
void
StackedLayerModel
::RotateDown()
{
  if( IsEmpty() )
    return;

  if( m_Current>=GetCount() )
    return;

  SizeType index = Next( m_Current );

  Swap( m_Current, index );

  m_Current = index;
}

/*****************************************************************************/
inline
void
StackedLayerModel
::RotateUp()
{
  if( IsEmpty() )
    return;

  if( m_Current>=GetCount() )
    return;

  SizeType index = Prev( m_Current );

  Swap( m_Current, index );

  m_Current = index;
}

/*****************************************************************************/
inline
void
StackedLayerModel
::SelectNext()
{
  if( IsEmpty() )
    return;

  SetCurrent( Next( m_Current ) );
}

/*****************************************************************************/
inline
void
StackedLayerModel
::SelectPrevious()
{
  if( IsEmpty() )
    return;

  SetCurrent( Prev( m_Current ) );
}

/*****************************************************************************/
inline
void
StackedLayerModel
::SetCurrent( SizeType index )
{
  // qDebug() << this << "::SetCurrentIndex(" << index << ")";

  if( index==m_Current )
    return;

  KeyType key( GetKey( index  ) );

  // qDebug()
  //   << QString( "'%1'" ).arg( GetCurrentKey().c_str() )
  //   << "->"
  //   << QString( "'%1'" ).arg( key.c_str() );

  emit AboutToChangeSelectedLayerModel( key );

  m_Current = index;

  emit SelectedLayerModelChanged( key );
}

/*****************************************************************************/
inline
void
StackedLayerModel
::Swap( SizeType i1, SizeType i2 )
{
  // qDebug()
  //   << "Swapping:"
  //   << QString( "%1" ).arg( m_Keys[ i1 ].c_str() )
  //   << "<->"
  //   << QString( "%1" ).arg( m_Keys[ i2 ].c_str() );

  KeyType tmp( m_Keys[ i1 ] );

  m_Keys[ i1 ] = m_Keys[ i2 ];
  m_Keys[ i2 ] = tmp;
}

} // end namespace 'mvd'

#endif // __mvdStackedLayerModel_h
