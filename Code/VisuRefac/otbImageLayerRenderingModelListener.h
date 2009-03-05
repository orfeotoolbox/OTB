/*=========================================================================

Program:   ORFEO Toolbox
Language:  C++
Date:      $Date$
Version:   $Revision$


Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
See OTBCopyright.txt for details.


This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbImageLayerRenderingModelListener_h
#define __otbImageLayerRenderingModelListener_h

namespace otb
{
/**
 * \class ImageLayerRenderingModelListener
 *
 *  Vectorization view interface specification for the model
 *
 */

class ImageLayerRenderingModelListener
{
public:
  /** Standard class typedefs */
  typedef ImageLayerRenderingModelListener Self;

  // Update the display
  virtual void ImageLayerRenderingModelNotify() = 0;

protected:
  /** Constructor */
  ImageLayerRenderingModelListener() {};
  /** Destructor */
  virtual ~ImageLayerRenderingModelListener() {};

private:
  ImageLayerRenderingModelListener(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};

} // end namespace otb
#endif
