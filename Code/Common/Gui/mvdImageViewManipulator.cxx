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
  AbstractViewManipulator( parent ),
  m_PreviousIsotropicZoom(1.)
{
}

/*****************************************************************************/
ImageViewManipulator
::~ImageViewManipulator()
{
}

/*****************************************************************************/
bool
ImageViewManipulator
::HasZoomChanged() const
{
  bool res = false;

  if (vcl_abs(m_IsotropicZoom - m_PreviousIsotropicZoom) > 0.00000001 )
    {
    res = true;
    }

  return res;
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

  m_MouseContext.moveOriginX = m_NavigationContext.m_ViewportImageRegion.GetIndex()[0];
  m_MouseContext.moveOriginY = m_NavigationContext.m_ViewportImageRegion.GetIndex()[1];
}

/******************************************************************************/
void
ImageViewManipulator
::mouseMoveEvent( QMouseEvent * event)
{
  m_PreviousIsotropicZoom = m_IsotropicZoom;

  // Update the context with the pressed position
  m_MouseContext.xMove = event->x();
  m_MouseContext.yMove = event->y();

  ImageRegionType::OffsetType offset;

  int dx = m_MouseContext.x - m_MouseContext.xMove;
  int dy = m_MouseContext.y - m_MouseContext.yMove;

  offset[0] = static_cast<ImageRegionType::OffsetType::OffsetValueType> (dx/m_IsotropicZoom + 0.5);
  offset[1] = static_cast<ImageRegionType::OffsetType::OffsetValueType> (dy/m_IsotropicZoom + 0.5);
  IndexType newOrigin;
  newOrigin[0] = m_MouseContext.moveOriginX;
  newOrigin[1] = m_MouseContext.moveOriginY;

  newOrigin+=offset;

  // Update the navigation context
  ImageRegionType & currentRegion = m_NavigationContext.m_ViewportImageRegion;
  
  currentRegion.SetIndex(newOrigin);

  // Constraint the region to the largestPossibleRegion
  this->ConstrainRegion(currentRegion, m_NavigationContext.m_ModelImageRegion);

  // tell the quicklook renderer to update the red square rendering
  this->PropagateViewportRegionChanged(currentRegion);  
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

  offset[0] = static_cast<ImageRegionType::OffsetType::OffsetValueType> (dx/m_IsotropicZoom + 0.5);
  offset[1] = static_cast<ImageRegionType::OffsetType::OffsetValueType> (dy/m_IsotropicZoom + 0.5);

  // Apply the offset to the (start) index of the stored region
  IndexType    index = currentRegion.GetIndex() + offset;
  currentRegion.SetIndex(index);

  // Constraint the region to the largestPossibleRegion
  this->ConstrainRegion(currentRegion, m_NavigationContext.m_ModelImageRegion);

  // tell the quicklook renderer to update the red square rendering
  this->PropagateViewportRegionChanged(currentRegion);  
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
  this->ResizeRegion( event->size().width(),
                      event->size().height());
}

/******************************************************************************/
void ImageViewManipulator
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
ImageViewManipulator
::wheelEvent(  QWheelEvent * event)
{
  // compute the new scale
  double scaleRatio = 1.25;
  double numDegrees = event->delta() / 8.;
  int    nbSteps    = static_cast<int>(numDegrees / 15.);

  double scale = vcl_pow(scaleRatio, nbSteps);

  // center the viewport on the center of the previous region
  this->CenterRegion(scale);

  // rescale the viewport region
  this->Zoom(scale);
}

/******************************************************************************/
void
ImageViewManipulator
::Zoom(const double scale)
{
  m_PreviousIsotropicZoom = m_IsotropicZoom;

  // compute the new size
  double sizeX = m_NavigationContext.m_SizeXBeforeConstrain / scale;
  double sizeY = m_NavigationContext.m_SizeYBeforeConstrain / scale;
  
  // check that the new size is greater than 1x1
  // check that the new isoZoom is not too low: 0.001
  if ( (unsigned int)(sizeX) > 1  && 
       (unsigned int)(sizeY) > 1  &&  
       m_IsotropicZoom * scale > 0.001 )
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

    // tell the quicklook renderer to update the red square rendering
    this->PropagateViewportRegionChanged(currentRegion); 

    // emit the scale change
    this->UpdateScale();
    }
}

