/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef otbVectorImageToASPixelAccessor_h
#define otbVectorImageToASPixelAccessor_h

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
 *
 * \ingroup OTBCommon
 */

template <class T>
class ITK_EXPORT VectorImageToASPixelAccessor
{
public:
  /** Standard class typedefs. */
  typedef VectorImageToASPixelAccessor Self;

  /** External typedef. It defines the external aspect
    * that this class will exhibit */
  typedef itk::FixedArray<T, 3> ExternalType;

  /** Internal typedef. It defines the internal real
   * representation of data */
  typedef itk::Vector<T, 3> InternalType;

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

} // end namespace Accessor
} // end namespace otb

#endif
