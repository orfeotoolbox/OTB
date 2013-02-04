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

#include "mvdImageViewManipulator.h"

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
  TRANSLATOR mvd::ImageViewManipulator

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/

/*****************************************************************************/
ImageViewManipulator
::ImageViewManipulator( QObject* parent ) :
  QObject( parent ),
  m_NavigationContext(),
  m_MouseContext(),
  m_IsotropicZoom(1)
{
  // TODO: Remove later because initialized in struct's default constructor and resizeEvent().
  this->InitializeContext(1,1);
}

/*****************************************************************************/
ImageViewManipulator
::~ImageViewManipulator()
{
}

/*******************************************************************************/
void
ImageViewManipulator
::InitializeContext(int width, int height)
{
  ImageRegionType::SizeType  initialSize;
  initialSize[0] = width;
  initialSize[1] = height;

  // initialize with the given size
  m_NavigationContext.m_ViewportImageRegion.SetSize(initialSize);
}

/******************************************************************************/
void
ImageViewManipulator
::mousePressEvent(QMouseEvent * event)
{
  // Update the context with the pressed position
  m_MouseContext.x = event->x();
  m_MouseContext.y = event->y();

  // Update the context with the pressed position for the mouseMoveEvent
  m_MouseContext.xMove = event->x();
  m_MouseContext.yMove = event->y();
}

/******************************************************************************/
void
ImageViewManipulator
::mouseMoveEvent( QMouseEvent * event)
{
  // Update the mouse context
  m_MouseContext.dx = -event->x() + m_MouseContext.xMove;
  m_MouseContext.dy = -event->y() + m_MouseContext.yMove;

  // moveRegion 
  this->moveRegion( m_MouseContext.dx,  m_MouseContext.dy);

  // Update the position of the first press to take into account the
  // last drag
  m_MouseContext.xMove -=  m_MouseContext.dx;
  m_MouseContext.yMove -=  m_MouseContext.dy;
}

/******************************************************************************/
void
ImageViewManipulator
::moveRegion(double dx, double dy)
{
  // Update the navigation context
  ImageRegionType & currentRegion = m_NavigationContext.m_ViewportImageRegion;

  // Apply the offset to the (start) index of the stored region
  ImageRegionType::OffsetType offset;
  offset[0] = dx/ m_IsotropicZoom;
  offset[1] = dy/m_IsotropicZoom;
 
  // Apply the offset to the (start) index of the stored region
  IndexType    index = currentRegion.GetIndex() + offset;
  currentRegion.SetIndex(index);

  // Constraint the region to the largestPossibleRegion
  this->ConstrainRegion(currentRegion, m_NavigationContext.m_ModelImageRegion);
}

/******************************************************************************/
void
ImageViewManipulator
::mouseReleaseEvent(  QMouseEvent * event)
{
  //TODO: Implement mouseReleaseEvent.
  //std::cout <<" Not Implemented yet ..." << std::endl;
}

/******************************************************************************/
void ImageViewManipulator
::resizeEvent( QResizeEvent * event )
{
  // Update the navigation context
  ImageRegionType & currentRegion = m_NavigationContext.m_ViewportImageRegion;

  // Get the new widget size
  ImageRegionType::SizeType size;
  size[0] = event->size().width();
  size[1] = event->size().height();
    
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
ImageViewManipulator
::wheelEvent(  QWheelEvent * event)
{
  // Compute the new scale
  double scaleRatio = 1.25;
  double numDegrees = event->delta() / 8.;
  int    nbSteps    = static_cast<int>(numDegrees / 15.);

  double scale = vcl_pow(scaleRatio, nbSteps);

  // store the position under the mouse cursor
  m_MouseContext.x = event->x();
  m_MouseContext.y = event->y();

  // The viewPort Region must be adapted to this zoom ratio
  ImageRegionType & currentRegion = m_NavigationContext.m_ViewportImageRegion;

  // center the region on the position under the cursor
  IndexType        origin = currentRegion.GetIndex();
  double centerX = origin[0] + currentRegion.GetSize()[0]/2;
  double centerY = origin[1] + currentRegion.GetSize()[1]/2;

  // new center
  double newCenterX = m_MouseContext.x * scale + ( ( centerX - m_MouseContext.x ) );
  double newCenterY = m_MouseContext.y * scale + ( ( centerY - m_MouseContext.y ) );

  // new index
  IndexType  newIndex;
  newIndex[0] = newCenterX - (m_NavigationContext.m_ViewportImageRegion.GetSize()[0] /2) ;
  newIndex[1] = newCenterY - (m_NavigationContext.m_ViewportImageRegion.GetSize()[1] /2) ;
  currentRegion.SetIndex(newIndex);

  // Constraint this region to the LargestPossibleRegion
  this->ConstrainRegion(currentRegion, m_NavigationContext.m_ModelImageRegion);
 
  // rescale the viewport region
  this->Zoom(scale);
}

// void
// ImageViewManipulator
// ::CenterRegion()
// {

// }

/******************************************************************************/
void
ImageViewManipulator
::Zoom(const double scale)
{
  // compute the new size
  double sizeX = m_NavigationContext.m_SizeXBeforeConstrain / scale;
  double sizeY = m_NavigationContext.m_SizeYBeforeConstrain / scale;
  
  // check that the new size is greater than 30x30
  // check that the new isoZoom is not too low and not too large
  // TODO : compute automatically the minSize and the isoZoom range ???
  if (sizeX > 30 && sizeY > 30  &&
      m_IsotropicZoom * scale > 0.01 &&
      m_IsotropicZoom * scale < 10.)
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
ImageViewManipulator
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
ImageViewManipulator
::keyPressEvent( QKeyEvent * event )
{

  switch(event->key())
    {
    case Qt::Key_Minus: 
      Zoom(0.8);
      break;
    case Qt::Key_Plus: 
      Zoom(1.25);
      break;
    case Qt::Key_Left:
      moveRegion(-10,0);
      break;
    case Qt::Key_Right:
      moveRegion(10,0);
      break;
    case Qt::Key_Up:
      moveRegion(0,10);
      break;
    case Qt::Key_Down:
      moveRegion(0,-10);
      break;
    default:
      break;
    }
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/

/*****************************************************************************/

} // end namespace 'mvd'
