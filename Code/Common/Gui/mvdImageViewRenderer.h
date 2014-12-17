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

#ifndef __mvdImageViewRenderer_h
#define __mvdImageViewRenderer_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"

//
#define USE_VIEW_SETTINGS_SIDE_EFFECT 1
//
#define USE_REMOTE_DESKTOP_DISABLED_RENDERING ((defined( _DEBUG ) && 0) || 0)

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
#include "otbGlActor.h"
#include "otbGlView.h"

//
// Monteverdi includes (sorted by alphabetic order)
#include "Gui/mvdAbstractImageViewRenderer.h"


/*****************************************************************************/
/* PRE-DECLARATION SECTION                                                   */

//
// External classes pre-declaration.
namespace
{
}

namespace otb
{
}

namespace mvd
{
//
// Internal classes pre-declaration.


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class ImageViewRenderer
 */
class Monteverdi2_EXPORT ImageViewRenderer :
    public AbstractImageViewRenderer
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public types.
public:

  /**
   */
  struct RenderingContext :
    public AbstractImageViewRenderer::RenderingContext
  {
    /**
     */
    inline
    RenderingContext() :
      AbstractImageViewRenderer::RenderingContext(),
      m_ViewSettings()
    {
    }

    virtual ~RenderingContext() {}

    otb::ViewSettings::Pointer m_ViewSettings;
  };

//
// Public methods.
public:
  /** Constructor */
  ImageViewRenderer( QObject* parent = NULL );

  /** Destructor */
  virtual ~ImageViewRenderer();

  /**
   */
  virtual bool CheckGLCapabilities() const;

  // Set/Get reference actor shader mode 
  void SetReferenceActorShaderMode(const std::string & mode)
  {
    m_ReferenceActorShaderMode = mode;
  }

  std::string GetReferenceActorShaderMode() const
  {
    return m_ReferenceActorShaderMode;
  }

  /**
   */
  inline const otb::ViewSettings::Pointer GetViewSettings() const;
  /**
   */
  inline otb::ViewSettings::Pointer GetViewSettings();

  //
  // AbstractImageViewRenderer overloads.

  virtual const AbstractLayerModel* GetReferenceModel() const;

  virtual AbstractLayerModel * GetReferenceModel();

  virtual void GetReferenceExtent( PointType& origin,
                                   PointType& extent ) const;

  virtual
  AbstractImageViewRenderer::RenderingContext* NewRenderingContext() const;

  virtual void InitializeGL();

  virtual void ResizeGL( int width, int height );

  virtual
  void PaintGL( const AbstractImageViewRenderer::RenderingContext* context );

  virtual bool Pick( const PointType& in,
                     PointType& out,
                     DefaultImageType::PixelType& pixel );

  virtual bool Transform( PointType& point,
                          const IndexType&,
                          bool isPhysical ) const;

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

// public slots
public slots:
  void OnPhysicalCursorPositionChanged(const PointType&,
                                       const DefaultImageType::PixelType& );

  void OnReferenceActorShaderModeChanged(const std::string & mode);

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// SIGNALS.
signals:

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:
  /**
   */
  inline otb::GlActor::Pointer GetReferenceActor();
  /**
   */
  template< typename T >
    inline typename T::Pointer GetReferenceActor();
  /**
   */
  template< typename T >
    inline typename T::ConstPointer GetReferenceActor() const;
  /**
   */
  virtual
  void UpdateActors( const AbstractImageViewRenderer::RenderingContext * );

//
// Protected attributes.
protected:
  /**
   */
  otb::GlView::Pointer m_GlView;

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Private types
private:
  /**
   */
  typedef
    std::pair< AbstractLayerModel *, otb::GlActor::Pointer >
    ModelActorPair;

  /**
   */
  // typedef std::map< std::string, ModelActorPair > ModelActorPairMap;

  /**
   */
  // typedef ModelActorPairMap::key_type Key;

//
// Private methods.
private:
  //
  // AbstractImageViewRenderer overloads.

  // virtual void virtual_SetLayerStack( StackedLayerModel * );
  virtual void virtual_UpdateScene();

//
// Private attributes.
private:
  /**
   */
  StackedLayerModel * m_StackedLayerModel;

  /**
   */
  ModelActorPair m_ReferencePair;

  /**
   */
  // ModelActorPairMap m_ModelActorPairs;

  /**
   */ 
  std::string m_ReferenceActorShaderMode;

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
inline
const otb::ViewSettings::Pointer
ImageViewRenderer
::GetViewSettings() const
{
  assert( !m_GlView.IsNull() );

  return m_GlView->GetSettings();
}

/*****************************************************************************/
inline
otb::ViewSettings::Pointer
ImageViewRenderer
::GetViewSettings()
{
  assert( !m_GlView.IsNull() );

  return m_GlView->GetSettings();
}

/*****************************************************************************/
inline
otb::GlActor::Pointer
ImageViewRenderer
::GetReferenceActor()
{
  return m_ReferencePair.second;
}

/*****************************************************************************/
template< typename T >
inline
typename T::Pointer
ImageViewRenderer
::GetReferenceActor()
{
  return otb::DynamicCast< T >( m_ReferencePair.second );
}

/*****************************************************************************/
template< typename T >
inline
typename T::ConstPointer
ImageViewRenderer
::GetReferenceActor() const
{
  return otb::DynamicCast< const T >( m_ReferencePair.second );
}

} // end namespace 'mvd'

#endif // __mvdImageViewRenderer_h
