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
#ifndef __otbSparseUnmixingImageFilter_h
#define __otbSparseUnmixingImageFilter_h

#include "otbMacro.h"
#include "itkImageToImageFilter.h"
#include "otbWaveletOperator.h"
#include "otbWaveletFilterBank.h"
#include "otbWaveletTransform.h"
#include "otbSparseWvltToAngleMapperListFilter.h"
#include "otbImageList.h"
#include "itkListSample.h"
#include "itkFixedArray.h"
#include "itkHistogram.h"
#include "otbAngularProjectionBinaryImageFilter.h"

namespace otb {

/** \class SparseUnmixingImageFilter
 * \brief This class detects linear dependencies from 2 wavelet decompositions
 *
 * This class perform a linear unmixing from a sparse representation of 2
 * signals (Through multiscale wavelet transforms) by means of line detection
 * in the scatterplot.
 *
 * It is implemeted as a mutli-filter that performs wavelet decomposition of the 2
 * images, Sample list construction of the scatter plot sample, histogram estimation
 * of the phase and source detection...
 *
 */
template < class TInputImage, class TOutputImage,
            class TPrecision = double,
            Wavelet::Wavelet TMotherWaveletOperator = Wavelet::SYMLET8 >
class ITK_EXPORT SparseUnmixingImageFilter
  : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef SparseUnmixingImageFilter                          Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(SparseUnmixingImageFilter, ImageToImageFilter);

  /** Dimension */
       itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
       itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /** Template parameters typedefs */
  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;
  typedef TPrecision PrecisionType;
  static const Wavelet::Wavelet MotherWaveletOperatorID = TMotherWaveletOperator;

  /** Filter types and related */
  typedef Image< PrecisionType, InputImageDimension > InternalImageType;

  typedef WaveletOperator< MotherWaveletOperatorID, Wavelet::FORWARD, PrecisionType, InputImageDimension > WaveletOperatorType;
  typedef WaveletFilterBank< InternalImageType, InternalImageType, WaveletOperatorType, Wavelet::FORWARD > FilterBankType;
  typedef WaveletTransform< InternalImageType, InternalImageType, FilterBankType, Wavelet::FORWARD > WvltFilterType;
  typedef typename WvltFilterType::Pointer WvltFilterPointerType;
  typedef typename WvltFilterType::OutputImageListType InternalImageListType;

  typedef itk::Statistics::ListSample< itk::FixedArray< PrecisionType, 1 > > SampleType;
  typedef SparseWvltToAngleMapperListFilter< InternalImageListType, SampleType > ListFilterType;
  typedef typename ListFilterType::Pointer ListFilterPointerType;
  typedef typename ListFilterType::OutputSampleListType InternalSampleListType;

  typedef typename itk::Statistics::Histogram< PrecisionType > HistogramType;
  typedef typename HistogramType::Pointer HistogramPointerType;

  typedef AngularProjectionBinaryImageFilter< InputImageType, OutputImageType, PrecisionType > TransformFilterType;
  typedef typename TransformFilterType::Pointer TransformFilterPointerType;

  /** Sets/Gets */
  void SetInput1 ( const InputImageType * );
  const InputImageType * GetInput1 () const;

  void SetInput2 ( const InputImageType * );
  const InputImageType * GetInput2 () const;

  void SetNumberOfDecomposition ( unsigned int nb )
  {
    m_WvltFilter1->SetNumberOfDecomposition( nb );
    m_WvltFilter2->SetNumberOfDecomposition( nb );
    this->Modified();
  }
  unsigned int GetNumberOfDecomposition () const
  {
    return m_WvltFilter1->GetNumberOfDecomposition();
  }

  void SetLowerThreshold( PrecisionType th )
  {
    m_ListFilter->GetFunctor().SetLowerThreshold( th );
    this->Modified();
  }
  PrecisionType GetLowerThreshold () const
  {
    return m_ListFilter->GetFunctor().GetLowerThreshold();
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

  itkGetConstMacro(WvltFilter1, WvltFilterType*);
  itkGetConstMacro(WvltFilter2, WvltFilterType*);
  itkGetConstMacro(ListFilter, ListFilterType*);
  itkGetConstMacro(Histogram, HistogramType*);
  itkGetConstMacro(Transformer, TransformFilterType*);

protected:
  SparseUnmixingImageFilter();
  virtual ~SparseUnmixingImageFilter() { }

  virtual void GenerateData();
  virtual void GenerateNumberOfComponentsRequired ();
private:
  SparseUnmixingImageFilter(const Self &); //purposely not implemented
  void operator=(const Self &); //purposely not implemented

  unsigned int m_NumberOfComponentsRequired;
  std::vector<PrecisionType> m_AngleValue;

  WvltFilterPointerType m_WvltFilter1;
  WvltFilterPointerType m_WvltFilter2;
  ListFilterPointerType m_ListFilter;
  HistogramPointerType  m_Histogram;
  TransformFilterPointerType m_Transformer;
}; // end of class

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSparseUnmixingImageFilter.txx"
#endif

#endif


