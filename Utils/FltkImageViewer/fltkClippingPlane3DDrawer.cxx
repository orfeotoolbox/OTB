/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkClippingPlane3DDrawer.cxx,v $
  Language:  C++
  Date:      $Date: 2002/10/15 15:16:54 $
  Version:   $Revision: 1.3 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef _itkClippingPlane3DDrawer_txx
#define _itkClippingPlane3DDrawer_txx

#include "fltkClippingPlane3DDrawer.h"
#include "itkImageRegionIteratorWithIndex.h"
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif
#include "fltkCommandEvents.h"


namespace fltk {


unsigned int ClippingPlane3DDrawer::counter = 0;


/**
 * Default constructor
 */
ClippingPlane3DDrawer
::ClippingPlane3DDrawer()
{

  m_X = 1.0f;
  m_Y = 0.0f;
  m_Z = 0.0f;
  m_R = 0.0f;

  m_DrawCommand = DrawCommandType::New();
  m_DrawCommand->SetCallbackFunction( this, &Self::glDraw );

  m_Id = counter;
  
  m_Enabled = false;

  counter++;

  this->UpdateTheGUI();

  rAdjuster->range( -100.0f, 100.0f );
  rAdjuster->step( 0.01f );

}


/**
 * Destructor
 */
ClippingPlane3DDrawer
::~ClippingPlane3DDrawer()
{

}



/**
 * Get Draw Command
 */
ClippingPlane3DDrawer::DrawCommandPointer
ClippingPlane3DDrawer
::GetDrawCommand(void) 
{
  return m_DrawCommand.GetPointer();
}



/**
 * Update The  GUI
 */
void
ClippingPlane3DDrawer
::UpdateTheGUI( void )
{
  
  xValueInput->value( m_X );
  yValueInput->value( m_Y );
  zValueInput->value( m_Z );

  rAdjuster->value( m_R );
  rValueOutput->value( m_R );

  if( m_Enabled )
    {
    enableCheckButton->value( 1 );
    }
  else 
    {
    enableCheckButton->value( 0 );
    }
}


/**
 * Update from  GUI
 */
void
ClippingPlane3DDrawer
::UpdateFromGUI( void )
{
  m_X = xValueInput->value();
  m_Y = yValueInput->value();
  m_Z = zValueInput->value();

  const double norm = sqrt( m_X * m_X + m_Y * m_Y + m_Z * m_Z );

  m_X /= norm;
  m_Y /= norm;
  m_Z /= norm;

  m_R = rAdjuster->value();

  m_Enabled =  ( enableCheckButton->value() == 1 )? true : false;

  this->InvokeEvent( ClippingPlaneEvent() );
}





/**
 *  Send the clipping plane data to OpenGL
 */
void 
ClippingPlane3DDrawer
::glDraw(void) const
{

  if( m_Enabled )
    {
    double equation[4];
    equation[0] = m_X;
    equation[1] = m_Y;
    equation[2] = m_Z;
    equation[3] = m_R;
    glClipPlane( GL_CLIP_PLANE0 + m_Id, equation );
    glEnable(    GL_CLIP_PLANE0 + m_Id );
    }
  else
    {
    glDisable(GL_CLIP_PLANE0 + m_Id );
    }

}




} // end namespace fltk


#endif
