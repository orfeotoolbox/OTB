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

#ifndef otbSOMbasedImageFilter_h
#define otbSOMbasedImageFilter_h

#include "itkMacro.h"
#include "itkUnaryFunctorImageFilter.h"

namespace otb {

namespace Functor {

/** \class SOMbasedImageFilterFunctor
 * \brief This class implements the functor that perform substitution between
 *  input image pixels and the corresponding winning neurons of a Kohonen map.
 *
 *  On the contrary to SOMClassifier, it does not yield labels but the
 *  value of the winning neurons. Also, it is threaded...
 *
 * \ingroup OTBSOM
 */
template <class TInput, class TOutput, class TDistanceMetric, class TMap>
class ITK_EXPORT SOMbasedImageFilterFunctor
{
public:
  /** Standard typedefs */
  typedef SOMbasedImageFilterFunctor  Self;
  typedef TMap                        MapType;
  typedef typename MapType::Pointer   MapPointerType;
  typedef typename MapType::IndexType IndexType;
  typedef typename MapType::PixelType NeuronType;

  typedef TDistanceMetric                DistanceType;
  typedef typename DistanceType::Pointer DistancePointerType;

  /** Standard Constructor */
  SOMbasedImageFilterFunctor () {}
  virtual ~SOMbasedImageFilterFunctor () {}

  /** Functor */
  TOutput operator ()(const TInput& input);

  /** Get/Set the Map */
  MapType * GetMap() const { return this->m_Map; }
  void SetMap(const MapType * theMap);

  /** Get the DistanceMetric */
  DistanceType *  GetDistance() { return this->m_Distance; }

protected:
  /** GetWinner redefinition by using its own TDistanceMetric that
   * could be different from the one of the SOM map */
  IndexType GetWinner(const NeuronType& sample);

  MapPointerType      m_Map;
  DistancePointerType m_Distance;

private:
  SOMbasedImageFilterFunctor (const Self &);     // not implemented
  void operator =(const Self&);      // not implemented
};

} // end of namespace Functor

/** \class SOMbasedImageFilter
 * \brief This class perform the substitution between initial input image
 * pixels and the winning neurons of a SOM.
 *
 * It performs the projection of the SOM onto the data. On the contrary to
 * otb::SOMClassifier, it does not yield labels but an image which
 * corresponds to the quantized version of the initial image when considering
 * SOMmap as a codebook of a vector quantization.
 *
 * SOMbasedImageFilter is templated over a TDistanceMetric since it could be
 * different from the initial DistanceMetric used for the SOM training. Moreover
 * TMap is a template for the SOM that can be a simple VectorImage.
 *
 * \ingroup Streamed
 * \sa EuclideanDistanceMetricWithMissingValue
 *
 * \ingroup OTBSOM
 */
template <class TInputImage, class TOutputImage,
    class TDistanceMetric, class TMap>
class ITK_EXPORT SOMbasedImageFilter
  : public itk::UnaryFunctorImageFilter<TInputImage, TOutputImage,
      Functor::SOMbasedImageFilterFunctor<typename TInputImage::PixelType,
          typename TOutputImage::PixelType,
          TDistanceMetric, TMap> >
{
public:
  /** Standard class typedefs. */
  typedef SOMbasedImageFilter Self;
  typedef typename itk::UnaryFunctorImageFilter<TInputImage, TOutputImage,
      Functor::SOMbasedImageFilterFunctor<
          typename TInputImage::PixelType,
          typename TOutputImage::PixelType,
          TDistanceMetric, TMap> >  Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SOMbasedImageFilter, UnaryFunctorImageFilter);

  /** Some convenient typedefs. */
  typedef TDistanceMetric                      DistanceMetricType;
  typedef typename DistanceMetricType::Pointer DistanceMetricPointerType;

  typedef TMap                      MapType;
  typedef typename MapType::Pointer MapPointerType;

  typedef Functor::SOMbasedImageFilterFunctor<
      typename TInputImage::PixelType,
      typename TOutputImage::PixelType,
      TDistanceMetric, TMap> FunctorType;

  /** Vectors that hold the training area */
  typedef typename TInputImage::PixelType MeasurementType;

  /** Set the SOM map (or VectorImage) */
  void SetMap(const MapType * theMap);

protected:
  // throw the Map to the functor
  void BeforeThreadedGenerateData(void) override;

  SOMbasedImageFilter ();
  ~SOMbasedImageFilter () override {}

private:
  SOMbasedImageFilter (const Self &);
  void operator =(const Self&) = delete;

  MapPointerType m_Map;

}; // end of class SOMbasedImageFilter

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSOMbasedImageFilter.hxx"
#endif

#endif
