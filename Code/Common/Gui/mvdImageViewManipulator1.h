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

#ifndef __mvdImageViewManipulator1_h
#define __mvdImageViewManipulator1_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

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
#include "Core/mvdTypes.h"
#include "Gui/mvdAbstractViewManipulator1.h"


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

/**
 * \class ImageViewManipulator1
 *
 *  \brief This class handles the event related to a QGLWidget. It also
 *  handles :
 *    - NavigationContext : to store the region of the image to be
 *      rendered.
 *    - MouseContext : to remember the user action as a mouse press,
 *                     mouse drag
 *  
 *   WIP
 */
class Monteverdi2_EXPORT ImageViewManipulator1 :
    public AbstractViewManipulator1
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:

  /** \brief Constructor. */
  ImageViewManipulator1( QObject* parent =NULL );

  /** \brief Destructor. */
  virtual ~ImageViewManipulator1();

  /** */
  void mouseMoveEvent ( QMouseEvent * event );
  /** */
  void mousePressEvent ( QMouseEvent * event );
  /** */
  void mouseReleaseEvent ( QMouseEvent * event );
  /** */
  void wheelEvent ( QWheelEvent* event);
  /** */
  void resizeEvent ( QResizeEvent * event );
  /** */
  void keyPressEvent( QKeyEvent * event );

  /** */
  bool HasZoomChanged() const;

  /*-[ PUBLIC SLOTS SECTION ]-----------------------------------------------**/

//
// Public SLOTS.
public slots:
  void OnModelImageRegionChanged(const ImageRegionType & largestRegion, 
                                 const SpacingType& spacing, 
                                 const PointType& origin );

  void OnModelImageRegionChanged(const ImageRegionType & largestRegion, 
                                 const SpacingType& spacing, 
                                 const PointType& origin,
                                 const PointType& centerPoint,
                                 double zoomLevel);

  void OnViewportRegionChanged(double Xpc, double Ypc);

  void OnUserScaleEditingFinished(const QString & scale);

  void OnUserZoomIn();

  void OnUserZoomOut();

  void OnUserZoomExtent();

  void OnUserZoomFull();

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:
  void ViewportRegionRepresentationChanged(const PointType&, const PointType&);
  void CurrentScaleUpdated(const QString&);
  

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:
  /** */
  void ConstrainRegion( ImageRegionType& region, const ImageRegionType& largest);

  /** */
  void CenterRegion(double scale);

  void KeepMousePosition(double scale);

  /** */
  void ResizeRegion(unsigned int w, unsigned int h);

  /** */
  void moveRegion(double dx, double dy);
  
//
// Protected attributes.
protected:

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Private types.
private:

//
// Private methods.
private:
  /** Zoom view by a given factor */
  void ZoomBy(const double factor);

  /** Zoom to a given scale */
  void ZoomTo(const double scale);

  /** */
  void PropagateViewportRegionChanged(const ImageRegionType& region);

  /** */
  void UpdateScale();
 
//
// Private attributes.
private:
  /** */
  double m_PreviousIsotropicZoom;

  /** Zoom multiplier: factor of zoom when zoom actions are triggered */
  double m_ZoomMultiplier;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:
};

} // end namespace 'mvd'

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{
} // end namespace 'mvd'

#endif // __mvdImageViewManipulator1_h
