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
#include "mvdTypes.h"
#include "mvdAbstractViewManipulator.h"


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
 * \class ImageViewManipulator
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
class Monteverdi2_EXPORT ImageViewManipulator :
    public AbstractViewManipulator
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:

  /** \brief Constructor. */
  ImageViewManipulator( QObject* parent =NULL );

  /** \brief Destructor. */
  virtual ~ImageViewManipulator();

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
                                 const SpacingType& spacing);
  void OnViewportRegionChanged(double Xpc, double Ypc);

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:
  /** */
  void ConstrainRegion( ImageRegionType& region, const ImageRegionType& largest);

  /** */
  void CenterRegion(double scale);

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
  /** */
  void Zoom(const double scale);

  /** */
  

//
// Private attributes.
private:
  /** */
  double m_PreviousIsotropicZoom;

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

#endif // __mvdImageViewManipulator_h
