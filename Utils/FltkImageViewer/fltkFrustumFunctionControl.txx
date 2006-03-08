/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkFrustumFunctionControl.txx,v $
  Language:  C++
  Date:      $Date: 2005/06/13 13:44:06 $
  Version:   $Revision: 1.5 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "fltkFrustumFunctionControl.h"

namespace fltk {
  
template <class ImagePixelType>
FrustumFunctionControl<ImagePixelType>
::FrustumFunctionControl()
{

  m_SpatialFunction = SpatialFunctionType::New();
  m_Shape           = ShapeType::New();

  ShapeType::ColorType color;
  color.SetRed(   0.8f );
  color.SetGreen( 1.0f );
  color.SetBlue(  0.8f );
  m_Shape->SetColor( color );

  m_DrawCommand  = DrawCommandType::New();
  m_DrawCommand->SetCallbackFunction( this, &Self::glDraw );

  m_RotationPlane = RotateInXZPlane;

}


  
template <class ImagePixelType>
FrustumFunctionControl<ImagePixelType>
::~FrustumFunctionControl()
{
}


   


template <class ImagePixelType>
void
FrustumFunctionControl<ImagePixelType>
::SetParametersFromGUI( void )
{ 
  this->SetAngleZ( angleZRoller->value() );

  this->SetApertureAngleX( apertureAngleXRoller->value() );
  this->SetApertureAngleY( apertureAngleYRoller->value() );

  this->SetApex( xAdjuster->value(),
                 yAdjuster->value(),
                 zAdjuster->value() );
}




template <class ImagePixelType>
void
FrustumFunctionControl<ImagePixelType>
::SetAngleZ( double angle )
{
 
  this->fltkFrustumFunctionControlGUI::SetAngleZ( angle );

  m_AngleZ = angle;
  m_SpatialFunction->SetAngleZ( angle );

  this->InvokeEvent( fltk::RedrawEvent() );

}




template <class ImagePixelType>
void
FrustumFunctionControl<ImagePixelType>
::SetApertureAngleX( double angle )
{
 
  this->fltkFrustumFunctionControlGUI::SetApertureAngleX( angle );

  m_Shape->SetApertureAngleX( angle );
  m_SpatialFunction->SetApertureAngleX( angle );

  this->InvokeEvent( fltk::RedrawEvent() );

}



template <class ImagePixelType>
void
FrustumFunctionControl<ImagePixelType>
::SetApertureAngleY( double angle )
{
 
  this->fltkFrustumFunctionControlGUI::SetApertureAngleY( angle );

  m_Shape->SetApertureAngleY( angle );
  m_SpatialFunction->SetApertureAngleY( angle );

  this->InvokeEvent( fltk::RedrawEvent() );

}




template <class ImagePixelType>
void
FrustumFunctionControl<ImagePixelType>
::SetBottomPlane( double bottomPlane )
{
 
  this->fltkFrustumFunctionControlGUI::SetBottomPlane( bottomPlane );

  m_Shape->SetBottomPlane( bottomPlane );
  m_SpatialFunction->SetBottomPlane( bottomPlane );

  this->InvokeEvent( fltk::RedrawEvent() );

}





template <class ImagePixelType>
void
FrustumFunctionControl<ImagePixelType>
::SetTopPlane( double topPlane )
{
 
  this->fltkFrustumFunctionControlGUI::SetTopPlane( topPlane );

  m_Shape->SetTopPlane( topPlane );
  m_SpatialFunction->SetTopPlane( topPlane );

  this->InvokeEvent( fltk::RedrawEvent() );

}




template <class ImagePixelType>
void
FrustumFunctionControl<ImagePixelType>
::SetApex( double x, double y, double z )
{
    
  typename SpatialFunctionType::InputType functionApex;
  functionApex[0] = x;
  functionApex[1] = y;
  functionApex[2] = z;

  m_SpatialFunction->SetApex( functionApex );

  this->fltkFrustumFunctionControlGUI::SetApexX( x );
  this->fltkFrustumFunctionControlGUI::SetApexY( y );
  this->fltkFrustumFunctionControlGUI::SetApexZ( z );

  this->InvokeEvent( fltk::RedrawEvent() );

}




template <class ImagePixelType>
void
FrustumFunctionControl<ImagePixelType>
::SetApexX( double x )
{
    
  const double y = yAdjuster->value();
  const double z = zAdjuster->value();

  this->SetApex( x, y , z );

}




template <class ImagePixelType>
void
FrustumFunctionControl<ImagePixelType>
::SetApexY( double y )
{
    
  const double x = xAdjuster->value();
  const double z = zAdjuster->value();

  this->SetApex( x, y , z );

}




template <class ImagePixelType>
void
FrustumFunctionControl<ImagePixelType>
::SetApexZ( double z )
{
    
  const double x = xAdjuster->value();
  const double y = yAdjuster->value();

  this->SetApex( x, y , z );

}




template <class ImagePixelType>
void
FrustumFunctionControl<ImagePixelType>
::glDraw( void ) const
{
  glPushMatrix();
  glTranslated( 
                xAdjuster->value(),
                yAdjuster->value(),
                zAdjuster->value() );
  glRotated(     180.0, 0.0,  1.0, 0.0 );

  if( m_RotationPlane == RotateInXZPlane )
    {
    glRotated(  m_AngleZ, 0.0, -1.0, 0.0 );
    }
  else 
    {
    glRotated(  m_AngleZ, -1.0, 0.0, 0.0 );
    }
 
  m_Shape->glDraw();
  glPopMatrix();

}



} // end namespace itk
 
