/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2007-2012 Institut Mines Telecom / Telecom Bretagne
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

#ifndef otbKullbackLeiblerDistanceImageFilter_h
#define otbKullbackLeiblerDistanceImageFilter_h

#include "itkVariableLengthVector.h"
#include "otbBinaryFunctorNeighborhoodImageFilter.h"

namespace otb
{

/** \class CumulantsForEdgeworth
 * \brief Helper class for KullbackLeiblerDistanceImageFilter. Please refer to KullbackLeiblerDistanceImageFilter.
 *
 *
 * \ingroup OTBChangeDetection
 */
template <class TInput>
class CumulantsForEdgeworth
{
public:
  CumulantsForEdgeworth (const TInput& input);
  CumulantsForEdgeworth (const itk::Image<typename TInput::ImageType::PixelType, 1> * input);
  virtual ~CumulantsForEdgeworth () {}

  /** KL Divergence calculation */
  template <class TInput2>
  double Divergence(const CumulantsForEdgeworth<TInput2>& cumulants);

  inline double  GetMean()  const
  {
    return this->fMean;
  }
  inline double  GetVariance()  const
  {
    return this->fVariance;
  }
  inline double  GetSkewness()  const
  {
    return this->fSkewness;
  }
  inline double  GetKurtosis()  const
  {
    return this->fKurtosis;
  }
  inline bool IsDataAvailable() const
  {
    return this->fDataAvailable;
  }

protected:

  /** Moment estimation from initial neighborhood */
  void  MakeSumAndMoments(const TInput& input);
  /** Moment estimation from raw data */
  void  MakeSumAndMoments(const itk::Image<typename TInput::ImageType::PixelType, 1> * input);
  /** transformation moment -> cumulants (for Edgeworth) */
  void MakeCumulants();

  double fSum0, fSum1, fSum2, fSum3, fSum4;
  double fMu1, fMu2, fMu3, fMu4;
  double fMean, fVariance, fSkewness, fKurtosis;

  bool fDataAvailable;
};

namespace Functor
{
/** \class KullbackLeiblerDistance
 * \brief Functor for KullbackLeiblerDistanceImageFilter. Please refer to KullbackLeiblerDistanceImageFilter.
 *
 *
 * \ingroup OTBChangeDetection
 */
template <class TInput1, class TInput2, class TOutput>
class KullbackLeiblerDistance
{
public:
  KullbackLeiblerDistance () {}
  virtual ~KullbackLeiblerDistance () {}
  TOutput operator ()(const TInput1& it1, const TInput2& it2)
  {
    CumulantsForEdgeworth<TInput1> cum1 (it1);
    if (!cum1.IsDataAvailable()) return static_cast<TOutput>(0.);

    CumulantsForEdgeworth<TInput2> cum2 (it2);
    if (!cum2.IsDataAvailable()) return static_cast<TOutput>(0.);

    return static_cast<TOutput> (cum1.Divergence(cum2)
                                 + cum2.Divergence(cum1));
  }
};

} // Functor

/** \class KullbackLeiblerDistanceImageFilter
 * \brief Implements neighborhood-wise the computation of KullbackLeibler distance over Edgeworth approximation.
 *
 * This filter is parametrized over the types of the two
 * input images and the type of the output image.
 *
 * Numeric conversions (castings) are done by the C++ defaults.
 *
 * The filter will walk over all the pixels in the two input images, and for
 * each one of them it will do the following:
 *
 * - cast the input 1 pixel value to \c double
 * - cast the input 2 pixel value to \c double
 * - compute the first four cumulants of the two pixel values
 * - compute the value of the Edgeorth approximation of the KL distance
 * - cast the \c double value resulting to the pixel type of the output image
 * - store the casted value into the output image.
 *
 * The filter expect all images to have the same dimension
 * (e.g. all 2D, or all 3D, or all ND)
 *
 * See article of  Lin Saito et Levine
 * "Edgeworth Approximation of the Kullback-Leibler Distance Towards Problems in Image Analysis"
 * and
 * "Edgeworth Expansions of the Kullback-Leibler Information" (submitted to JASA, nov 25, 1999)
 * http://www.math.ucdavis.edu/~saito/publications
 *
 * \ingroup IntensityImageFilters Multithreaded
 *
 * \ingroup OTBChangeDetection
 */
template <class TInputImage1, class TInputImage2, class TOutputImage>
class ITK_EXPORT KullbackLeiblerDistanceImageFilter :
  public otb::BinaryFunctorNeighborhoodImageFilter<
      TInputImage1, TInputImage2, TOutputImage,
      Functor::KullbackLeiblerDistance<
          typename itk::ConstNeighborhoodIterator<TInputImage1>,
          typename itk::ConstNeighborhoodIterator<TInputImage2>,
          typename TOutputImage::PixelType> >
{
public:
  /** Standard class typedefs. */
  typedef KullbackLeiblerDistanceImageFilter Self;
  typedef otb::BinaryFunctorNeighborhoodImageFilter<
      TInputImage1, TInputImage2, TOutputImage,
      Functor::KullbackLeiblerDistance<
          typename itk::ConstNeighborhoodIterator<TInputImage1>,
          typename itk::ConstNeighborhoodIterator<TInputImage2>,
          typename TOutputImage::PixelType>
      >  Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

protected:
  KullbackLeiblerDistanceImageFilter() {}
  ~KullbackLeiblerDistanceImageFilter() override {}

private:
  KullbackLeiblerDistanceImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbKullbackLeiblerDistanceImageFilter.hxx"
#endif

#endif
