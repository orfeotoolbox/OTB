/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Mines-Telecom. All rights reserved.
  See IMTCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbWaveletFilterBank_h
#define __otbWaveletFilterBank_h

#include "itkProgressReporter.h"
#include "itkImageToImageFilter.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkNeighborhoodInnerProduct.h"

#include "otbWaveletOperatorBase.h"

namespace otb {

/** \class WaveletFilterBank
 * \brief One level stationary wavelet transform
 *
 * This implementation performs a low-pass / high-pass wavelet transformation
 * of an image. The wavelet transformation is defined by a inner product
 * (ie. convolution-like operation).
 *
 * the inner operator are supposed to be defined through 1D filters. Then, the
 * forward transformation yields \f$ 2^{\text{Dim}} \f$ output images, while the inverse
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
 * \todo: At present version, there is not consideration on meta data information that can be transmited
 * from the input(s) to the output(s)...
 *
 * The two choice (Wavelet::FORWARD/Wavelet::INVERSE) yield specific implementation of the templates (header redeclaration
 * is given at bottom of otbWaveletFilterBank.h for the Wavelet::INVERSE case)
 *
 * \sa WaveletOperator
 *
 * \ingroup Streamed
 *
 * \ingroup OTBWavelet
 */
template <class TInputImage, class TOutputImage,
    class TWaveletOperator,
    Wavelet::WaveletDirection TDirectionOfTransformation>
class ITK_EXPORT WaveletFilterBank
  : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef WaveletFilterBank                                  Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(WaveletFilterBank, ImageToImageFilter);

protected:
  WaveletFilterBank();
  virtual ~WaveletFilterBank();

private:
  WaveletFilterBank(const Self &);
  void operator =(const Self&);
}; // end of class

/** \class WaveletFilterBank
 * \brief Template specialization of FilterBank for forward transformation
 *
 * This implementation performs a low-pass / high-pass wavelet transformation
 * of an image. The wavelet transformation is defined by a inner product
 * (ie. convolution-like operation).
 *
 * The inner operator are supposed to be defined through 1D filters. Then, the
 * forward transformation yields \f$ 2^{\text{Dim}} \f$ output images, while the inverse
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
 * \todo: At present version, there is not consideration on meta data information that can be transmited
 * from the input(s) to the output(s)...
 *
 * \sa WaveletOperator
 *
 * \ingroup OTBWavelet
 */
template <class TInputImage, class TOutputImage, class TWaveletOperator>
class ITK_EXPORT WaveletFilterBank<TInputImage, TOutputImage, TWaveletOperator, Wavelet::FORWARD>
  : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef WaveletFilterBank                                  Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(WaveletFilterBank, ImageToImageFilter);

  /** Template parameters typedefs */
  typedef TInputImage                         InputImageType;
  typedef typename InputImageType::Pointer    InputImagePointerType;
  typedef typename InputImageType::RegionType InputImageRegionType;
  typedef typename InputImageType::SizeType   InputSizeType;
  typedef typename InputImageType::IndexType  InputIndexType;
  typedef typename InputImageType::PixelType  InputPixelType;

  typedef TOutputImage                         OutputImageType;
  typedef typename OutputImageType::Pointer    OutputImagePointerType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;
  typedef typename OutputImageType::SizeType   OutputSizeType;
  typedef typename OutputImageType::IndexType  OutputIndexType;
  typedef typename OutputImageType::PixelType  OutputPixelType;

  typedef TWaveletOperator                               WaveletOperatorType;
  typedef typename WaveletOperatorType::LowPassOperator  LowPassOperatorType;
  typedef typename WaveletOperatorType::HighPassOperator HighPassOperatorType;

  typedef Wavelet::WaveletDirection DirectionOfTransformationEnumType;
  itkStaticConstMacro(DirectionOfTransformation, DirectionOfTransformationEnumType, Wavelet::FORWARD);

  /** Dimension */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /**
   * Set/Get the level of up sampling of the filter used in the A-trou algorithm.
   */
  itkGetMacro(UpSampleFilterFactor, unsigned int);
  itkSetMacro(UpSampleFilterFactor, unsigned int);

  /**
   * Set/Get the level of down sampling of the image used in forward algorithm.
   * (or upsampling in the inverse case)
   *
   * In this implementation, we are not dealing with M-band decomposition then m_SubsampleImageFactor
   * is most likely to be 1 or 2... but in any case integer and not real...
   */
  itkGetMacro(SubsampleImageFactor, unsigned int);
  itkSetMacro(SubsampleImageFactor, unsigned int);

