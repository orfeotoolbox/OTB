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

#ifndef __mvdAbstractViewManipulator_h
#define __mvdAbstractViewManipulator_h

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
#include "mvdTypes.h"


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

/** \class AbstractViewManipulator
 *  Base class for QWidget manipulation
 */
class Monteverdi2_EXPORT AbstractViewManipulator :
    public QObject
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:

  /** \rrief Constructor. */
  AbstractViewManipulator( QObject* parent =NULL );

  /** \brief Destructor. */
  virtual ~AbstractViewManipulator();

  /** */
  virtual void mouseMoveEvent ( QMouseEvent * event ) = 0;
  virtual void mousePressEvent ( QMouseEvent * event ) = 0;
  virtual void mouseReleaseEvent ( QMouseEvent * event ) = 0;
  virtual void wheelEvent ( QWheelEvent* event) = 0;
  virtual void resizeEvent ( QResizeEvent * event ) = 0;
  virtual void keyPressEvent( QKeyEvent * event )  = 0;

  virtual bool HasZoomChanged() const = 0;

  /** */
  inline const ImageRegionType& GetViewportImageRegion() const;

  /** */
  inline double GetIsotropicZoom() const;


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
  struct NavigationContextType
  {
    // TODO: Implement default construtor (to safely initialize POD).
    // TODO: Rename NavigationContextType struct into NagivationContext (this is not a typedef).
    ImageRegionType m_ViewportImageRegion;
    ImageRegionType m_ModelImageRegion;
    // Stored as double to keep precision when dividing 
    // by scale 
    double          m_SizeXBeforeConstrain;
    double          m_SizeYBeforeConstrain;
  };

  /** Mouse context */
  struct MouseContextType
  {
    // TODO: Rename MouseContextType struct into NagivationContext (this is not a typedef).

    /** Default constructor */
    MouseContextType() :
      x( 0 ),
      y( 0 ),
      xMove( 0 ),
      yMove( 0 ),
      dx( 0 ),
      dy( 0 )
    {
    }

    int x;  // mousePress x
    int y;  // mousePress y
    int xMove;  // for mouseMove x
    int yMove;  // for mouseMove y
    int dx; // mouseMove in x (Drag)
    int dy; // mouseMove in y (Drag)
  };

//
// Protected methods.
protected:
  
//
// Protected attributes.
protected:

  /** */
  NavigationContextType m_NavigationContext;

  /** */
  MouseContextType m_MouseContext;

  /** */
  double m_IsotropicZoom;

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
const ImageRegionType&
AbstractViewManipulator
::GetViewportImageRegion() const
{
  return m_NavigationContext.m_ViewportImageRegion;
}

/*****************************************************************************/
inline 
double
AbstractViewManipulator
::GetIsotropicZoom() const
{
  return m_IsotropicZoom;
}

} // end namespace 'mvd'

#endif // __mvdAbstractViewManipulator_h
