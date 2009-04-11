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

#ifndef __otbStationaryFilterBank_h
#define __otbStationaryFilterBank_h

#include "itkProgressReporter.h"
#include "itkImageToImageFilter.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkNeighborhoodInnerProduct.h"

namespace otb {

/** \class StationaryFilterBank
 * \brief One level stationary wavelet transform
 *
 * This implementation performs a low-pass / high-pass wavelet transformation
 * of an image. The wavelet transformation is defined by a inner product 
 * (ie. convolution-like operation).
 *
 * the inner operator are supposed to be defined through 1D filters. Then, the
 * forward transformation yields \f$ 2^{\test{Dim}} \f$ output images, while the inverse 
 * transformation requires \f$ 2^{\text{Dim}} \f$ input image for one output.
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
 * And conversely in the inverse transformation.
 *
 * TODO: At present version, there is not consideration on meta data information that can be transmited
 * from the input(s) to the output(s)...
 *
 * \sa LowPassHaarOperator
 * \sa HighPassHaarOperator
 * \sa LowPassSplineBiOrthogonalOperator
 * \sa HighPassSplineBiOrthogonalOperator
 *
 * \ingroup Streamed
 */
template < class TInputImage, class TOutputImage, 
            class TLowPassOperator, class THighPassOperator, 
            InverseOrForwardTransformationEnum TDirectionOfTransformation >
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

  typedef InverseOrForwardTransformationEnum DirectionOfTransformationEnumType;
  itkStaticConstMacro(DirectionOfTransformation,DirectionOfTransformationEnumType,TDirectionOfTransformation);

  /** Inner product iterators */
  typedef itk::ConstNeighborhoodIterator< InputImageType > NeighborhoodIteratorType;
  typedef itk::NeighborhoodInnerProduct< InputImageType > InnerProductType;
  typedef itk::ImageRegionIterator< InputImageType > IteratorType;
  typedef typename itk::NeighborhoodAlgorithm  
    ::ImageBoundaryFacesCalculator< InputImageType > FaceCalculatorType;
  typedef typename FaceCalculatorType::FaceListType FaceListType;
  typedef typename FaceListType::iterator FaceListIterator;

  /** Dimension */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /**
   * Set/Get the level of up sampling of the filter used in the A-trou algorithm.
   */
  itkGetMacro(UpSampleFilterFactor,unsigned int);
  itkSetMacro(UpSampleFilterFactor,unsigned int);

  /**
   * Set/Get the level of down sampling of the image used in forward algorithm. 
   * (or upsampling in the inverse case)
   *
   * In this implementation, we are dealing with M-band decomposition then m_SubSampleImageFactor
   * is most likely to be 1 or 2... but in any case integer and not real...
   */
  itkGetMacro(SubSampleImageFactor,unsigned int);
  itkSetMacro(SubSampleImageFactor,unsigned int);


protected:
  StationaryFilterBank();
  virtual ~StationaryFilterBank();

  /** GenerateOutputInformation 
	 * Set the size of the output image depending on the decimation factor
	 * Copy informations from the input image if existing.
	 **/
	virtual void GenerateOutputInformation();

  /** BeforeThreadedGenerateData
   * If SubSampleImageFactor neq 1, it is necessary to up sample input images in the INVERSE mode
   */
  virtual void BeforeThreadedGenerateData ();

  /** AfterThreadedGenerateData
   * If SubSampleImageFactor neq 1, it is necessary to down sample output images in the FORWARD mode
   */
  virtual void AfterThreadedGenerateData ();

  /** Generate data redefinition */
  virtual void ThreadedGenerateData ( const OutputImageRegionType& outputRegionForThread, int threadId );

  /** Specification-like in the case where DirectionOfTransformation stands for FORWARD or INVERSE */
  virtual void ThreadedForwardGenerateData ( const OutputImageRegionType& outputRegionForThread, int threadId );
  virtual void ThreadedInverseGenerateData ( const OutputImageRegionType& outputRegionForThread, int threadId );

  /** Specific case applied on the input image in forward transformation */
  virtual void ThreadedForwardGenerateData ( itk::ProgressReporter & reporter,
                                              const OutputImageRegionType& outputRegionForThread, int threadId );

  /** Iterative call to the forward filter bank at each dimension */
  virtual void ThreadedForwardGenerateData ( unsigned int idx, unsigned int direction, 
                                              itk::ProgressReporter & reporter,
                                              const OutputImageRegionType& outputRegionForThread, int threadId );

  /** Iterative call to the inverse filter bank at each dimension */
  virtual void ThreadedInverseGenerateData ( unsigned int idx, unsigned int direction, 
                                              InputImagePointerType & outputImage,
                                              itk::ProgressReporter & reporter,
                                              const InputImageRegionType& inputRegionForThread, int threadId );
  
  /** Specific  case applied on the last reconstruction in inverse transformation */
  virtual void ThreadedInverseGenerateData ( itk::ProgressReporter & reporter,
                                              const OutputImageRegionType& outputRegionForThread, int threadId );

private:
  StationaryFilterBank( const Self & );
  void operator=( const Self & );

  unsigned int m_UpSampleFilterFactor;
  unsigned int m_SubSampleImageFactor;

  std::vector< InputImagePointerType > m_SubsampledInputImages;

}; // end of class
  
} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStationaryFilterBank.txx"
#endif

#endif

