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
#ifndef __otbRAndGAndNIRVegetationIndexImageFilter_h
#define __otbRAndGAndNIRVegetationIndexImageFilter_h
#define DEPRECATED "This class is deprecated, use RAndGAndNIRIndexImageFilter"

#include "itkTernaryFunctorImageFilter.h"
#include "otbVegetationIndicesFunctor.h"

namespace otb
{

/** \class RAndGAndNIRVegetationIndexImageFilter
 * \brief
 *
 */

template <class TInputImageR, class TInputImageG, class TInputImageNIR, class TOutputImage,
class TFunction = Functor::AVI<              typename TInputImageR::PixelType,
typename TInputImageG::PixelType,
typename TInputImageNIR::PixelType,
typename TOutputImage::PixelType > >
class ITK_EXPORT RAndGAndNIRVegetationIndexImageFilter :  public itk::TernaryFunctorImageFilter< TInputImageR, TInputImageG, TInputImageNIR, TOutputImage, TFunction >
{
public:

  /** Standard typedefs */
  typedef RAndGAndNIRVegetationIndexImageFilter      Self;
  typedef itk::TernaryFunctorImageFilter< TInputImageR, TInputImageG, TInputImageNIR, TOutputImage, TFunction >  Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(RAndGAndNIRVegetationIndexImageFilter,TernaryFunctorImageFilter);

  void SetInputR( const TInputImageR * image );
  void SetInputG( const TInputImageG * image );
  void SetInputNIR( const TInputImageNIR * image );

  /** Template parameters typedefs */
  typedef typename Superclass::Input1ImageType RInputImageType;
  typedef typename Superclass::Input2ImageType GInputImageType;
  typedef typename Superclass::Input3ImageType NIRInputImageType;
  typedef typename Superclass::OutputImageType OutputImageType;
  typedef typename Superclass::FunctorType FunctorType;

protected:
  RAndGAndNIRVegetationIndexImageFilter();
  virtual ~RAndGAndNIRVegetationIndexImageFilter() {};

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  RAndGAndNIRVegetationIndexImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented


};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRAndGAndNIRVegetationIndexImageFilter.txx"
#endif


#endif
