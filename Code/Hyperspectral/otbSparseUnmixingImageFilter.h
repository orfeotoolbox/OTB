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
#include "otbWaveletTransformImageFilter.h"
#include "otbSparseWvltToAngleMapperListFilter.h"
#include "otbImageList.h"
#include "itkListSample.h"
#include "itkFixedArray.h"
#include "itkHistogram.h"
#include "otbMatrixMultiplyImageFilter.h"

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
            class TPixelPrecision = double, 
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
  typedef TMotherWaveletOperator MotherWaveletOperatorID;

  /** Filter types and related */
  typedef Image< PrecisionType, InputImageDimension > InternalImageType;

  typedef WaveletOperator< MotherWaveletOperatorID, Wavelet::FORWARD, Precision, InputImageDimension > WaveletOperator;
  typedef WaveletFilterBank< InputImageType, InternalImageType, WaveletOperator, Wavelet::FORWARD > FilterBankType;
  typedef WaveletTransform< InputImageType, InternalImageType, FilterBankType, Wavelet::FORWARD > WvltFilterType;
  typedef typename WvltFilterType::Pointer WvltFilterPointerType;
  typedef typename WvltFilterType::OutputImageListType InternalImageListType;

  typedef itk::Statistics::ListSample< itk::FixedArray< PrecisionType, 1 > > SampleType;
  typedef SparseWvltToAngleMapperListFilter< InternalImageListType, SampleType > ListFilterType;
  typedef typename ListFilterType::Pointer ListFilterPointerType;

  typedef typename itk::Statistics::Histogram< PrecisionType > HistogramType;
  typedef typename HistogramType::Pointer HistogramPointerType;

  typedef MatrixMultiplyImageFilter< InternalImageType, OutputImageType, PrecisionType > TransformFilterType;
  typedef typename TransformFilterType::Pointer TransformFilterPointerType;

  /** Sets/Gets */
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

protected:
  SparseUnmixingImageFilter();
  virtual ~SparseUnmixingImageFilter() { }

private:
  SparseUnmixingImageFilter(const Self &); // not implemented
  void operator=(const Self &);


  WvltFilterPointerType m_WvltFilter1;
  WvltFilterPointerType m_WvltFilter2;
  ListFilterPointerType m_ListFilter;
  HistogramPointerType  m_Histogram;
  TransformFilterPointerType m_Transformer;
}; // end of class 

} // end of namespace otb
#endif


