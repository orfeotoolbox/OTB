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

#include "otbQuaternaryFunctorImageFilter.h"
#include "otbPolarimetricFunctor.h"
#include "itkFixedArray.h"
#include <complex>

namespace otb
{

/** \class otbPolarimetricSynthesisFilter
 * \brief 
 * 
 */

template <class TInputImageHH,class TInputImageHV,class TInputImageVH,class TInputImageVV,class TOutputImage, 
          class TFunction = Functor::PolarimetricFunctor<              
                                                        typename TInputImageHH::PixelType,
                                                        typename TInputImageHV::PixelType,
                                                        typename TInputImageVH::PixelType,
                                                        typename TInputImageVV::PixelType,                                                        
                                                        typename TOutputImage::PixelType > >
class ITK_EXPORT PolarimetricSynthesisFilter :  public otb::QuaternaryFunctorImageFilter< TInputImageHH,
                        TInputImageHV, TInputImageVH, TInputImageVV, TOutputImage, TFunction >
{
public:

  /** Standard typedefs */
  typedef PolarimetricSynthesisFilter       Self;
  typedef otb::QuaternaryFunctorImageFilter< TInputImageHH, TInputImageHV, 
                TInputImageVH, TInputImageVV, TOutputImage, TFunction >  Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;
  
  /** Type macro */
  itkNewMacro(Self);
  
  /** Creation through object factory macro */
  itkTypeMacro(PolarimetricSynthesisFilter,QuaternaryFunctorImageFilter);
  
  /** Template parameters typedefs */
  typedef std::complex <double>                   InputPixelType;
  typedef otb::Image< InputPixelType,  2 >        InputImageType;  
  typedef typename Superclass::Input1ImageType    HHInputImageType;
  typedef typename Superclass::Input2ImageType    HVInputImageType;
  typedef typename Superclass::Input3ImageType    VHInputImageType;
  typedef typename Superclass::Input4ImageType    VVInputImageType;
  typedef typename Superclass::OutputImageType    OutputImageType;
  typedef typename Superclass::FunctorType        FunctorType;
  typedef typename std::complex <double>          ComplexType;
  typedef typename itk::FixedArray<ComplexType,2> ComplexArrayType;   

  void SetInputHH( const TInputImageHH * image );
  void SetInputHV( const TInputImageHV * image );
  void SetInputVH( const TInputImageVH * image );
  void SetInputVV( const TInputImageVV * image );

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
  /** Set/Get Mode */    
  itkSetMacro(Mode,int);
  itkGetMacro(Mode,int);
  
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
  
  void ForceCoPolar();  
  
  void ForceCrossPolar();
  
protected:
  PolarimetricSynthesisFilter();
  virtual ~PolarimetricSynthesisFilter() {};

  /** PolarimetricSynthesisFilter
   *
   */
  
  virtual void BeforeThreadedGenerateData(); 
  
  /** Computation of the electromagnetic fields Ei Er */ 
  void ComputeElectromagneticFields();

  /** Verify and force the inputs, if only  2 or 3 channels are present */
  void VerifyAndForceInputs();
  
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  void Print();  

private:

  PolarimetricSynthesisFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
  /** Psi Incident */
  double m_PsiI;
  /** Khi Incident */
  double m_KhiI;
  /** Psi Reflected */
  double m_PsiR;
  /** Khi Reflected */
  double m_KhiR;
   
  /** None = 0 , copolar = 1 , crosspolar = 2 */
  int m_Mode;

  /** Champs Electromagnetic Incident */
  ComplexArrayType m_Ei;
  /** Champs Electromagnetic Reflechi */
  ComplexArrayType m_Er;  
   
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPolarimetricSynthesisFilter.txx"
#endif

  
#endif
