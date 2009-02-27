/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom / Telecom Bretagne. All rights reserved.
  See ITCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbStationaryFilterBank__h
#define __otbStationaryFilterBank__h

#include "itkProgressReporter.h"
#include "itkImageToImageFilter.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkNeighborhoodInnerProduct.h"

namespace otb
{

/** \class StationaryFilterBank
 * \brief One level stationary wavelet transform
 *
 * This implementation performs a low-pass / high-pass wavelet transformation
 * of an image. The wavelet transformation is defined by a inner product
 * (ie. convolution-like operation).
 *
 * the inner operator are supposed to be defined through 1D filters. Then, the
 * transformation yields \$ 2^Dim \$ output images.
 *
 * In case of 1D, GetOutput(0) -> LowPass
 *
 *                GetOutput(1) -> HighPass
 *
 * In case of 2D,                 Line (Dim 1) Col (Dim 0)
 *
 *                GetOutput(0) -> LowPass,      LowPass
 *
 *                GetOutput(1) -> LowPass,      HighPass
 *
 *                GetOutput(2) -> HighPass,     LowPass
 *
 *                GetOutput(3) -> HighPass,     HighPass
 *
 * In case of nD data, assume x_n=0 stands for LowPass and x_n=1 stands for HighPass
 * at a give dimension n. Then
 *
 * GetOutput( x_(n-1) << (n-1) + x_(n-2) << (n-2) + ... + x_1 << 1 + x_0 )
 *
 *     Dim (n-1)   Dim (n-2)   Dim (n-3)   ...   Dim 1  Dim 0
 *
 *  ->  x_(n-1)     x_(n-2)     x_(n-3)           x_1    x_0
 *
 *
 *
 * \sa LowPassHaarOperator
 * \sa HighPassHaarOperator
 * \sa LowPass_9_7_Operator
 * \sa HighPass_9_7_Operator
 *
 * \ingroup Streamed
 */
template < class TInputImage, class TOutputImage, class TLowPassOperator, class THighPassOperator >
class ITK_EXPORT StationaryFilterBank
      : public itk::ImageToImageFilter< TInputImage, TOutputImage >
{
public:
  /** Standard typedefs */
  typedef StationaryFilterBank Self;
  typedef itk::ImageToImageFilter< TInputImage, TOutputImage > Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(StationaryFilterBank,ImageToImageFilter);

  /** Template parameters typedefs */
  typedef TInputImage InputImageType;
  typedef typename InputImageType::Pointer InputImagePointerType;
  typedef typename InputImageType::RegionType InputImageRegionType;
  typedef typename InputImageType::SizeType InputSizeType;
  typedef typename InputImageType::IndexType InputIndexType;
  typedef typename InputImageType::PixelType InputPixelType;

  typedef TOutputImage OutputImageType;
  typedef typename OutputImageType::Pointer OutputImagePointerType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;
  typedef typename OutputImageType::SizeType OutputSizeType;
  typedef typename OutputImageType::IndexType OutputIndexType;
  typedef typename OutputImageType::PixelType OutputPixelType;

  typedef TLowPassOperator LowPassOperatorType;
  typedef THighPassOperator HighPassOperatorType;

  /** Inner product iterators */
  typedef itk::ConstNeighborhoodIterator< OutputImageType > NeighborhoodIteratorType;
  typedef itk::NeighborhoodInnerProduct< OutputImageType > InnerProductType;
  typedef itk::ImageRegionIterator< OutputImageType > IteratorType;
  typedef typename itk::NeighborhoodAlgorithm
  ::ImageBoundaryFacesCalculator< OutputImageType > FaceCalculatorType;
  typedef typename FaceCalculatorType::FaceListType FaceListType;
  typedef typename FaceListType::iterator FaceListIterator;

  /** Dimension */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /**
   * Set/Get the level of up sampling of the filter used in the A-trou algorithm
   */
  itkGetMacro(UpSampleFactor,unsigned int);
  itkSetMacro(UpSampleFactor,unsigned int);

protected:
  StationaryFilterBank();
  virtual ~StationaryFilterBank() { }

  /** Generate data redefinition */
  virtual void ThreadedGenerateData ( const OutputImageRegionType& outputRegionForThread, int threadId );

  /** Specific case applied on the input image */
  virtual void ThreadedGenerateData ( itk::ProgressReporter & reporter,
                                      const OutputImageRegionType& outputRegionForThread, int threadId );

  /** Iterative call to the filter bank at each dimension */
  virtual void ThreadedGenerateData ( unsigned int idx, unsigned int direction,
                                      itk::ProgressReporter & reporter,
                                      const OutputImageRegionType& outputRegionForThread, int threadId );

  unsigned int m_UpSampleFactor;

private:
  StationaryFilterBank( const Self & );
  void operator=( const Self & );

}; // end of class

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStationaryFilterBank.txx"
#endif

#endif

