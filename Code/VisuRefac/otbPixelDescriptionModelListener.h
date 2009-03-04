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
#ifndef __otbPixelDescriptionModelListener_h
#define __otbPixelDescriptionModelListener_h

namespace otb
{
/**
 * \class PixelDescriptionModelListener
 *
 */

class PixelDescriptionModelListener
{
public:
  /** Standard class typedefs */
  typedef PixelDescriptionModelListener Self;

  // Update the display
  virtual void PixelDescriptionModelNotify() = 0;

protected:
  /** Constructor */
  PixelDescriptionModelListener() {};
  /** Destructor */
  virtual ~PixelDescriptionModelListener() {};

private:
  PixelDescriptionModelListener(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};

} // end namespace otb
#endif
