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

/** Public methods */

void ImageModelRenderer::paintGL( const RenderingContext& context )
{
  // the VectorImageModel used for the rendering
  VectorImageModel * viModel = dynamic_cast<  VectorImageModel *>(
    const_cast<AbstractImageModel*>(context.m_AbstractImageModel)
    );

  // the region of the image to render
  const ImageRegionType&    region = context.m_ImageRegion;
  unsigned char* buffer = viModel->RasterizeRegion(region);
  
  // if buffer not null do the rendering
  if (buffer != NULL)
    {
    unsigned int nb_displayed_cols = context.m_ImageRegion.GetSize()[ 0 ];
    unsigned int nb_displayed_rows = context.m_ImageRegion.GetSize()[ 1 ];
    unsigned int first_displayed_col = context.m_ImageRegion.GetIndex()[ 0 ];
    unsigned int first_displayed_row = context.m_ImageRegion.GetIndex()[ 1 ];

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, context.m_ImageRegion.GetSize()[0]);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, first_displayed_col);
    glPixelStorei(GL_UNPACK_SKIP_ROWS,first_displayed_row);

    // TODO : need an accessor to isotropic zoom
    glPixelZoom(1.0/*m_IsotropicZoom,m_IsotropicZoom*/,1.0);

    glRasterPos2f(first_displayed_col, first_displayed_row);
    glDrawPixels(nb_displayed_cols,
                 nb_displayed_rows,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 buffer);
    glFlush();
    }
}


/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/

/*****************************************************************************/

} // end namespace 'mvd'
