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
#include "Gui/mvdAbstractImageViewManipulator.h"


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
class AbstractImageViewRenderer;
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

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

//
// Public SLOTS.
public slots:

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// SIGNALS.
signals:

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:
  virtual void initializeGL();
  virtual void resizeGL( int widgth, int height );
  virtual void paintGL();

  virtual void mouseMoveEvent( QMouseEvent* event );
  virtual void mouseReleaseEvent( QMouseEvent* event );
  virtual void mousePressEvent( QMouseEvent* event );

  virtual void wheelEvent( QWheelEvent* event);

  virtual void keyPressEvent( QKeyEvent* event );

  virtual void resizeEvent( QResizeEvent* event );


//
// Protected attributes.
protected:

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Private methods.
private:
  /** Construction code (factorizes constructors initializations). */
  void Initialize( AbstractImageViewManipulator* manipulator,
                   AbstractImageViewRenderer* renderer );

  /**
   */
  void ListGlVersions() const;

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

} // end of namespace 'mvd'.

#endif // __mvdImageViewWidget_h
