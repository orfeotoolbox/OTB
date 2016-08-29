/*=========================================================================

  Program:   Monteverdi
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi is distributed under the CeCILL licence version 2. See
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
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "ConfigureMonteverdi.h"
#endif //tag=QT4-boost-compatibility


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
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "mvdAbstractModel.h"
#endif //tag=QT4-boost-compatibility
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
class AbstractLayerModel;

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class StackedLayerModel
 *
 * \ingroup MonteverdiCore
 *
 * \brief WIP.
 */
class ITK_EXPORT StackedLayerModel :
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
  static const SizeType NIL_INDEX;

//
// Public methods.
public:

  /** \brief Constructor. */
  StackedLayerModel( QObject* p =NULL );

  /** \brief Destructor. */
  virtual ~StackedLayerModel();

  inline const AbstractLayerModel * operator[]( SizeType ) const;
  inline AbstractLayerModel * operator[]( SizeType );

  KeyType Add( AbstractLayerModel * );

  inline const AbstractLayerModel * At( SizeType ) const;
  inline AbstractLayerModel * At( SizeType );

  inline ConstIterator Begin() const;

  inline void BeginEditPixelInfo() {};

  void BeginEditResolutions() {};

  inline bool Contains( const KeyType & key ) const;
  bool Contains( const AbstractLayerModel * ) const;

  void CountSRT( size_t & unknown,
		 size_t & carto,
		 size_t & geo,
		 size_t & sensor ) const;

  inline ConstIterator End() const;

  inline void EndEditPixelInfo( const QPoint &, const PointType & );

  void EndEditResolutions();

  /*
  inline const AbstractLayerModel * Front() const;
  inline AbstractLayerModel * Front();
  */
  inline ConstIterator Find( const AbstractLayerModel * ) const;

  inline const AbstractLayerModel * Get( const KeyType & ) const;
  inline AbstractLayerModel * Get( const KeyType & );

  inline SizeType GetCount() const;

  inline const KeyType & GetKey( SizeType ) const;

  inline const KeyVector & GetKeys() const;

  template< typename T >
    T * GetCurrent() const;

  template< typename T >
    T * GetCurrent();

  AbstractLayerModel * GetCurrent() const;
  AbstractLayerModel * GetCurrent();

  inline SizeType GetCurrentIndex() const;

  inline const KeyType & GetCurrentKey() const;

  template< typename T >
    T * GetReference() const;

  template< typename T >
    T * GetReference();

  AbstractLayerModel * GetReference() const;
  AbstractLayerModel * GetReference();

  inline SizeType GetReferenceIndex() const;

  inline bool HasCurrent() const;

  inline bool HasReference() const;

  inline SizeType IndexOf( const AbstractLayerModel * ) const;

  KeyType Insert( AbstractLayerModel *, SizeType index );

  inline bool IsEmpty() const;

  inline const PixelInfo::Vector & PixelInfos() const;
  inline PixelInfo::Vector & PixelInfos();

  inline void SetCurrent( SizeType, bool =false );
  void SetCurrent( const KeyType & );
  void SetCurrent( const AbstractLayerModel * );

  inline void SetReference( SizeType, bool =false );
  void SetReference( const KeyType & );
  void SetReference( const AbstractLayerModel * );

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

//
// Public SLOTS.
public slots:
  void Clear();
  inline void Delete( const KeyType & );
  inline void DeleteCurrent();
  inline void LowerCurrent();
  inline void MoveCurrentToBottom();
  inline void MoveCurrentToTop();
  inline void RaiseCurrent();
  inline void RotateLayers( int );
  inline void SelectFirst();
  inline void SelectLast();
  inline void SelectPrevious();
  inline void SelectNext();

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:
  void AboutToChangeSelectedLayerModel( const StackedLayerModel::KeyType & );
  void SelectedLayerModelChanged( const StackedLayerModel::KeyType & );

  void ContentAboutToBeChanged();
  void ContentAboutToBeReset();
  void ContentChanged();
  void ContentReset();

  void CurrentAboutToBeChanged( size_t );
  void CurrentChanged( size_t );

  void LayerAboutToBeDeleted( size_t index );
  void LayerAdded( size_t index );
  void LayerDeleted( size_t index );

  void OrderAboutToBeChanged();
  void OrderChanged();

  void PixelInfoChanged( const QPoint &, const PointType &, const PixelInfo::Vector & );

  void ReferenceAboutToBeChanged( size_t );
  void ReferenceChanged( size_t );

  void ResolutionsChanged( const PixelInfo::Vector & );

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

  inline void ClearPixelInfos();

  void Delete( SizeType );

  inline SizeType FindKey( const KeyType & ) const;

  inline SizeType Next( SizeType );
  inline SizeType Prev( SizeType );

  void RaiseLayer( SizeType );
  void LowerLayer( SizeType );

  void RotateLayerUp( SizeType );
  void RotateLayerDown( SizeType );

  void MoveTo( SizeType index, SizeType position );
  void MoveToTop( SizeType );
  void MoveToBottom( SizeType );

  // inline void Swap( SizeType, SizeType );

  inline const KeyType & KeyOf( const AbstractLayerModel * ) const;

