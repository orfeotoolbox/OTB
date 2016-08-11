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

#ifndef __mvdAbstractImageViewRenderer_h
#define __mvdAbstractImageViewRenderer_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"


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
#include "mvdStackedLayerModel.h"

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
  virtual bool CheckGLCapabilities( int * ) =0;

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
  virtual void GetLayerExtent( const StackedLayerModel::KeyType & key,
                               PointType& origin,
                               PointType& extent ) const =0;

  /**
   */
  virtual void GetReferenceExtent( PointType& origin,
                                   PointType& extent ) const =0;

  /**
   */
  virtual void GetViewExtent( PointType& origin,
                              PointType& extent ) const =0;

  /**
   */
  virtual bool GetLayerDynamics( const StackedLayerModel::KeyType & key,
				 ParametersType & params,
				 bool isGlobal ) const =0;

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
  virtual void Pick( const PointType & view,
                     PixelInfo::Vector & pixels ) const = 0;

  /**
   */
  virtual void GetResolutions( PixelInfo::Vector & pixels ) const =0;

  /**
   */
  virtual bool TransformToView( PointType & point,
				const StackedLayerModel::KeyType &,
				const IndexType &,
				bool isPhysical ) const =0;

  /**
   */
  virtual void SaveScreenshot( const QString & ) const =0;

  /**
   */
  inline bool SetBypassRenderingEnabled( bool );
  /**
   */
  inline bool IsBypassRenderingEnabled() const;
  /**
   */
  inline bool SetGLSLEnabled( bool );
  /**
   */
  inline bool IsGLSLEnabled() const;
  /**
   */
  inline bool ZoomToRegion( const PointType & origin,
			    const PointType & extent,
			    PointType & center,
			    SpacingType & spacing ) const;
  /**
   */
  inline bool ZoomToExtent( PointType & center, SpacingType & spacing ) const;
  /**
   */
  inline bool ZoomToLayer( const StackedLayerModel::KeyType & key,
			   PointType & center,
			   SpacingType & spacing ) const;
  /**
   */
  inline bool ZoomToFull( const StackedLayerModel::KeyType & key,
			  PointType & center,
			  SpacingType & spacing ) const;

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

// public slots
public slots:
  inline void UpdateScene();
  inline void RefreshScene();

  virtual void UpdatePixelInfo( const QPoint & point,
				const PointType & view,
				const PixelInfo::Vector & pixels ) =0;

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
    m_StackedLayerModel( NULL ),
    m_BypassRenderingEnabled( false ),
    m_GLSLEnabled( true )
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
  virtual void virtual_RefreshScene() {};
  /**
   */
  // virtual void virtual_SetLayerStack( const StackedLayerModel & ) =0;
  /**
   */
  virtual void virtual_FinishScene() {};

  /**
   */
  virtual bool virtual_ZoomToRegion( const PointType &,
				     const PointType &,
				     PointType &,
				     SpacingType & ) const
  { return false; }
  /**
   */
  virtual bool virtual_ZoomToExtent( PointType &, SpacingType & ) const
  { return false; }
  /**
   */
  virtual bool virtual_ZoomToLayer( const StackedLayerModel::KeyType &,
				    PointType &,
				    SpacingType & ) const
  { return false; }
  /**
   */
  virtual bool virtual_ZoomToFull( const StackedLayerModel::KeyType &,
				   PointType &,
				   SpacingType & ) const
  { return false; }

//
// Private attributes.
private:
  /**
   */
  StackedLayerModel * m_StackedLayerModel;
  /**
   */
  bool m_BypassRenderingEnabled: 1;
  /**
   */
  bool m_GLSLEnabled: 1;

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

/*****************************************************************************/
inline
bool
AbstractImageViewRenderer
::IsBypassRenderingEnabled() const
{
  return m_BypassRenderingEnabled;
}

/*****************************************************************************/
inline
bool
AbstractImageViewRenderer
::SetBypassRenderingEnabled( bool isEnabled )
{
  bool bypass = m_BypassRenderingEnabled;

  m_BypassRenderingEnabled = isEnabled;

  return bypass;
}

/*****************************************************************************/
inline
bool
AbstractImageViewRenderer
::IsGLSLEnabled() const
{
  return m_GLSLEnabled;
}

/*****************************************************************************/
inline
bool
AbstractImageViewRenderer
::SetGLSLEnabled( bool isEnabled )
{
  bool wasEnabled = m_GLSLEnabled;

  m_GLSLEnabled = isEnabled;

  return wasEnabled;
}

/*****************************************************************************/
inline
void
AbstractImageViewRenderer
::UpdateScene()
{
  virtual_PrepareScene();

  virtual_UpdateScene();

  virtual_FinishScene();
}

/*****************************************************************************/
inline
void
AbstractImageViewRenderer
::RefreshScene()
{
  virtual_RefreshScene();
}

/*****************************************************************************/
inline
bool
AbstractImageViewRenderer
::ZoomToExtent( PointType & center, SpacingType & spacing ) const
{
  return virtual_ZoomToExtent( center, spacing );
}

/*****************************************************************************/
inline
bool
AbstractImageViewRenderer
::ZoomToLayer( const StackedLayerModel::KeyType & key,
	       PointType & center,
	       SpacingType & spacing) const
{
  return virtual_ZoomToLayer( key, center, spacing );
}

/*****************************************************************************/
inline
bool
AbstractImageViewRenderer
::ZoomToRegion( const PointType & origin,
		const PointType & extent,
		PointType & center,
		SpacingType & spacing ) const
{
  return virtual_ZoomToRegion( origin, extent, center, spacing );
}

/*****************************************************************************/
inline
bool
AbstractImageViewRenderer
::ZoomToFull( const StackedLayerModel::KeyType & key,
	      PointType & center,
	      SpacingType & spacing ) const
{
  return virtual_ZoomToFull( key, center, spacing );
}

} // end namespace 'mvd'

#endif // __mvdAbstractImageViewRenderer_h
