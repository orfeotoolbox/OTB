/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) CS Systemes d'information. All rights reserved.
  See CSCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbThresholdVectorImageFilter_h
#define __otbThresholdVectorImageFilter_h

#include "itkImageToImageFilter.h"

namespace otb
{

/** \class ThresholdVectorImageFilter
 * \brief Set image values to a user-specified value if they are below,
 * above, or between simple threshold values.
 *
 * ThresholdVectorImageFilter sets image values to a user-specified "outside"
 * value (by default, "black") if the image values are below, above, or
 * between simple threshold values.
 *
 * The pixels must support the operators >= and <=.
 *
 * \ingroup IntensityImageFilters Multithreaded
 */
  template <class TInputImage, class TOutputImage=TInputImage>
  class ITK_EXPORT ThresholdVectorImageFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef ThresholdVectorImageFilter               Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage>  Superclass;
  typedef itk::SmartPointer<Self>                 Pointer;
  typedef itk::SmartPointer<const Self>           ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ThresholdVectorImageFilter, itk::ImageToImageFilter);


  /** Some additional typedefs.  */
  typedef TInputImage                                InputImageType;
  typedef typename InputImageType::ConstPointer InputImagePointer;
  typedef typename InputImageType::RegionType   InputImageRegionType;
  typedef typename InputImageType::PixelType    InputImagePixelType;
  typedef typename InputImageType::InternalPixelType    InputImageInternalPixelType;

  /** Some additional typedefs.  */
  typedef TOutputImage                             OutputImageType;
  typedef typename OutputImageType::Pointer     OutputImagePointer;
  typedef typename OutputImageType::RegionType  OutputImageRegionType;
  typedef typename OutputImageType::PixelType   OutputImagePixelType;
  typedef typename OutputImageType::InternalPixelType   OutputImageInternalPixelType;

  /** Set the "outside" pixel value. The default value
   * NumericTraits<PixelType>::Zero. */
  itkSetMacro(OutsideValue, OutputImageInternalPixelType);
  
  /** Get the "outside" pixel value. */
  itkGetConstMacro(OutsideValue, OutputImageInternalPixelType);
                 
  /** The values greater than or equal to the value are set to OutsideValue. */
  void ThresholdAbove(const InputImageInternalPixelType &thresh);
  
  /** The values less than or equal to the value are set to OutsideValue. */
  void ThresholdBelow(const InputImageInternalPixelType &thresh);

  /** The values outside the range are set to OutsideValue. */
  void ThresholdOutside(const InputImageInternalPixelType &lower, const InputImageInternalPixelType &upper);
  
  /** Set/Get methods to set the lower threshold */
  itkSetMacro(Lower, InputImageInternalPixelType);
  itkGetConstMacro(Lower, InputImageInternalPixelType);

  /** Set/Get methods to set the upper threshold */
  itkSetMacro(Upper, InputImageInternalPixelType);
  itkGetConstMacro(Upper, InputImageInternalPixelType);


protected:
  ThresholdVectorImageFilter();
  virtual ~ThresholdVectorImageFilter() {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** ThresholdVectorImageFilter can be implemented as a multithreaded filter.
   * Therefore, this implementation provides a ThreadedGenerateData() routine
   * which is called for each processing thread. The output image data is
   * allocated automatically by the superclass prior to calling
   * ThreadedGenerateData().  ThreadedGenerateData can only write to the
   * portion of the output image specified by the parameter
   * "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData()  */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            int threadId );

  void GenerateOutputInformation(void)
  {
    Superclass::GenerateOutputInformation();
    
    this->GetOutput()->SetNumberOfComponentsPerPixel( this->GetInput()->GetNumberOfComponentsPerPixel() );
  }

private:
  ThresholdVectorImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  OutputImageInternalPixelType m_OutsideValue;
  InputImageInternalPixelType m_Lower;
  InputImageInternalPixelType m_Upper;
};

  
} // end namespace otb
  
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbThresholdVectorImageFilter.txx"
#endif
  
#endif