protected:
  WaveletFilterBank();
  virtual ~WaveletFilterBank() {}

  /** GenerateOutputInformation
    * Set the size of the output image depending on the decimation factor
    * Copy information from the input image if existing.
    **/
  virtual void GenerateOutputInformation();

  /** The forward transformation needs a larger input requested
   * region than the output requested region (larger by subsampling
   * but also by the kernel size used in the filter bank).
   *
   * Then, the class needs to provide an implementation
   * for GenerateInputRequestedRegion() in order to inform the
   * pipeline execution model.
   *
   * \sa ImageToImageFilter::GenerateInputRequestedRegion() */
  virtual void GenerateInputRequestedRegion()
    throw (itk::InvalidRequestedRegionError);

  /** BeforeThreadedGenerateData.
   * It allocates also internal images
   */
  virtual void BeforeThreadedGenerateData();

  /** Internal Data Allocation
   * If m_SubsampleImageFactor != 1, internal data with progressive region size
   * subsampling if required...
   */
  virtual void AllocateInternalData(const OutputImageRegionType& outputRegion);

  /** AfterThreadedGenerateData.
   * It enforce memory destruction of internal images
   */
  virtual void AfterThreadedGenerateData();

  /** CallCopyOutputRegionToInputRegion
   * Since input and output image may be of different size when a
   * subsampling factor has tp be applied, Region estimation
   * functions has to be reimplemented
   */
  virtual void CallCopyOutputRegionToInputRegion
    (InputImageRegionType& destRegion, const OutputImageRegionType& srcRegion);
  virtual void CallCopyInputRegionToOutputRegion
    (OutputImageRegionType& destRegion, const InputImageRegionType& srcRegion);

  /** CallCopyOutputRegionToInputRegion
   * This function is also redefined in order to adapt the shape of the regions with
   * resect to the direction (among the dimensions) of the filtering.
   */
  virtual void CallCopyOutputRegionToInputRegion(unsigned int direction,
                                                 InputImageRegionType& destRegion,
                                                 const OutputImageRegionType& srcRegion);
  virtual void CallCopyInputRegionToOutputRegion(unsigned int direction,
                                                 OutputImageRegionType& destRegion,
                                                 const InputImageRegionType& srcRegion);

  /** Generate data redefinition */
  virtual void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);

  /** Iterative call to the forward filter bank at each dimension. */
  virtual void ThreadedGenerateDataAtDimensionN(unsigned int idx, unsigned int direction,
                                                itk::ProgressReporter& reporter,
                                                const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);

private:
  WaveletFilterBank(const Self &);
  void operator =(const Self&);

  unsigned int m_UpSampleFilterFactor;
  unsigned int m_SubsampleImageFactor;

  /** the easiest way to store internal images is to keep track of the splits
   * at each direction. Then, std::vector< InternalImagesTabular > is a tab of
   * size ImageDimension-1 and each InternalImagesTabular contains intermediate
   * images.
   */
  typedef std::vector<OutputImagePointerType> InternalImagesTabular;
  std::vector<InternalImagesTabular> m_InternalImages;
}; // end of class

/** \class WaveletFilterBank
 * \brief Template specialization of FilterBank for inverse transformation
 *
 * This implementation performs a low-pass / high-pass wavelet transformation
 * of an image. The wavelet transformation is defined by a inner product
 * (ie. convolution-like operation).
 *
 * The inner operator are supposed to be defined through 1D filters. Then, the
 * forward transformation yields \f$ 2^{\text{Dim}} \f$ output images, while the inverse
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
 * \todo: At present version, there is not consideration on meta data information that can be transmited
 * from the input(s) to the output(s)...
 *
 * \sa WaveletOperator
 *
 * \ingroup OTBWavelet
 */
