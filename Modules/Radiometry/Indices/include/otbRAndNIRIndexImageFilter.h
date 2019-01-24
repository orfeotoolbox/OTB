/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbRAndNIRIndexImageFilter_h
#define otbRAndNIRIndexImageFilter_h

#include "itkBinaryFunctorImageFilter.h"
#include "otbVegetationIndicesFunctor.h"

namespace otb
{

/** \class RAndNIRIndexImageFilter
 * \brief TODO
 *
   * \ingroup Radiometry
 *
 * \ingroup OTBIndices
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
  ~RAndNIRIndexImageFilter() override {}

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /*  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                              itk::ThreadIdType threadId );
  */
private:
  RAndNIRIndexImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRAndNIRIndexImageFilter.hxx"
#endif

#endif
