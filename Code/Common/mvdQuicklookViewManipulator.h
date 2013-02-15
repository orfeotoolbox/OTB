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

#ifndef __mvdQuicklookViewManipulator_h
#define __mvdQuicklookViewManipulator_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"

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
#include "mvdAbstractViewManipulator.h"

//
// External classes pre-declaration.
namespace
{
}

namespace mvd
{
//
// Internal classes pre-declaration.

/** \class QuicklookViewManipulator
 *  Brief this class handles the event related to a QGLWidget. It also
 *  handles :
 *    - NavigationContext : to store the region of the image to be
 *      rendered.
 *    - MouseContext : to remember the user action as a mouse press,
 *                     mouse drag
 *  
 *   WIP
 */
class Monteverdi2_EXPORT QuicklookViewManipulator :
    public AbstractViewManipulator
{
  Q_OBJECT;

//
// Public methods.
public:

  /** Constructor */
  QuicklookViewManipulator( QObject* parent =NULL );

  /** Destructor */
  virtual ~QuicklookViewManipulator();

  /** */
  void mouseMoveEvent ( QMouseEvent * event );
  void mousePressEvent ( QMouseEvent * event );
  void mouseReleaseEvent ( QMouseEvent * event );
  void wheelEvent ( QWheelEvent* event);
  void resizeEvent ( QResizeEvent * event );
  void keyPressEvent( QKeyEvent * event );

  inline
    bool HasZoomChanged() const
  {
    return false;
  }
  
//
// SIGNALS.
signals:

//
// Public SLOTS.
public slots:
  void InitializeContext(int width, int height);
  void OnModelImageRegionChanged(const ImageRegionType & largestRegion);

//
// Protected methods.
protected:
  void ConstrainRegion( ImageRegionType& region, const ImageRegionType& largest);
  void CenterRegion(double scale);
  void ResizeRegion(unsigned int w, unsigned int h);
  
//
// Protected attributes.
protected:

//
// Private types.
private:
  /** Navigation context  */
  struct NavigationContextType
  {
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
// Private methods.
private:

  void Zoom(const double scale);
  void moveRegion(double dx, double dy);

//
// Private attributes.
private:
  double   m_PreviousIsotropicZoom;
//
// SLOTS.
private slots:
};

} // end namespace 'mvd'

#endif // __mvdQuicklookViewManipulator_h
