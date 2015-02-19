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
#ifndef __otbGeodesicMorphologyLevelingFilter_h
#define __otbGeodesicMorphologyLevelingFilter_h

#include "itkTernaryFunctorImageFilter.h"

namespace otb
{
namespace Functor
{
/** \class LevelingFunctor
 * \brief This functor performs the pixel-wise leveling operation needed in the
 * geodesic morphology decomposition filter. For more details, please refer to
 * the documentation of this filter.
 * \sa GeodesicMorphologyDecompositionImageFilter
 *
 * \ingroup OTBMorphologicalProfiles
 */
template <class TInput, class TInputMap, class TOutput>
class LevelingFunctor
{
public:
  /// Constructor
  LevelingFunctor() {}
  /// Destructor
  virtual ~LevelingFunctor() {}

  inline TOutput operator ()(const TInput& pixel, const TInputMap& convexPixel, const TInputMap& concavePixel)
  {
    TOutput result;

    if (convexPixel > concavePixel)
      {
      result = static_cast<TOutput>(pixel - convexPixel);
      }
    else if (convexPixel < concavePixel)
      {
      result = static_cast<TOutput>(concavePixel + pixel);
      }
    else
      {
      result = static_cast<TOutput>(pixel);
      }
    return result;
  }
};
} // end namespace Functor

/** \class GeodesicMorphologyLevelingFilter
 *  \brief This filter performs the leveling operation defined in the documentation of
 *  the geodesic decomposition image filter, given the original image, convex and concave membership
 *  functions. Please refer to the documentation of this filter for more details.
 *
 * \sa GeodesicMorphologyDecompositionImageFilter
 *
 * \ingroup OTBMorphologicalProfiles
 */
template <class TInputImage, class TInputMaps, class TOutputImage>
class ITK_EXPORT GeodesicMorphologyLevelingFilter
  : public itk::TernaryFunctorImageFilter<TInputImage, TInputImage,
      TInputImage, TOutputImage,
      Functor::LevelingFunctor<typename TInputImage::PixelType,
          typename TInputMaps::PixelType,
          typename TOutputImage::PixelType> >
{
public:
  /** Standard typedefs */
  typedef GeodesicMorphologyLevelingFilter Self;
  typedef   itk::TernaryFunctorImageFilter<TInputImage, TInputImage,
      TInputImage, TOutputImage,
      Functor::LevelingFunctor<typename TInputImage::PixelType,
          typename TInputMaps::PixelType,
          typename TOutputImage::PixelType> > Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(GeodesicMorphologyLevelingFilter, TernaryFunctorImageFilter);

  /**
   * Set the convex membership image.
   */
  void SetInputConvexMap(const TInputMaps * convexMap)
  {
    this->SetInput2(convexMap);
  }
  /**
   * Set the concave membership image.
   */
  void SetInputConcaveMap(const TInputMaps * concaveMap)
  {
    this->SetInput3(concaveMap);
  }
  /**
   * Set the original input image
   */
  using Superclass::SetInput;
  void SetInput(const TInputImage * input)
  {
    this->SetInput1(input);
  }

protected:
  /** Constructor */
  GeodesicMorphologyLevelingFilter() {};
  /** Destructor */
  virtual ~GeodesicMorphologyLevelingFilter() {}
  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
  }

private:
  GeodesicMorphologyLevelingFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};
} // End namespace otb
#endif
