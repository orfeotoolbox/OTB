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
#ifndef __otbGAndRIndexImageFilter_h
#define __otbGAndRIndexImageFilter_h

#include "itkBinaryFunctorImageFilter.h"
#include "otbSoilIndicesFunctor.h"

namespace otb
{

/** \class GAndRIndexImageFilter
   * \brief TODO
   *
   * \ingroup Radiometry
   *
 */

template <class TInputImageG, class TInputImageR, class TOutputImage,
    class TFunction = Functor::IR<typename TInputImageG::PixelType,
        typename TInputImageR::PixelType,
        typename TOutputImage::PixelType> >
class ITK_EXPORT GAndRIndexImageFilter :  public itk::BinaryFunctorImageFilter<TInputImageG, TInputImageR, TOutputImage,
      TFunction>
{
public:

  /** Standard typedefs */
  typedef GAndRIndexImageFilter                                                              Self;
  typedef itk::BinaryFunctorImageFilter<TInputImageG, TInputImageR, TOutputImage, TFunction> Superclass;
  typedef itk::SmartPointer<Self>                                                            Pointer;
  typedef itk::SmartPointer<const Self>                                                      ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(GAndRIndexImageFilter, BinaryFunctorImageFilter);

  void SetInputG(const TInputImageG * image1);
  void SetInputR(const TInputImageR * image2);

  /** Template parameters typedefs */
  typedef typename Superclass::Input1ImageType       GInputImageType;
  typedef typename Superclass::Input1ImagePointer    GInputImagePointer;
  typedef typename Superclass::Input2ImageType       RInputImageType;
  typedef typename Superclass::Input2ImagePointer    RInputImagePointer;
  typedef typename Superclass::OutputImageType       OutputImageType;
  typedef typename Superclass::OutputImagePointer    OutputImagePointer;
  typedef typename Superclass::FunctorType           FunctorType;
  typedef typename Superclass::OutputImageRegionType OutputImageRegionType;

protected:
  GAndRIndexImageFilter();
  virtual ~GAndRIndexImageFilter() {}

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /*  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                              itk::ThreadIdType threadId );
  */
private:
  GAndRIndexImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGAndRIndexImageFilter.txx"
#endif

#endif
