/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkSphereFunctionControl.txx,v $
  Language:  C++
  Date:      $Date: 2002/01/15 20:25:09 $
  Version:   $Revision: 1.4 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "fltkSphereFunctionControl.h"

namespace fltk {
  
template <class ImagePixelType>
SphereFunctionControl<ImagePixelType>
::SphereFunctionControl()
{

  m_SpatialFunction = SpatialFunctionType::New();
  m_Shape           = ShapeType::New();

  ShapeType::ColorType color;
  color.SetRed(   0.8f );
  color.SetGreen( 1.0f );
  color.SetBlue(  0.8f );
  m_Shape->SetColor( color );
  m_Shape->SetNumberOfSlices( 60 );
  m_Shape->SetNumberOfStacks( 30 );

  m_DrawCommand  = DrawCommandType::New();
  m_DrawCommand->SetCallbackFunction( this, &Self::glDraw );

}


  
template <class ImagePixelType>
SphereFunctionControl<ImagePixelType>
::~SphereFunctionControl()
{
}


   


template <class ImagePixelType>
void
SphereFunctionControl<ImagePixelType>
::SetParametersFromGUI( void )
{ 
  this->SetRadius( radiusAdjuster->value() );
  this->SetCenter( xAdjuster->value(),
                   yAdjuster->value(),
                   zAdjuster->value() );
}




template <class ImagePixelType>
void
SphereFunctionControl<ImagePixelType>
::SetRadius( double radius )
{
 
  this->fltkSphereFunctionControlGUI::SetRadius( radius );

  m_Shape->SetRadius( radius );
  m_SpatialFunction->SetRadius( radius );

  this->InvokeEvent( fltk::RedrawEvent() );

}





template <class ImagePixelType>
void
SphereFunctionControl<ImagePixelType>
::SetCenter( double x, double y, double z )
{
    
  typename SpatialFunctionType::InputType functionCenter;
  functionCenter[0] = x;
  functionCenter[1] = y;
  functionCenter[2] = z;

  m_SpatialFunction->SetCenter( functionCenter );

  this->fltkSphereFunctionControlGUI::SetCenterX( x );
  this->fltkSphereFunctionControlGUI::SetCenterY( y );
  this->fltkSphereFunctionControlGUI::SetCenterZ( z );

  this->InvokeEvent( fltk::RedrawEvent() );

}




template <class ImagePixelType>
void
SphereFunctionControl<ImagePixelType>
::SetCenterX( double x )
{
    
  const double y = yAdjuster->value();
  const double z = zAdjuster->value();

  this->SetCenter( x, y , z );

}




template <class ImagePixelType>
void
SphereFunctionControl<ImagePixelType>
::SetCenterY( double y )
{
    
  const double x = xAdjuster->value();
  const double z = zAdjuster->value();

  this->SetCenter( x, y , z );

}




template <class ImagePixelType>
void
SphereFunctionControl<ImagePixelType>
::SetCenterZ( double z )
{
    
  const double x = xAdjuster->value();
  const double y = yAdjuster->value();

  this->SetCenter( x, y , z );

}




template <class ImagePixelType>
void
SphereFunctionControl<ImagePixelType>
::glDraw( void ) const
{
  glPushMatrix();
  glTranslated( 
                xAdjuster->value(),
                yAdjuster->value(),
                zAdjuster->value() );

  m_Shape->glDraw();
  glPopMatrix();

}



} // end namespace itk
