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
#ifndef __otbBinaryFunctorNeighborhoodImageFilter_h
#define __otbBinaryFunctorNeighborhoodImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkSize.h"

namespace otb
{

/** \class BinaryFunctorNeighborhoodImageFilter
 * \brief Implements neighborhood-wise generic operation of two images.
 *
 * This class is parameterized over the types of the two input images
 * and the type of the output image.  It is also parameterized by the
 * operation to be applied.  A Functor style is used.
 *
 * \ingroup IntensityImageFilters   Multithreaded
 *
 * \example ChangeDetection/ChangeDetectionFrameworkExample.cxx
 *
 */
template <class TInputImage1, class TInputImage2,
    class TOutputImage, class TFunction>
class ITK_EXPORT BinaryFunctorNeighborhoodImageFilter : public itk::ImageToImageFilter<TInputImage1, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef BinaryFunctorNeighborhoodImageFilter                Self;
  typedef itk::ImageToImageFilter<TInputImage1, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                             Pointer;
  typedef itk::SmartPointer<const Self>                       ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(BinaryFunctorNeighborhoodImageFilter, BinaryFunctorImageFilter);

  /** Some convenient typedefs. */
  typedef TFunction                              FunctorType;
  typedef TInputImage1                           Input1ImageType;
  typedef typename Input1ImageType::ConstPointer Input1ImageConstPointer;
  typedef typename Input1ImageType::Pointer      Input1ImagePointer;
  typedef typename Input1ImageType::RegionType   Input1ImageRegionType;
  typedef typename Input1ImageType::PixelType    Input1ImagePixelType;
  typedef TInputImage2                           Input2ImageType;
  typedef typename Input2ImageType::ConstPointer Input2ImageConstPointer;
  typedef typename Input2ImageType::Pointer      Input2ImagePointer;
  typedef typename Input2ImageType::RegionType   Input2ImageRegionType;
  typedef typename Input2ImageType::PixelType    Input2ImagePixelType;
  typedef TOutputImage                           OutputImageType;
  typedef typename OutputImageType::Pointer      OutputImagePointer;
  typedef typename OutputImageType::RegionType   OutputImageRegionType;
  typedef typename OutputImageType::PixelType    OutputImagePixelType;

  typedef itk::ProcessObject ProcessObjectType;

  /** Connect one of the operands for pixel-wise addition */
  void SetInput1(const TInputImage1 * image1);

  /** Connect one of the operands for pixel-wise addition */
  void SetInput2(const TInputImage2 * image2);

  /** Get the inputs */
  const TInputImage1 * GetInput1();
  const TInputImage2 * GetInput2();

  /** Get the functor object.  The functor is returned by reference.
   * (Functors do not have to derive from itk::LightObject, so they do
   * not necessarily have a reference count. So we cannot return a
   * SmartPointer.) */
  FunctorType& GetFunctor()
  {
    return m_Functor;
  }

  /** Set the functor object.  This replaces the current Functor with a
   * copy of the specified Functor. This allows the user to specify a
   * functor that has ivars set differently than the default functor.
   * This method requires an operator!=() be defined on the functor
   * (or the compiler's default implementation of operator!=() being
   * appropriate). */
  void SetFunctor(const FunctorType& functor)
  {
    m_Functor = functor;
    this->Modified();
  }

  typedef itk::ConstNeighborhoodIterator<TInputImage1>
  NeighborhoodIteratorType1;
  typedef itk::ConstNeighborhoodIterator<TInputImage2>
  NeighborhoodIteratorType2;

  typedef typename NeighborhoodIteratorType1::RadiusType RadiusType1;
  typedef typename NeighborhoodIteratorType2::RadiusType RadiusType2;

  typedef typename itk::Size<Input1ImageType::ImageDimension> RadiusSizeType;

  itkSetMacro(Radius, RadiusSizeType);

  /** Set unsigned int radius */
  void SetRadius(unsigned int radius)
  {
    m_Radius.Fill(radius);
  }

protected:
  BinaryFunctorNeighborhoodImageFilter();
  virtual ~BinaryFunctorNeighborhoodImageFilter() {}

  /** BinaryFunctorNeighborhoodImageFilter can be implemented as a multithreaded filter.
   * Therefore, this implementation provides a ThreadedGenerateData() routine
   * which is called for each processing thread. The output image data is
   * allocated automatically by the superclass prior to calling
   * ThreadedGenerateData().  ThreadedGenerateData can only write to the
   * portion of the output image specified by the parameter
   * "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData()  */
  virtual void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                                    itk::ThreadIdType threadId);

  /**
   * Pad the inputs requested regions by radius
   */
  virtual void GenerateInputRequestedRegion(void);

  RadiusSizeType m_Radius;

private:
  BinaryFunctorNeighborhoodImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  FunctorType m_Functor;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbBinaryFunctorNeighborhoodImageFilter.txx"
#endif

#endif
