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


//
// External classes pre-declaration.
namespace
{
}

namespace mvd
{
//
// Internal classes pre-declaration.

/** \class ImageViewManipulator
 *  Brief this class handles the event related to a QGLWidget. It also
 *  handles :
 *    - NavigationContext : to store the region of the image to be
 *      rendered.
 *    - MouseContext : to remember the user action as a mouse press,
 *                     mouse drag
 *  
 *   WIP
 */
class Monteverdi2_EXPORT ImageViewManipulator :
    public QObject
{
  Q_OBJECT;

//
// Public methods.
public:

  /** Constructor */
  ImageViewManipulator( QObject* parent =NULL );

  /** Destructor */
  virtual ~ImageViewManipulator();

  /** */
  void mouseMoveEvent ( QMouseEvent * event );
  void mousePressEvent ( QMouseEvent * event );
  void mouseReleaseEvent ( QMouseEvent * event );
  void wheelEvent ( QWheelEvent* event);
  void resizeEvent ( QResizeEvent * event );
  void keyPressEvent( QKeyEvent * event );

  /** Methods to access to the context */
  /**
  const NavigationContextType& GetNavigationContext() const
  {
    return m_NavigationContext;
  }
  */
  inline
    const ImageRegionType&
    GetViewportImageRegion() const
  {
    return m_NavigationContext.m_ViewportImageRegion;
  }

  inline 
    double GetIsotropicZoom() const
  {
    return m_IsotropicZoom;
  }

  /*
  const MouseContextType&  GetMouseContextType() const
  {
    return m_MouseContext;
  }
  */

  void SetImageLargestRegion(const ImageRegionType & largestRegion)
  {
    // Compute the intial scale factor 
    double factorX = (double)m_NavigationContext.m_ViewportImageRegion.GetSize()[0]
      /(double)(largestRegion.GetSize()[0]);
    double factorY = (double)m_NavigationContext.m_ViewportImageRegion.GetSize()[1]
      /(double)(largestRegion.GetSize()[1]);

    double scale = std::min(factorX, factorY);
    this->Zoom(scale);

    // store the image region
    m_NavigationContext.m_ModelImageRegion = largestRegion;
    
    // Need to call ConstrainRegion here cause not called when the input image
    // is opened
    this->ConstrainRegion(
      m_NavigationContext.m_ViewportImageRegion,
      m_NavigationContext.m_ModelImageRegion
      );
  }

//
// SIGNALS.
signals:

//
// Public SLOTS.
public slots:
  void InitializeContext(int width, int height);

//
// Protected methods.
protected:
  void ConstrainRegion( ImageRegionType& region, const ImageRegionType& largest);
  
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
  NavigationContextType  m_NavigationContext;
  MouseContextType       m_MouseContext;

  double                 m_IsotropicZoom;

//
// SLOTS.
private slots:
};

} // end namespace 'mvd'

#endif // __mvdImageViewManipulator_h
