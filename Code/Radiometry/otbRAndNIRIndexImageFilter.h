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
#ifndef __otbRAndNIRIndexImageFilter_h
#define __otbRAndNIRIndexImageFilter_h

#include "itkBinaryFunctorImageFilter.h"
#include "otbVegetationIndicesFunctor.h"

namespace otb
{

/** \class RAndNIRIndexImageFilter
 * \brief TODO
 *
   * \ingroup Radiometry
 */

template <class TInputImageR, class TInputImageNIR, class TOutputImage,
    class TFunction = Functor::NDVI<typename TInputImageR::PixelType,
        typename TInputImageNIR::PixelType,
        typename TOutputImage::PixelType> >
class ITK_EXPORT RAndNIRIndexImageFilter :  public itk::BinaryFunctorImageFilter<TInputImageR, TInputImageNIR,
      TOutputImage, TFunction>
{
public:

  /** Standard typedefs */
  typedef RAndNIRIndexImageFilter                                                              Self;
  typedef itk::BinaryFunctorImageFilter<TInputImageR, TInputImageNIR, TOutputImage, TFunction> Superclass;
  typedef itk::SmartPointer<Self>                                                              Pointer;
  typedef itk::SmartPointer<const Self>                                                        ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(RAndNIRIndexImageFilter, BinaryFunctorImageFilter);

  void SetInputR(const TInputImageR * image1);
  void SetInputNIR(const TInputImageNIR * image2);

  /** Template parameters typedefs */
  typedef typename Superclass::Input1ImageType       RInputImageType;
  typedef typename Superclass::Input1ImagePointer    RInputImagePointer;
  typedef typename Superclass::Input2ImageType       NIRInputImageType;
  typedef typename Superclass::Input2ImagePointer    NIRInputImagePointer;
  typedef typename Superclass::OutputImageType       OutputImageType;
  typedef typename Superclass::OutputImagePointer    OutputImagePointer;
  typedef typename Superclass::FunctorType           FunctorType;
  typedef typename Superclass::OutputImageRegionType OutputImageRegionType;

protected:
  RAndNIRIndexImageFilter();
  virtual ~RAndNIRIndexImageFilter() {}

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /*  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                              itk::ThreadIdType threadId );
  */
private:
  RAndNIRIndexImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRAndNIRIndexImageFilter.txx"
#endif

#endif
