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
#ifndef __otbNonMaxRemovalByDirectionFilter_h
#define __otbNonMaxRemovalByDirectionFilter_h

#include "otbModulusAndDirectionImageToImageFilter.h"
#include "otbBinaryFunctorNeighborhoodImageFilter.h"

#include "otbMath.h"

namespace otb
{
namespace Functor
{
/** \class NonMaxRemovalByDirectionFunctor
 *  \brief This functor is used by the NonMaxRemovalByDirectionFilter
 *  \sa NonMaxRemovalByDirectionFilter
 *  \ingroup Functor
 */
template <class TInput1, class TInput2, class TOutput>
class NonMaxRemovalByDirectionFunctor
{
public:
  NonMaxRemovalByDirectionFunctor() {}
  virtual ~NonMaxRemovalByDirectionFunctor() {}
  inline TOutput operator ()(const TInput1& itA, const TInput2& itB)
  {
    TOutput resp = 0;
    if (itA.GetCenterPixel() != 0)
      {
      typename TInput1::OffsetType offset1, offset2;
      offset1.Fill(0);
      offset2.Fill(0);
      int neighborhoodNumber;
      if (itB.GetCenterPixel() > 0)
        {
        neighborhoodNumber = static_cast<int>(itB.GetCenterPixel() / (CONST_PI_4) -1);
        }
      else
        {
        neighborhoodNumber = static_cast<int>((itB.GetCenterPixel() + CONST_PI) / (CONST_PI_4) -1);
        }
      switch (neighborhoodNumber)
        {
        case 0:
          offset1[0] =  1;
          offset1[1] = -1;
          offset2[0] = -1;
          offset2[1] =  1;
          break;
        case 1:
          offset1[0] =  1;
          offset1[1] =  0;
          offset2[0] = -1;
          offset2[1] =  0;
          break;
        case 2:
          offset1[0] =  1;
          offset1[1] =  1;
          offset2[0] = -1;
          offset2[1] = -1;
          break;
        case 3:
          offset1[0] =  0;
          offset1[1] =  1;
          offset2[0] =  0;
          offset2[1] = -1;
          break;
        }
      if ((itA.GetCenterPixel() > itA.GetPixel(offset1))
          && (itA.GetCenterPixel() > itA.GetPixel(offset2)))
        {
        resp =  itA.GetCenterPixel();
        }
      }
    return resp;
  }
};
}
/** \class NonMaxRemovalByDirectionFilter
 *  \brief This filters removes (sets to null intensity) pixels which are not the maxima of the
 *  scalar product modulus value in the given direction.
 *
 * \ingroup Streamed
 * \ingroup Threaded
 */
template <class TInputModulus, class TInputDirection, class TOutputImage>
class ITK_EXPORT NonMaxRemovalByDirectionFilter
  : public ModulusAndDirectionImageToImageFilter<TInputModulus, TInputDirection, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef NonMaxRemovalByDirectionFilter                                                      Self;
  typedef ModulusAndDirectionImageToImageFilter<TInputModulus, TInputDirection, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                                                             Pointer;
  typedef itk::SmartPointer<const Self>                                                       ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(NonMaxRemovalByDirectionFilter, ModulusAndDirectionImageToImageFilter);

  /** typedef of the computing filter (this allows us to derive from ModulusAndDirectionToImageFilter as well as
      using the BinaryFunctorNeighBorhoodImageFilter, which is appropriate here */
  typedef Functor::NonMaxRemovalByDirectionFunctor<
      typename itk::ConstNeighborhoodIterator<TInputModulus>,
      typename itk::ConstNeighborhoodIterator<TInputDirection>,
      typename TOutputImage::PixelType>  FunctorType;
  typedef otb::BinaryFunctorNeighborhoodImageFilter<TInputModulus, TInputDirection, TOutputImage,
      FunctorType> ComputingFilterType;

protected:
  /** Constructor */
  NonMaxRemovalByDirectionFilter() {};
  /** Destructor */
  virtual ~NonMaxRemovalByDirectionFilter() {}
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
  NonMaxRemovalByDirectionFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};
} // End namespace otb
#endif