template <class TInputImage, class TOutputImage, class TWaveletOperator>
class ITK_EXPORT WaveletFilterBank<TInputImage, TOutputImage, TWaveletOperator, Wavelet::INVERSE>
  : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef WaveletFilterBank                                  Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(WaveletFilterBank, ImageToImageFilter);

  /** Template parameters typedefs */
  typedef TInputImage                         InputImageType;
  typedef typename InputImageType::Pointer    InputImagePointerType;
  typedef typename InputImageType::RegionType InputImageRegionType;
  typedef typename InputImageType::SizeType   InputSizeType;
  typedef typename InputImageType::IndexType  InputIndexType;
  typedef typename InputImageType::PixelType  InputPixelType;

  typedef TOutputImage                         OutputImageType;
  typedef typename OutputImageType::Pointer    OutputImagePointerType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;
  typedef typename OutputImageType::SizeType   OutputSizeType;
  typedef typename OutputImageType::IndexType  OutputIndexType;
  typedef typename OutputImageType::PixelType  OutputPixelType;

  typedef TWaveletOperator                               WaveletOperatorType;
  typedef typename WaveletOperatorType::LowPassOperator  LowPassOperatorType;
  typedef typename WaveletOperatorType::HighPassOperator HighPassOperatorType;

  typedef Wavelet::WaveletDirection DirectionOfTransformationEnumType;
  itkStaticConstMacro(DirectionOfTransformation, DirectionOfTransformationEnumType, Wavelet::INVERSE);

  /** Dimension */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /**
   * Set/Get the level of up sampling of the filter used in the A-trou algorithm.
   */
  itkGetMacro(UpSampleFilterFactor, unsigned int);
  itkSetMacro(UpSampleFilterFactor, unsigned int);

  /**
   * Set/Get the level of down sampling of the image used in forward algorithm.
   * (or upsampling in the inverse case)
   *
   * In this implementation, we are dealing with M-band decomposition then m_SubsampleImageFactor
   * is most likely to be 1 or 2... but in any case integer and not real...
   */
  itkGetMacro(SubsampleImageFactor, unsigned int);
  itkSetMacro(SubsampleImageFactor, unsigned int);

protected:
  WaveletFilterBank();
  virtual ~WaveletFilterBank() {}

  /** GenerateOutputInformation
    * Set the size of the output image depending on the decimation factor
    * Copy information from the input image if existing.
    **/
  virtual void GenerateOutputInformation();

  /** The inverse transformation needs larger inputs requested
   * region than the output requested region (larger by subsampling
   * but also by the kernel size used in the filter bank).
   *
   * Then, the class needs to provide an implementation
   * for GenerateInputRequestedRegion() in order to inform the
   * pipeline execution model.
   *
   * \sa ImageToImageFilter::GenerateInputRequestedRegion() */
  virtual void GenerateInputRequestedRegion()
    throw (itk::InvalidRequestedRegionError);

  /** BeforeThreadedGenerateData
   * If SubsampleImageFactor neq 1, it is necessary to up sample input images in the Wavelet::INVERSE mode
   */
  virtual void BeforeThreadedGenerateData();

  /** Internal Data Allocation
   * If m_SubsampleImageFactor != 1, internal data with progressive region size
   * subsampling if required...
   */
  virtual void AllocateInternalData(const OutputImageRegionType& outputRegion);

  /** AfterThreadedGenerateData.
   * It enforce memory destruction of internal images
   */
  virtual void AfterThreadedGenerateData();

  /** CallCopyOutputRegionToInputRegion
   * Since input and output image may be of different size when a
   * subsampling factor has tp be applied, Region estimation
   * functions has to be reimplemented
   */
  virtual void CallCopyOutputRegionToInputRegion
    (InputImageRegionType& destRegion, const OutputImageRegionType& srcRegion);
  virtual void CallCopyInputRegionToOutputRegion
    (OutputImageRegionType& destRegion, const InputImageRegionType& srcRegion);

  /** CallCopyOutputRegionToInputRegion
   * This function is also redefined in order to adapt the shape of the regions with
   * resect to the direction (among the dimensions) of the filtering.
   */
  virtual void CallCopyOutputRegionToInputRegion(unsigned int direction,
                                                 InputImageRegionType& destRegion,
                                                 const OutputImageRegionType& srcRegion);
  virtual void CallCopyInputRegionToOutputRegion(unsigned int direction,
                                                 OutputImageRegionType& destRegion,
                                                 const InputImageRegionType& srcRegion);

  /** Generate data redefinition */
  virtual void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);

  /**
   * Iterative call to the forward filter bank at each dimension.
   * Used for the multiresolution case only.
   */
  virtual void ThreadedGenerateDataAtDimensionN(unsigned int direction,
                                                itk::ProgressReporter& reporter,
                                                const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);

private:
  WaveletFilterBank(const Self &);
  void operator =(const Self&);

  unsigned int m_UpSampleFilterFactor;
  unsigned int m_SubsampleImageFactor;

  /** the easiest way to store internal images is to keep track of the splits
   * at each direction. Then, std::vector< InternalImagesTabular > is a tab of
   * size ImageDimension-1 and each InternalImagesTabular contains intermediate
   * images. Internal images are used for multiresolution case only.
   */
  typedef std::vector<OutputImagePointerType> InternalImagesTabular;
  std::vector<InternalImagesTabular> m_InternalImages;

}; // end of class

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbWaveletFilterBank.txx"
#endif

#endif
