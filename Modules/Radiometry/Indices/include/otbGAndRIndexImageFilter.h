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

#ifndef otbGAndRIndexImageFilter_h
#define otbGAndRIndexImageFilter_h

#include "itkBinaryFunctorImageFilter.h"
#include "otbSoilIndicesFunctor.h"

namespace otb
{

/** \class GAndRIndexImageFilter
   * \brief TODO
   *
   * \ingroup Radiometry
   *
 *
 * \ingroup OTBIndices
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
  ~GAndRIndexImageFilter() override {}

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /*  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                              itk::ThreadIdType threadId );
  */
private:
  GAndRIndexImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGAndRIndexImageFilter.hxx"
#endif

#endif
