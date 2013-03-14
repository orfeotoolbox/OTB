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
  /** Constructor */
  GLImageWidget( AbstractViewManipulator * manipulator,
                 AbstractModelRenderer * renderer,
                 QWidget* parent =NULL,
		 const QGLWidget* shareWidget =NULL,
		 Qt::WindowFlags f =0 );

  /** Constructor */
  GLImageWidget( AbstractViewManipulator * manipulator,
                 AbstractModelRenderer * renderer,
                 QGLContext* context,
		 QWidget* parent =NULL,
		 const QGLWidget* shareWidget =NULL,
		 Qt::WindowFlags f =0 );
  
  /** Constructor */
  GLImageWidget( AbstractViewManipulator * manipulator,
                 AbstractModelRenderer * renderer,
                 const QGLFormat& format,
		 QWidget* parent =NULL,
		 const QGLWidget *shareWidget =NULL,
		 Qt::WindowFlags f =0 );

  /** Constructor */
  // GLImageWidget( QWidget* parent =NULL, Qt::WindowFlags flags=0);

  /** Destructor */
  virtual ~GLImageWidget();

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
  inline void SetImageModel(core::AbstractImageModel* model);

//
// Public SLOTS.
public slots:
  void OnSpacingChanged(const core::SpacingType& spacing);

//
// SIGNALS.
signals:
  void movingMouse();
  void releasingMouse();
  void ModelImageRegionChanged(const core::ImageRegionType &, const core::SpacingType&, const core::PointType&);
  void CentralWidgetUpdated();

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

//
// Protected attributes.
protected:

//
// Private methods.
private:
  /** Construction code (factorizes constructors initializations). */
  void Initialize(AbstractViewManipulator * manipulator,
                  AbstractModelRenderer * renderer);

//
// Private attributes.
private:
  /** Space to screen transform */
  //AffineTransformType::Pointer m_ImageToScreenTransform;
  //AffineTransformType::Pointer m_ScreenToImageTransform;

  /** Event handler pointer */
  AbstractViewManipulator* m_ImageViewManipulator;

  /** Model Renderer pointer */
  AbstractModelRenderer*   m_ImageModelRenderer;

  core::AbstractImageModel*      m_ImageModel;
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
void 
GLImageWidget
::SetImageModel(core::AbstractImageModel* model)
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

}

#endif // __mvdGLImageWidget_h
