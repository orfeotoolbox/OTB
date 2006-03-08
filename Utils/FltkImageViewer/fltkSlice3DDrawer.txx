/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkSlice3DDrawer.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/05 21:52:21 $
  Version:   $Revision: 1.21 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef _itkSlice3DDrawer_txx
#define _itkSlice3DDrawer_txx

#include "fltkSlice3DDrawer.h"
#include "itkImageRegionIteratorWithIndex.h"
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif
#include "fltkCommandEvents.h"


namespace fltk {


/**
 * Default constructor
 */
template <class TImage>
Slice3DDrawer<TImage>
::Slice3DDrawer()
{

  m_TextureX = 0;
  m_TextureY = 0;
  m_TextureZ = 0;

  m_SliceX = 0;
  m_SliceY = 0;
  m_SliceZ = 0;

  m_TexturesGenerated = false;
  
  m_TextureBindedX = false;
  m_TextureBindedY = false;
  m_TextureBindedZ = false;
  
  m_DrawCommand = DrawCommandType::New();
  m_DrawCommand->SetCallbackFunction( this, &Self::glDraw );

}


/**
 * Destructor
 */
template <class TImage>
Slice3DDrawer<TImage>
::~Slice3DDrawer()
{


  if( m_TextureX ) 
  {
    delete [] m_TextureX;
    m_TextureX = 0;
  }

  if( m_TextureY ) 
  {
    delete [] m_TextureY;
    m_TextureY = 0;
  }

  if( m_TextureZ ) 
  {
    delete [] m_TextureZ;
    m_TextureZ = 0;
  }

  if( m_SliceX ) 
  {
    delete [] m_SliceX;
    m_SliceX = 0;
  }

  if( m_SliceY ) 
  {
    delete [] m_SliceY;
    m_SliceY = 0;
  }

  if( m_SliceZ ) 
  {
    delete [] m_SliceZ;
    m_SliceZ = 0;
  }




}



/**
 * SetInput
 */
template <class TImage>
void
Slice3DDrawer<TImage>
::SetInput( ImageType * image )
{

  m_Image = image;

  RefreshGUI();

}



/**
 * Get Draw Command
 */
template <class TImage>
typename Slice3DDrawer<TImage>::DrawCommandPointer
Slice3DDrawer<TImage>
::GetDrawCommand(void) 
{
  return m_DrawCommand.GetPointer();
}




/**
 * Refresh GUI
 */
template <class TImage>
void
Slice3DDrawer<TImage>
::RefreshGUI( void )
{

  if( ! m_Image ) 
  {
    VolumeWindow->deactivate();
    return;
  }

  RegionType region = m_Image->GetRequestedRegion();
  if( region.GetNumberOfPixels() == 0 )
  {
    return;
  }

  SizeType size = region.GetSize();

  xScrollBar->value( size[0]/2, 1, 0, size[0] );
  yScrollBar->value( size[1]/2, 1, 0, size[1] );
  zScrollBar->value( size[2]/2, 1, 0, size[2] );

  xScrollBar->linesize( 1 );
  yScrollBar->linesize( 1 );
  zScrollBar->linesize( 1 );

  typedef  itk::ImageRegionIteratorWithIndex< ImageType > IteratorType;

  IteratorType it( m_Image, region );

  PixelType  max = it.Get();
  while( ! it.IsAtEnd() )
    {
    const PixelType val = it.Get();
    if( max < val )
      {
      max = val;
      }
    ++it;
    }
  m_Max_Value = max;


  m_Nx = size[0];
  m_Ny = size[1];
  m_Nz = size[2];

  m_Nxr = (int)pow((float)2.0, (float)floor( log(static_cast<float>(m_Nx))/log(2.0f)+1 ) );
  m_Nyr = (int)pow((float)2.0, (float)floor( log(static_cast<float>(m_Ny))/log(2.0f)+1 ) );
  m_Nzr = (int)pow((float)2.0, (float)floor( log(static_cast<float>(m_Nz))/log(2.0f)+1 ) );

  m_Dx = m_Image->GetSpacing()[0];
  m_Dy = m_Image->GetSpacing()[1];
  m_Dz = m_Image->GetSpacing()[2];

  RegionType totalRegion = m_Image->GetLargestPossibleRegion();

  m_Cx = totalRegion.GetSize()[0] * m_Dx / 2.0;
  m_Cy = totalRegion.GetSize()[1] * m_Dy / 2.0;
  m_Cz = totalRegion.GetSize()[2] * m_Dz / 2.0;

  m_Ox = m_Image->GetOrigin()[0];
  m_Oy = m_Image->GetOrigin()[1];
  m_Oz = m_Image->GetOrigin()[2];


  if( m_SliceX ) 
  {
    delete [] m_SliceX;
  }

  if( m_SliceY ) 
  {
    delete [] m_SliceY;
  }

  if( m_SliceZ ) 
  {
    delete [] m_SliceZ;
  }

  m_SliceX = new unsigned char [ 4 * m_Ny * m_Nz ];
  m_SliceY = new unsigned char [ 4 * m_Nz * m_Nx ];
  m_SliceZ = new unsigned char [ 4 * m_Nx * m_Ny ];

  if( m_TextureX ) 
  {
    delete [] m_TextureX;
  }

  if( m_TextureY ) 
  {
    delete [] m_TextureY;
  }

  if( m_TextureZ ) 
  {
    delete [] m_TextureZ;
  }

  m_TextureX = new unsigned char [ 4 * m_Nyr * m_Nzr ];
  m_TextureY = new unsigned char [ 4 * m_Nzr * m_Nxr ];
  m_TextureZ = new unsigned char [ 4 * m_Nxr * m_Nyr ];

  VolumeWindow->activate();
}





/**
 * Show
 */
template <class ImagePixelType>
void
Slice3DDrawer<ImagePixelType>
::Show(void)
{

  VolumeWindow->show();

}






/**
 * Hide
 */
template <class ImagePixelType>
void
Slice3DDrawer<ImagePixelType>
::Hide(void)
{

  VolumeWindow->hide();

}








/**
* Select slice X to display
*/
template <class ImagePixelType>
void 
Slice3DDrawer<ImagePixelType>
::SelectSliceX(void)
{
    xValueOutput->value( xScrollBar->value() );
    m_TextureBindedX = false;
    this->InvokeEvent( fltk::VolumeReslicedEvent() );
}






/**
* Select slice Y to display
*/
template <class ImagePixelType>
void 
Slice3DDrawer<ImagePixelType>
::SelectSliceY(void)
{
    yValueOutput->value( yScrollBar->value() );
    m_TextureBindedY = false;
    this->InvokeEvent( fltk::VolumeReslicedEvent() );
}





/**
* Select slice Z to display
*/
template <class ImagePixelType>
void 
Slice3DDrawer<ImagePixelType>
::SelectSliceZ(void)
{
    zValueOutput->value( zScrollBar->value() );
    m_TextureBindedZ = false;
    this->InvokeEvent( fltk::VolumeReslicedEvent() );
}






/**
 * Draw the target as textures in OpenGL
 */
template <class ImagePixelType>
void 
Slice3DDrawer<ImagePixelType>
::glDraw(void) const
{

  
  if( !(m_Image) )
  {
    return;
  }

  if( !m_TexturesGenerated )
    {
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    glGenTextures(3, m_TextureName);
    m_TexturesGenerated = true;
    }

  if( !m_TextureBindedX )
    {
    this->BindTextureX();
    m_TextureBindedX = true;
    }
  
  if( !m_TextureBindedY )
    {
    this->BindTextureY();
    m_TextureBindedY = true;
    }

  if( !m_TextureBindedZ )
    {
    this->BindTextureZ();
    m_TextureBindedZ = true;
    }



  RegionType region = m_Image->GetRequestedRegion();
  if( region.GetNumberOfPixels() == 0 )
  {
    return;
  }

  glDisable( GL_LIGHTING );

  glPushMatrix();

  glTranslatef( -m_Ox, -m_Oy, -m_Oy );

  const GLfloat qx = -m_Cx; 
  const GLfloat qy = -m_Cy; 
  const GLfloat qz = -m_Cz; 
    

  glColor3f(0.7,0.5,0.5);

  glBegin(GL_LINE_LOOP);
  glVertex3f(-qx, -qy, -qz);
  glVertex3f(-qx,  qy, -qz);
  glVertex3f( qx,  qy, -qz);
  glVertex3f( qx, -qy, -qz);
  glEnd();

  glBegin(GL_LINE_LOOP);
  glVertex3f(-qx, -qy,  qz);
  glVertex3f(-qx,  qy,  qz);
  glVertex3f( qx,  qy,  qz);
  glVertex3f( qx, -qy,  qz);
  glEnd();

  glBegin(GL_LINES);
  glVertex3f(-qx, -qy, -qz);
  glVertex3f(-qx, -qy,  qz);
  glVertex3f(-qx,  qy, -qz);
  glVertex3f(-qx,  qy,  qz);
  glVertex3f( qx,  qy, -qz);
  glVertex3f( qx,  qy,  qz);
  glVertex3f( qx, -qy, -qz);
  glVertex3f( qx, -qy,  qz);
  glEnd();

  glTranslatef( qx, qy, qz );

  glGetError();        // Clearing Error buffer
  glColor3f(1.0,1.0,1.0);

  if( xCheckButton->value() && m_TextureBindedX ) 
  {
    int px = xScrollBar->value();
    GLfloat pnx =   px    * m_Dx;
    GLfloat pny =   m_Ny  * m_Dy;
    GLfloat pnz =   m_Nz  * m_Dz;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_TextureName[0]);
    glBegin(GL_QUADS);
      glNormal3f( 1.0,0.0,0.0); glTexCoord2f( 0.0, 0.0 ); glVertex3f( pnx, 0.0, 0.0);
      glNormal3f( 1.0,0.0,0.0); glTexCoord2f( 0.0, 1.0 ); glVertex3f( pnx, 0.0, pnz);
      glNormal3f( 1.0,0.0,0.0); glTexCoord2f( 1.0, 1.0 ); glVertex3f( pnx, pny, pnz);
      glNormal3f( 1.0,0.0,0.0); glTexCoord2f( 1.0, 0.0 ); glVertex3f( pnx, pny, 0.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
  }



  if( yCheckButton->value() && m_TextureBindedY ) 
  {
    int py = yScrollBar->value();
    GLfloat pnx =   m_Nx   * m_Dx;
    GLfloat pny =   py     * m_Dy;
    GLfloat pnz =   m_Nz   * m_Dz;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_TextureName[1]);
    glBegin(GL_QUADS);
      glNormal3f( 0.0,1.0,0.0); glTexCoord2f( 0.0, 0.0 ); glVertex3f( 0.0, pny, 0.0);
      glNormal3f( 0.0,1.0,0.0); glTexCoord2f( 0.0, 1.0 ); glVertex3f( 0.0, pny, pnz);
      glNormal3f( 0.0,1.0,0.0); glTexCoord2f( 1.0, 1.0 ); glVertex3f( pnx, pny, pnz);
      glNormal3f( 0.0,1.0,0.0); glTexCoord2f( 1.0, 0.0 ); glVertex3f( pnx, pny, 0.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
  }



  if( zCheckButton->value() && m_TextureBindedZ ) 
  {
    int pz = zScrollBar->value();
    GLfloat pnx =   m_Nx   * m_Dx;
    GLfloat pny =   m_Ny   * m_Dy;
    GLfloat pnz =   pz     * m_Dz;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_TextureName[2]);
    glBegin(GL_QUADS);
      glNormal3f( 0.0,0.0,1.0 ); glTexCoord2f( 0.0, 0.0 ); glVertex3f( 0.0, 0.0, pnz);
      glNormal3f( 0.0,0.0,1.0 ); glTexCoord2f( 0.0, 1.0 ); glVertex3f( 0.0, pny, pnz);
      glNormal3f( 0.0,0.0,1.0 ); glTexCoord2f( 1.0, 1.0 ); glVertex3f( pnx, pny, pnz);
      glNormal3f( 0.0,0.0,1.0 ); glTexCoord2f( 1.0, 0.0 ); glVertex3f( pnx, 0.0, pnz);
    glEnd();
    glDisable(GL_TEXTURE_2D);
  }

