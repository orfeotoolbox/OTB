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
#include "otbPolarimetricSynthesisFunctor.h"
#include "otbPolarimetricData.h"
#include "itkFixedArray.h"
#include <complex>

namespace otb
{

/** \class PolarimetricSynthesisFilter
 * \brief
 *
 * This class compute the polarimetric synthesis from two to four radar images,
 * depening on the polarimetric architecture:
 *
 *    - HH_HV : two channels are available: $S_{HH}$ and $S_{HV}$.
 *                  Emit polarisation is fixed to horizontal orientation: $\psi_{i}=0$ and $\chi_{i}=0$.
 *    - VV_VH : two channels are available: $S_{VV}$ and $S_{VH}$.
 *                  Emit polarisation is fixed to vertical orientation: $\psi_{i}=90^\circ$ and $\chi_{i}=0$.
 *    - HH_HV_VV : three channels are available: $S_{HH}$, $S_{HV}$ and $S_{VV}$.
 *                     we make the assumption that cross polarisation are reciprocal ($S_{HV} =  S_{VH}$).
 *    - HH_HV_VH_VV: four channels are available $S_{HH}$, $S_{HV}$, $S_{VH}$ and $S_{VV}$.
 *
 * To resolve the synthesis, four parameters are required: $\psi_{i}$ , $\chi_{i}$, $\psi_{r}$ and $\chi_{r}$.
 * These parameters depend on the polarimetric architecture describe below.
 *
 * The result of the synthesis is a scalar image. Three modes are available:
 *
 *     - none: set the four parameters;
 *     - co: $\psi_{r} = \psi_{i}$ and $\chi_{r} = \chi_{i}$
 *     - cross: $\psi_{r} = \psi_{i} + 90^\circ$ and $\chi_{r} = -\chi_{i}$
 *
 * This class is parameterized over the type of the input images and
 * the type of the output image.  It is also parameterized by the
 * operation to be applied, using a Functor style.
 *
 */

template <class TInputImageHH,class TInputImageHV,class TInputImageVH,class TInputImageVV,class TOutputImage,
class TFunction = Functor::PolarimetricSynthesisFunctor<
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
  typedef typename Superclass::Input1ImagePointer HHInputImagePointer;
  typedef typename Superclass::Input2ImageType    HVInputImageType;
  typedef typename Superclass::Input2ImagePointer HVInputImagePointer;
  typedef typename Superclass::Input3ImageType    VHInputImageType;
  typedef typename Superclass::Input3ImagePointer VHInputImagePointer;
  typedef typename Superclass::Input4ImageType    VVInputImageType;
  typedef typename Superclass::Input4ImagePointer VVInputImagePointer;
  typedef typename Superclass::OutputImageType    OutputImageType;
  typedef typename OutputImageType::Pointer       OutputImagePointer;
  typedef typename OutputImageType::RegionType    OutputImageRegionType;
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
  /** Set the gain */
  itkSetMacro(Gain,double);
  /** Set the ElectroMagneticField Incident */
  void SetEi(ComplexArrayType ei)
  {
    m_Ei = ei;
    this->GetFunctor().SetEi(ei);
    this->Modified();
  }
  /** Set the ElectroMagneticField Reflected */
  void SetEr(ComplexArrayType er)
  {
    m_Er = er;
    this->GetFunctor().SetEr(er);
    this->Modified();
  }
  /** Force the copolar mode */
  void ForceCoPolar();
  /** Force the crosspolar mode */
  void ForceCrossPolar();

protected:
  /**  Constructor */
  PolarimetricSynthesisFilter();
  /**  Destructor */
  virtual ~PolarimetricSynthesisFilter() {};

  virtual void GenerateOutputInformation();

  virtual void BeforeThreadedGenerateData();

  /** ThreadedGenerateData().  ThreadedGenerateData can only write to the
   * portion of the output image specified by the parameter
   * "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData()  */
  virtual void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                                    int threadId );

  /** Computation of the electromagnetic fields Ei Er */
  void ComputeElectromagneticFields();

  /** Verify and force the inputs, if only  2 or 3 channels are present */
  void VerifyAndForceInputs();

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

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
  bool m_PresentInputImages[4];

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPolarimetricSynthesisFilter.txx"
#endif


#endif
