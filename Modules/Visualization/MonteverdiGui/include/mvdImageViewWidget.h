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
#ifndef __mvdImageViewWidget_h
#define __mvdImageViewWidget_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"

/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
#include <QtOpenGL>

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdCore.h"
#include "mvdAbstractImageViewRenderer.h"
#include "mvdGui.h"


#define USE_XP_REGION_OPTIM 0

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
class AbstractImageViewManipulator;


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/** \class ImageViewWidget
 *
 * \ingroup OTBMonteverdiGui
 *
 */
class ITK_EXPORT ImageViewWidget :
    public QGLWidget
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public types.
public:

//
// Public methods.
public:

  /** \brief Constructor. */
  ImageViewWidget( AbstractImageViewManipulator* manipulator,
                   AbstractImageViewRenderer* renderer,
                   QWidget* p =NULL,
                   const QGLWidget* shareWidget =NULL,
                   Qt::WindowFlags f =0 );

  /** \brief Constructor. */
  ImageViewWidget( AbstractImageViewManipulator* manipulator,
                   AbstractImageViewRenderer* renderer,
                   QGLContext* context,
                   QWidget* p =NULL,
                   const QGLWidget* shareWidget =NULL,
                   Qt::WindowFlags f =0 );

  /** \brief Constructor. */
  ImageViewWidget( AbstractImageViewManipulator* manipulator,
                   AbstractImageViewRenderer* renderer,
                   const QGLFormat& format,
                   QWidget* p =NULL,
                   const QGLWidget *shareWidget =NULL,
                   Qt::WindowFlags f =0 );

  /** \brief Destructor. */
  virtual ~ImageViewWidget();

  /**
   */
  void SetLayerStack( StackedLayerModel * stackedLayerModel,
                      const PointType& center,
                      double scale );

  /**
   */
  void SetLayerStack( StackedLayerModel * );

  /**
   */
  inline const StackedLayerModel * GetLayerStack() const;

  /**
   */
  inline StackedLayerModel * GetLayerStack();

  /**
   * \brief Access the abstract view-manipualtor of this image-view.
   *
   * \return A pointer to the abstract view-manipulator of this image-view.
   */
  inline const AbstractImageViewManipulator* GetManipulator() const;

  /**
   * \brief Access the abstract model-renderer of this image-view.
   *
   * \return A pointer to the abstract model-renderer of this image-view.
   */
  inline const AbstractImageViewRenderer * GetRenderer() const;
  inline AbstractImageViewRenderer * GetRenderer();

  /**
   */
  bool SetBypassRenderingEnabled( bool );
  /**
   */
  bool IsBypassRenderingEnabled() const;
  /**
   */
  void SetPickingEnabled( bool );

  /**
   * This allows to set the fallback behaviour for picking
   */
  void SetPickingDefaultStatus( bool );
  /**
   */
  bool IsPickingEnabled() const;

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

//
// Public SLOTS.
public slots:
  /**
   */
  void UpdateScene();
  /**
   */
  void CenterOnSelected( const IndexType& index );
  /**
   */
  void CenterOn( const PointType& center, double scale );
  /**
   */
  void SaveScreenshot( bool );
  /**
   */
  void ZoomToExtent();
  /**
   */
  void ZoomToFullResolution();
  /**
   */
  void ZoomToLayerExtent();
  /**
   */
  void ZoomIn();
  /**
   */
  void ZoomOut();

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// SIGNALS.
signals:
  /**
   */
  void PhysicalCursorPositionChanged( const QPoint & screen,
                                      const PointType & view,
                                      const PointType & physical,
                                      const DefaultImageType::PixelType & );

  /**
   */
  void PixelInfoChanged( const QPoint & screen,
			 const PointType & view,
			 const PixelInfo::Vector & pixels );

  /**
   */
  void ReferenceActorShaderModeChanged(const std::string & mode);
  /**
   */
  void CenterRoiRequested( const PointType& center );
  /**
   */
  void RoiChanged( const PointType& origin,
                   const SizeType& size,
                   const SpacingType& spacing,
                   const PointType& center );
  /**
   */
  void RoiChanged( const PointType& center, double sx, double sy );
  /**
   */
  void CenterChanged( const PointType& center );
  /**
   */
  void ScaleChanged( double sx, double sy );
  /**
   */
  void ModelUpdated();

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

  //
  // Qt overloads.

  virtual void initializeGL();
  virtual void resizeGL( int widgth, int height );
  virtual void paintGL();

  virtual void mouseMoveEvent( QMouseEvent* event );
  virtual void mouseReleaseEvent( QMouseEvent* event );
  virtual void mousePressEvent( QMouseEvent* event );
  virtual void mouseDoubleClickEvent( QMouseEvent * event );

  virtual void wheelEvent( QWheelEvent* event);

  virtual void keyPressEvent( QKeyEvent* event );
  virtual void keyReleaseEvent( QKeyEvent* event );

  virtual void resizeEvent( QResizeEvent* event );


