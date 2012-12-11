/*=========================================================================

  Program:   Monteverdi2
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "mvdImageViewManipulator.h"

//
// System includes (sorted by alphabetic order)

//
// Qt includes (sorted by alphabetic order)

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
  std::cout <<" !!!! ImageViewManipulator::MousePressEvent  " << std::endl;
  std::cout <<"x " << event->x()<< std::endl;

  // m_MousePressEventX = event->x();
  // m_MousePressEventY = event->y();
}

void ImageViewManipulator::mouseMoveEvent(  QMouseEvent * event)
{
  std::cout <<" ImageViewManipulator::MouseMoveEvent" << std::endl;
}

void ImageViewManipulator::mouseReleaseEvent(  QMouseEvent * event)
{
  std::cout <<" ImageViewManipulator::MouseReleaseEvent" << std::endl;
}

void ImageViewManipulator::resizeEvent ( QResizeEvent * event )
{
  std::cout <<" ImageViewManipulator::resizeEvent" << std::endl;
  
  std::cout <<"Widget Resized with new size "<< event->size().width() << ","
            << event->size().height() << std::endl;

  //emit widgetResized(event->size().width(), event->size().height());
}


/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/

/*****************************************************************************/

} // end namespace 'mvd'
