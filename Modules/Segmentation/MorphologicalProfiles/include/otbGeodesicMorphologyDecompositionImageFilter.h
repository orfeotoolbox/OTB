/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbGeodesicMorphologyDecompositionImageFilter_h
#define otbGeodesicMorphologyDecompositionImageFilter_h

#include "otbGeodesicMorphologyLevelingFilter.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkOpeningByReconstructionImageFilter.h"
#include "itkClosingByReconstructionImageFilter.h"
#include "otbMacro.h"

namespace otb
{
/** \class GeodesicMorphologyDecompositionImageFilter
 *  \brief This class implements a geodesic morphology based image analysis algorithm.
 * \par
 * This algorithm is based on the following publication:
 * \par
 * Martino Pesaresi and Jon Alti Benediktsson, Member, IEEE:
 * A new approach for the morphological segmentation of high resolution
 * satellite imagery. IEEE Transactions on geoscience and remote sensing, vol. 39,
 * NO. 2, February 2001, p. 309-320.
 * \par
 *
 *  Given \f$\gamma^{\star}_{N}\f$ the geodesic opening by reconstruction morphological operator
 *  and \f$\varphi^{\star}_{N}\f$ the geodesic closing  by reconstruction morphological operator
 *  for a structuring element of size \f$N\f$, one can define the two following membership functions for
 *  the input image \f$f\f$:
 *
 *  \f[
 *  \stackrel{\smile}{\mu} = f-\gamma^{\star}_{N}(f)
 *  \f]
 *  \f[
 *  \stackrel{\frown}{\mu} = \phi^{\star}_{N}(f)-f
 *  \f]
 *
 * Where \f$\stackrel{\smile}{\mu}\f$ denotes the likelihood of the current pixel to belong to a convex structure
 * of the input image \f$f\f$ (i.e. brighter than the surrounding background) with respect to the size of the
 * structuring element \f$N\f$, and \f$\stackrel{\frown}{\mu}\f$ denotes the likelihood of the current pixel to
 * belong to a concave structure.
 *
 * This two membership functions can be used to define a simplification operator \f$\psi_{N}\f$ called leveling,
 * which removes from the input image \f$f\f$ the structures located by \f$\stackrel{\smile}{\mu}\f$ and
 * \f$\stackrel{\frown}{\mu}\f$:
 *
 * \f[
 *  \psi_{N}(f)= \left\{\begin{array}{lcl}
 *  \gamma^{\star}_{N}(f)&:& \stackrel{\smile}{\mu}>\stackrel{\frown}{\mu}\\
 *  \varphi^{\star}_{N}(f)&:& \stackrel{\frown}{\mu}>\stackrel{\smile}{\mu}\\
 *   f&:&\stackrel{\frown}{\mu}=\stackrel{\smile}{\mu}
 *  \end{array}\right.
 * \f]
 *
 * This filter performs this image decomposition at a given structuring element size. The GetOutput() method returns
 * \f$\psi_{N}(f)\f$, the GetConvexMap() method returns \f$\stackrel{\smile}{\mu}\f$ and the GetConcaveMap() method
 *  returns \f$\stackrel{\frown}{\mu}\f$.
 *
 * The PreserveIntensities and the FullyConnected flags reflects the option of the geodesic morphology filters from ITK.
 *
 * \sa GeodesicMorphologyLevelingFilter
 * \sa itk::OpeningByReconstructionImageFilter
 * \sa itk::ClosingByReconstructionImageFilter
 *
 * \ingroup OTBMorphologicalProfiles
 */
template <class TInputImage, class TOutputImage, class TStructuringElement>
class ITK_EXPORT GeodesicMorphologyDecompositionImageFilter
  : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef GeodesicMorphologyDecompositionImageFilter         Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(GeodesicMorphologyDecompositionImageFilter, ImageToImageFilter);

  /** Template parameters typedefs */
  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;

  typedef TStructuringElement
  StructuringElementType;
  typedef typename StructuringElementType::RadiusType RadiusType;
  typedef itk::OpeningByReconstructionImageFilter<InputImageType, InputImageType,
      StructuringElementType> OpeningFilterType;
  typedef itk::ClosingByReconstructionImageFilter<InputImageType, InputImageType,
      StructuringElementType> ClosingFilterType;
  typedef itk::SubtractImageFilter<InputImageType, InputImageType,
      OutputImageType>                       ConvexFilterType;
  typedef itk::SubtractImageFilter<OutputImageType, InputImageType,
      OutputImageType>                      ConcaveFilterType;
  typedef otb::GeodesicMorphologyLevelingFilter<InputImageType, OutputImageType,
      OutputImageType>         LevelingFilterType;

  /** Pointers typedefs*/
  typedef typename OpeningFilterType::Pointer  OpeningFilterPointerType;
  typedef typename ClosingFilterType::Pointer  ClosingFilterPointerType;
  typedef typename LevelingFilterType::Pointer LevelingFilterPointerType;
  typedef typename ConvexFilterType::Pointer   ConvexFilterPointerType;
  typedef typename ConcaveFilterType::Pointer  ConcaveFilterPointerType;

  /** Radius of the structuring element */
  itkSetMacro(Radius, RadiusType);
  itkGetConstReferenceMacro(Radius, RadiusType);

  /**
   * Get the convex likehood map.
   * \return The convex likehood map.
   */
  OutputImageType * GetConvexMap(void);

  /**
   * Get the concave likehood map.
   * \return The concave likehood map.
   */
  OutputImageType * GetConcaveMap(void);

  /** FullyConnected flag */
  itkSetMacro(FullyConnected, bool);
  itkGetMacro(FullyConnected, bool);
  /** Preserve intensities flag */
  itkSetMacro(PreserveIntensities, bool);
  itkGetMacro(PreserveIntensities, bool);

protected:
  /** GenerateData */
  void GenerateData(void) override;
  /** Constructor */
  GeodesicMorphologyDecompositionImageFilter();
  /** Destructor */
  ~GeodesicMorphologyDecompositionImageFilter() override {}
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  GeodesicMorphologyDecompositionImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  /** Radius of the structuring element */
  RadiusType m_Radius;
  /** Opening filter */
  OpeningFilterPointerType m_OpeningFilter;
  /** Closing filter */
  ClosingFilterPointerType m_ClosingFilter;
  /** First subtract filter (output is the convex map)*/
  ConvexFilterPointerType m_ConvexFilter;
  /** Second subtract filter (output is the concave map) */
  ConcaveFilterPointerType m_ConcaveFilter;
  /** Leveling function */
  LevelingFilterPointerType m_LevelingFilter;
  /** Preserve intensities in morphological operators */
  bool m_PreserveIntensities;
  /** Use fully connected morphological operators */
  bool m_FullyConnected;
  /** Progress accumulator to report internal filter progress */
  itk::ProgressAccumulator::Pointer m_Progress;

};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGeodesicMorphologyDecompositionImageFilter.hxx"
#endif

#endif
