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

#ifndef __mvdImageViewManipulator_h
#define __mvdImageViewManipulator_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)
#include "otbViewSettings.h"

//
// Monteverdi includes (sorted by alphabetic order)
#include "Gui/mvdAbstractImageViewManipulator.h"
#include "Gui/mvdImageViewRenderer.h"


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
 *  \brief TODO.
 */
class Monteverdi2_EXPORT ImageViewManipulator :
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
  static const int DEFAULT_ZOOM_GRANULARITY;
  /**
   */
  static const int DEFAULT_SCROLL_GRANULARITY;

//
// Public methods.
public:

  /** \brief Constructor. */
#if USE_VIEW_SETTINGS_SIDE_EFFECT
  ImageViewManipulator( const otb::ViewSettings::Pointer& viewSettings,
                        QObject* parent =NULL );
#else // USE_VIEW_SETTINGS_SIDE_EFFECT
  ImageViewManipulator( QObject* parent =NULL );
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
  void SelectNextLayerRequested();
  /**
   */
  void SelectPreviousLayerRequested();


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
  int m_ZoomGranularity;
  /**
   */
  int m_ScrollGranularity;
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