  glPopMatrix();

  glEnable( GL_LIGHTING );

}





//----------------------------------------------------
//
//    Prepare Textures
//
//----------------------------------------------------
template <class ImagePixelType>
void 
Slice3DDrawer<ImagePixelType>
::BindTextureX(void) const
{

  const GLint level  = 0;  // Level-of-Detail (sub-sampling...)
  const GLint border = 0;  // Texture image border

  if( xCheckButton->value() )
  {

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);  
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    float factor = 255.0 / m_Max_Value;

    IndexType index;
    const int px = xScrollBar->value();
    int pix = 0;
    index[0] = px;
    for(int z=0; z<m_Nz; z++) {
      index[2] = z;
      for(int y=0; y<m_Ny; y++) {
        index[1] = y;
        GLubyte val = (GLubyte)(
          m_Image->GetPixel(index) * factor );
        m_SliceX[pix++] = val;
        m_SliceX[pix++] = val;
        m_SliceX[pix++] = val;
        m_SliceX[pix++] = 255;
        }
      }

    glBindTexture(GL_TEXTURE_2D, m_TextureName[0]);

    gluScaleImage( GL_RGBA, m_Ny, m_Nz,
                   GL_UNSIGNED_BYTE, m_SliceX,
                   m_Nyr, m_Nzr,
                   GL_UNSIGNED_BYTE, m_TextureX);

    glTexImage2D(GL_TEXTURE_2D,level,GL_RGBA,
                 m_Nyr,m_Nzr,
                 border,GL_RGBA,GL_UNSIGNED_BYTE,m_TextureX);

    }

}



