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
#ifndef otbGeodesicMorphologyIterativeDecompositionImageFilter_h
#define otbGeodesicMorphologyIterativeDecompositionImageFilter_h

#include "otbImageToImageListFilter.h"
#include "otbGeodesicMorphologyDecompositionImageFilter.h"

namespace otb
{
/** \class GeodesicMorphologyIterativeDecompositionImageFilter
 * \brief This class recursively apply geodesic decomposition.
 *
 * \par
 * This algorithm is derived from the following publication:
 * \par
 * Martino Pesaresi and Jon Alti Benediktsson, Member, IEEE:
 * A new approach for the morphological segmentation of high resolution
 * satellite imagery. IEEE Transactions on geoscience and remote sensing, vol. 39,
 * NO. 2, February 2001, p. 309-320.
 * \par
 *
 * It provides a geodesic decomposition of the input image, with the following scheme.
 * Let \f$f_0\f$ denote the input image,\f$\stackrel{\smile}{\mu}_{N}(f)\f$ denote
 * the convex membership function, \f$ \stackrel{\frown}{\mu}_{N}(f) \f$ denote the concave
 *  membership function and \f$\psi_{N}(f)\f$ denote the leveling function, for a given radius \f$ N \f$
 * as defined in the documentation  of the GeodesicMorphologyDecompositionImageFilter .
 * Let \f$[N_{1},\ldots, N_{n}]\f$ denote a range of increasing radius (or scales). The iterative
 * decomposition is defined as follows:
 *
 * \f[
 *  f_{n} = \psi_{N_{n}}(f_{n-1})
 * \f]
 *
 * \f[
 *  \stackrel{\frown}{f}_{n} = \stackrel{\frown}{\mu}_{N_{N}}(f_{n})
 * \f]
 *
 * \f[
 *  \stackrel{\smile}{f}_{n} = \stackrel{\smile}{\mu}_{N_{N}}(f_{n})
 * \f]
 *
 * The \f$ \stackrel{\frown}{f}_{n}\f$ and \f$\stackrel{\frown}{f}_{n}\f$ are membership function
 * for the convex (resp. concave) objects whose size is comprised between \f$N_{n-1} \f$ and \f$N_n\f$ .
 *
 * \sa GeodescicMorphologyDecompositionImageFilter
 *
 * \ingroup OTBMorphologicalProfiles
 */
template <class TImage, class TStructuringElement>
class ITK_EXPORT GeodesicMorphologyIterativeDecompositionImageFilter
  : public ImageToImageListFilter<TImage, TImage>
{
public:
  /** Standard typedefs */
  typedef GeodesicMorphologyIterativeDecompositionImageFilter Self;
  typedef ImageToImageListFilter<TImage, TImage>              Superclass;
  typedef itk::SmartPointer<Self>                             Pointer;
  typedef itk::SmartPointer<const Self>                       ConstPointer;
  /** Creation through object factory macro */
  itkNewMacro(Self);
  /** Type macro */
  itkTypeMacro(GeodesicMorphologyIterativeDecompositionImageFilter, ImageToImageListFilter);
  /** Input image dimension constant macro */
  itkStaticConstMacro(InputImageDimension, unsigned int, TImage::ImageDimension);
  /** Output related typedefs */
  typedef typename Superclass::OutputImageType        OutputImageType;
  typedef typename Superclass::OutputImageListType    OutputImageListType;
  typedef typename OutputImageListType::Pointer       OutputImageListPointerType;
  typedef typename Superclass::OutputImagePointerType OutputImagePointerType;
  /** Input related typedefs */
  typedef typename Superclass::InputImageType       InputImageType;
  typedef typename Superclass::InputImageRegionType InputImageRegionType;
  typedef typename InputImageType::Pointer          InputImagePointerType;
  typedef typename InputImageType::ConstPointer     InputImageConstPointerType;
  typedef typename InputImageType::SizeType         SizeType;
  typedef typename InputImageType::ValueType        ValueType;
  typedef typename InputImageType::PixelType        PixelType;
  typedef typename InputImageType::SpacingType      SpacingType;

  typedef TStructuringElement StructuringElementType;

  /** Typedef of the decomposition filter */
  typedef GeodesicMorphologyDecompositionImageFilter<InputImageType, InputImageType,
      StructuringElementType> DecompositionFilterType;
  typedef typename DecompositionFilterType::Pointer
  DecompositionFilterPointerType;

  /** Accessors */
  itkSetMacro(Step, unsigned int);
  itkGetMacro(Step, unsigned int);
  itkSetMacro(InitialValue, unsigned int);
  itkGetMacro(InitialValue, unsigned int);
  itkSetMacro(NumberOfIterations, unsigned int);
  itkGetMacro(NumberOfIterations, unsigned int);

  /**
   * Get The leveling images for each scale.
   * \return The leveling images for each scale.
   */
  OutputImageListType* GetOutput(void) ITK_OVERRIDE;
  /**
   * Get convex membership function for each scale
   * \return The convex membership function for each scale.
   */
  OutputImageListType * GetConvexOutput(void);
  /**
   * Get The concave membership function for each scale
   * \return The concave membership function for each scale.
   */
  OutputImageListType * GetConcaveOutput(void);

protected:
  /** Constructor */
  GeodesicMorphologyIterativeDecompositionImageFilter();
  /** Destructor */
  ~GeodesicMorphologyIterativeDecompositionImageFilter() ITK_OVERRIDE {}
  /** Generate output information */
  void GenerateOutputInformation() ITK_OVERRIDE;
  /** Generate input requested region */
  void GenerateInputRequestedRegion() ITK_OVERRIDE;
  /** Main computation method */
  void GenerateData() ITK_OVERRIDE;
  /** Printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

private:
  /** The step for the scale analysis */
  unsigned int m_Step;
  /** The initial value for the scale */
  unsigned int m_InitialValue;
  /** The number of iterations */
  unsigned int m_NumberOfIterations;

};
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGeodesicMorphologyIterativeDecompositionImageFilter.txx"
#endif

#endif
