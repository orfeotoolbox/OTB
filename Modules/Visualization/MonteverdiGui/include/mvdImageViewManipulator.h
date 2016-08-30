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

#ifndef __mvdImageViewManipulator_h
#define __mvdImageViewManipulator_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)
#include <cmath>

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)
#include "otbViewSettings.h"

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdCore.h"
#include "mvdAbstractImageViewManipulator.h"
#include "mvdImageViewRenderer.h"


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


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class ImageViewManipulator
 *
 * \ingroup OTBMonteverdiGui
 *
 *  \brief TODO.
 */
class ITK_EXPORT ImageViewManipulator :
    public AbstractImageViewManipulator
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Constants
public:
  /**
   */
  static const double DEFAULT_DELTA;
  /**
   */
  static const int DEFAULT_GRANULARITY;
  /**
   */
  static const int DEFAULT_ALPHA_GRANULARITY;
  /**
   */
  static const double DEFAULT_DYNAMICS_SHIFT_GRANULARITY;
  /**
   */
  static const int DEFAULT_SCROLL_GRANULARITY;
  /**
   */
  static const int DEFAULT_ZOOM_GRANULARITY;

//
// Public methods.
public:

  /** \brief Constructor. */
#if USE_VIEW_SETTINGS_SIDE_EFFECT
  ImageViewManipulator( const otb::ViewSettings::Pointer& viewSettings,
                        QObject* p =NULL );
#else // USE_VIEW_SETTINGS_SIDE_EFFECT
  ImageViewManipulator( QObject* p =NULL );
#endif // USE_VIEW_SETTINGS_SIDE_EFFECT

  /** \brief Destructor. */
  virtual ~ImageViewManipulator();

  //
  // AbstractImageViewManipulator overloads.
  //

  //
  // Accessors.

  virtual void SetViewportSize( int width, int height );

  virtual SizeType GetViewportSize() const;

  virtual void SetOrigin( const PointType& origin );

  virtual PointType GetOrigin() const;

  virtual void SetSpacing( const SpacingType& spacing );

  virtual SpacingType GetSpacing() const;

  virtual void SetNativeSpacing( const SpacingType& );

  virtual void SetWkt( const std::string& wkt );

  virtual void SetKeywordList(
    const DefaultImageType::ImageKeywordlistType& kwl
  );

  virtual PointType GetCenter() const;

  virtual ZoomType GetFixedZoomType() const;

  //
  // Controls.

  virtual
    void
    SetupRenderingContext(
      AbstractImageViewRenderer::RenderingContext * const ) const;

  virtual void ZoomIn();

  virtual void ZoomOut();

  virtual const PointType& Transform( PointType&, const QPoint& ) const;

  //
  // Events.

  virtual void MouseMoveEvent( QMouseEvent* event );

  virtual void MousePressEvent( QMouseEvent* event );

  virtual void MouseReleaseEvent( QMouseEvent* event );

  virtual void MouseDoubleClickEvent( QMouseEvent * );

  virtual void WheelEvent( QWheelEvent* event);

  virtual void ResizeEvent( QResizeEvent* event );

  virtual void KeyPressEvent( QKeyEvent* event );

  virtual void KeyReleaseEvent( QKeyEvent* event );

  /*-[ PUBLIC SLOTS SECTION ]-----------------------------------------------**/

//
// Public SLOTS.
public slots:

  //
  // AbstractImageViewManipulator overloads.

  virtual void CenterOn( const PointType& point );

  virtual void ZoomTo( double scale );

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:
  /**
   */
  void RaiseLayerRequested();
  /**
   */
  void LowerLayerRequested();
  /**
   */
  void LayerToTopRequested();
  /**
   */
  void LayerToBottomRequested();
  /**
   */
  void SelectNextLayerRequested();
  /**
   */
  void SelectPreviousLayerRequested();
  /**
   */
  void SelectFirstLayerRequested();
  /**
   */
  void SelectLastLayerRequested();
  /**
   */
  void DeleteAllRequested();
  /**
   */
  // TODO: Rename signal into DeleteLayerRequested().
  void DeleteSelectedRequested();
  /**
   */
  void ToggleLayerVisibilityRequested( bool );
  /**
   */
  void RotateLayersRequested( int );
  /**
   */
  void ZoomToFullResolutionRequested();
  /**
   */
  void ZoomToLayerExtentRequested();
  /**
   */
  void ZoomToFullExtentRequested();

  /**
   */
  void ShiftAlphaRequested( double );
  /**
   */
  void UpdateGammaRequested( double );
  /**
   */
  void ResizeShaderRequested( double );
  /**
   */
  void ReparamShaderRequested( double );
  /**
   */
  void ShiftDynamicsRequested( double );
  /**
   */
  void ScaleDynamicsRequested( double );
  /**
   */
  void ResetQuantilesRequested( bool );
  /**
   */
  void ShaderEffectRequested( Effect );
  /**
   */
  void ApplyAllRequested();
  /**
   */
  void SetReferenceRequested();
  /**
   */
  void TakeScreenshotRequested( bool isQuickMode );

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:
  /**
   */
  static PointType Translate( const QPoint& vector,
                              const PointType& origin,
                              const SpacingType& spacing );

