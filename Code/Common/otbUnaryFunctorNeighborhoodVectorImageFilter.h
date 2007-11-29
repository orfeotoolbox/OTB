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
#ifndef __otbUnaryFunctorNeighborhoodVectorImageFilter_h
#define __otbUnaryFunctorNeighborhoodVectorImageFilter_h

#include "itkInPlaceImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkProcessObject.h"

namespace otb {
/** \class UnaryFunctorNeighborhoodVectorImageFilter
 * \brief Implements neighborhood-wise generic operation of image beeing vector images.
 *
 * This class is parameterized over the input image type
 * and the type of the output image.  It is also parameterized by the
 * operation to be applied.  A Functor style is used.
 * 
 * \ingroup IntensityImageFilters   Multithreaded
 */	
template <class TInputImage, class TOutputImage, class TFunction >
class ITK_EXPORT UnaryFunctorNeighborhoodVectorImageFilter 
	: public itk::InPlaceImageFilter<TInputImage,TOutputImage> 
{
public:
  /** Standard class typedefs. */
  typedef UnaryFunctorNeighborhoodVectorImageFilter           Self;
  typedef itk::InPlaceImageFilter<TInputImage,TOutputImage >  Superclass;
  typedef itk::SmartPointer<Self>                             Pointer;
  typedef itk::SmartPointer<const Self>                       ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(UnaryFunctorNeighborhoodVectorImageFilter,InPlaceImageFilter);


  /** Some convenient typedefs. */
  typedef TFunction                             FunctorType;
  typedef typename Superclass::InputImageType   InputImageType;
  typedef typename InputImageType::ConstPointer InputImagePointer;
  typedef typename InputImageType::RegionType   InputImageRegionType; 
  typedef typename InputImageType::PixelType    InputImagePixelType;
  typedef typename InputImageType::SizeType     InputImageSizeType;
  typedef typename Superclass::OutputImageType  OutputImageType;
  typedef typename OutputImageType::Pointer     OutputImagePointer;
  typedef typename OutputImageType::RegionType  OutputImageRegionType;
  typedef typename OutputImageType::PixelType   OutputImagePixelType;

  typedef itk::ProcessObject ProcessObjectType;

  /**Set/Get the radius of neighborhood.*/
  itkSetMacro(Radius,unsigned int);
  itkGetMacro(Radius,unsigned int);

  /** Get the functor object.  The functor is returned by reference.
   * (Functors do not have to derive from itk::LightObject, so they do
   * not necessarily have a reference count. So we cannot return a
   * SmartPointer.) */
  FunctorType& GetFunctor()
  {
        this->Modified();
        return m_Functor;
  }

  /** Get the functor object.  The functor is returned by reference.
   * (Functors do not have to derive from itk::LightObject, so they do
   * not necessarily have a reference count. So we cannot return a
   * SmartPointer.) */
  const FunctorType& GetFunctor() const { return m_Functor; };

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


  
  typedef itk::ConstNeighborhoodIterator<TInputImage>     NeighborhoodIteratorType; 
  typedef typename NeighborhoodIteratorType::RadiusType   RadiusType;
  typedef unsigned char RadiusSizeType;

protected:
  UnaryFunctorNeighborhoodVectorImageFilter();
  virtual ~UnaryFunctorNeighborhoodVectorImageFilter() {};

  /** UnaryFunctorNeighborhoodVectorImageFilter can be implemented as a multithreaded filter.
   * Therefore, this implementation provides a ThreadedGenerateData() routine
   * which is called for each processing thread. The output image data is
   * allocated automatically by the superclass prior to calling
   * ThreadedGenerateData().  ThreadedGenerateData can only write to the
   * portion of the output image specified by the parameter
   * "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData()  */
  virtual void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, int threadId );

  
private:
  UnaryFunctorNeighborhoodVectorImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  unsigned int m_Radius;
  FunctorType m_Functor;
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbUnaryFunctorNeighborhoodVectorImageFilter.txx"
#endif



#endif
