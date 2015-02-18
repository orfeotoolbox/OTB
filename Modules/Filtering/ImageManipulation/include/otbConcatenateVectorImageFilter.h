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
#ifndef __otbConcatenateVectorImageFilter_h
#define __otbConcatenateVectorImageFilter_h

#include "itkImageToImageFilter.h"
#include "otbVectorImage.h"

namespace otb
{
/** \class ConcatenateVectorImageFilter
 * \brief This filter concatenates the vector pixel of the first
 * image with the vector pixel of the second image.
 */
template<class TInputImage1, class TInputImage2, class TOutputImage>
class ITK_EXPORT ConcatenateVectorImageFilter
  : public itk::ImageToImageFilter<TInputImage1, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef ConcatenateVectorImageFilter                        Self;
  typedef itk::ImageToImageFilter<TInputImage1, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                             Pointer;
  typedef itk::SmartPointer<const Self>                       ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorImageToImagePixelAccessor, ImageToImageFilter);

  /** Template related typedefs */
  typedef TInputImage1 InputImage1Type;
  typedef TInputImage2 InputImage2Type;
  typedef TOutputImage OutputImageType;

  typedef typename InputImage1Type::Pointer InputImage1PointerType;
  typedef typename InputImage2Type::Pointer InputImage2PointerType;
  typedef typename OutputImageType::Pointer OutputImagePointerType;

  typedef typename OutputImageType::PixelType         OutputPixelType;
  typedef typename OutputImageType::InternalPixelType OutputInternalPixelType;
  typedef typename OutputImageType::RegionType        OutputImageRegionType;

  /**
   * Set The first input image.
   * \param image The first input image.
   */
  void SetInput1(const TInputImage1 * image);
  /**
   * Get the first input image.
   * \return The first input image.
   */
  InputImage1Type * GetInput1(void);
  /**
   * Set The second input image.
   * \param image The second input image.
   */
  void SetInput2(const TInputImage2 * image);
  /**
   * Get the second input image.
   * \return The second input image.
   */
  InputImage2Type * GetInput2(void);

  /** ImageDimension constant */
  itkStaticConstMacro(InputImage1Dimension, unsigned int, TInputImage1::ImageDimension);
  itkStaticConstMacro(InputImage2Dimension, unsigned int, TInputImage2::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

protected:
  /** Constructor. */
  ConcatenateVectorImageFilter();
  /** Destructor. */
  virtual ~ConcatenateVectorImageFilter();
  virtual void GenerateOutputInformation();
  virtual void BeforeThreadedGenerateData();
  /** Main computation method. */
  virtual void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ConcatenateVectorImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};
} // end namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbConcatenateVectorImageFilter.txx"
#endif
#endif
