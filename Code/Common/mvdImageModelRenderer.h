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

#ifndef __mvdImageModelRenderer_h
#define __mvdImageModelRenderer_h

//
// System includes (sorted by alphabetic order)

//
// Qt includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)
#include "itkImageRegion.h"

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdTypes.h"

//
// External classes pre-declaration.
namespace
{
}

namespace mvd
{
//
// Internal classes pre-declaration.

/** \class ImageModelRenderer
 *
 */
class ImageModelRenderer :
    public QObject
{
  Q_OBJECT;

//
// Public methods.
public:

  typedef itk::ImageRegion<2>           ImageRegionType;

  /** Constructor */
  ImageModelRenderer( QObject* parent = NULL );

  /** Destructor */
  virtual ~ImageModelRenderer();

  /** */
  void paintGL(){}

public slots:
  void updateGL(const ImageRegionType& region){}

//
// SIGNALS.
signals:

//
// Protected methods.
protected:

//
// Protected attributes.
protected:

//
// Private methods.
private:


//
// Private attributes.
private:

//
// SLOTS.
private slots:
};

} // end namespace 'mvd'

#endif // __mvdImageModelRenderer_h
