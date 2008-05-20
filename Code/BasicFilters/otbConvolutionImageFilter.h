/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkConvolutionImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2006/03/29 14:53:40 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbConvolutionImageFilter_h
#define __otbConvolutionImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkNumericTraits.h"
#include "itkArray.h"

namespace otb
{
/** \class ConvolutionImageFilter
 * \brief Applies a convolution filter to an image
 *
 * Computes an image which is the convolution of the input image
 * with a filter.
 * 
 * The radius of the input filter is provided by the \code SetInput() \endcode 
 * method and the filters coefficients are given by an itk::Array passed to the 
 * \code SetFilter() \endcode method.
 *
 * \sa Image
 * \sa Neighborhood
 * \sa NeighborhoodOperator
 * \sa NeighborhoodIterator
 * 
 * \ingroup IntensityImageFilters
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT ConvolutionImageFilter :
    public itk::ImageToImageFilter< TInputImage, TOutputImage >
{
public:
  /** Extract dimension from input and output image. */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  /** Convenient typedefs for simplifying declarations. */
  typedef TInputImage InputImageType;
  typedef TOutputImage OutputImageType;

  /** Standard class typedefs. */
  typedef ConvolutionImageFilter Self;
  typedef itk::ImageToImageFilter< InputImageType, OutputImageType> Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ConvolutionImageFilter, ImageToImageFilter);
  
  /** Image typedef support. */
  typedef typename InputImageType::PixelType InputPixelType;
  typedef typename OutputImageType::PixelType OutputPixelType;
  typedef typename itk::NumericTraits<InputPixelType>::RealType InputRealType;
  
  typedef typename InputImageType::RegionType InputImageRegionType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;

  typedef typename InputImageType::SizeType InputSizeType;

  typedef typename itk::Array<InputRealType> ArrayType;
  
  /** Set the radius of the neighborhood used to compute the mean. */
  virtual void SetRadius (const InputSizeType rad) 
  { 
      itkDebugMacro("setting radius to " << rad); 
          if (this->m_Radius != rad) 
          { 
              this->m_Radius = rad;
              unsigned int arraySize=1;
              for (unsigned int i=0; i<m_Radius.GetSizeDimension(); i++)
              {
                arraySize *= 2*this->m_Radius[i]+1;
              }
              this->m_Filter.SetSize(arraySize);
              this->m_Filter.Fill(1);
              this->Modified(); 
          } 
  }

  /** Get the radius of the neighborhood used to compute the mean */
  itkGetConstReferenceMacro(Radius, InputSizeType);
  
  /** Set the input filter */
  itkSetMacro(Filter, ArrayType);
  itkGetConstReferenceMacro(Filter, ArrayType);
  
  /** ConvolutionImageFilter needs a larger input requested region than
   * the output requested region.  As such, ConvolutionImageFilter needs
   * to provide an implementation for GenerateInputRequestedRegion()
   * in order to inform the pipeline execution model.
   *
   * \sa ImageToImageFilter::GenerateInputRequestedRegion() */
  virtual void GenerateInputRequestedRegion() throw(itk::InvalidRequestedRegionError);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputHasNumericTraitsCheck,
                  (itk::Concept::HasNumericTraits<InputPixelType>));
  /** End concept checking */
#endif

protected:
  ConvolutionImageFilter();
  virtual ~ConvolutionImageFilter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** ConvolutionImageFilter can be implemented as a multithreaded filter.
   * Therefore, this implementation provides a ThreadedGenerateData()
   * routine which is called for each processing thread. The output
   * image data is allocated automatically by the superclass prior to
   * calling ThreadedGenerateData().  ThreadedGenerateData can only
   * write to the portion of the output image specified by the
   * parameter "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData() */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            int threadId );

private:
  ConvolutionImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  InputSizeType m_Radius;
  ArrayType m_Filter;
  
};
  
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "otbConvolutionImageFilter.txx"
#endif

#endif
