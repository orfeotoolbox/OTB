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

#ifndef __mvdAbstractImageViewRenderer_h
#define __mvdAbstractImageViewRenderer_h

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
class AbstractLayerModel;
class StackedLayerModel;

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class AbstractImageViewRenderer
 *
 * \brief TODO.
 */
class AbstractImageViewRenderer :
    public QObject
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public types.
public:
  /**
   */
  struct RenderingContext
  {
    /**
     */
    inline
    RenderingContext() :
      m_RenderMode( RENDER_MODE_FULL )
    {
    }

    /**
     */
    virtual ~RenderingContext() {}

    /**
     */
    enum RenderMode
    {
      RENDER_MODE_LIGHT = 0,
      RENDER_MODE_FULL
    };

    RenderMode m_RenderMode;
  };

//
// Public methods.
public:
  /**
   * Destructor.
   */
  virtual ~AbstractImageViewRenderer(){};

  /**
   */
  inline void SetLayerStack( StackedLayerModel * );

  /**
   */
  inline const StackedLayerModel * GetLayerStack() const;

  /**
   */
  inline StackedLayerModel * GetLayerStack();

  /**
   */
  virtual bool CheckGLCapabilities() const =0;

  template< typename T >
  const T * GetReferenceModel() const;

  template< typename T >
  T * GetReferenceModel();

  /**
   */
  virtual const AbstractLayerModel * GetReferenceModel() const =0;

  /**
   */
  virtual AbstractLayerModel * GetReferenceModel() =0;

  /**
   */
  virtual void GetReferenceExtent( PointType& origin,
                                   PointType& extent ) const =0;

  /**
   */
  virtual
    AbstractImageViewRenderer::RenderingContext* NewRenderingContext() const =0;

  /**
   */
  virtual void InitializeGL() =0;

  /**
   */
  virtual void ResizeGL( int width, int height ) =0;

  /**
   */
  virtual void PaintGL( RenderingContext const * context ) =0;

  /**
   */
  virtual bool Pick( const PointType& in,
                     PointType& out,
                     DefaultImageType::PixelType& pixel ) =0;

  /**
   */
  virtual bool Transform( PointType& point,
                          const IndexType&,
                          bool isPhysical ) const =0;

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

// public slots
public slots:

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// SIGNALS.
signals:

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

  /**
   * Constructor.
   */
  AbstractImageViewRenderer( QObject* parent = NULL ) :
    QObject( parent ),
    m_StackedLayerModel( NULL )
  {
  }


//
// Protected attributes.
protected:

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Private methods.
private:
  /**
   */
  // virtual void virtual_ClearScene() {};
  /**
   */
  virtual void virtual_PrepareScene() {};
  /**
   */
  virtual void virtual_UpdateScene() {};
  /**
   */
  // virtual void virtual_SetLayerStack( const StackedLayerModel & ) =0;
  /**
   */
  virtual void virtual_FinishScene() {};

//
// Private attributes.
private:
  /**
   */
  StackedLayerModel * m_StackedLayerModel;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// SLOTS.
private slots:
};

} // end namespace 'mvd'

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{

/*****************************************************************************/
template< typename T >
const T *
AbstractImageViewRenderer
::GetReferenceModel() const
{
  return dynamic_cast< const T * >( GetReferenceModel() );
}

/*****************************************************************************/
template< typename T >
T *
AbstractImageViewRenderer
::GetReferenceModel()
{
  return dynamic_cast< T * >( GetReferenceModel() );
}

/*****************************************************************************/
inline
StackedLayerModel *
AbstractImageViewRenderer
::GetLayerStack()
{
  return m_StackedLayerModel;
}

/*****************************************************************************/
inline
const StackedLayerModel *
AbstractImageViewRenderer
::GetLayerStack() const
{
  return m_StackedLayerModel;
}

/*****************************************************************************/
inline
void
AbstractImageViewRenderer
::SetLayerStack( StackedLayerModel * stackedLayerModel )
{
  virtual_PrepareScene();

  m_StackedLayerModel = stackedLayerModel;

  virtual_UpdateScene();

  virtual_FinishScene();
}

} // end namespace 'mvd'

#endif // __mvdAbstractImageViewRenderer_h