//
// Protected attributes.
protected:
  /**
   */
  bool m_IsPickingEnabled : 1;
  bool m_PickingDefaultStatus : 1;

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Private types.
private:

//
// Private methods.
private:

  /** Construction code (factorizes constructors initializations). */
  void Initialize( AbstractImageViewManipulator* manipulator,
                   AbstractImageViewRenderer* renderer );

  /**
   */
  void ListGlVersions() const;

  /**
   */
  void Center( const PointType& point, double sx, double sy );

  /**
   */
  void Center( ZoomType zoom =ZOOM_TYPE_NONE );

  /**
   */
  void Connect( AbstractLayerModel * );

  /**
   */
  void Disconnect( AbstractLayerModel * );

  /**
   */
  bool ApplyFixedZoomType();

//
// Private attributes.
private:
  /** Event handler pointer */
  AbstractImageViewManipulator* m_Manipulator;

  /** Model Renderer pointer */
  AbstractImageViewRenderer* m_Renderer;

  /** */
#if USE_XP_REGION_OPTIM
  PointType m_Position;
#endif // USE_XP_REGION_OPTIM

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/
private slots:
  /**
   */
  void OnRoiChanged( const PointType&,
                     const SizeType&,
                     const SpacingType&,
                     const PointType& );
  /**
   */
  void OnSelectFirstLayerRequested();
  /**
   */
  void OnSelectLastLayerRequested();
  /**
   */
  void OnSelectPreviousLayerRequested();
  /**
   */
  void OnSelectNextLayerRequested();
  /**
   */
  void OnLowerLayerRequested();
  /**
   */
  void OnRaiseLayerRequested();
  /**
   */
  void OnDeleteAllRequested();
  /**
   */
  void OnDeleteSelectedRequested();
  /**
   */
  void OnReferenceChanged( size_t );
  /**
   */
  void OnClearProjectionRequired();
  /**
   */
  void OnSetProjectionRequired();
  /**
   */
  void OnUpdateProjectionRequired();
  /**
   */
  void OnLayerAdded( size_t );
  /**
   */
  void OnLayerAboutToBeDeleted( size_t );
  /**
   */
  void OnToggleLayerVisibilityRequested( bool );
  /**
   */
  void OnResizeShaderRequested( double );
  /**
   */
  void OnReparamShaderRequested( double );
  /**
   */
  void OnShiftAlphaRequested( double );
  /**
   */
  void OnUpdateGammaRequested( double );
  /**
   */
  void OnShiftDynamicsRequested( double );
  /**
   */
  void OnScaleDynamicsRequested( double );
  /**
   */
  void OnResetQuantilesRequested( bool );
  /**
   */
  void OnShaderEffectRequested( Effect );
  /**
   */
  void OnApplyAllRequested();
  /**
   */
  void OnSetReferenceRequested();
  /**
   */
  void OnContentChanged();
  /**
   */
  void OnContentReset();
  /**
   */
  void OnRefreshViewRequested();
};

}// end namespace 'mvd'

/*****************************************************************************/
/* INLINE SECTION                                                            */

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdHistogramModel.h"
#include "mvdAbstractImageViewManipulator.h"

namespace mvd
{

/*****************************************************************************/
inline
const AbstractImageViewManipulator*
ImageViewWidget
::GetManipulator() const
{
  return m_Manipulator;
}

/*****************************************************************************/
inline
const AbstractImageViewRenderer*
ImageViewWidget
::GetRenderer() const
{
  return m_Renderer;
}

/*****************************************************************************/
inline
AbstractImageViewRenderer*
ImageViewWidget
::GetRenderer()
{
  return m_Renderer;
}

/*****************************************************************************/
inline
const StackedLayerModel *
ImageViewWidget
::GetLayerStack() const
{
  assert( m_Renderer!=NULL );

  return m_Renderer->GetLayerStack();
}

/*****************************************************************************/
inline
StackedLayerModel *
ImageViewWidget
::GetLayerStack()
{
  assert( m_Renderer!=NULL );

  return m_Renderer->GetLayerStack();
}

} // End of namespace 'mvd'

#endif // __mvdImageViewWidget_h
