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

#ifndef otbRemoveIsolatedByDirectionFilter_h
#define otbRemoveIsolatedByDirectionFilter_h

#include "otbModulusAndDirectionImageToImageFilter.h"
#include "otbBinaryFunctorNeighborhoodImageFilter.h"

namespace otb
{
namespace Functor
{
/** \class RemoveIsolatedByDirectionFunctor
 *  \brief Binary neighborhood functor to remove isolated pixels by direction.
 *  Used by the RemoveIsolatedByDirectionFilter.
 *  \sa RemoveIsolatedByDirectionFilter
 *  \ingroup Functor
 *
 * \ingroup OTBRoadExtraction
 */
template <class TInput1, class TInput2, class TOutput>
class RemoveIsolatedByDirectionFunctor
{
public:
  RemoveIsolatedByDirectionFunctor() {}
  virtual ~RemoveIsolatedByDirectionFunctor() {}
  inline TOutput operator ()(const TInput1& itA, const TInput2& itB)
  {
    double currentDirection = itB.GetCenterPixel();
    int    nEqualNeighbors = 0;
    for (int neighborhoodIndex = 0; neighborhoodIndex < 9; ++neighborhoodIndex)
      {
      if (itB.GetPixel(neighborhoodIndex) == currentDirection)
        {
        ++nEqualNeighbors;
        }
      }
    if (nEqualNeighbors <= 1)
      {
      //should never be 0 as it is at least equal to itself
      return 0;
      }
    else
      {
      return static_cast<TOutput>(itA.GetCenterPixel());
      }
  }
};
}
/** \class RemoveIsolatedByDirectionFilter
 *  \brief This filter removes (sets to null intensity) pixels isolated by direction.
 *
 *  For a given pixel, the filter walk through its neighborhood in direction image, counting pixels having
 *  the same direction as the center pixel. If there is no such a pixel, the center pixel is considered to be
 *  isolated in direction, and thus will be removed (set to 0). If the pixel is not isolated in direction, the output
 *  value is the value of the pixel in the modulus image.
 *  Of course this filter requires the direction to be discrete, in order to be able to count the directions.
 *
 * \ingroup Streamed
 * \ingroup Threaded
 *
 * \ingroup OTBRoadExtraction
 */
template <class TInputModulus, class TInputDirection, class TOutputImage>
class ITK_EXPORT RemoveIsolatedByDirectionFilter
  : public ModulusAndDirectionImageToImageFilter<TInputModulus, TInputDirection, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef RemoveIsolatedByDirectionFilter                                                     Self;
  typedef ModulusAndDirectionImageToImageFilter<TInputModulus, TInputDirection, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                                                             Pointer;
  typedef itk::SmartPointer<const Self>                                                       ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(RemoveIsolatedByDirectionFilter, ModulusAndDirectionImageToImageFilter);

  /** typedef of the computing filter (this allows us to derive from ModulusAndDirectionToImageFilter as well as
      using the BinaryFunctorNeighBorhoodImageFilter, which is appropriate here */
  typedef Functor::RemoveIsolatedByDirectionFunctor<
      typename itk::ConstNeighborhoodIterator<TInputModulus>,
      typename itk::ConstNeighborhoodIterator<TInputDirection>,
      typename TOutputImage::PixelType>  FunctorType;
  typedef otb::BinaryFunctorNeighborhoodImageFilter<TInputModulus, TInputDirection, TOutputImage,
      FunctorType> ComputingFilterType;

protected:
  /** Constructor */
  RemoveIsolatedByDirectionFilter() {};
  /** Destructor */
  ~RemoveIsolatedByDirectionFilter() override {}
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
  RemoveIsolatedByDirectionFilter(const Self &) = delete;
  void operator =(const Self&) = delete;
};
} // End namespace otb
#endif
