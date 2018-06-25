/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbSparseUnmixingImageFilter_h
#define otbSparseUnmixingImageFilter_h

#include "itkListSample.h"
#include "itkHistogram.h"

#include "otbMacro.h"
#include "otbImage.h"
#include "otbWaveletOperator.h"
#include "otbWaveletFilterBank.h"
#include "otbWaveletTransform.h"
#include "otbSparseWvltToAngleMapperListFilter.h"
#include "otbAngularProjectionSetImageFilter.h"

namespace otb {

/** \class SparseUnmixingImageFilter
 * \brief This class detects linear dependencies from N wavelet decompositions
 *
 * This class perform a linear unmixing from a sparse representation of N
 * signals (Through multiscale wavelet transforms) by means of line detection
 * in the scatterplot.
 *
 * It is implemented as a multi-filter that performs wavelet decomposition of the N
 * images, Sample list construction of the scatter plot sample, histogram estimation
 * of the phase and source detection...
 *
 * N is given in template value
 *
 *
 * \ingroup OTBUnmixing
 */
template < class TInputImage, class TOutputImage,
            unsigned int VNbInputImage,
            class TPrecision = double,
            Wavelet::Wavelet TMotherWaveletOperator = Wavelet::SYMLET8 >
class ITK_EXPORT SparseUnmixingImageFilter
  : public ImageToImageListFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef SparseUnmixingImageFilter     Self;
  typedef ImageToImageListFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(SparseUnmixingImageFilter, ImageToImageListFilter);

  /** Dimension */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);
  itkStaticConstMacro(NumberOfInputImages, unsigned int, VNbInputImage);

  /** Template parameters typedefs */
  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;
  typedef TPrecision PrecisionType;
  static const Wavelet::Wavelet MotherWaveletOperatorID = TMotherWaveletOperator;

  /** Filter types and related */
  typedef Image< PrecisionType, InputImageDimension > InternalImageType;

  typedef WaveletOperator< MotherWaveletOperatorID, Wavelet::FORWARD, PrecisionType, InputImageDimension > WaveletOperatorType;
  typedef WaveletFilterBank< InternalImageType, InternalImageType, WaveletOperatorType, Wavelet::FORWARD > FilterBankType;
  typedef WaveletTransform< InternalImageType, InternalImageType, FilterBankType, Wavelet::FORWARD >       WvltFilterType;
  typedef typename WvltFilterType::Pointer             WvltFilterPointerType;
  typedef typename WvltFilterType::OutputImageListType InternalImageListType;
  typedef ObjectList< WvltFilterType >                 WvltFilterListType;
  typedef typename WvltFilterListType::Pointer         WvltFilterListPointerType;

//  typedef itk::FixedArray< PrecisionType, NumberOfInputImages-1 > AngleType;
  typedef itk::Array< PrecisionType >                             AngleType;
  typedef itk::Statistics::ListSample< AngleType >                AngleListType;
  typedef typename AngleListType::Pointer                         AngleListPointerType;
  typedef SparseWvltToAngleMapperListFilter< InternalImageListType, AngleListType, NumberOfInputImages > AngleListFilterType;
  typedef typename AngleListFilterType::Pointer                   AngleListFilterPointerType;
  typedef typename AngleListFilterType::OutputSampleListType      InternalSampleListType;

  typedef typename itk::Statistics::Histogram< PrecisionType > HistogramType;
  typedef typename HistogramType::Pointer               HistogramPointerType;
  typedef typename HistogramType::SizeType              HistogramSizeType;
  typedef typename HistogramType::IndexType             HistogramIndexType;
  typedef typename HistogramType::MeasurementVectorType MeasurementVectorType;
  typedef typename HistogramType::MeasurementType       MeasurementType;

  typedef AngularProjectionSetImageFilter< InputImageType, OutputImageType, AngleListType, PrecisionType > TransformFilterType;
  typedef typename TransformFilterType::Pointer                    TransformFilterPointerType;
  typedef typename TransformFilterType::OutputImageListType        OutputImageListType;
  typedef typename TransformFilterType::OutputImageListPointerType OutputImageListPointerType;
  typedef typename TransformFilterType::OutputImageIterator        OutputImageIterator;

  using Superclass::SetInput;
  void SetInput ( unsigned int i, const InputImageType * );
  const InputImageType * GetInput( unsigned int i ) const;

  void SetNumberOfDecomposition ( unsigned int nb )
  {
    for ( unsigned int i = 0; i < NumberOfInputImages; ++i )
    {
      m_WvltFilterList->GetNthElement(i)->SetNumberOfDecomposition( nb );
    }
    this->Modified();
  }
  unsigned int GetNumberOfDecomposition () const
  {
    return m_WvltFilterList->GetNthElement(0)->GetNumberOfDecomposition();
  }

  void SetThresholdValue( PrecisionType th )
  {
    m_AngleListFilter->SetThresholdValue( th );
    this->Modified();
  }
  PrecisionType GetThresholdValue () const
  {
    return m_AngleListFilter->GetThresholdValue();
  }

  /**
   * This number of end-members is estimated from number of modes in the histogram.
   */
  unsigned int GetNumberOfComponentsRequired () const
  {
    if ( m_NumberOfComponentsRequired == 0 )
      GenerateNumberOfComponentsRequired();
    return m_NumberOfComponentsRequired;
  }
  itkSetMacro(NumberOfComponentsRequired, unsigned int);

  itkSetMacro(NumberOfHistogramBins, unsigned int);
  itkGetMacro(NumberOfHistogramBins, unsigned int);

  itkGetConstMacro(AngleList, AngleListType*);
  itkGetConstMacro(WvltFilterList, WvltFilterListType*);
  itkGetConstMacro(AngleListFilter, AngleListFilterType*);
  itkGetConstMacro(Histogram, HistogramType*);
  itkGetConstMacro(Transformer, TransformFilterType*);

protected:
  SparseUnmixingImageFilter();
  ~SparseUnmixingImageFilter() override { }

  void GenerateData() override;
  virtual void GenerateNumberOfComponentsRequired ();
private:
  SparseUnmixingImageFilter(const Self &) = delete;
  void operator=(const Self &) = delete;

  unsigned int         m_NumberOfComponentsRequired;
  unsigned int         m_NumberOfHistogramBins;
  AngleListPointerType m_AngleList;

  WvltFilterListPointerType  m_WvltFilterList;
  AngleListFilterPointerType m_AngleListFilter;
  HistogramPointerType       m_Histogram;
  TransformFilterPointerType m_Transformer;
}; // end of class

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSparseUnmixingImageFilter.hxx"
#endif

#endif


