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

#ifndef __otbVectorImageToASPixelAccessor_h
#define __otbVectorImageToASPixelAccessor_h

#include "itkVector.h"

namespace otb
{
namespace Accessor
{
/**
 * \class VectorImageToASPixelAccessor
 * \brief Give access to a Vector pixel type as if it were a RGBPixel type.
 *
 * This class is intended to be used as parameter of
 * an ImageAdaptor to make an Vector image appear as being
 * an image of RGB pixel type.
 *
 * \sa ImageAdaptor
 * \ingroup ImageAdaptors
 *
 */

template <class T>
class ITK_EXPORT VectorImageToASPixelAccessor
{
public:
  /** Standard class typedefs. */
  typedef   VectorImageToASPixelAccessor Self;

  /** External typedef. It defines the external aspect
    * that this class will exhibit */
  typedef  itk::FixedArray<T, 3> ExternalType;

  /** Internal typedef. It defines the internal real
   * representation of data */
  typedef  itk::Vector<T, 3> InternalType;

  /** Write access to the VectorToRGB component */
  inline void Set(InternalType& output, const ExternalType& input) const
  {
    output[0] = input[0];
    output[1] = input[1];
    output[2] = input[2];
  }

  /** Read access to the VectorToRGB component */
  inline ExternalType Get(const InternalType& input) const
  {
    ExternalType rgb(input.GetDataPointer());
    return rgb;
  }

private:
};

}  // end namespace Accessor
}  // end namespace otb

#endif