//
// Protected attributes.
protected:
  /**
   */
  QPoint m_MousePressPosition;
  /**
   */
  otb::ViewSettings::Pointer m_ViewSettings;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Private types.
private:

//
// Private methods.
private:
  /**
   */
  template< typename T1, typename T2, typename T3, typename T4 >
    inline static
    double Factor( T1 wheel,
		   T2 degrees,
		   T3 delta,
		   T4 granularity );
  /**
   */
  template< typename T1, typename T2 >
    inline static
    double Factor( T1 wheel,
		   T2 degrees );

  /**
   */
  void Translate( const QPoint& vector );
  /**
   */
  void Scale( const QPoint& center, int degrees, PointType* centerPoint =NULL );

  /**
   */
  // inline void SetRenderMode( const QInputEvent* event );
  /**
   */
  inline void SetFastRenderMode( bool isEnabled );

//
// Private attributes.
private:
  /**
   */
  SpacingType m_NativeSpacing;
  /**
   */
  otb::ViewSettings::PointType m_MousePressOrigin;
  /**
   */
  AbstractImageViewRenderer::RenderingContext::RenderMode m_RenderMode;
  /**
   */
  double m_ZoomFactor;
  /**
   */
  int m_AlphaGranularity;
  /**
   */
  double m_DynamicsShiftGranularity;
  /**
   */
  int m_ScrollGranularity;
  /**
   */
  int m_ZoomGranularity;
  /**
   */
  bool m_IsMouseDragging : 1;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:
};

} // end namespace 'mvd'

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{

/*****************************************************************************/
template< typename T1, typename T2, typename T3, typename T4 >
inline
double
ImageViewManipulator
::Factor( T1 wheel, T2 degrees, T3 delta, T4 granularity )
{
  // qDebug() << "wheel:" << wheel;
  // qDebug() << "degrees:" << degrees;
  // qDebug() << "delta:" << delta;
  // qDebug() << "granularity:" << granularity;

  if( abs( granularity )<=std::numeric_limits< T1 >::min() )
    granularity = 1;

  return
    pow(
      ( 1.0 + static_cast< double >( delta ) ),
      static_cast< double >( wheel ) /
      ( static_cast< double >( granularity ) * static_cast< double >( degrees ) )
    );
}

/*****************************************************************************/
template< typename T1, typename T2 >
inline
double
ImageViewManipulator
::Factor( T1 wheel, T2 degrees )
{
  return
    ImageViewManipulator::Factor(
      wheel,
      degrees,
      ImageViewManipulator::DEFAULT_DELTA,
      ImageViewManipulator::DEFAULT_GRANULARITY
    );
}

/*****************************************************************************/
/*
inline
void
ImageViewManipulator
::SetRenderMode( const QInputEvent* event )
{
  m_RenderMode =
    ( event!=NULL &&
      ( event->modifiers() & Qt::ControlModifier )==Qt::ControlModifier
    )
    ? AbstractImageViewRenderer::RenderingContext::RENDER_MODE_LIGHT
    : AbstractImageViewRenderer::RenderingContext::RENDER_MODE_FULL;

  qDebug() << "Render-mode:" << m_RenderMode;
}
*/

/*****************************************************************************/
inline
void
ImageViewManipulator
::SetFastRenderMode( bool isEnabled )
{
  m_RenderMode =
    isEnabled
    ? AbstractImageViewRenderer::RenderingContext::RENDER_MODE_LIGHT
    : AbstractImageViewRenderer::RenderingContext::RENDER_MODE_FULL;

  // qDebug() << "Render-mode:" << m_RenderMode;
}

} // end namespace 'mvd'

#endif // __mvdImageViewManipulator_h
