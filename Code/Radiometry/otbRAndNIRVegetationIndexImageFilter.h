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
#ifndef __otbRAndNIRVegetationIndexImageFilter_h
#define __otbRAndNIRVegetationIndexImageFilter_h

#include "itkBinaryFunctorImageFilter.h"
#include "otbVegetationIndex.h"

namespace otb
{

/** \class RAndNIRVegetationIndexImageFilter
 * \brief
 *
 */

template <class TInputImageR, class TInputImageNIR, class TOutputImage,
          class TFunction = Functor::NDVI<              typename TInputImageR::PixelType,
                                                        typename TInputImageNIR::PixelType,
                                                        typename TOutputImage::PixelType > >
class ITK_EXPORT RAndNIRVegetationIndexImageFilter :  public itk::BinaryFunctorImageFilter< TInputImageR, TInputImageNIR, TOutputImage, TFunction >
{
public:

  /** Standard typedefs */
  typedef RAndNIRVegetationIndexImageFilter      Self;
  typedef itk::BinaryFunctorImageFilter< TInputImageR, TInputImageNIR, TOutputImage, TFunction >  Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(RAndNIRVegetationIndexImageFilter,BinaryFunctorImageFilter);

  void SetInputR( const TInputImageR * image1 );
  void SetInputNIR( const TInputImageNIR * image2 );

  /** Template parameters typedefs */
  typedef typename Superclass::Input1ImageType RInputImageType;
  typedef typename Superclass::Input1ImagePointer RInputImagePointer;
  typedef typename Superclass::Input2ImageType NIRInputImageType;
  typedef typename Superclass::Input2ImagePointer NIRInputImagePointer;
  typedef typename Superclass::OutputImageType OutputImageType;
  typedef typename Superclass::OutputImagePointer OutputImagePointer;
  typedef typename Superclass::FunctorType FunctorType;
  typedef typename Superclass::OutputImageRegionType OutputImageRegionType;

protected:
  RAndNIRVegetationIndexImageFilter();
  virtual ~RAndNIRVegetationIndexImageFilter() {};

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

/*  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            int threadId );
*/
private:
  RAndNIRVegetationIndexImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented


};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRAndNIRVegetationIndexImageFilter.txx"
#endif


#endif
