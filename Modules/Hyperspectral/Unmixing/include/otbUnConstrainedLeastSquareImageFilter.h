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

#ifndef otbUnConstrainedLeastSquareImageFilter_h
#define otbUnConstrainedLeastSquareImageFilter_h

#include "itkMacro.h"
#include "otbFunctorImageFilter.h"
#include "vnl/algo/vnl_svd.h"
#include <memory>

namespace otb
{

namespace Functor
{

/** \class UnConstrainedLeastSquareFunctor
 *
 * \brief Solves a least square system on a pixel
 *
 * \sa UnConstrainedLeastSquareImageFilter
 *
 * \ingroup OTBUnmixing
 */
template <class TInput, class TOutput, class TPrecision>
class UnConstrainedLeastSquareFunctor
{
public:
  typedef TInput     InputType;
  typedef TOutput    OutputType;
  typedef TPrecision PrecisionType;

  typedef vnl_vector<PrecisionType> VectorType;
  typedef vnl_matrix<PrecisionType> MatrixType;

  UnConstrainedLeastSquareFunctor() : m_OutputSize(0){};
  virtual ~UnConstrainedLeastSquareFunctor() = default;

  size_t OutputSize(const std::array<size_t, 1>& nbBands) const;

  void SetMatrix(const MatrixType& m);

  OutputType operator()(const InputType& in) const;

private:
  typedef vnl_svd<PrecisionType>     SVDType;
  typedef std::shared_ptr<SVDType> SVDPointerType;

  unsigned int   m_OutputSize;
  SVDPointerType m_Svd;
  MatrixType     m_Inv;
};
}

/** \typedef UnConstrainedLeastSquareImageFilter
 *
 * \brief Solves a least square system for each pixel
 *
 * This filter takes as input a multiband image and a matrix.
 * If the matrix is called $A$, it solves, for each pixel $p$, the system
 * \f$A \cdot x = p\f$ in the least square sense, and saves the result \f$\hat{x}\f$
 * in the output pixel.
 *
 * It can be used as a simple way to unmix an hyperspectral dataset,
 * where \f$A\f$ is the matrix in which each row corresponds to an endmember signature,
 * although better algorithms can be found for this particular task.
 *
 * The number of rows in \f$A\f$ must match the input image number of bands.
 * The number of bands in the output image will be the number of columns of \f$A\f$
 *
 * \ingroup Streamed
 * \ingroup Threaded
 *
 * \ingroup OTBUnmixing
 */
template <typename TInputImage, typename TOutputImage, typename TPrecision>
using UnConstrainedLeastSquareImageFilter =
    FunctorImageFilter<Functor::UnConstrainedLeastSquareFunctor<typename TInputImage::PixelType, typename TOutputImage::PixelType, TPrecision>>;

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbUnConstrainedLeastSquareImageFilter.hxx"
#endif

#endif
