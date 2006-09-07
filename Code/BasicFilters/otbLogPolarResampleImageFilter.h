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

#ifndef __otbLogPolarResampleImageFilter_h
#define __otbLogPolarResampleImageFilter_h

#include "otbImage.h"
#include "itkImageToImageFilter.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkSize.h"

namespace otb
{

/** \class LogPolarResampleImageFilter
 * \brief Resample an image onto a log-polar coordinate transform
 *
 * LogPolarResampleImageFilter resamples an existing image to a log-polar coordinate
 * system and interpolate via some image function.  The class is templated
 * over the types of the input and output images.
 *
 * Note that the choice of interpolator function can be important.
 * This function is set via SetInterpolator().  The default is
 * itk::LinearInterpolateImageFunction<InputImageType, TInterpolatorPrecisionType>, which
 *
 * Output information (spacing, size and direction) for the output
 * image should be set. This information has the normal defaults of
 * unit spacing, zero origin and identity direction. Optionally, the
 * output information can be obtained from a reference image. If the
 * reference image is provided and UseReferenceImage is On, then the
 * spacing, origin and direction of the reference image will be used.
 *
 * Since this filter produces an image which is a different size than
 * its input, it needs to override several of the methods defined
 * in ProcessObject in order to properly manage the pipeline execution model.
 * In particular, this filter overrides
 * ProcessObject::GenerateInputRequestedRegion() and
 * ProcessObject::GenerateOutputInformation().
 *
 * This filter is implemented as a multithreaded filter.  It provides a 
 * ThreadedGenerateData() method for its implementation.
 *
 * \ingroup GeometricTransforms
 */
template <class TInputImage, 
	  class TInterpolator = itk::LinearInterpolateImageFunction<TInputImage> >
class ITK_EXPORT LogPolarResampleImageFilter:
    public itk::ImageToImageFilter<TInputImage, TInputImage>
{
public:

  /** Standard class typedefs. */
  typedef LogPolarResampleImageFilter                 	 	Self;
  typedef itk::ImageToImageFilter<TInputImage,TInputImage>  	Superclass;
  typedef itk::SmartPointer<Self>        			Pointer;
  typedef itk::SmartPointer<const Self>  			ConstPointer;

  typedef TInputImage 						InputImageType;
  typedef typename InputImageType::Pointer 			InputImagePointer;
  typedef typename InputImageType::ConstPointer 		InputImageConstPointer;
  typedef typename InputImageType::PixelType 			InputPixelType;
  typedef typename InputImageType::RegionType 			InputImageRegionType;
  typedef typename InputImageType::SizeType 			SizeType;
  
  typedef TInputImage 						OutputImageType;
  typedef typename OutputImageType::Pointer 			OutputImagePointer;
  typedef typename OutputImageType::PixelType 			OutputPixelType;
  typedef typename OutputImageType::RegionType 			OutputImageRegionType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);  

  /** Run-time type information (and related methods). */
  itkTypeMacro(LogPolarResampleImageFilter, itk::ImageToImageFilter);

  /** Number of dimensions. */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  /** Typedefs to describe and access Interpolator */
  typedef TInterpolator 					InterpolatorType;
  typedef typename InterpolatorType::Pointer 			InterpolatorPointer;
  typedef typename InterpolatorType::CoordRepType 		CoordRepType;
  typedef typename InterpolatorType::PointType   		PointType;
  typedef typename InterpolatorType::CoordRepType   		CoordRepType;

  /** Image size typedef. */
  typedef itk::Size<itkGetStaticConstMacro(OutputImageDimension)> SizeType;
  typedef typename SizeType::SizeValueType			  SizeValueType;

  /** Image index typedef. */
  typedef typename OutputImageType::IndexType 		IndexType;


   /** Image spacing,origin and direction typedef */
  typedef typename OutputImageType::SpacingType 		SpacingType;
  typedef typename OutputImageType::PointType   		OriginPointType;
  typedef typename OutputImageType::DirectionType 		DirectionType;
  
      

  /** Set the pixel value when a transformed pixel is outside of the
   * image.  The default default pixel value is 0. */
  itkSetMacro(DefaultPixelValue,OutputPixelType);

  /** Get the pixel value when a transformed pixel is outside of the image */
  itkGetMacro(DefaultPixelValue,OutputPixelType);

  /** Set the Angular spacing. */
  itkSetMacro(AngularStep, double);
  /** Get the Angular spacing. */
  itkGetConstReferenceMacro(AngularStep, double);

  /** Set the Radial number of samples. */
  itkSetMacro(RadialNumberOfSamples, double);
  /** Get the Radial number of samples. */
  itkGetConstReferenceMacro(RadialNumberOfSamples, double);

  /** Set the Sigma value. */
  itkSetMacro(Sigma, double);
  /** Get the Sigma value. */
  itkGetConstReferenceMacro(Sigma, double);

  /** LogPolarResampleImageFilter produces an image which is a different size
   * than its input.  As such, it needs to provide an implementation
   * for GenerateOutputInformation() in order to inform the pipeline
   * execution model.  The original documentation of this method is
   * below. \sa ProcessObject::GenerateOutputInformaton() */
  virtual void GenerateOutputInformation();

  /** LogPolarResampleImageFilter needs a different input requested region than
   * the output requested region.  As such, LogPolarResampleImageFilter needs
   * to provide an implementation for GenerateInputRequestedRegion()
   * in order to inform the pipeline execution model.
   * \sa ProcessObject::GenerateInputRequestedRegion() */
  virtual void GenerateInputRequestedRegion();

  /** This method is used to set the state of the filter before 
   * multi-threading. */
  virtual void BeforeThreadedGenerateData();

  /** This method is used to set the state of the filter after 
   * multi-threading. */
  virtual void AfterThreadedGenerateData();

  /** Method Compute the Modified Time based on changed to the components. */
  unsigned long GetMTime( void ) const;


  itkSetMacro(IsOriginAtCenter, bool);
  itkBooleanMacro(IsOriginAtCenter);
  itkGetMacro(IsOriginAtCenter, bool);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(OutputHasNumericTraitsCheck,
                  (Concept::HasNumericTraits<OutputPixelType>));
  /** End concept checking */
#endif

protected:
  LogPolarResampleImageFilter();
  ~LogPolarResampleImageFilter() {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** LogPolarResampleImageFilter can be implemented as a multithreaded filter.  Therefore,
   * this implementation provides a ThreadedGenerateData() routine which
   * is called for each processing thread. The output image data is allocated
   * automatically by the superclass prior to calling ThreadedGenerateData().
   * ThreadedGenerateData can only write to the portion of the output image
   * specified by the parameter "outputRegionForThread"
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData() */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            int threadId );
  

private:
  LogPolarResampleImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  OutputImagePointer      m_OutputImage;

  InterpolatorPointer     m_Interpolator;        // Image function for interpolation
  OutputPixelType         m_DefaultPixelValue;   // default pixel value if the point 
                                                 // is outside the image
  bool			  m_IsOriginAtCenter;    // true if input image origin is at center of the image
  double		  m_AngularStep;
  double		  m_RadialStep;
  double		  m_RadialNumberOfSamples;
  double		  m_AngularNumberOfSamples;
  double 		  m_Sigma;
};

  
} // end namespace otb
  
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLogPolarResampleImageFilter.txx"
#endif
  
#endif
