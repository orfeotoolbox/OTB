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
class AbstractImageModel;

/** \class ImageModelRenderer
 *
 */
class ImageModelRenderer :
    public QObject
{
  Q_OBJECT;

//
// Public types.
public:
  struct RenderingContext
  {
    inline
    RenderingContext( const AbstractImageModel* model =NULL,
		      const ImageRegionType& region =ImageRegionType() ) :
      m_AbstractImageModel( model ),
      m_ImageRegion( region )
    {
    }

    const AbstractImageModel* m_AbstractImageModel;
    ImageRegionType m_ImageRegion;
  };

//
// Public methods.
public:

  //typedef mvd::Types::ImageRegionType           ImageRegionType;
  typedef itk::ImageRegion<2>   ImageRegionType;

  /** Constructor */
  ImageModelRenderer( QObject* parent = NULL );

  /** Destructor */
  virtual ~ImageModelRenderer();

  /** */
  void paintGL( const RenderingContext& context );


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
