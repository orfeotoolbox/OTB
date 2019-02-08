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

#ifndef otbFastICAInternalOptimizerVectorImageFilter_h
#define otbFastICAInternalOptimizerVectorImageFilter_h

#include "otbMacro.h"
#include "otbMatrixImageFilter.h"
#include "otbStreamingStatisticsVectorImageFilter.h"
#include "otbPersistentImageFilter.h"

namespace otb
{
/** \class FastICAInternalOptimizerVectorImageFilter
 * \brief Internal optimisation of the FastICA unmixing filter
 *
 * This class implements the internal search for the unmixing matrix W
 * in the FastICA technique.
 *
 * The class takes 2 inputs (initial image and its projection with the W matrix).
 *
 * \ingroup Multithreaded
 * \sa FastICAImageFilter
 *
 * \ingroup OTBDimensionalityReduction
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT FastICAInternalOptimizerVectorImageFilter
  : public PersistentImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef FastICAInternalOptimizerVectorImageFilter          Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(FastICAInternalOptimizerVectorImageFilter, ImageToImageFilter);

  /** Dimension */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /** Template parameters typedefs */
  typedef TInputImage  InputImageType;
  typedef typename InputImageType::RegionType InputRegionType;

  typedef TOutputImage OutputImageType;
  typedef typename OutputImageType::RegionType OutputRegionType;

  /** Filter types and related */
  typedef StreamingStatisticsVectorImageFilter< InputImageType > CovarianceEstimatorFilterType;
  typedef typename CovarianceEstimatorFilterType::Pointer CovarianceEstimatorFilterPointerType;

  typedef typename CovarianceEstimatorFilterType::RealType RealType;
  typedef typename CovarianceEstimatorFilterType::RealPixelType VectorType;
  typedef typename CovarianceEstimatorFilterType::MatrixObjectType MatrixObjectType;
  typedef typename MatrixObjectType::ComponentType MatrixType;
  typedef typename MatrixType::InternalMatrixType InternalMatrixType;
  typedef typename InternalMatrixType::element_type MatrixElementType;

  typedef MatrixImageFilter< TInputImage, TOutputImage > TransformFilterType;
  typedef typename TransformFilterType::Pointer TransformFilterPointerType;

  typedef std::function<double(double)> NonLinearityType;

  itkSetMacro(CurrentBandForLoop, unsigned int);
  itkGetMacro(CurrentBandForLoop, unsigned int);

  itkGetMacro(W, InternalMatrixType);
  itkSetMacro(W, InternalMatrixType);

  void SetNonLinearity(NonLinearityType NonLinearity,
                            NonLinearityType NonLinearityDerivative)
  {
    m_NonLinearity = NonLinearity;
    m_NonLinearityDerivative = NonLinearityDerivative;
    this->Modified();
  }
  
  itkGetMacro(Beta, double);
  itkGetMacro(Den, double);

  virtual void Reset() override;
  
  virtual void Synthetize() override;

protected:
  FastICAInternalOptimizerVectorImageFilter();
  ~FastICAInternalOptimizerVectorImageFilter() override { }

  void GenerateOutputInformation() override;
  void ThreadedGenerateData ( const OutputRegionType &, itk::ThreadIdType ) override;

  unsigned int m_CurrentBandForLoop;

  std::vector<double> m_BetaVector;
  std::vector<double> m_DenVector;
  std::vector<double> m_NbSamples;

  double m_Beta;
  double m_Den;

  InternalMatrixType m_W;
  NonLinearityType m_NonLinearity;
  NonLinearityType m_NonLinearityDerivative;
  TransformFilterPointerType m_TransformFilter;
private:
  FastICAInternalOptimizerVectorImageFilter( const Self & ); // not implemented
  void operator= ( const Self & ); // not implemented
}; // end of class

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbFastICAInternalOptimizerVectorImageFilter.hxx"
#endif


#endif


