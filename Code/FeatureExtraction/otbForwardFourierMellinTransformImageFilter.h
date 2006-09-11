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

#ifndef _otbForwardFourierMellinTransformImageFilter_h
#define _otbForwardFourierMellinTransformImageFilter_h

#include "otbImage.h"
#include "otbLogPolarResampleImageFilter.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkImageToImageFilter.h"
#include "itkVnlFFTRealToComplexConjugateImageFilter.h"

namespace otb
{

/**
 * \class ForwardFourierMellinTransformImageFilter
 * \brief Calculate the Fourier-Mellin transform over an image.
 *
 * This class implements a composite filter. It combines two filters : 
 * \begin{itemize}
 *   \item otb::LogPolarResampleImageFilter
 *   \item itk:: ...
 * \end{itemize}
 * 
 * \ingroup ImageFunctions
 */

template < class TPixel, 
           class TInterpol,
	   unsigned int  Dimension = 2 >
	   
class ITK_EXPORT ForwardFourierMellinTransformImageFilter :
    public itk::ImageToImageFilter<Image< TPixel , Dimension >,
                                   Image< std::complex<TPixel> , Dimension > >
{
public:

  typedef TPixel						     PixelType;
  typedef otb::Image< TPixel , Dimension >                           InputImageType;
  typedef otb::Image< std::complex< TPixel > , Dimension >           OutputImageType;

  /** Standard class typedefs. */
  typedef ForwardFourierMellinTransformImageFilter                    Self;
  typedef itk::ImageToImageFilter< InputImageType, OutputImageType>   Superclass;
  typedef itk::SmartPointer<Self>                                     Pointer;
  typedef itk::SmartPointer<const Self>                               ConstPointer;
  


  /** Run-time type information (and related methods). */
  itkTypeMacro(	ForwardFourierMellinTransformImageFilter, itk::ImageToImageFilter);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputImageType typedef support. */
  typedef typename InputImageType::PixelType            PixeltType;
  typedef typename InputImageType::IndexType            IndexType;
  typedef typename InputImageType::Pointer              ImagePointer;
  typedef typename InputImageType::ConstPointer         ImageConstPointer;
  
  /** InputImageType typedef support. */
  typedef typename OutputImageType::PixelType            OutputPixeltType;
  typedef typename OutputImageType::IndexType            OutputIndexType;
  typedef typename OutputImageType::Pointer              OutputImagePointer;
  typedef typename OutputImageType::ConstPointer         OutputImageConstPointer;
  typedef typename OutputImageType::RegionType 	  	 OutputImageRegionType;

 
  typedef typename OutputImageType::PixelType            ComplexType;

  /** Typedefs to describe and access Interpolator */
  typedef TInterpol 					        InterpolatorType;
  typedef typename InterpolatorType::Pointer 			InterpolatorPointer;
  typedef typename InterpolatorType::ConstPointer 		InterpolatorConstPointer;
  typedef typename InterpolatorType::CoordRepType 		CoordRepType;
  typedef typename InterpolatorType::PointType   		PointType;
  //typedef typename InterpolatorType::CoordRepType   		CoordRepType;


  typedef LogPolarResampleImageFilter<InputImageType,TInterpol>   LogPolarResampleImageFilterType;
  typedef typename LogPolarResampleImageFilterType::Pointer                LogPolarResampleImageFilterPointer;

  typedef typename itk::VnlFFTRealToComplexConjugateImageFilter<PixeltType,Dimension> FourierImageFilterType;
  typedef typename FourierImageFilterType::Pointer 			FourierImageFilterPointer;
  typedef typename FourierImageFilterType::ConstPointer 		FourierImageFilterConstPointer;

  /** Set/Get Angular Step */
  void SetAngularStep( double angularStep );
  const double GetAngularStep( void );  

  /** Set/Get Radial Step */
  void SetRadialStep( double radialStep );
  const double GetRadialStep( void );  

  /** Set/Get Radial number of samples */
  void SetRadialNumberOfSamples( double radialNumberOfSamples );
  const double GetRadialNumberOfSamples( void );  

  /** Set/Get Angular number of samples */
  void SetAngularNumberOfSamples( double angularNumberOfSamples );
  const double GetAngularNumberOfSamples( void );  

  /** Set/Get the Sigma value for the Log-polar resampler  */
  void SetSimga( double sigma );
  const double GetSigma( void );  

  /** Set/Get the IsAtCenter value for the Log-polar resampler  */
  void SetIsAtCenter( bool isAtCenter );
  const bool GetIsAtCenter( void );  

  /** Set/Get the Default pixel value for the Log-polar resampler  */
  void SetDefaultPixelValue( PixelType pixelValue );
//  const PixelType GetDefaultPixelValue( void );  

  /** Set/Get the Interpolator pointer for the Log-polar resampler  */
  void SetInterpolator( InterpolatorPointer interpolator );
//  InterpolatorConstPointer GetInterpolator( void );  

  /** ForwardFourierMellinTransformImageFilter produces an image which is a different size
   * than its input.  As such, it needs to provide an implementation
   * for GenerateOutputInformation() in order to inform the pipeline
   * execution model.  The original documentation of this method is
   * below. \sa ProcessObject::GenerateOutputInformaton() */
  virtual void GenerateOutputInformation();


protected:
  ForwardFourierMellinTransformImageFilter();
  ~ForwardFourierMellinTransformImageFilter(){};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;
  void GenerateData();

  
private:
  ForwardFourierMellinTransformImageFilter( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  LogPolarResampleImageFilterPointer	   m_LogPolarResample;
  FourierImageFilterPointer 	           m_FourierTransform;
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbForwardFourierMellinTransformImageFilter.txx"
#endif

#endif

