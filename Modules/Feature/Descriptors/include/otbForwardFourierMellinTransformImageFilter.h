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

#ifndef __otbForwardFourierMellinTransformImageFilter_h
#define __otbForwardFourierMellinTransformImageFilter_h

#include "otbImage.h"
#include "itkVnlForwardFFTImageFilter.h"
#include "otbLogPolarTransform.h"
#include "itkResampleImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"

namespace otb
{

/**
 * \class ForwardFourierMellinTransformImageFilter
 * \brief Calculate the Fourier-Mellin transform over an image.
 *
 * This class implements a composite filter. It combines two filters :
 *   - otb::LogPolarResampleImageFilter : converting the input image in a
 log-polar system coordinate
 *   - itk::VnlFFTRealToComplexConjugateImageFilter : applying the Forward
 Fourier transform of the log-polar image.
 *
 * \note the Fourier transform image is coded from 0.0 to 1.0
 *
 * \ingroup ImageFunctions
 */

template <class TPixel,
    class TInterpol,
    unsigned int Dimension = 2>

class ITK_EXPORT ForwardFourierMellinTransformImageFilter :
  public itk::ImageToImageFilter<Image<TPixel, Dimension>,
                                 itk::Image<std::complex<TPixel>, Dimension> >
{
public:

  //typedef TPixel                 PixelType;
  typedef Image<TPixel, Dimension> InputImageType;
  /*   typedef otb::Image< std::complex< TPixel > , Dimension >           OutputImageType; */

  typedef typename itk::VnlForwardFFTImageFilter<InputImageType>                   FourierImageFilterType;
  typedef typename FourierImageFilterType::OutputImageType                         OutputImageType;

  /** Standard class typedefs. */
  typedef ForwardFourierMellinTransformImageFilter                 Self;
  typedef itk::ImageToImageFilter<InputImageType, OutputImageType> Superclass;
  typedef itk::SmartPointer<Self>                                  Pointer;
  typedef itk::SmartPointer<const Self>                            ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ForwardFourierMellinTransformImageFilter, itk::ImageToImageFilter);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputImageType typedef support. */
  typedef typename InputImageType::PixelType    PixelType;
  typedef typename InputImageType::IndexType    IndexType;
  typedef typename InputImageType::Pointer      ImagePointer;
  typedef typename InputImageType::ConstPointer ImageConstPointer;
  typedef typename InputImageType::SizeType     SizeType;
  typedef typename InputImageType::SpacingType  SpacingType;

  /** InputImageType typedef support. */
  typedef typename OutputImageType::PixelType    OutputPixelType;
  typedef typename OutputImageType::IndexType    OutputIndexType;
  typedef typename OutputImageType::SizeType     OutputSizeType;
  typedef typename OutputImageType::Pointer      OutputImagePointer;
  typedef typename OutputImageType::ConstPointer OutputImageConstPointer;
  typedef typename OutputImageType::RegionType   OutputImageRegionType;

  typedef typename OutputImageType::PixelType ComplexType;

  /** Typedefs to describe and access Interpolator */
  typedef TInterpol                               InterpolatorType;
  typedef typename InterpolatorType::Pointer      InterpolatorPointerType;
  typedef typename InterpolatorType::ConstPointer InterpolatorConstPointerType;
  typedef typename InterpolatorType::CoordRepType CoordRepType;
  typedef typename InterpolatorType::PointType    PointType;

  typedef LogPolarTransform<CoordRepType>                                        LogPolarTransformType;
  typedef typename LogPolarTransformType::Pointer                                LogPolarTransformPointerType;
  typedef itk::ResampleImageFilter<InputImageType, InputImageType, CoordRepType> ResampleFilterType;
  typedef typename ResampleFilterType::Pointer                                   ResampleFilterPointerType;
  typedef itk::ImageRegionIteratorWithIndex<InputImageType>                      IteratorType;

  typedef typename FourierImageFilterType::Pointer      FourierImageFilterPointer;
  typedef typename FourierImageFilterType::ConstPointer FourierImageFilterConstPointer;

  /** Set/Get the output size of the transform  (should be powers of two in both directions */
  itkSetMacro(OutputSize, OutputSizeType);
  itkGetMacro(OutputSize, OutputSizeType);

  /** Set/Get the Sigma value for the Log-polar resampler  */
  itkSetMacro(Sigma, double);
  itkGetMacro(Sigma, double);

  /** Set/Get the Default pixel value for the Log-polar resampler  */
  itkSetMacro(DefaultPixelValue, PixelType);
  itkGetMacro(DefaultPixelValue, PixelType);

  /** Set/Get the Interpolator pointer for the Log-polar resampler  */
  itkSetObjectMacro(Interpolator, InterpolatorType);
  itkGetObjectMacro(Interpolator, InterpolatorType);

protected:
  ForwardFourierMellinTransformImageFilter();
  virtual ~ForwardFourierMellinTransformImageFilter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  virtual void GenerateOutputInformation(void);

  virtual void GenerateInputRequestedRegion(void);

  /** Main Computation Method */
  void GenerateData();

private:
  ForwardFourierMellinTransformImageFilter(const Self &);  //purposely not implemented
  void operator =(const Self&);  //purposely not implemented

  /** Sigma for normalization */
  double m_Sigma;

  /** Output size */
  OutputSizeType m_OutputSize;

  /** Output pixel default value */
  PixelType m_DefaultPixelValue;

  /** Interpolator */
  InterpolatorPointerType m_Interpolator;

  /** Transform */
  LogPolarTransformPointerType m_Transform;

  /** Resampler */
  ResampleFilterPointerType m_ResampleFilter;

  /** FFT Filter */
  FourierImageFilterPointer m_FFTFilter;

};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbForwardFourierMellinTransformImageFilter.txx"
#endif

#endif