/******************************************************************************/
void
ImageViewManipulator
::CenterRegion(double scale)
{
  if( m_IsotropicZoom * scale > 0.001 )
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
      CenterRegion(0.8);
      Zoom(0.8);
      break;
    case Qt::Key_Plus: 
      CenterRegion(1.25);
      Zoom(1.25);
      break;
    case Qt::Key_Left:
      moveRegion(-static_cast<double>(m_NavigationContext.m_ViewportImageRegion.GetSize(0))*m_IsotropicZoom/4 ,0);
      break;
    case Qt::Key_Right:
      moveRegion(static_cast<double>(m_NavigationContext.m_ViewportImageRegion.GetSize(0))*m_IsotropicZoom/4 ,0);
      break;
    case Qt::Key_Up:
      moveRegion(0,-static_cast<double>(m_NavigationContext.m_ViewportImageRegion.GetSize(1))*m_IsotropicZoom/4 );
      break;
    case Qt::Key_Down:
      moveRegion(0,static_cast<double>(m_NavigationContext.m_ViewportImageRegion.GetSize(1))*m_IsotropicZoom/4 );
      break;
    default:
      break;
    }
}

/*****************************************************************************/
void
ImageViewManipulator
::PropagateViewportRegionChanged(const ImageRegionType& region)
{
  // the region computed in this class are relative to the lod 0 of
  // the input image. We need then in order to transform the indicies to
  // physical points, the native spacing (lod 0) and origin
  // of the method
  PointType ul, lr;
  ul[0] = (double)(region.GetIndex()[0]) * m_NativeSpacing[0] + GetOrigin()[0];
  ul[1] = (double)(region.GetIndex()[1]) * vcl_abs(m_NativeSpacing[1]) + GetOrigin()[1];

  lr[0] = (double)( region.GetIndex()[0] + region.GetSize()[0] ) * m_NativeSpacing[0] + GetOrigin()[0];
  lr[1] = (double)( region.GetIndex()[1] + region.GetSize()[1] ) * vcl_abs(m_NativeSpacing[1]) + GetOrigin()[1];

  emit ViewportRegionRepresentationChanged(ul, lr);
}

/*****************************************************************************/
void
ImageViewManipulator
::UpdateScale()
{
  // emit the scale change
  std::ostringstream oss;

  if ( GetIsotropicZoom() > 1. )
    oss << GetIsotropicZoom() <<":1";
  else
    oss <<"1:"<< 1./GetIsotropicZoom();
    
  QString scaleString(oss.str().c_str());
  emit CurrentScaleUpdated(scaleString);
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/

/*****************************************************************************/
void ImageViewManipulator
::OnModelImageRegionChanged(const ImageRegionType & largestRegion, 
                            const SpacingType& spacing,
  const PointType& origin)
{
  // update the spacing
  SetSpacing(spacing);

  // update the origin
  SetOrigin(origin);

  // store the native spacing too (for physical coordinate
  // computations) 
  m_NativeSpacing = spacing;

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
void ImageViewManipulator
::OnViewportRegionChanged(double Xpc, double Ypc)
{
  // Update the navigation context
  ImageRegionType & currentRegion = m_NavigationContext.m_ViewportImageRegion;

  // center the region on the position under the cursor  
  IndexType   origin;
  origin[0] = static_cast<unsigned int>( (Xpc - GetOrigin()[0] ) / vcl_abs(m_NativeSpacing[0]) ) 
    - currentRegion.GetSize()[0] / 2 ;
  origin[1] = static_cast<unsigned int>( (Ypc - GetOrigin()[1] ) / vcl_abs(m_NativeSpacing[1]) ) 
    - currentRegion.GetSize()[1] / 2 ;
  currentRegion.SetIndex(origin);
  
  // Constraint this region to the LargestPossibleRegion
  this->ConstrainRegion(currentRegion, m_NavigationContext.m_ModelImageRegion);

  // tell the quicklook renderer to update the red square rendering
  this->PropagateViewportRegionChanged(currentRegion);  
  
  // force repaintGL
  qobject_cast< QWidget* >( parent() )->update();
}

/*****************************************************************************/
void ImageViewManipulator
::OnUserScaleEditingFinished(const QString & scale)
{
  //
  // extract the coordinates from the given text
  QStringList parts = scale.split( ':' );
  if ( parts.size() != 2 )
    return;

  bool xOk;
  double x = parts.at( 0 ).toDouble( &xOk );
  if ( !xOk )
    return;

  bool yOk;
  double y = parts.at( 1 ).toDouble( &yOk );
  if ( !yOk )
    return;

  // compute scale
  if (x != 0. && y != 0.)
    {
    //
    // compute the scale
    double scale = x/y;

    //
    // center the viewport
    this->CenterRegion( scale / GetIsotropicZoom() );

    //
    // apply the zoom
    this->Zoom(scale / GetIsotropicZoom());

    // force repaintGL
    qobject_cast< QWidget* >( parent() )->update();     
    }

}

/*****************************************************************************/
} // end namespace 'mvd'
