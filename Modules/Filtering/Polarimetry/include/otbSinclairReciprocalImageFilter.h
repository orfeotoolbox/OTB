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
#ifndef __otbSinclairReciprocalImageFilter_h
#define __otbSinclairReciprocalImageFilter_h

#include "itkTernaryFunctorImageFilter.h"
#include "otbSinclairToReciprocalCovarianceMatrixFunctor.h"
#include <complex>

namespace otb
{

/** \class SinclairReciprocalImageFilter
 * \brief Convert the Sinclair reciprocal matrix.
 *
 * This class is parameterized over the type of the input images and
 * the type of the output image. It is also parameterized by the
 * operation to be applied, using a Functor style.
 *
 * The output image has 6 channels : the diagonal and the upper element of the reciprocal matrix.
 * Element a sotored from left to right, line by line.
 *
 * The class is templated by the 3 input image (HH, HV_VH and VV) and the used functor.
 * Available functors are :
 * - SinclairToReciprocalCovarianceMatrixFunctor (default one)
 * - SinclairToReciprocalCircularCovarianceMatrixFunctor
 * - SinclairToReciprocalCoherencyMatrixFunctor
 *
 *  \ingroup SARPolarimetry
 *
 *  \sa SinclairImageFilter
 *  \sa SinclairToReciprocalCircularCovarianceMatrixFunctor
 *  \sa SinclairToReciprocalCoherencyMatrixFunctor
 *  \sa SinclairToReciprocalCovarianceMatrixFunctor
 *
 *
 * \ingroup OTBPolarimetry
 */


template <class TInputImageHH, class TInputImageHV_VH,
          class TInputImageVV, class TOutputImage,
    class TFunction = Functor::SinclairToReciprocalCovarianceMatrixFunctor<
        typename TInputImageHH::PixelType,
        typename TInputImageHV_VH::PixelType,
        typename TInputImageVV::PixelType,
        typename TOutputImage::PixelType> >
class ITK_EXPORT SinclairReciprocalImageFilter :  public itk::TernaryFunctorImageFilter<TInputImageHH,
      TInputImageHV_VH, TInputImageVV,
      TOutputImage, TFunction>
{
public:

  /** Standard typedefs */
  typedef SinclairReciprocalImageFilter Self;
  typedef itk::TernaryFunctorImageFilter<TInputImageHH, TInputImageHV_VH,
      TInputImageVV, TOutputImage, TFunction>  Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(SinclairReciprocalImageFilter, itk::TernaryFunctorImageFilter);

  /** Template parameters typedefs */
  typedef typename Superclass::Input1ImageType     HHInputImageType;
  typedef typename Superclass::Input1ImagePointer  HHInputImagePointer;
  typedef typename Superclass::Input2ImageType     HV_VHInputImageType;
  typedef typename Superclass::Input2ImagePointer  HV_VHInputImagePointer;
  typedef typename Superclass::Input3ImageType     VVInputImageType;
  typedef typename Superclass::Input3ImagePointer  VVInputImagePointer;
  typedef typename Superclass::OutputImageType     OutputImageType;
  typedef typename OutputImageType::Pointer        OutputImagePointer;
  typedef typename OutputImageType::RegionType     OutputImageRegionType;
  typedef typename Superclass::FunctorType         FunctorType;

  void SetInputHH(const TInputImageHH * image);
  // This method set the second input, same as SetInputVH
  void SetInputHV(const TInputImageHV_VH * image);
  // This method set the second input, same as SetInputHV
  void SetInputVH(const TInputImageHV_VH * image);
  // This method set the second input, same as SetInputHV and SetInputHV
  void SetInputHV_VH(const TInputImageHV_VH * image);
  void SetInputVV(const TInputImageVV * image);


protected:
  /**  Constructor */
  SinclairReciprocalImageFilter() {}
  /**  Destructor */
  ~SinclairReciprocalImageFilter() ITK_OVERRIDE {}

  void GenerateOutputInformation() ITK_OVERRIDE;

  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

private:

  SinclairReciprocalImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};


} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSinclairReciprocalImageFilter.txx"
#endif

#endif
