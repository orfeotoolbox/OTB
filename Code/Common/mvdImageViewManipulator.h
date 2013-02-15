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
    public AbstractViewManipulator
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

  inline
    bool HasZoomChanged() const
  {
    bool res = false;

    if (vcl_abs(m_IsotropicZoom - m_PreviousIsotropicZoom) > 0.00000001 )
      {
      res = true;
      }

    return res;
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

//
// Private methods.
private:

  void Zoom(const double scale);
  void moveRegion(double dx, double dy);

//
// Private attributes.
private:
  double                 m_PreviousIsotropicZoom;

//
// SLOTS.
private slots:
};

} // end namespace 'mvd'

#endif // __mvdImageViewManipulator_h
