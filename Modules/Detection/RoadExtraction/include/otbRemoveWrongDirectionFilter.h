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

#ifndef otbRemoveWrongDirectionFilter_h
#define otbRemoveWrongDirectionFilter_h

#include "otbModulusAndDirectionImageToImageFilter.h"
#include "itkBinaryFunctorImageFilter.h"
#include "otbMath.h"

namespace otb
{
namespace Functor
{
/** \class RemoveWrongDirectionFunctor
 *  \brief This functor is used by the RemoveWrongDirectionFilter
 *
 *  \sa RemoveWrongDirectionFilter
 *  \ingroup Functor
 *
 * \example FeatureExtraction/ExtractRoadByStepsExample.cxx
 *
 *
 * \ingroup OTBRoadExtraction
 */
template <class TInput1, class TInput2, class TOutput>
class RemoveWrongDirectionFunctor
{
public:
  RemoveWrongDirectionFunctor() {}
  virtual ~RemoveWrongDirectionFunctor() {}
  inline TOutput operator ()(const TInput1& A, const TInput2& B)
  {
    if (B < CONST_PI_8)
      {
      return 0;
      }
    else
      {
      return A;
      }
  }
};
}
/** \class RemoveWrongDirectionFilter
 *  \brief This filter removes (sets to null intensity) pixels with wrong direction.
 *
 *  This filter is part of the road extraction framework. By using the Spectral Angle filter,
 *  we can assume that the direction of a road on our scalar product image is positive (greater
 *  than \f$ \pi/8 \f$).
 *  Therefore in the input modulus and direction images, pixels whose direction is lower than
 *  this threshold are suppressed
 *  (in fact, their intensity is set to 0).
 *
 * \sa NeighborhoodScalarProductFilter
 * \ingroup Streamed
 * \ingroup Threaded
 *
 * \ingroup OTBRoadExtraction
 */
template <class TInputModulus, class TInputDirection, class TOutputImage>
class ITK_EXPORT RemoveWrongDirectionFilter
  : public ModulusAndDirectionImageToImageFilter<TInputModulus, TInputDirection, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef RemoveWrongDirectionFilter                                                          Self;
  typedef ModulusAndDirectionImageToImageFilter<TInputModulus, TInputDirection, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                                                             Pointer;
  typedef itk::SmartPointer<const Self>                                                       ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(RemoveWrongDirectionFilter, ModulusAndDirectionImageToImageFilter);

  /** typedef of the computing filter (this allows us to derive from ModulusAndDirectionToImageFilter as well as
      using the BinaryFunctorImageFilter, which is appropriate here */
  typedef Functor::RemoveWrongDirectionFunctor<
      typename TInputModulus::PixelType,
      typename TInputDirection::PixelType,
      typename TOutputImage::PixelType>  FunctorType;
  typedef itk::BinaryFunctorImageFilter<TInputModulus, TInputDirection, TOutputImage, FunctorType> ComputingFilterType;

protected:
  /** Constructor */
  RemoveWrongDirectionFilter() {};
  /** Destructor */
  ~RemoveWrongDirectionFilter() override {}
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override
  {
    Superclass::PrintSelf(os, indent);
  }
  /** Main computation method */
  void GenerateData(void) override
  {
    typename ComputingFilterType::Pointer filter = ComputingFilterType::New();
    filter->SetInput1(this->GetInput());
    filter->SetInput2(this->GetInputDirection());
    filter->GraftOutput(this->GetOutput());
    filter->Update();
    this->GraftOutput(filter->GetOutput());
  }

private:
  RemoveWrongDirectionFilter(const Self &) = delete;
  void operator =(const Self&) = delete;
};
} // End namespace otb
#endif
