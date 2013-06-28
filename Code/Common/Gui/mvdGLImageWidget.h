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
#ifndef __mvdGLImageWidget_h
#define __mvdGLImageWidget_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"

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
#include "Core/mvdTypes.h"
#include "Gui/mvdAbstractModelRenderer.h"
#include "Gui/mvdAbstractViewManipulator.h"

//
// External classes pre-declaration.
namespace
{
}

namespace mvd
{
//
// Internal classes pre-declaration.
class ImageModelRenderer;
class ImageViewManipulator;

/** \class GLImageWidget
 *
 */
class Monteverdi2_EXPORT GLImageWidget :
    public QGLWidget
{
  Q_OBJECT;

//
// Public methods.
public:
  /** \brief Constructor. */
  GLImageWidget( AbstractViewManipulator * manipulator,
                 AbstractModelRenderer * renderer,
                 QWidget* parent =NULL,
		 const QGLWidget* shareWidget =NULL,
		 Qt::WindowFlags f =0 );

  /** \brief Constructor. */
  GLImageWidget( AbstractViewManipulator * manipulator,
                 AbstractModelRenderer * renderer,
                 QGLContext* context,
		 QWidget* parent =NULL,
		 const QGLWidget* shareWidget =NULL,
		 Qt::WindowFlags f =0 );

  /** \brief Constructor. */
  GLImageWidget( AbstractViewManipulator * manipulator,
                 AbstractModelRenderer * renderer,
                 const QGLFormat& format,
		 QWidget* parent =NULL,
		 const QGLWidget *shareWidget =NULL,
		 Qt::WindowFlags f =0 );

  /** \brief Destructor. */
  virtual ~GLImageWidget();

  /**
   * \brief Access the abstract view-manipualtor of this image-view.
   *
   * \return A pointer to the abstract view-manipulator of this image-view.
   */
  inline
    const AbstractViewManipulator* GetImageViewManipulator() const;

  /**
   * \brief Access the abstract model-renderer of this image-view.
   *
   * \return A pointer to the abstract model-renderer of this image-view.
   */
  inline
    const AbstractModelRenderer* GetImageModelRenderer() const;

  /** Set Isotropic-zoom level. */
  inline
    void
    SetIsotropicZoom(double value)
  {
    //TODO: Implement setter.
  }

  /** Get Isotropic-zoom level. */
  inline
    double
    GetIsotropicZoom() const
  {
    // TODO: Implement getter.
    return 1.0;
  }

  /** Set image model */
  inline void SetImageModel(AbstractImageModel* model);

  inline void SetImageModel(AbstractImageModel* model, 
                            const PointType& centerPoint, 
                            double zoomLevel);

//
// Public SLOTS.
public slots:
  void OnSpacingChanged(const SpacingType& spacing);

//
// SIGNALS.
signals:
  void movingMouse();
  void releasingMouse();
  void ModelImageRegionChanged(const ImageRegionType &, const SpacingType&, const PointType& );

  void ModelImageRegionChanged(const ImageRegionType &, 
                               const SpacingType&, 
                               const PointType&, 
                               const PointType&, 
                               double);
  void CentralWidgetUpdated();
  void ImageToImportDropped(const QString &);

  void RenderingContextChanged( const PointType&, double);

//
// Protected methods.
protected:
  void initializeGL();
  void resizeGL(int widgth, int height);
  void paintGL();
  void mouseMoveEvent ( QMouseEvent * event );
  void mouseReleaseEvent ( QMouseEvent * event );
  void mousePressEvent ( QMouseEvent * event );
  void wheelEvent( QWheelEvent *event);
  void resizeEvent(QResizeEvent* event);
  void keyPressEvent( QKeyEvent * event );

  void dragEnterEvent(QDragEnterEvent *event);
  void dragMoveEvent(QDragMoveEvent *event);
  void dropEvent(QDropEvent *event);
  

//
// Protected attributes.
protected:

//
// Private methods.
private:
  /** Construction code (factorizes constructors initializations). */
  void Initialize(AbstractViewManipulator * manipulator,
                  AbstractModelRenderer * renderer);

  /**
   */
  void listGlVersions() const;

//
// Private attributes.
private:
  /** Space to screen transform */
  //AffineTransformType::Pointer m_ImageToScreenTransform;
  //AffineTransformType::Pointer m_ScreenToImageTransform;

  /** Event handler pointer */
  AbstractViewManipulator* m_ImageViewManipulator;

  /** Model Renderer pointer */
  AbstractModelRenderer* m_ImageModelRenderer;

  /**
   */
  AbstractImageModel* m_ImageModel;
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
#include "Core/mvdHistogramModel.h"

namespace mvd
{
/*****************************************************************************/
inline
const AbstractViewManipulator*
GLImageWidget
::GetImageViewManipulator() const
{
  return m_ImageViewManipulator;
}

/*****************************************************************************/
inline
const AbstractModelRenderer*
GLImageWidget
::GetImageModelRenderer() const
{
  return m_ImageModelRenderer;
}

/*****************************************************************************/
inline
void
GLImageWidget
::SetImageModel(AbstractImageModel* model)
{
  m_ImageModel = model;

  //
  // REFRESH DISPLAY.

  // set the largest possible region of the image and the initial
  // spacing (may change when zooming)
  // TODO:  rename signal name when handling DataSets collections
  // TODO: move signal into mvdApplication and link it to DockWidget
  // and ImageView.
  if( model!=NULL )
    {
    emit ModelImageRegionChanged(
      model->GetNativeLargestRegion(),      
      model->GetNativeSpacing(), 
      model->GetOrigin() // origin is the same for ql and central image
      );
    }
}

/*****************************************************************************/
inline
void
GLImageWidget
::SetImageModel(AbstractImageModel* model, 
                const PointType& centerPoint, 
                double zoomLevel)
{
  m_ImageModel = model;
  
  //
  // REFRESH DISPLAY.

  // set the largest possible region of the image and the initial
  // spacing (may change when zooming)
  // TODO:  rename signal name when handling DataSets collections
  // TODO: move signal into mvdApplication and link it to DockWidget
  // and ImageView.
  if( model!=NULL )
    {
    emit ModelImageRegionChanged(
      model->GetNativeLargestRegion(),      
      model->GetNativeSpacing(), 
      model->GetOrigin(), // origin is the same for ql and central
                          // image
      centerPoint,
      zoomLevel
      );
    }
}

}

#endif // __mvdGLImageWidget_h
