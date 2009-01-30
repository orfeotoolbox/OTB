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
#ifndef __otbMultiChannelRAndBAndNIRVegetationIndexImageFilter_h
#define __otbMultiChannelRAndBAndNIRVegetationIndexImageFilter_h

#include "itkInPlaceImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "otbVegetationIndex.h"

namespace otb
{

/** \class MultiChannelRAndBAndNIRVegetationIndexImageFilter
 * \brief Implements mutli channel R and B and NIR vegetation index operation, pixel-wise generic vegetation index operation on one vector image.
 *
 * This class is parameterized over the type of the input image and
 * the type of the output image.  It is also parameterized by the
 * operation to be applied, using a Functor style.
 *
 */
template <class TInputImage, class TOutputImage,
class TFunction = Functor::ARVI<        typename TInputImage::InternalPixelType,
typename TInputImage::InternalPixelType,
typename TInputImage::InternalPixelType,
typename TOutputImage::PixelType>  >
class ITK_EXPORT MultiChannelRAndBAndNIRVegetationIndexImageFilter : public itk::InPlaceImageFilter<TInputImage,TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef MultiChannelRAndBAndNIRVegetationIndexImageFilter  Self;
  typedef itk::InPlaceImageFilter<TInputImage,TOutputImage>  Superclass;
  typedef itk::SmartPointer<Self>   Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MultiChannelRAndBAndNIRVegetationIndexImageFilter, InPlaceImageFilter);

  /** Some typedefs. */
  typedef TFunction   FunctorType;
  typedef TInputImage InputImageType;
  typedef typename    InputImageType::ConstPointer    InputImagePointer;
  typedef typename    InputImageType::RegionType InputImageRegionType;
  typedef typename    InputImageType::PixelType  InputImagePixelType;
  typedef TOutputImage OutputImageType;
  typedef typename     OutputImageType::Pointer    OutputImagePointer;
  typedef typename     OutputImageType::RegionType OutputImageRegionType;
  typedef typename     OutputImageType::PixelType  OutputImagePixelType;

  /** Get the functor object.  The functor is returned by reference.
   * (Functors do not have to derive from itk::LightObject, so they do
   * not necessarily have a reference count. So we cannot return a
   * SmartPointer.) */
  FunctorType& GetFunctor()
  {
    return m_Functor;
  };
  const FunctorType& GetFunctor() const
  {
    return m_Functor;
  };

  /** Set the functor object.  This replaces the current Functor with a
   * copy of the specified Functor. This allows the user to specify a
   * functor that has ivars set differently than the default functor.
   * This method requires an operator!=() be defined on the functor
   * (or the compiler's default implementation of operator!=() being
   * appropriate). */
  void SetFunctor(const FunctorType& functor)
  {
    if (m_Functor != functor)
    {
      m_Functor = functor;
      this->Modified();
    }
  }

  /** Set/Get the red channel index. Value must be in [1...[ */
  itkSetMacro(RedIndex,unsigned int);
  itkGetMacro(RedIndex,unsigned int);
  /** Set/Get the blue channel index. Value must be in [1...[ */
  itkSetMacro(BlueIndex,unsigned int);
  itkGetMacro(BlueIndex,unsigned int);
  /** Set/Get the nir channel index. Value must be in [1...[ */
  itkSetMacro(NIRIndex,unsigned int);
  itkGetMacro(NIRIndex,unsigned int);

protected:
  MultiChannelRAndBAndNIRVegetationIndexImageFilter();
  virtual ~MultiChannelRAndBAndNIRVegetationIndexImageFilter() {};

  /** MultiChannelRAndBAndNIRVegetationIndexImageFilter can produce an image which is a different
   * resolution than its input image.  As such, MultiChannelRAndBAndNIRVegetationIndexImageFilter
   * needs to provide an implementation for
   * GenerateOutputInformation() in order to inform the pipeline
   * execution model.  The original documentation of this method is
   * below.
   *
   * \sa ProcessObject::GenerateOutputInformaton()  */
  virtual void GenerateOutputInformation();

  /** MultiChannelRAndBAndNIRVegetationIndexImageFilter can be implemented as a multithreaded filter.
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

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  MultiChannelRAndBAndNIRVegetationIndexImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Red channel index */
  unsigned int m_RedIndex;
  /** Blue channel index */
  unsigned int m_BlueIndex;
  /** NIR channel index */
  unsigned int m_NIRIndex;
  FunctorType m_Functor;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMultiChannelRAndBAndNIRVegetationIndexImageFilter.txx"
#endif

#endif
