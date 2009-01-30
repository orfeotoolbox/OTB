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
#ifndef __otbRAndBAndNIRVegetationIndexImageFilter_h
#define __otbRAndBAndNIRVegetationIndexImageFilter_h

#include "itkTernaryFunctorImageFilter.h"
#include "otbVegetationIndex.h"

namespace otb
{

/** \class RAndBAndNIRVegetationIndexImageFilter
 * \brief
 *
 */

template <class TInputImageR, class TInputImageB, class TInputImageNIR, class TOutputImage,
class TFunction = Functor::ARVI<              typename TInputImageR::PixelType,
typename TInputImageB::PixelType,
typename TInputImageNIR::PixelType,
typename TOutputImage::PixelType > >
class ITK_EXPORT RAndBAndNIRVegetationIndexImageFilter :  public itk::TernaryFunctorImageFilter< TInputImageR, TInputImageB, TInputImageNIR, TOutputImage, TFunction >
{
public:

  /** Standard typedefs */
  typedef RAndBAndNIRVegetationIndexImageFilter      Self;
  typedef itk::TernaryFunctorImageFilter< TInputImageR, TInputImageB, TInputImageNIR, TOutputImage, TFunction >  Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(RAndBAndNIRVegetationIndexImageFilter,TernaryFunctorImageFilter);

  void SetInputR( const TInputImageR * image );
  void SetInputB( const TInputImageB * image );
  void SetInputNIR( const TInputImageNIR * image );

  /** Template parameters typedefs */
  typedef typename Superclass::Input1ImageType RInputImageType;
  typedef typename Superclass::Input2ImageType BInputImageType;
  typedef typename Superclass::Input3ImageType NIRInputImageType;
  typedef typename Superclass::OutputImageType OutputImageType;
  typedef typename Superclass::FunctorType FunctorType;

protected:
  RAndBAndNIRVegetationIndexImageFilter();
  virtual ~RAndBAndNIRVegetationIndexImageFilter() {};

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  RAndBAndNIRVegetationIndexImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented


};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRAndBAndNIRVegetationIndexImageFilter.txx"
#endif


#endif
