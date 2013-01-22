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

//
// System includes (sorted by alphabetic order)
// necessary for the opengl variables and methods

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdImageModelRenderer.h"
#include "mvdVectorImageModel.h"

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
  QObject( parent )
{
}

/*****************************************************************************/
ImageModelRenderer
::~ImageModelRenderer()
{
}

/*****************************************************************************/
void ImageModelRenderer::paintGL( const RenderingContext& context )
{
  // the VectorImageModel used for the rendering
  VectorImageModel * viModel = qobject_cast< VectorImageModel * >(
    const_cast<AbstractImageModel*>(context.m_AbstractImageModel)
  );

  // the region of the image to be rendered
  const ImageRegionType&  region = context.m_ImageRegion;

  // If the image is a j2k image
  int lod = 0;
  ImageRegionType  scaledRegion = region;
  if ( viModel->GetBestLevelOfDetail(context.m_IsotropicZoom, lod) )
    {
    // if the level of detail is an overview
    // recompute the region to request
    if (lod != 0)
      {
      ImageRegionType::SizeType  scaledSize;
      ImageRegionType::IndexType scaledOrigin;
      
      scaledSize[0] = region.GetSize()[0]/(1<<lod);
      scaledSize[1] = region.GetSize()[1]/(1<<lod);
      
      scaledOrigin[0] = region.GetIndex()[0]/(1<<lod);
      scaledOrigin[1] = region.GetIndex()[1]/(1<<lod);
      
      scaledRegion.SetSize(scaledSize) ;
      scaledRegion.SetIndex(scaledOrigin);
      }
    }
  
  // request the data for the current region
  m_Buffer = viModel->RasterizeRegion(scaledRegion, context.m_IsotropicZoom);

  // Current resolution
  double currentResolutionFactor = 1 << lod;
  
  // if buffer not null do the rendering
  if (m_Buffer != NULL)
    {
      unsigned int nb_displayed_cols = scaledRegion.GetSize()[ 0 ] ;
      unsigned int nb_displayed_rows = scaledRegion.GetSize()[ 1 ] ;
      
      unsigned int first_displayed_col = 0;
      if ( context.m_WidgetWidth  > 
           scaledRegion.GetSize()[0] * context.m_IsotropicZoom*currentResolutionFactor  )
        {
        first_displayed_col = (context.m_WidgetWidth  
                               - scaledRegion.GetSize()[0] * context.m_IsotropicZoom*currentResolutionFactor) /2;
        }

      unsigned int first_displayed_row = 0;
      if (context.m_WidgetHeight >
          scaledRegion.GetSize()[1] * context.m_IsotropicZoom*currentResolutionFactor)
        {
        first_displayed_row = (context.m_WidgetHeight 
                               - scaledRegion.GetSize()[1] * context.m_IsotropicZoom*currentResolutionFactor)/2;
        }

      // std::cout <<"\tImageModeRenderer : contex.Zoom  "<<   context.m_IsotropicZoom 
      //           << " currentResolutFactor "<< currentResolutionFactor << std::endl;
      // std::cout <<"\tImageModeRenderer : finalZoom to apply:  "<<   
      //   context.m_IsotropicZoom*currentResolutionFactor << std::endl;

      // Render the buffer
      glPixelZoom(context.m_IsotropicZoom*currentResolutionFactor, 
                  context.m_IsotropicZoom*currentResolutionFactor);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      glRasterPos2f(first_displayed_col, first_displayed_row);
      glDrawPixels(nb_displayed_cols,
                   nb_displayed_rows,
                   GL_RGB,
                   GL_UNSIGNED_BYTE,
                   m_Buffer);


      // glEnable(GL_TEXTURE_2D);
      // //glColor4f(1.0, 1.0, 1.0, 0.0);
      // GLuint texture;
      // glGenTextures(1, &texture);
      // glBindTexture(GL_TEXTURE_2D, texture);
      // glTexImage2D(GL_TEXTURE_2D, 0, 3, nb_displayed_cols,
      //              nb_displayed_rows, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);
      // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      // glBindTexture (GL_TEXTURE_2D, texture);
      // glBegin (GL_QUADS);
      // glTexCoord2f (0.0, 1.0);
      // glVertex3f (first_displayed_col, first_displayed_row, 0.0);
      // glTexCoord2f (1.0, 1.0);
      // glVertex3f (first_displayed_col + scaledRegion.GetSize()[0], first_displayed_row, 0.0);
      // glTexCoord2f (1.0, 0.0);
      // glVertex3f (first_displayed_col + scaledRegion.GetSize()[0], first_displayed_row + scaledRegion.GetSize()[1], 0.0);
      // glTexCoord2f (0.0, 0.0);
      // glVertex3f (first_displayed_col, first_displayed_row + scaledRegion.GetSize()[1], 0.0);
      // glEnd ();
      // glDeleteTextures(1, &texture);
      // glDisable(GL_TEXTURE_2D);

      //glFlush();
    }
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/

/*****************************************************************************/

} // end namespace 'mvd'
