/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbISRAUnmixingImageFilter_h
#define otbISRAUnmixingImageFilter_h

#include "itkNumericTraits.h"
#include "otbFunctorImageFilter.h"
#include "vnl/algo/vnl_svd.h"
#include <boost/shared_ptr.hpp>

namespace otb
{

namespace Functor
{

/** \class ISRAUnmixingFunctor
 *
 * \brief Perform fully constrained least squares on a pixel
 *
 * \sa ISRAUnmixingImageFilter
 *
 * \ingroup OTBUnmixing
 */
template <class TInput, class TOutput, class TPrecision>
class ISRAUnmixingFunctor
{
public:
  typedef ISRAUnmixingFunctor<TInput, TOutput, TPrecision> Self;

  typedef TInput     InputType;
  typedef TOutput    OutputType;
  typedef TPrecision PrecisionType;

  typedef vnl_vector<PrecisionType> VectorType;
  typedef vnl_matrix<PrecisionType> MatrixType;

  ISRAUnmixingFunctor();
  virtual ~ISRAUnmixingFunctor() = default;

  size_t OutputSize(const std::array<size_t, 1>& nbBands) const;

  void SetEndmembersMatrix(const MatrixType& U);
  const MatrixType& GetEndmembersMatrix(void) const;

  void SetMaxIteration(unsigned int val)
  {
    m_MaxIteration = val;
  }

  unsigned int GetMaxIteration() const
  {
    return m_MaxIteration;
  }

  OutputType operator()(const InputType& in) const;

private:
  static bool IsNonNegative(PrecisionType val)
  {
    return val >= 0;
  }

  typedef vnl_svd<PrecisionType>     SVDType;
  typedef boost::shared_ptr<SVDType> SVDPointerType;

  MatrixType     m_U;
  SVDPointerType m_Svd; // SVD of U
  unsigned int   m_OutputSize;
  unsigned int   m_MaxIteration;
};
}

/** \typedef ISRAUnmixingImageFilter
 *
 * \brief Performs fully constrained least squares on each pixel of a VectorImage
 *
 * This filter takes as input a multiband image and a matrix.
 * If the matrix is called \f$A\f$, it solves, for each pixel \f$p\f$, the system
 * \f$A \cdot x = p\f$ in the least square sense, with additional constraints on the solution
 * \f$\hat{x}\f$ ensuring positivity (each component is positive) and additivity (the sum of
 * all components is 1).
 *
 * The main use of this filter is to unmix an hyperspectral dataset,
 * where \f$A\f$ is the mixing matrix, in which each row corresponds to an endmember signature.
 *
 * The number of rows in \f$A\f$ must match the input image number of bands.
 * The number of bands in the output image will be the number of columns of $A$
 *
 * References
 *   "Fully Constrained Least-Squares Based Linear Unmixing." Daniel Heinz,
 *   Chein-I Chang, and Mark L.G. Althouse. IEEE. 1999.
 *
 * \ingroup Hyperspectral
 * \ingroup Streamed
 * \ingroup Threaded
 *
 * \ingroup OTBUnmixing
 */
template <typename TInputImage, typename TOutputImage, typename TPrecision>
using ISRAUnmixingImageFilter = FunctorImageFilter<Functor::ISRAUnmixingFunctor<typename TInputImage::PixelType, typename TOutputImage::PixelType, TPrecision>>;

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbISRAUnmixingImageFilter.hxx"
#endif

#endif
