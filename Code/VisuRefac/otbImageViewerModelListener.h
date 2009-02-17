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
#ifndef __otbImageViewerModelListener_h
#define __otbImageViewerModelListener_h

namespace otb
{
/**
 * \class ImageViewerModelListener
 *
 *  Vectorization view interface specification for the model
 *
 */

class ImageViewerModelListener
{
public:
  /** Standard class typedefs */
  typedef ImageViewerModelListener Self;

  // Update the display
  virtual void ImageViewerNotify() = 0;

protected:
  /** Constructor */
  ImageViewerModelListener() {};
  /** Destructor */
  virtual ~ImageViewerModelListener() {};

private:
  ImageViewerModelListener(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};

} // end namespace otb
#endif
