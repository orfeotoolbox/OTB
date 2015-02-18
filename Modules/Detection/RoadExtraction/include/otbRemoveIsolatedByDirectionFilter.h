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
#ifndef __otbRemoveIsolatedByDirectionFilter_h
#define __otbRemoveIsolatedByDirectionFilter_h

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
  virtual ~RemoveIsolatedByDirectionFilter() {}
  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
  }
  /** Main computation method */
  virtual void GenerateData(void)
  {
    typename ComputingFilterType::Pointer filter = ComputingFilterType::New();
    filter->SetInput1(this->GetInput());
    filter->SetInput2(this->GetInputDirection());
    filter->GraftOutput(this->GetOutput());
    filter->Update();
    this->GraftOutput(filter->GetOutput());
  }

private:
  RemoveIsolatedByDirectionFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};
} // End namespace otb
#endif
