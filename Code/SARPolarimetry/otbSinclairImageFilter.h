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
#ifndef __otbSinclairImageFilter_h
#define __otbSinclairImageFilter_h

#include "otbQuaternaryFunctorImageFilter.h"
#include "otbSinclairToReciprocalCovarianceFunctor.h"
#include "itkFixedArray.h"
#include <complex>

namespace otb
{

/** \class SinclairImageFilter
 * \brief Convert the Sinclair matrix
 *
 * This class is parameterized over the type of the input images and
 * the type of the output image.  It is also parameterized by the
 * operation to be applied, using a Functor style.
 *
 *  \ingroup SARPolarimetry
 *
 *  \sa SinclairImageFilter
 *  \sa SinclairToCircularCovarianceMatrixFunctor
 *  \sa SinclairToCoherencyFunctor
 *  \sa SinclairToCovarianceFunctor
 *  \sa SinclairToMuellerFunctor
 *  \sa SinclairToReciprocalCircularCovarianceMatrixFunctor
 *  \sa SinclairToReciprocalCoherencyFunctor
 *  \sa SinclairToReciprocalCovarianceFunctor
 */

template <class TInputImageHH, class TInputImageHV,
          class TInputImageVH, class TInputImageVV, class TOutputImage,
    class TFunction = Functor::SinclairToReciprocalCovarianceFunctor<
        typename TInputImageHH::PixelType,
        typename TInputImageHV::PixelType,
        typename TInputImageVH::PixelType,
        typename TInputImageVV::PixelType,
        typename TOutputImage::PixelType> >
class ITK_EXPORT SinclairImageFilter :  public otb::QuaternaryFunctorImageFilter<TInputImageHH,
      TInputImageHV, TInputImageVH,
      TInputImageVV, TOutputImage,
      TFunction>
{
public:

  /** Standard typedefs */
  typedef SinclairImageFilter Self;
  typedef otb::QuaternaryFunctorImageFilter<TInputImageHH, TInputImageHV,
      TInputImageVH, TInputImageVV, TOutputImage, TFunction>  Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(SinclairImageFilter, QuaternaryFunctorImageFilter);

  /** Template parameters typedefs */
  typedef std::complex <double>                    InputPixelType;
  typedef otb::Image<InputPixelType,  2>           InputImageType;
  typedef typename Superclass::Input1ImageType     HHInputImageType;
  typedef typename Superclass::Input1ImagePointer  HHInputImagePointer;
  typedef typename Superclass::Input2ImageType     HVInputImageType;
  typedef typename Superclass::Input2ImagePointer  HVInputImagePointer;
  typedef typename Superclass::Input3ImageType     VHInputImageType;
  typedef typename Superclass::Input3ImagePointer  VHInputImagePointer;
  typedef typename Superclass::Input4ImageType     VVInputImageType;
  typedef typename Superclass::Input4ImagePointer  VVInputImagePointer;
  typedef typename Superclass::OutputImageType     OutputImageType;
  typedef typename OutputImageType::Pointer        OutputImagePointer;
  typedef typename OutputImageType::RegionType     OutputImageRegionType;
  typedef typename Superclass::FunctorType         FunctorType;
  typedef typename std::complex <double>           ComplexType;
  typedef typename itk::FixedArray<ComplexType, 2> ComplexArrayType;

  void SetInputHH(const TInputImageHH * image);
  void SetInputHV(const TInputImageHV * image);
  void SetInputVH(const TInputImageVH * image);
  void SetInputVV(const TInputImageVV * image);


protected:
  /**  Constructor */
  SinclairImageFilter() {}
  /**  Destructor */
  virtual ~SinclairImageFilter() {}

  virtual void GenerateOutputInformation();

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:

  SinclairImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSinclairImageFilter.txx"
#endif

#endif
