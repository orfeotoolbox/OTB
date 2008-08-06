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
#ifndef __otbPolarimetricSynthesisFilter_h
#define __otbPolarimetricSynthesisFilter_h

#include "itkInPlaceImageFilter.h"
#include "otbPolarimetricFunctor.h"

namespace otb
{

/** \class PolarimetricSynthesisFilter
 * \brief Implements 
 *
 * This class is parameterized over the type of the input image and
 * the type of the output image.  It is also parameterized by the
 * operation to be applied, using a Functor style.
 *
 */


template <class TInputImage, class TOutputImage,
        class TFunction = Functor::PolarimetricFunctor2Channels<        
                                                typename TInputImage::InternalPixelType,
                                                typename TInputImage::InternalPixelType,
                                                typename TOutputImage::PixelType>  >
class ITK_EXPORT PolarimetricSynthesisFilter : public itk::InPlaceImageFilter<TInputImage,TOutputImage> 
{
public:
  /** Standard class typedefs. */
  typedef PolarimetricSynthesisFilter Self;
  typedef itk::InPlaceImageFilter<TInputImage,TOutputImage>  Superclass;
  typedef itk::SmartPointer<Self>   Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;  
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(PolarimetricSynthesisFilter, InPlaceImageFilter);  
  
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
  FunctorType& GetFunctor() { return m_Functor; };
  const FunctorType& GetFunctor() const { return m_Functor; };  
  

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

  /** Set/Get the Phi */
  itkSetMacro(PhiE,float);
  itkGetMacro(PhiE,float);
  /** Set/Get  */
  itkSetMacro(KhiE,float);
  itkGetMacro(KhiE,float);
  /** Set/Get  */
  itkSetMacro(PhiR,float);
  itkGetMacro(PhiR,float);
  /** Set/Get  */
  itkSetMacro(KhiR,float);
  itkGetMacro(KhiR,float);

protected:
  PolarimetricSynthesisFilter();
  virtual ~ PolarimetricSynthesisFilter();
  
  /** PolarimetricSynthesisFilter
   *
   *
   * \sa ProcessObject::GenerateOutputInformaton()  */
  virtual void GenerateOutputInformation();
  
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            int threadId );

  void PrintSelf(std::ostream& os, itk::Indent indent) const;  

private :
  PolarimetricSynthesisFilter(const Self&); //purposely not implemented
  //void operator=(const Self&); //purposely not implemented
  
  /** Phi Emission */
  float m_PhiE;
  /** Khi Emission */
  float m_KhiE;
  /** Phi Reception */
  float m_PhiR;
  /** Khi Reception */
  float m_KhiR;
  /** Channel indexes 0 1 2 3 on verra */
  //unsigned int m_RedIndex;  
  FunctorType m_Functor;
  
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPolarimetricSynthesisFilter.txx"
#endif

#endif
