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

#include "mvdQuicklookViewManipulator1.h"

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

namespace mvd
{
/*
  TRANSLATOR mvd::QuicklookViewManipulator1

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/

/*****************************************************************************/
QuicklookViewManipulator1
::QuicklookViewManipulator1( QObject* parent ) :
  AbstractViewManipulator1( parent ),
  m_PreviousIsotropicZoom(1.)
{
}

/*****************************************************************************/
QuicklookViewManipulator1
::~QuicklookViewManipulator1()
{
}

/*****************************************************************************/
bool
QuicklookViewManipulator1
::HasZoomChanged() const
{
  return false;
}

/******************************************************************************/
void
QuicklookViewManipulator1
::mousePressEvent(QMouseEvent * event)
{
  // Update the pressed flag
  m_MouseContext.pressed = true;

  // Update the context with the pressed position
  m_MouseContext.x = event->x();
  m_MouseContext.y = event->y();

  // Update the context with the pressed position for the mouseMoveEvent
  m_MouseContext.xMove = event->x();
  m_MouseContext.yMove = event->y();

  // compute the physcial coordinates
  // computation takes into the origin of the viewport in the widget,
  // the spacing of the rendered image and the isotropicZoom
  IndexType index;
  index[0] = event->x();
  index[1] = event->y();
  PointType physicalPt = ScreenIndexToPhysicalPoint(index);

  // emit the new viewport center on physical coordinates
  emit ViewportRegionChanged(physicalPt[0], physicalPt[1]);
}

/******************************************************************************/
void
QuicklookViewManipulator1
::mouseMoveEvent( QMouseEvent * event)
{
  //If the mouse is not pressed just update the context
  if(!m_MouseContext.pressed)
    {
    // Update the context with the pressed position
    m_MouseContext.x = event->x();
    m_MouseContext.y = event->y();

    // Update the context with the pressed position for the mouseMoveEvent
    m_MouseContext.xMove = event->x();
    m_MouseContext.yMove = event->y();
    }
  //In case where the mouse is pressed, update the quicklook context

  //FIXME Bug when you move the cursor to the left or to the upper region (negative y
  //coordinate) where red square is shifted to the other side
  //It can be fix easily in the second case by replacing m_MouseContext.yMove = event->y() by
  //std::max(0,event.y())
  //But I don't know how to solve this issue regarding the X coordinate as the
  //xmin coordinate of the quicklook is not 0
  else
    {
    // Update the context with the pressed position for the mouseMoveEvent
    m_MouseContext.xMove = event->x();
    m_MouseContext.yMove = event->y();

    // compute the physical coordinates
    // computation takes into the origin of the viewport in the widget,
    // the spacing of the rendered image and the isotropicZoom
    IndexType index;
    index[0] = event->x();
    //FIXME replace event->y() by std::max(0,event->y())?
    index[1] = event->y();
    PointType physicalPt = ScreenIndexToPhysicalPoint(index);

    // emit the new viewport center on physical coordinates
    emit ViewportRegionChanged(physicalPt[0], physicalPt[1]);
    }
}

/******************************************************************************/
void
QuicklookViewManipulator1
::moveRegion(double dx, double dy)
{
  // Update the navigation context
  ImageRegionType & currentRegion = m_NavigationContext.m_ViewportImageRegion;

  // Apply the offset to the (start) index of the stored region
  ImageRegionType::OffsetType offset;
  offset[0] = static_cast<ImageRegionType::OffsetType::OffsetValueType> (dx/m_IsotropicZoom + 0.5);
  offset[1] = static_cast<ImageRegionType::OffsetType::OffsetValueType> (dy/m_IsotropicZoom + 0.5);

  // Apply the offset to the (start) index of the stored region
  IndexType    index = currentRegion.GetIndex() + offset;
  currentRegion.SetIndex(index);

  // Constraint the region to the largestPossibleRegion
  this->ConstrainRegion(currentRegion,
                        m_NavigationContext.m_ModelImageRegion);
}

/******************************************************************************/
void
QuicklookViewManipulator1
::mouseReleaseEvent(  QMouseEvent * event)
{
  //TODO: Implement mouseReleaseEvent.
  //std::cout <<" Not Implemented yet ..." << std::endl;

  // Update the pressed flag
  m_MouseContext.pressed = false;

  // Update the context with the pressed position
  m_MouseContext.x = event->x();
  m_MouseContext.y = event->y();

  // Update the context with the pressed position for the mouseMoveEvent
  m_MouseContext.xMove = event->x();
  m_MouseContext.yMove = event->y();
}

/******************************************************************************/
void QuicklookViewManipulator1
::resizeEvent( QResizeEvent * event )
{
  // set back the zoom to 1
  m_IsotropicZoom = 1.;

  m_PreviousIsotropicZoom = 1.;

  this->ResizeRegion( event->size().width(),
                      event->size().height());

  // compute the intial scale factor to fit to screen
  double factorX = (double)event->size().width()
    /(double)(m_NavigationContext.m_ModelImageRegion.GetSize()[0]);
  double factorY = (double)event->size().height()
    /(double)(m_NavigationContext.m_ModelImageRegion.GetSize()[1]);

  double scale = std::min(factorX, factorY);

  this->Zoom(scale);
}

/******************************************************************************/
void QuicklookViewManipulator1
::ResizeRegion(unsigned int w, unsigned int h)
{
  // Update the navigation context
  ImageRegionType & currentRegion = m_NavigationContext.m_ViewportImageRegion;

  // Get the new widget size
  ImageRegionType::SizeType size;
  size[0] = w;
  size[1] = h;

  // Update the stored region with the new size
  currentRegion.SetSize(size);

  // Recompute the size before
  m_NavigationContext.m_SizeXBeforeConstrain = (double)size[0] / this->GetIsotropicZoom();
  m_NavigationContext.m_SizeYBeforeConstrain = (double)size[1] / this->GetIsotropicZoom();

  // Constraint this region to the LargestPossibleRegion
  this->ConstrainRegion(currentRegion, m_NavigationContext.m_ModelImageRegion);

  // call the rescale method with the same zoom as before (scale = 1)
  this->Zoom(1.);
}

/******************************************************************************/
void
QuicklookViewManipulator1
::wheelEvent(  QWheelEvent * event)
{
  //std::cout <<"QuicklookViewManipulator1::wheelEvent" << std::endl;
  // // compute the new scale
  // double scaleRatio = 1.25;
  // double numDegrees = event->delta() / 8.;
  // int    nbSteps    = static_cast<int>(numDegrees / 15.);

  // double scale = vcl_pow(scaleRatio, nbSteps);

  // // center the region on the center of the previous region
  // this->CenterRegion(scale);

  // // rescale the viewport region
  // this->Zoom(scale);
}

/******************************************************************************/
void
QuicklookViewManipulator1
::Zoom(const double scale)
{
  m_PreviousIsotropicZoom = m_IsotropicZoom;

  // compute the new size
  double sizeX = m_NavigationContext.m_SizeXBeforeConstrain / scale;
  double sizeY = m_NavigationContext.m_SizeYBeforeConstrain / scale;

  // check that the new size is greater than 30x30
  // check that the new isoZoom is not too low and not too large
  // TODO : compute automatically the minSize and the isoZoom range ???
  if (sizeX > 30 && sizeY > 30)
    {
    // Update the the sizeBeforeConstrain
    m_NavigationContext.m_SizeXBeforeConstrain = sizeX;
    m_NavigationContext.m_SizeYBeforeConstrain = sizeY;

    // Update the viewort region with the new size
    ImageRegionType::SizeType size;
    size[0] = static_cast<unsigned int>(sizeX);
    size[1] = static_cast<unsigned int>(sizeY);

    // The viewPort Region must be adapted to this zoom ratio
    ImageRegionType & currentRegion = m_NavigationContext.m_ViewportImageRegion;

    // Update the stored region with the new size
    currentRegion.SetSize(size);

    // Constraint this region to the LargestPossibleRegion
    this->ConstrainRegion(currentRegion, m_NavigationContext.m_ModelImageRegion);

    // Update the isotropicZoom
    m_IsotropicZoom *= scale;
    }
}

/******************************************************************************/
void
QuicklookViewManipulator1
::CenterRegion(double scale)
{
  if( m_IsotropicZoom * scale > 0.01 && m_IsotropicZoom * scale < 10.)
    {
    // The viewPort Region must be adapted to this zoom ratio
    ImageRegionType & currentRegion = m_NavigationContext.m_ViewportImageRegion;

    // center the region on the position under the cursor
    IndexType        origin = currentRegion.GetIndex();
    double centerX = (double)(origin[0]) + (double)(currentRegion.GetSize()[0])/2.;
    double centerY = (double)(origin[1]) + (double)(currentRegion.GetSize()[1])/2.;

    // new origin
    IndexType  newIndex;
    newIndex[0] = centerX - currentRegion.GetSize()[0]/scale/2.;
    if (newIndex[0] < 0) newIndex[0] = 0;

    newIndex[1] = centerY - currentRegion.GetSize()[1]/scale/2.;
    if (newIndex[1] < 0) newIndex[1] = 0;

    // set the new origin
    currentRegion.SetIndex(newIndex);

    // Constraint this region to the LargestPossibleRegion
    this->ConstrainRegion(currentRegion, m_NavigationContext.m_ModelImageRegion);
    }
}

/******************************************************************************/
void
QuicklookViewManipulator1
::ConstrainRegion( ImageRegionType& region, const ImageRegionType& largest)
{
  ImageRegionType::SizeType zeroSize;
  zeroSize.Fill(0);

  if (largest.GetSize() != zeroSize)
    {
    // Else we can constrain it
    IndexType                     index = region.GetIndex();
    ImageRegionType::SizeType size = region.GetSize();

    // If region is larger than big, then crop
    if (region.GetSize()[0] > largest.GetSize()[0])
      {
      size[0] = largest.GetSize()[0];
      }
    if (region.GetSize()[1] > largest.GetSize()[1])
      {
      size[1] = largest.GetSize()[1];
      }

    // Else we can constrain it
    // For each dimension
    for (unsigned int dim = 0; dim < ImageRegionType::ImageDimension; ++dim)
      {
      // push left if necessary
      if (region.GetIndex()[dim] < largest.GetIndex()[dim])
        {
        index[dim] = largest.GetIndex()[dim];
        }
      // push right if necessary
      if (index[dim] + size[dim] >= largest.GetIndex()[dim] + largest.GetSize()[dim])
        {
        index[dim] = largest.GetIndex()[dim] + largest.GetSize()[dim] - size[dim];
        }
      }
    region.SetSize(size);
    region.SetIndex(index);
    }
}

/******************************************************************************/
void
QuicklookViewManipulator1
::keyPressEvent( QKeyEvent * event )
{
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/
void
QuicklookViewManipulator1
::OnModelImageRegionChanged(const ImageRegionType & largestRegion,
                            const SpacingType & spacing,
                            const PointType& origin )
{
  // update spacing
  SetSpacing(spacing);

  // native spacing
  m_NativeSpacing = spacing;

  // update origin
  SetOrigin(origin);

  // set back the zoom to 1
  m_IsotropicZoom = 1.;

  m_PreviousIsotropicZoom = 1.;

  // store the image largest region
  m_NavigationContext.m_ModelImageRegion = largestRegion;

  // set back the origin to O
  IndexType nullIndex;
  nullIndex.Fill(0);
  m_NavigationContext.m_ViewportImageRegion.SetIndex(nullIndex);

  // get the widget size and use it to resize the Viewport region
  QWidget* parent_widget = qobject_cast< QWidget* >( parent() );

  if (parent_widget)
    {
    this->ResizeRegion(parent_widget->width(), parent_widget->height());
    }

  // compute the intial scale factor to fit to screen
  double factorX = (double)m_NavigationContext.m_ViewportImageRegion.GetSize()[0]
    /(double)(largestRegion.GetSize()[0]);
  double factorY = (double)m_NavigationContext.m_ViewportImageRegion.GetSize()[1]
    /(double)(largestRegion.GetSize()[1]);

  double scale = std::min(factorX, factorY);

  this->Zoom(scale);
}
/*****************************************************************************/

} // end namespace 'mvd'
