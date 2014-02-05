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

#ifndef __mvdAbstractImageViewManipulator_h
#define __mvdAbstractImageViewManipulator_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtGui>

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "Core/mvdTypes.h"
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

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/** \class AbstractImageViewManipulator
 *
 *  \brief Base class for QWidget manipulation
 */
class Monteverdi2_EXPORT AbstractImageViewManipulator :
    public QObject
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:

  /** \brief Constructor. */
  AbstractImageViewManipulator( QObject* parent =NULL );

  /** \brief Destructor. */
  virtual ~AbstractImageViewManipulator();

  /**
   */
  inline void SetViewportSize( int width, int height );
  /**
   */
  inline void SetOrigin( const PointType& origin );
  /**
   */
  inline void SetSpacing( const SpacingType& spacing );
  /**
   */
  inline void SetWkt( const std::string& wkt );

  /**
   */
  inline
    void
    SetupRenderingContext(
      AbstractImageViewRenderer::RenderingContext * const ) const;

  /**
   */
  inline void MouseMoveEvent( QMouseEvent* event );
  /**
   */
  inline void MousePressEvent( QMouseEvent* event );
  /**
   */
  inline void MouseReleaseEvent( QMouseEvent* event );

  /**
   */
  inline void WheelEvent( QWheelEvent* event);

  /**
   */
  inline void KeyPressEvent( QKeyEvent * event );

  /**
   */
  inline void ResizeEvent( QResizeEvent* event );

//
// Public SLOTS.
public slots:

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// SIGNALS.
signals:

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected types.
protected:
  /** Navigation context  */
  struct NavigationContext
  {
    /**
     * \brief Constructor.
     *
     * Default constructor (safely) initializes POD (Plain Old Data)
     * structure.
     */
    NavigationContext() :
      m_Origin(),
      m_Spacing(),
      m_ViewportSize(),
      m_Wkt()
    {
      m_Origin.Fill( 0 );
      m_Spacing.Fill( 1 );
      m_ViewportSize.Fill( 0 );
    }

    PointType m_Origin;
    SpacingType m_Spacing;
    SizeType m_ViewportSize;
    std::string m_Wkt;
  };

  /** Mouse context */
  struct MouseContext
  {
    /**
     * \brief Default constructor.
     */
    MouseContext()
    {
    }
  };

//
// Protected methods.
protected:

  /**
   */
  virtual void virtual_SetViewportSize( int width, int height ) = 0;
  /**
   */
  virtual void virtual_SetOrigin( const PointType& origin ) = 0;
  /**
   */
  virtual void virtual_SetSpacing( const SpacingType& spacing ) = 0;
  /**
   */
  virtual void virtual_SetWkt( const std::string& wkt ) = 0;

  /**
   */
  virtual void virtual_MouseMoveEvent( QMouseEvent* event ) = 0;

  /**
   */
  virtual void virtual_MousePressEvent( QMouseEvent* event ) = 0;
  /**
   */
  virtual void virtual_MouseReleaseEvent( QMouseEvent* event ) = 0;

  /**
   */
  virtual void virtual_WheelEvent( QWheelEvent* event) = 0;

  /**
   */
  virtual void virtual_KeyPressEvent( QKeyEvent * event )  = 0;

  /**
   */
  virtual void virtual_ResizeEvent( QResizeEvent* event ) = 0;

  /**
   */
  virtual
    void
    virtual_SetupRenderingContext(
      AbstractImageViewRenderer::RenderingContext * const ) const = 0;

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

//
// Private attributes.
private:

  /**
   */
  NavigationContext m_NavigationContext;

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
void
AbstractImageViewManipulator
::SetViewportSize( int width, int height )
{
  m_NavigationContext.m_ViewportSize[ 0 ] = width;
  m_NavigationContext.m_ViewportSize[ 1 ] = height;

  virtual_SetViewportSize( width, height );
}

/*****************************************************************************/
inline
void
AbstractImageViewManipulator
::SetOrigin( const PointType& origin )
{
  m_NavigationContext.m_Origin = origin;

  virtual_SetOrigin( origin );
}

/*****************************************************************************/
inline
void
AbstractImageViewManipulator
::SetSpacing( const SpacingType& spacing )
{
  m_NavigationContext.m_Spacing = spacing;

  virtual_SetSpacing( spacing );
}

/*****************************************************************************/
inline
void
AbstractImageViewManipulator
::SetWkt( const std::string& wkt )
{
  m_NavigationContext.m_Wkt = wkt;

  virtual_SetWkt( wkt );
}

/*****************************************************************************/
inline
void
AbstractImageViewManipulator
::SetupRenderingContext(
  AbstractImageViewRenderer::RenderingContext * const context ) const
{
  assert( context!=NULL );

  virtual_SetupRenderingContext( context );
}

/*****************************************************************************/
inline
void
AbstractImageViewManipulator
::MouseMoveEvent( QMouseEvent* event )
{
  virtual_MouseMoveEvent( event );
}

/*****************************************************************************/
inline
void
AbstractImageViewManipulator
::MousePressEvent( QMouseEvent* event )
{
  virtual_MousePressEvent( event );
}

/*****************************************************************************/
inline
void
AbstractImageViewManipulator
::MouseReleaseEvent( QMouseEvent* event )
{
  virtual_MouseReleaseEvent( event );
}

/*****************************************************************************/
inline
void
AbstractImageViewManipulator
::WheelEvent( QWheelEvent* event)
{
  virtual_WheelEvent( event );
}

/*****************************************************************************/
inline
void
AbstractImageViewManipulator
::KeyPressEvent( QKeyEvent * event )
{
  virtual_KeyPressEvent( event );
}

/*****************************************************************************/
inline
void
AbstractImageViewManipulator
::ResizeEvent( QResizeEvent* event )
{
  virtual_ResizeEvent( event );
}

} // end namespace 'mvd'

#endif // __mvdAbstractImageViewManipulator_h
