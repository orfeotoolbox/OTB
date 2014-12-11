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
  typedef std::vector< AbstractLayerModel * > LayerModelVector;
  // typedef std::map< std::string, AbstractLayerModel * > LayerModelMap;
  // typedef std::list< std::string > KeyList;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public types.
public:
  typedef LayerModelVector::size_type SizeType;

//
// Public methods.
public:

  /** \brief Constructor. */
  StackedLayerModel( QObject* parent =NULL );

  /** \brief Destructor. */
  virtual ~StackedLayerModel();

  inline const AbstractLayerModel * operator[]( SizeType ) const;
  inline AbstractLayerModel * operator[]( SizeType );

  inline void Add( AbstractLayerModel * );

  inline SizeType Count() const;

  /*
  inline const AbstractLayerModel * Front() const;
  inline AbstractLayerModel * Front();
  */

  const AbstractLayerModel * GetCurrent() const;
  AbstractLayerModel * GetCurrent();

  inline bool IsEmpty() const;

  inline void SetCurrent( SizeType i );
  inline void SetCurrent( AbstractLayerModel * );

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

//
// Public SLOTS.
public slots:

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:
  void AboutToChangeSelectedLayerModel( const AbstractLayerModel * );
  void SelectedLayerModelChanged( AbstractLayerModel * );
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

//
// Private attributes.
private:
  LayerModelVector m_LayerModels;
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
inline
const AbstractLayerModel *
StackedLayerModel
::operator[]( SizeType i ) const
{
  return m_LayerModels[ i ];
}

/*****************************************************************************/
inline
AbstractLayerModel *
StackedLayerModel
::operator[]( SizeType i )
{
  return m_LayerModels[ i ];
}

/*****************************************************************************/
inline
void
StackedLayerModel
::Add( AbstractLayerModel * model )
{
  m_LayerModels.push_back( model );

  emit StackContentChanged();
}

/*****************************************************************************/
inline
StackedLayerModel::SizeType
StackedLayerModel
::Count() const
{
  return m_LayerModels.size();
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
inline
AbstractLayerModel *
StackedLayerModel
::GetCurrent()
{
  return
    m_Current>=m_LayerModels.size()
    ? NULL
    : ( *this )[ m_Current ];
}

/*****************************************************************************/
inline
const AbstractLayerModel *
StackedLayerModel
::GetCurrent() const
{
  return const_cast< StackedLayerModel * >( this )->GetCurrent();
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
void
StackedLayerModel
::SetCurrent( AbstractLayerModel * layer )
{
  SizeType current = m_LayerModels.size();

  for( SizeType i=0; i<m_LayerModels.size(); ++i )
    if( m_LayerModels[ i ]==layer )
      {
      current = i;
      break;
      }

  SetCurrent( current );
}

/*****************************************************************************/
inline
void
StackedLayerModel
::SetCurrent( SizeType i )
{
  if( i==m_Current )
    return;

  emit AboutToChangeSelectedLayerModel( m_LayerModels[ i ] );

  m_Current = i;

  emit SelectedLayerModelChanged( GetCurrent() );
}

} // end namespace 'mvd'

#endif // __mvdStackedLayerModel_h
