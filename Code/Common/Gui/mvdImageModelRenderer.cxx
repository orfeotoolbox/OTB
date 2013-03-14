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

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtOpenGL>

// On MSVC10, default GL.h does not define GL_BGRA
#if !defined(GL_BGRA) && defined(GL_BGRA_EXT)
#  define GL_BGRA GL_BGRA_EXT
#endif

//
// System includes (sorted by alphabetic order)
// necessary for the opengl variables and methods

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "Core/mvdVectorImageModel.h"
//
#include "Gui/mvdImageModelRenderer.h"

namespace mvd
{
/*
  TRANSLATOR mvd::ImageModelRenderer

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/

/*****************************************************************************/
ImageModelRenderer
::ImageModelRenderer( QObject* parent ) :
  AbstractModelRenderer( parent ),
  m_IsMoving(false)
{
  m_PreviousOriginX = 0.;
  m_PreviousOriginY = 0.;
  m_SquarePointUL.Fill(0.);
  m_SquarePointLR.Fill(0.);
}

/*****************************************************************************/
ImageModelRenderer
::~ImageModelRenderer()
{
}

/*****************************************************************************/
void ImageModelRenderer::paintGL( const RenderingContext& context )
{
  if( context.m_ImageModel==NULL )
    return;

  // the VectorImageModel used for the rendering
  core::VectorImageModel* viModel = qobject_cast< core::VectorImageModel * >(
    const_cast< core::AbstractImageModel* >( context.m_ImageModel )
    );
  assert( viModel );

  // the region of the image to be rendered
  const core::ImageRegionType&  region = context.m_ImageRegion;

  core::ImageRegionType  scaledRegion = region;
  core::CountType bestLod = viModel->ComputeBestLevelOfDetail(context.m_IsotropicZoom);

  // If it is not the original level of detail recompute the region to request
  double currentResolutionFactorX = 1.;
  double currentResolutionFactorY = 1.;

  if (bestLod != 0)
    {
    core::ImageRegionType::SizeType  scaledSize;
    core::ImageRegionType::IndexType scaledOrigin;

    scaledSize[0] = region.GetSize()[0]/(1<<bestLod);
    scaledSize[1] = region.GetSize()[1]/(1<<bestLod);

    currentResolutionFactorX = static_cast<double>(region.GetSize()[0])/static_cast<double>(scaledSize[0]);
    currentResolutionFactorY = static_cast<double>(region.GetSize()[1])/static_cast<double>(scaledSize[1]);

    scaledOrigin[0] = region.GetIndex()[0]/(1<<bestLod);
    scaledOrigin[1] = region.GetIndex()[1]/(1<<bestLod);

    scaledRegion.SetSize(scaledSize) ;
    scaledRegion.SetIndex(scaledOrigin);
    }

  if (!m_IsMoving)
    {
    // request the data for the current region
    m_Buffer = viModel->RasterizeRegion(scaledRegion, 
                                        context.m_IsotropicZoom, 
                                        context.m_ForceRefresh);
    }

  // final zoom factor : take into account the current resolution of
  // the file and the wheel zoom
  double finalZoomFactorX = context.m_IsotropicZoom * currentResolutionFactorX; 
  double finalZoomFactorY = context.m_IsotropicZoom * currentResolutionFactorY;

  // if buffer not null do the rendering
  if (m_Buffer != NULL)
    {
    //unsigned int first_displayed_col = 0;
    double originX = 0.;
    double originY = 0.;

    if (!m_IsMoving) // center
      {
      // originX
      if ( context.m_WidgetWidth  > 
           scaledRegion.GetSize()[0] * finalZoomFactorX  )
        {
        originX = (static_cast<double>(context.m_WidgetWidth) - 
                   static_cast<double>(scaledRegion.GetSize()[0]) * finalZoomFactorX)/2;
        }
      // originY
      if (context.m_WidgetHeight >
          scaledRegion.GetSize()[1] * finalZoomFactorY)
        {
        originY = (static_cast<double>(context.m_WidgetHeight) - 
                   static_cast<double>(scaledRegion.GetSize()[1]) * finalZoomFactorY)/2;
        }

      // when mouse is released, initialize the moving origin with the
      // values computed below. This is needed when the widget is
      // resized and the first displayed column is not at index 0
      // anymore. 
      m_MovingOriginX = originX;
      m_MovingOriginY = originY;
      }

    if (m_IsMoving)// if moving, only displace the rectangle (Gl_QUADS) origin
      {
      double dx = m_PreviousOriginX - static_cast<double>(scaledRegion.GetIndex()[0]) * finalZoomFactorX;
      double dy = m_PreviousOriginY - static_cast<double>(scaledRegion.GetIndex()[1]) * finalZoomFactorY;

      // incremenet the moving origin with the computed delta
      m_MovingOriginX+=dx;
      m_MovingOriginY-=dy;
      
      // set the quads origin to the moving origin
      originX =  m_MovingOriginX;
      originY =  m_MovingOriginY;
      }

    // real size of the region to be rendered
    double sizeX  = static_cast<double>(scaledRegion.GetSize()[0]) * finalZoomFactorX;
    double sizeY  = static_cast<double>(scaledRegion.GetSize()[1]) * finalZoomFactorY;

    // needed cause RGB not RGBA rendering. 
    //glPixelStorei(GL_UNPACK_ALIGNMENT,1); 
                                
    // Load texture 
    if(!m_IsMoving)
      {
      glDeleteTextures(1,&m_Texture);
      glGenTextures(1, &m_Texture);
      glBindTexture(GL_TEXTURE_2D, m_Texture);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
                   scaledRegion.GetSize()[0],
                   scaledRegion.GetSize()[1], 
                   0, GL_BGRA, GL_UNSIGNED_BYTE, m_Buffer);

      // With MSVC10, GL_TEXTURE_BASE_LEVEL && GL_TEXTURE_MAX_LEVEL
      // are not defined
#if defined(GL_TEXTURE_BASE_LEVEL) && defined(GL_TEXTURE_MAX_LEVEL)
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
#endif
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glBindTexture (GL_TEXTURE_2D, m_Texture);
      }
  
    // rectangle where to draw
    glEnable(GL_TEXTURE_2D);

    // Reset color before rendering
    glColor3d(1.0f,1.0f,1.0f);

    glBegin (GL_QUADS);
      glTexCoord2f (0.0, 1.0); glVertex2f(originX, originY + sizeY);
      glTexCoord2f (1.0, 1.0); glVertex2f(originX + sizeX, originY + sizeY);
      glTexCoord2f (1.0, 0.0); glVertex2f(originX + sizeX, originY);
      glTexCoord2f (0.0, 0.0); glVertex2f(originX, originY);
    glEnd ();

    // free texture
    glDisable(GL_TEXTURE_2D);

    // 
    // translate the corner of the red square in the ql widget
    // coordinates 
    core::PointType nullPoint;
    nullPoint.Fill(0.);
    if ( m_SquarePointUL != nullPoint || m_SquarePointLR != nullPoint )
      {
      //
      // physical point to index (in ql)
      core::PointType sp1, sp2;
      sp1[0] = (m_SquarePointUL[0] - viModel->GetOrigin()[0] ) * finalZoomFactorX / vcl_abs(viModel->GetSpacing()[0]) + originX;
      sp1[1] = (m_SquarePointUL[1] - viModel->GetOrigin()[1] ) * finalZoomFactorY / vcl_abs(viModel->GetSpacing()[1]) + originY;
      sp1[1] = context.m_WidgetHeight - sp1[1]; // y axis is flipped

      sp2[0] = (m_SquarePointLR[0] - viModel->GetOrigin()[0] ) * finalZoomFactorX / vcl_abs(viModel->GetSpacing()[0]) + originX; 
      sp2[1] = (m_SquarePointLR[1] - viModel->GetOrigin()[1] ) * finalZoomFactorY / vcl_abs(viModel->GetSpacing()[1]) + originY; 
      sp2[1] = context.m_WidgetHeight - sp2[1]; // y axis is flipped

      // draw the red square
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glColor3d(1.,0.,0.);

      glBegin(GL_LINE_LOOP);
       glVertex2d(sp1[0], sp2[1]);
       glVertex2d(sp2[0], sp2[1]);
       glVertex2d(sp2[0], sp1[1]);
       glVertex2d(sp1[0], sp1[1]);
      glEnd();

      glDisable(GL_BLEND);
      }

    // emit the new origin of the extent
    core::IndexType origin;
    origin[0]  = originX;
    origin[1]  = originY;
    emit ViewportOriginChanged(origin);
    }

  // save the previous scaled region origin
  m_PreviousOriginX = static_cast<double>(scaledRegion.GetIndex()[0]) * finalZoomFactorX;
  m_PreviousOriginY = static_cast<double>(scaledRegion.GetIndex()[1]) * finalZoomFactorY;
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/
void ImageModelRenderer
::OnViewportRegionRepresentationChanged(const core::PointType& ul, const core::PointType& lr)
{
  m_SquarePointUL = ul;
  m_SquarePointLR = lr;
}
/*****************************************************************************/

} // end namespace 'mvd'
