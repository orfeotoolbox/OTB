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
#ifndef __otbRemoveWrongDirectionFilter_h
#define __otbRemoveWrongDirectionFilter_h

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
  virtual ~RemoveWrongDirectionFilter() {}
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
  RemoveWrongDirectionFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};
} // End namespace otb
#endif