//----------------------------------------------------
//
//    Prepare Textures
//
//----------------------------------------------------
template <class ImagePixelType>
void 
Slice3DDrawer<ImagePixelType>
::BindTextureY(void) const 
{

  const GLint level  = 0;  // Level-of-Detail (sub-sampling...)
  const GLint border = 0;  // Texture image border

  if( yCheckButton->value() )
  {

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);  
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    float factor = 255.0 / m_Max_Value;

    IndexType index;
    const int py = yScrollBar->value();
    int piy = 0;
    index[1] = py;
    for(int z=0; z<m_Nz; z++) {
      index[2] = z;
      for(int x=0; x<m_Nx; x++) {
        index[0] = x;
        GLubyte val = (GLubyte)(
          m_Image->GetPixel(index) * factor );
        m_SliceY[piy++] = val;
        m_SliceY[piy++] = val;
        m_SliceY[piy++] = val;
        m_SliceY[piy++] = 255;
        }
      }

    glBindTexture(GL_TEXTURE_2D, m_TextureName[1]);

    gluScaleImage( GL_RGBA, m_Nx, m_Nz,
                   GL_UNSIGNED_BYTE, m_SliceY,
                   m_Nxr, m_Nzr,
                   GL_UNSIGNED_BYTE, m_TextureY);

    glTexImage2D(GL_TEXTURE_2D,level,GL_RGBA,
                 m_Nxr,m_Nzr,
                 border,GL_RGBA,GL_UNSIGNED_BYTE,m_TextureY);

    }



}


