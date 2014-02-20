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
#ifndef __mvdImageViewWidget_h
#define __mvdImageViewWidget_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"

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
#include "Gui/mvdAbstractImageViewRenderer.h"

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
 */
class Monteverdi2_EXPORT ImageViewWidget :
    public QGLWidget
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public types.
public:
  /**
   */
  typedef
    AbstractImageViewRenderer::VectorImageModelList
    VectorImageModelList;
  /**
   */
  enum ZoomType
  {
    ZOOM_TYPE_NONE = -1,
    //
    ZOOM_TYPE_FULL,
    ZOOM_TYPE_EXTENT,
    //
    ZOOM_TYPE_COUNT
  };

//
// Public methods.
public:
  /** \brief Constructor. */
  ImageViewWidget( AbstractImageViewManipulator* manipulator,
                   AbstractImageViewRenderer* renderer,
                   QWidget* parent =NULL,
                   const QGLWidget* shareWidget =NULL,
                   Qt::WindowFlags f =0 );

  /** \brief Constructor. */
  ImageViewWidget( AbstractImageViewManipulator* manipulator,
                   AbstractImageViewRenderer* renderer,
                   QGLContext* context,
                   QWidget* parent =NULL,
                   const QGLWidget* shareWidget =NULL,
                   Qt::WindowFlags f =0 );

  /** \brief Constructor. */
  ImageViewWidget( AbstractImageViewManipulator* manipulator,
                   AbstractImageViewRenderer* renderer,
                   const QGLFormat& format,
                   QWidget* parent =NULL,
                   const QGLWidget *shareWidget =NULL,
                   Qt::WindowFlags f =0 );

  /** \brief Destructor. */
  virtual ~ImageViewWidget();

  /**
   */
  void SetImageList( const VectorImageModelList& images,
                     ZoomType zoom =ZOOM_TYPE_NONE );

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

//
// Public SLOTS.
public slots:
  /**
   */
  void ZoomToExtent();
  /**
   */
  void ZoomToFullResolution();
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

  virtual void wheelEvent( QWheelEvent* event);

  virtual void keyPressEvent( QKeyEvent* event );
  virtual void keyReleaseEvent( QKeyEvent* event );

  virtual void resizeEvent( QResizeEvent* event );


//
// Protected attributes.
protected:

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Private types.
private:

//
// Private methods.
private:

#if 0
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
  inline const AbstractImageViewRenderer* GetRenderer() const;
#endif

  /** Construction code (factorizes constructors initializations). */
  void Initialize( AbstractImageViewManipulator* manipulator,
                   AbstractImageViewRenderer* renderer );

  /**
   */
  void ListGlVersions() const;

  /**
   */
  void Center( ZoomType zoom =ZOOM_TYPE_NONE );

//
// Private attributes.
private:
  /** Event handler pointer */
  AbstractImageViewManipulator* m_Manipulator;

  /** Model Renderer pointer */
  AbstractImageViewRenderer* m_Renderer;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/
private slots:
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
#include "Gui/mvdAbstractImageViewManipulator.h"

namespace mvd
{
#if 0

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

#endif

} // end of namespace 'mvd'.

#endif // __mvdImageViewWidget_h
