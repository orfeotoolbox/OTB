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
#include "otbPolarimetricSynthesisFunctor.h"
#include "otbPolarimetricData.h"
#include "itkFixedArray.h"
#include <complex>

namespace otb
{

/** \class otbPolarimetricSynthesisFilter
 * \brief 
 *
 * This class compute the polarimetric synthesis from a radar vector image,
 * depening on the polarimetric architecture :
 * \begin{enumerate}
 *    \item HH_HV : two channels are available: $S_{HH}$ and $S_{HV}$. 
 *                  Emit polarisation is fixed to horizontal orientation: $\psi_{i}=0$ and $\chi_{i}=0$.
 *    \item VV_VH : two channels are available: $S_{VV}$ and $S_{VH}$.
 *                  Emit polarisation is fixed to vertical orientation: $\psi_{i}=90°$ and $\chi_{i}=0$.
 *    \item HH_HV_VV : three channels are available: $S_{HH}$, $S_{HV}$ and $S_{VV}$. 
 *                     we make the assumption that cross polarisation are reciprocal ($S_{HV} =  S_{VH}$).
 *    \item HH_HV_VH_VV: four channels are available $S_{HH}$, $S_{HV}$, $S_{VH}$ and $S_{VV}$.
 * \end{enumerate}
 * 
 * \begin{enumerate}
 *    \item emissionH : if two images are contained into the vector, emissionH enables to determine that
 *                      the type of architecture is HH_HV.
 *    \item emissionV : In the same way, with onlyt two images emissionV enables to determine that the type
 *                      of architecture is VH_VV. 
 * \end{enumerate}
 *
 * To resolve the synthesis, four parameters are required: $\psi_{i}$ , $\chi_{i}$, $\psi_{r}$ and $\chi_{r}$. 
 * These parameters depend on the polarimetric architecture describe below.
 * 
 * The result of the synthesis is a scalar image. Three modes are available:
 * \begin{enumerate}
 *     \item none: set the four parameters;
 *     \item co: $\psi_{r} = \psi_{i}$ and $\chi_{r} = \chi_{i}$
 *     \item cross: $\psi_{r} = \psi_{i} + 90°$ and $\chi_{r} = -\chi_{i}$ 
 * \end{enumerate}
 *
 * This class is parameterized over the type of the input images and
 * the type of the output image.  It is also parameterized by the
 * operation to be applied, using a Functor style.  
 * 
 */

template <class TInputImage, class TOutputImage,
        class TFunction = Functor::PolarimetricSynthesisFunctor<        
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
  typedef std::complex <double>                   InputPixelType;  
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
  typedef typename     itk::FixedArray<int,4>         IndexArrayType;

  
    
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
  /** Set the Incident ElectroMagneticField */ 
  void SetEi(ComplexArrayType ei)
  {
       m_Ei = ei;
       this->GetFunctor().SetEi(ei);
       this->Modified();
  }
  /** Set the Reflected ElectroMagneticField */
  void SetEr(ComplexArrayType er)
  {
       m_Er = er;
       this->GetFunctor().SetEr(er);
       this->Modified();
  }
  
  /** Set/Get PsiI */
  itkSetMacro(PsiI,double);
  itkGetMacro(PsiI,double);
  /** Set/Get KhiI */
  itkSetMacro(KhiI,double);
  itkGetMacro(KhiI,double);
  /** Set/Get PsiR */
  itkSetMacro(PsiR,double);
  itkGetMacro(PsiR,double);
  /** Set/Get KhiR */
  itkSetMacro(KhiR,double);
  itkGetMacro(KhiR,double);
  /** Set/Get EmissionH */  
  itkSetMacro(EmissionH,bool);
  itkGetMacro(EmissionH,bool);
  /** Set/Get EmissionV */    
  itkSetMacro(EmissionV,bool);
  itkGetMacro(EmissionV,bool);
  /** Set/Get Mode */    
  itkSetMacro(Mode,int);
  itkGetMacro(Mode,int);
  /** Set the gain */
  itkSetMacro(Gain,double);
  itkGetMacro(Gain,double);  
  /** Force the copolar mode */
  void ForceCoPolar();  
  /** Force the crosspolar mode */
  void ForceCrossPolar();  


protected:
  /** Constructor */
  MultiChannelsPolarimetricSynthesisFilter();
  /** Destructor */
  virtual ~MultiChannelsPolarimetricSynthesisFilter(){};

  /** MultiChannelsPolarimetricSynthesisFilter can produce an image  
   * which is a synthesis of channels HH, HV, VH and VV.
   *
   * As such, MultiChannelsPolarimetricSynthesisFilter
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
  
  void Print();    

private :
  MultiChannelsPolarimetricSynthesisFilter(const Self&); //purposely not implemented
  
  /** Psi Incident */
  double m_PsiI;
  /** Khi Incident */
  double m_KhiI;
  /** Psi Reflected */
  double m_PsiR;
  /** Khi Reflected */
  double m_KhiR;
  
  /** Gain */
  double m_Gain;
  
  /** None = 0 , copolar = 1 , crosspolar = 2 */
  int m_Mode;
  
  /** Champs Electromagnetic Incident */
  ComplexArrayType m_Ei;
  /** Champs Electromagnetic Reflechi */
  ComplexArrayType m_Er;  
   
  /** Architecture Type */
  PolarimetricData::Pointer m_ArchitectureType;

  FunctorType m_Functor;
  
  /** Emission mode */
  bool m_EmissionH;
  bool m_EmissionV;
   
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMultiChannelsPolarimetricSynthesisFilter.txx"
#endif

#endif
