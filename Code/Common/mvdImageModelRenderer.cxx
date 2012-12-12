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

#include "mvdImageModelRenderer.h"

//
// System includes (sorted by alphabetic order)

//
// Qt includes (sorted by alphabetic order)
#include <QGLWidget>

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)

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

// TEMP : to do use an AbstractImageModel (or a ImageModel to get the
// buffer to draw)
void ImageModelRenderer::paintGL(unsigned char* buffer, const ImageRegionType& region)
{
  if (buffer != NULL)
    {
    unsigned int nb_displayed_cols = region.GetSize()[ 0 ];
    unsigned int nb_displayed_rows = region.GetSize()[ 1 ];
    unsigned int first_displayed_col = region.GetIndex()[ 0 ];
    unsigned int first_displayed_row = region.GetIndex()[ 1 ];

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, region.GetSize()[0]);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, first_displayed_col);
    glPixelStorei(GL_UNPACK_SKIP_ROWS,first_displayed_row);

    glClear(GL_COLOR_BUFFER_BIT);

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