//
// Private attributes.
private:
  static const KeyType NIL_KEY;

  static SizeType m_LayerCount;

  LayerModelMap m_LayerModels;
  KeyVector m_Keys;
  SizeType m_Current;
  SizeType m_Reference;

  // Usually, information is a singular term which denotes a
  // plural. Since there's a container of several pixel-infos and we
  // usually name container variables by using the plural form, an 's'
  // is appended to PixelInfo. 
  PixelInfo::Vector m_PixelInfos;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:
};

#if _DEBUG
inline void trace( const std::vector< std::string > & );
#endif

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
inline
const AbstractLayerModel *
StackedLayerModel
::operator[]( SizeType i ) const
{
  return At( i );
}

/*****************************************************************************/
inline
AbstractLayerModel *
StackedLayerModel
::operator[]( SizeType i )
{
  return At( i );
}

/*****************************************************************************/
inline
const AbstractLayerModel *
StackedLayerModel
::At( SizeType i ) const
{
  return const_cast< StackedLayerModel * >( this )->At( i );
}

/*****************************************************************************/
inline
AbstractLayerModel *
StackedLayerModel
::At( SizeType i )
{
  if( i>=GetCount() )
    return NULL;

  assert( !GetKey( i ).empty() );

  LayerModelMap::const_iterator it(
    m_LayerModels.find( GetKey( i ) )
  );

  assert( it!=m_LayerModels.end() );

  return it->second;
}

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
void
StackedLayerModel
::ClearPixelInfos()
{
  m_PixelInfos.reserve( 0 );
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
::Delete( const KeyType & key )
{
  // qDebug() << this << "::Delete(" << key << ")";

  Delete( FindKey( key ) );
}

/*****************************************************************************/
inline
void
StackedLayerModel
::DeleteCurrent()
{
  // qDebug() << this << "::DeleteCurrent()";

  Delete( m_Current );
}

/*****************************************************************************/
inline
StackedLayerModel::SizeType
StackedLayerModel
::FindKey( const KeyType & key ) const
{
  KeyVector::const_iterator it( std::find( m_Keys.begin(), m_Keys.end(), key ) );

  if( it==m_Keys.end() )
    return StackedLayerModel::NIL_INDEX;

  return std::distance( m_Keys.begin(), it );
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
bool
StackedLayerModel
::HasCurrent() const
{
  return m_Current<GetCount();
}

/*****************************************************************************/
inline
bool
StackedLayerModel
::HasReference() const
{
  return m_Reference<GetCount();
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
inline
void
StackedLayerModel
::EndEditPixelInfo( const QPoint & screen, const PointType & view )
{
  emit PixelInfoChanged( screen, view, m_PixelInfos );
}

/*****************************************************************************/
StackedLayerModel::ConstIterator
StackedLayerModel
::Find( const AbstractLayerModel * layer ) const
{
  for( LayerModelMap::const_iterator it( m_LayerModels.begin() );
       it!=m_LayerModels.end();
       ++it )
    if( it->second==layer )
      return it;

  return m_LayerModels.end();
}

/*****************************************************************************/
const AbstractLayerModel *
StackedLayerModel
::Get( const KeyType & key ) const
{
  return const_cast< StackedLayerModel * >( this )->Get( key );
}

/*****************************************************************************/
AbstractLayerModel *
StackedLayerModel
::Get( const KeyType & key )
{
  ConstIterator it( m_LayerModels.find( key ) );

  return
    it==m_LayerModels.end()
    ? NULL
    : it->second;  
}

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
StackedLayerModel::SizeType
StackedLayerModel
::GetCurrentIndex() const
{
  return m_Current;
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
template< typename T >
inline
T *
StackedLayerModel
::GetReference()
{
  return dynamic_cast< T * >( GetReference() );
}

/*****************************************************************************/
template< typename T >
inline
T *
StackedLayerModel
::GetReference() const
{
  return dynamic_cast< T * >( GetReference() );
}

/*****************************************************************************/
inline
AbstractLayerModel *
StackedLayerModel
::GetReference()
{
  if( m_Reference>=GetCount() )
    return NULL;

  LayerModelMap::const_iterator it(
    m_LayerModels.find( m_Keys[ m_Reference ] )
  );

  if( it==m_LayerModels.end() )
    return NULL;

  return it->second;
}

/*****************************************************************************/
inline
AbstractLayerModel *
StackedLayerModel
::GetReference() const
{
  return const_cast< StackedLayerModel * >( this )->GetReference();
}

/*****************************************************************************/
inline
StackedLayerModel::SizeType
StackedLayerModel
::GetReferenceIndex() const
{
  return m_Reference;
}


/*****************************************************************************/
inline
StackedLayerModel::SizeType
StackedLayerModel
::IndexOf( const AbstractLayerModel * layer ) const
{
  KeyType key( KeyOf( layer ) );

  if( key==StackedLayerModel::NIL_KEY )
    return StackedLayerModel::NIL_INDEX;

  for( KeyVector::size_type i=0;
       i<m_Keys.size();
       ++i )
    if( m_Keys[ i ]==key )
      return i;

  return StackedLayerModel::NIL_INDEX;
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
const StackedLayerModel::KeyType &
StackedLayerModel
::KeyOf( const AbstractLayerModel * layerModel ) const
{
  for( LayerModelMap::const_iterator it( m_LayerModels.begin() );
       it!=m_LayerModels.end();
       ++it )
    if( it->second==layerModel )
      return it->first;

  return StackedLayerModel::NIL_KEY;
}

/*****************************************************************************/
inline
void
StackedLayerModel
::LowerCurrent()
{
  if( GetCount()<2 )
    return;

  assert( m_Current<GetCount() );

  if( m_Current==GetCount()-1 )
    RotateLayerDown( 1 );
  else
    LowerLayer( m_Current );
}

/*****************************************************************************/
void
StackedLayerModel
::MoveCurrentToBottom()
{
  if( m_Current>=GetCount() )
    return;

  MoveToBottom( m_Current );
}

/*****************************************************************************/
void
StackedLayerModel
::MoveCurrentToTop()
{
  if( m_Current>=GetCount() )
    return;

  MoveToTop( m_Current );
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
const PixelInfo::Vector &
StackedLayerModel
::PixelInfos() const
{
  return m_PixelInfos;
}

/*****************************************************************************/
inline
PixelInfo::Vector &
StackedLayerModel
::PixelInfos()
{
  return m_PixelInfos;
}

/*****************************************************************************/
inline
StackedLayerModel::SizeType
StackedLayerModel
::Prev( SizeType index )
{
  return
    index>=GetCount() || index==0
    ? GetCount() - 1
    : index - 1;
}

/*****************************************************************************/
inline
void
StackedLayerModel
::RaiseCurrent()
{
  if( GetCount()<2 )
    return;

  assert( m_Current<GetCount() );

  if( m_Current==0 )
    RotateLayerUp( 1 );
  else
    RaiseLayer( m_Current );
}

/*****************************************************************************/
inline
void
StackedLayerModel
::RotateLayers( int steps )
{
  if( IsEmpty() )
    return;

  if( steps>=0 )
    RotateLayerUp( steps % GetCount() );

  else
    RotateLayerDown( ( -steps ) % GetCount() );
}

/*****************************************************************************/
inline
void
StackedLayerModel
::SelectFirst()
{
  if( GetCount()<1 )
    return;

  SetCurrent( SizeType( 0 ) );
}

/*****************************************************************************/
inline
void
StackedLayerModel
::SelectLast()
{
  if( GetCount()<1 )
    return;

  SetCurrent( m_Keys.size() - 1 );
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
::SetCurrent( SizeType index, bool force )
{
  // qDebug() << this << "::SetCurrent(" << index << ")";

  if( index==m_Current && !force )
    return;

  KeyType key( GetKey( index  ) );

  // qDebug()
  //   << QString( "'%1'" ).arg( GetCurrentKey().c_str() )
  //   << "->"
  //   << QString( "'%1'" ).arg( key.c_str() );

  emit CurrentAboutToBeChanged( index );
  emit AboutToChangeSelectedLayerModel( key );

  m_Current = index;

  emit CurrentChanged( index );
  emit SelectedLayerModelChanged( key );
}

/*****************************************************************************/
inline
void
StackedLayerModel
::SetReference( SizeType index, bool force )
{
  // qDebug() << this << "::SetReference(" << index << ")";

  if( index==m_Reference && !force )
    return;

  emit ReferenceAboutToBeChanged( index );

  m_Reference = index;

  emit ReferenceChanged( index );
}

/*****************************************************************************/
#if _DEBUG

inline
void
trace( const std::vector< std::string > & strv )
{
  typedef std::vector< std::string > StringVector;

  qDebug() << "{";

  for( StringVector::const_iterator it( strv.begin() );
       it!=strv.end();
       ++it )
    qDebug() << QString( "%1" ).arg( it->c_str() );

  qDebug() << "}";
}

#endif

} // end namespace 'mvd'

#endif // __mvdStackedLayerModel_h
