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
#ifndef __otbMultiChannelsPolarimetricSynthesisFilter_h
#define __otbMultiChannelsPolarimetricSynthesisFilter_h

#include "itkInPlaceImageFilter.h"
#include "otbPolarimetricFunctor.h"
#include "itkFixedArray.h"
#include <complex>

namespace otb
{

/** \class MultiChannelsPolarimetricSynthesisFilter
 * \brief Implements 
 *
 * This class is parameterized over the type of the input image and
 * the type of the output image.  It is also parameterized by the
 * operation to be applied, using a Functor style.
 *
 */


template <class TInputImage, class TOutputImage,
        class TFunction = Functor::PolarimetricFunctor<        
                                                typename TInputImage::InternalPixelType,
                                                typename TInputImage::InternalPixelType,
                                                typename TInputImage::InternalPixelType,
                                                typename TInputImage::InternalPixelType,                                                                                                                                                
                                                typename TOutputImage::PixelType>  >
class ITK_EXPORT MultiChannelsPolarimetricSynthesisFilter : public itk::InPlaceImageFilter<TInputImage,TOutputImage> 
{
public:
  /** Standard class typedefs. */
  typedef MultiChannelsPolarimetricSynthesisFilter Self;
  typedef itk::InPlaceImageFilter<TInputImage,TOutputImage>  Superclass;
  typedef itk::SmartPointer<Self>   Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(MultiChannelsPolarimetricSynthesisFilter, InPlaceImageFilter);  
  
  /** Some typedefs. */
  typedef TFunction    FunctorType;
  typedef TInputImage  InputImageType;
  typedef typename     InputImageType::ConstPointer   InputImagePointer;
  typedef typename     InputImageType::RegionType     InputImageRegionType; 
  typedef typename     InputImageType::PixelType      InputImagePixelType; 
  typedef TOutputImage OutputImageType;
  typedef typename     OutputImageType::Pointer       OutputImagePointer;
  typedef typename     OutputImageType::RegionType    OutputImageRegionType;
  typedef typename     OutputImageType::PixelType     OutputImagePixelType;
  typedef typename     std::complex <double>          ComplexType;
  typedef typename     itk::FixedArray<ComplexType,2> ComplexArrayType;  
    
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
  
  void SetEi(ComplexArrayType ei)
  {
       m_Ei = ei;
       this->GetFunctor().SetEi(ei);
       this->Modified();
  }
  
  void SetEr(ComplexArrayType er)
  {
       m_Er = er;
       this->GetFunctor().SetEr(er);
       this->Modified();
  }
  
  /** Set/Get PsiI */
  itkSetMacro(PsiI,double);
  itkGetMacro(PsiI,double);
  /** Set/Get TauI */
  itkSetMacro(TauI,double);
  itkGetMacro(TauI,double);
  /** Set/Get PsiR */
  itkSetMacro(PsiR,double);
  itkGetMacro(PsiR,double);
  /** Set/Get TauR */
  itkSetMacro(TauR,double);
  itkGetMacro(TauR,double);


protected:
  MultiChannelsPolarimetricSynthesisFilter();
  virtual ~MultiChannelsPolarimetricSynthesisFilter(){};

  /** MultiChannelsPolarimetricSynthesisFilter can produce an image which is a different
   * resolution than its input image.  As such, MultiChannelsPolarimetricSynthesisFilter
   * needs to provide an implementation for
   * GenerateOutputInformation() in order to inform the pipeline
   * execution model.  The original documentation of this method is
   * below.
   *
   * \sa ProcessObject::GenerateOutputInformaton()  */
  virtual void GenerateOutputInformation();
  
  virtual void BeforeThreadedGenerateData(); 
  
  /** MultiChannelsPolarimetricSynthesisFilter can be implemented as a multithreaded filter.
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
  
  /** Computation of the electromagnetic fields Ei Er */ 
  void ComputeElectromagneticFields();  
  
  /** Verify and force the inputs, if only  2 or 3 channels are present */
  void VerifyAndForceInputs();  

  void PrintSelf(std::ostream& os, itk::Indent indent) const;  

private :
  MultiChannelsPolarimetricSynthesisFilter(const Self&); //purposely not implemented
  
  /** Psi Incident */
  double m_PsiI;
  /** Tau Incident */
  double m_TauI;
  /** Psi Relechi */
  double m_PsiR;
  /** Tau Relechi */
  double m_TauR;

  /** Champs Electromagnetic Incident */
  ComplexArrayType m_Ei;
  /** Champs Electromagnetic Reflechi */
  ComplexArrayType m_Er;  
  
  /** Conversion coefficient Degre To Radian */
  static const double m_DTOR=M_PI/180;
  
  /** Channel indexes 0 1 2 3 on verra */

  FunctorType m_Functor;
  
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMultiChannelsPolarimetricSynthesisFilter.txx"
#endif

#endif
