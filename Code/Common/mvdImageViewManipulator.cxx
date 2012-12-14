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
  QObject( parent )
{
  this->InitializeContext(500,500);
}

/*****************************************************************************/
ImageViewManipulator
::~ImageViewManipulator()
{
}


void ImageViewManipulator::InitializeContext(int width, int height)
{
  ImageRegionType::SizeType  initialSize;
  initialSize[0] = width;
  initialSize[1] = height;
  
  // initialize with the given size
  m_NavigationContext.bufferedRegion.SetSize(initialSize);
}


void ImageViewManipulator::mousePressEvent(  QMouseEvent * event)
{
  // Update the context with the pressed position
  m_MouseContext.x = event->x();
  m_MouseContext.y = event->y();
}

void ImageViewManipulator::mouseMoveEvent(  QMouseEvent * event)
{
  //std::cout <<" ImageViewManipulator::MouseMoveEvent" << std::endl;

  // Update the mouse context
  m_MouseContext.dx = -event->x() + m_MouseContext.x;
  m_MouseContext.dy = -event->y() + m_MouseContext.y;

  // Update the navigation context
  ImageRegionType & currentRegion = m_NavigationContext.bufferedRegion;

  // print the region
  //std::cout << "Region Before offset : "<<   currentRegion << std::endl;

  // Apply the offset to the (start) index of the stored region
  ImageRegionType::OffsetType offset;
  offset[0] = m_MouseContext.dx;
  offset[1] = m_MouseContext.dy;
 
  // Apply the offset to the (start) index of the stored region
  IndexType    index = currentRegion.GetIndex() + offset;
  currentRegion.SetIndex(index);

  // print the region
  //std::cout << "Region After offset : "<< m_NavigationContext.bufferedRegion   << std::endl;    
}

void ImageViewManipulator::mouseReleaseEvent(  QMouseEvent * event)
{
  std::cout <<" Not Implemented yet ..." << std::endl;
}

void ImageViewManipulator::resizeEvent ( QResizeEvent * event )
{
  //std::cout <<" ImageViewManipulator::resizeEvent" << std::endl;
  //std::cout <<"Widget Resized with new size "<< event->size().width() << ","
  //<< event->size().height() << std::endl;

  // Update the navigation context
  ImageRegionType & currentRegion = m_NavigationContext.bufferedRegion;

  // print the region
  //std::cout << "Region Before offset : "<<   currentRegion << std::endl;

  // Get the new widget size
  SizeType size;
  size[0] = event->size().width();
  size[1] = event->size().height();
 
  // Update the stored region with the new size
  currentRegion.SetSize(size);
  //std::cout << "Region After Resize : "<<   m_NavigationContext.bufferedRegion << std::endl;    

  //emit widgetResized(event->size().width(), event->size().height());
}


/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/

/*****************************************************************************/

} // end namespace 'mvd'