//----------------------------------------------------
//
//    Prepare Textures
//
//----------------------------------------------------
template <class ImagePixelType>
void 
Slice3DDrawer<ImagePixelType>
::BindTextureZ(void) const
{

  const GLint level  = 0;  // Level-of-Detail (sub-sampling...)
  const GLint border = 0;  // Texture image border

  if( zCheckButton->value() )
  {

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);  
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    float factor = 255.0 / m_Max_Value;

    IndexType index;
    const int pz = zScrollBar->value();
    int piz = 0;
    index[2] = pz;
    for(int y=0; y<m_Ny; y++) {
      index[1] = y;
      for(int x=0; x<m_Nx; x++) {
        index[0] = x;
        GLubyte val = (GLubyte)(
          m_Image->GetPixel(index) * factor );
        m_SliceZ[piz++] = val;
        m_SliceZ[piz++] = val;
        m_SliceZ[piz++] = val;
        m_SliceZ[piz++] = 255;
        }
      }

    glBindTexture(GL_TEXTURE_2D, m_TextureName[2]);

    gluScaleImage( GL_RGBA, m_Nx, m_Ny,
                   GL_UNSIGNED_BYTE, m_SliceZ,
                   m_Nxr, m_Nyr,
                   GL_UNSIGNED_BYTE, m_TextureZ);

    glTexImage2D(GL_TEXTURE_2D,level,GL_RGBA,
                 m_Nxr,m_Nyr,
                 border,GL_RGBA,GL_UNSIGNED_BYTE,m_TextureZ);

    }


}



} // end namespace fltk


#endif
