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
#ifndef __otbRadiometricAttributesLabelMapFilter_h
#define __otbRadiometricAttributesLabelMapFilter_h

#include "otbStatisticsAttributesLabelMapFilter.h"

#include "otbVegetationIndicesFunctor.h"
#include "otbSoilIndicesFunctor.h"
#include "otbWaterIndicesFunctor.h"
#include "otbMultiChannelRAndNIRIndexImageFilter.h"
#include "otbMultiChannelGAndRIndexImageFilter.h"
#include "otbMultiChannelRAndGAndNIRVegetationIndexImageFilter.h"
#include "otbVectorImageToIntensityImageFilter.h"
#include "otbMultiToMonoChannelExtractROI.h"

namespace otb
{
namespace Functor
{
/** \class MultiStatsAttributesLabelObjectFunctor
*   \brief Functor to compute multiple statistics attributes.
*
* For one label object, this functors applies the
* StatisticsAttributesLabelObjectFunctor
* for a set of feature images along with their feature names.
*
* As such, it allows to compute in one pass statistics related to
* mulitple features. It is used in the
* RadiometricAttributesLabelMapFilter.
*
* Features can be added, removed or cleared via the appropriate
* methods.
*
*   \sa RadiometricAttributesLabelMapFilter
*   \sa StatisticsAttributesLabelObjectFunctor
*/
template <class TLabelObject, class TFeatureImage>
class MultiStatsAttributesLabelObjectFunctor
{
public:
  // Self typedef
  typedef MultiStatsAttributesLabelObjectFunctor Self;

  /// Typedef of the feature image type
  typedef typename TFeatureImage::PixelType FeatureType;

  /// Typedef of the label object
  typedef TLabelObject LabelObjectType;

  /// Feature image const pointer
  typedef typename TFeatureImage::ConstPointer FeatureImageConstPointer;

  /// Statistics functor
  typedef StatisticsAttributesLabelObjectFunctor
  <TLabelObject, TFeatureImage>                           StatsFunctorType;

  /// Map to store the functors
  typedef std::map<std::string, StatsFunctorType> StatsFunctorsMapType;

  /** Constructor */
  MultiStatsAttributesLabelObjectFunctor();

  /** Destructor */
  virtual ~MultiStatsAttributesLabelObjectFunctor();

  /** The comparators */
  bool operator !=(const Self& self);
  bool operator ==(const Self& self);

  /** This is the functor implementation
   *  Calling the functor on a label object
   *  will update its statistics attributes */
  inline void operator ()(LabelObjectType * lo) const;

  /** Add a feature with the given name */
  void AddFeature(const std::string& name, const TFeatureImage * img);

  /** Remove the feature with this name if it exists */
  bool RemoveFeature(const std::string& name);

  /** Get the feature image with this name */
  const TFeatureImage * GetFeature(const std::string& name) const;

  /** Clear all the features */
  void ClearAllFeatures();

  /** Get the number of features */
  unsigned int GetNumberOfFeatures() const;

  /** Set the reduced attribute set */
  void SetReducedAttributeSet(bool flag);

  /** Get the reduced attribute set */
  bool GetReducedAttributeSet() const;

private:
  /// True to compute only a reduced attribute set
  bool m_ReducedAttributeSet;

  /// The Stat functors map
  StatsFunctorsMapType m_StatsFunctorsMap;
};
} // End namespace Functor

/** \class RadiometricAttributesLabelMapFilter
 *  \brief This filter computes radiometric attributes for each object.
 *
 * Images are supposed to be standard 4-bands image (B,G,R,NIR). The
 * index of each channel can be set via the Set***ChannelIndex()
 * accessors.
 *
 * This filter internally applies the
 * StatisticsAttributesLabelMapFilter to the following features: GEMI,
 * NDVI, IR, IC, IB, NDWI2, Intensity and original B, G, R and NIR channels.
 *
 * The feature name is constructed as:
 * 'STATS' + '::' + RadiometricFeatureName + '::' + StatisticName
 * For example, accessing the mean of NDVI on a LabelObject with the
 * attribute: 'STATS::Ndvi::Mean'
 * is done by writing labelObject->GetAttribute("STATS::Ndvi::Mean")
 *
 * The ReducedAttributesSet flag allows to tell the internal
 * statistics filter to compute only the main attributes.
 *
 * \sa MultiStatsAttributesLabelObjectFunctor AttributesMapLabelObject
 * \sa GEMI NDVI IR IC IB NDWI2
 * \sa VectorIntensityImageFilter
 *
 * \ingroup ImageEnhancement MathematicalMorphologyImageFilters
 */
template<class TImage, class TFeatureImage>
class ITK_EXPORT RadiometricAttributesLabelMapFilter
  : public LabelMapFeaturesFunctorImageFilter
  <TImage,
      typename Functor::MultiStatsAttributesLabelObjectFunctor
      <typename TImage::LabelObjectType, otb::Image<double, 2> > >
{
public:
  /** Some convenient typedefs. */
  typedef TImage                                       ImageType;
  typedef typename ImageType::LabelObjectType          LabelObjectType;
  typedef TFeatureImage                                FeatureImageType;
  typedef typename FeatureImageType::InternalPixelType FeatureInternalPixelType;
  typedef double                                       InternalPrecisionType;
  typedef Image<InternalPrecisionType, 2>              InternalImageType;

  /** Functor typedef */
  typedef Functor::MultiStatsAttributesLabelObjectFunctor
  <LabelObjectType, InternalImageType>                    FunctorType;

  /** Standard class typedefs. */
  typedef RadiometricAttributesLabelMapFilter Self;
  typedef LabelMapFeaturesFunctorImageFilter
  <ImageType, FunctorType>                                Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** ImageDimension constants */
  itkStaticConstMacro(ImageDimension, unsigned int, TImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(RadiometricAttributesLabelMapFilter, LabelMapFeaturesFunctorImageFilter);

  /// Fuctors typedef

  // GEMI
  typedef Functor::GEMI <FeatureInternalPixelType,
      FeatureInternalPixelType, InternalPrecisionType>        GEMIFunctorType;
  typedef MultiChannelRAndNIRIndexImageFilter
  <FeatureImageType, InternalImageType, GEMIFunctorType>  GEMIFilterType;
  typedef typename GEMIFilterType::Pointer GEMIFilterPointerType;

  // NDVI
  typedef Functor::NDVI <FeatureInternalPixelType,
      FeatureInternalPixelType, InternalPrecisionType>        NDVIFunctorType;
  typedef MultiChannelRAndNIRIndexImageFilter
  <FeatureImageType, InternalImageType, NDVIFunctorType>  NDVIFilterType;
  typedef typename NDVIFilterType::Pointer NDVIFilterPointerType;

  // IR
  typedef Functor::IR <FeatureInternalPixelType,
      FeatureInternalPixelType, InternalPrecisionType>        IRFunctorType;
  typedef MultiChannelGAndRIndexImageFilter
  <FeatureImageType, InternalImageType, IRFunctorType>    IRFilterType;
  typedef typename IRFilterType::Pointer IRFilterPointerType;

  // IC
  typedef Functor::IC <FeatureInternalPixelType,
      FeatureInternalPixelType, InternalPrecisionType>        ICFunctorType;
  typedef MultiChannelGAndRIndexImageFilter
  <FeatureImageType, InternalImageType, ICFunctorType>    ICFilterType;
  typedef typename ICFilterType::Pointer ICFilterPointerType;

  // IB
  typedef Functor::IB <FeatureInternalPixelType,
      FeatureInternalPixelType, InternalPrecisionType>        IBFunctorType;
  typedef MultiChannelGAndRIndexImageFilter
  <FeatureImageType, InternalImageType, IBFunctorType>    IBFilterType;
  typedef typename IBFilterType::Pointer IBFilterPointerType;

  // NDWI2
  typedef Functor::NDWI2 <FeatureInternalPixelType,
      FeatureInternalPixelType, InternalPrecisionType>        NDWI2FunctorType;
  typedef itk::UnaryFunctorImageFilter
  <FeatureImageType, InternalImageType, NDWI2FunctorType> NDWI2FilterType;
  typedef typename NDWI2FilterType::Pointer NDWI2FilterPointerType;

  // Intensity
  typedef VectorImageToIntensityImageFilter
  <FeatureImageType, InternalImageType>                   IntensityFilterType;
  typedef typename IntensityFilterType::Pointer IntensityFilterPointerType;

  // Channels
  typedef MultiToMonoChannelExtractROI
  <FeatureInternalPixelType, InternalPrecisionType>       ChannelFilterType;
  typedef typename ChannelFilterType::Pointer ChannelFilterPointerType;

  /** Set the feature image */
  void SetFeatureImage(const TFeatureImage *input);

  /** Get the feature image */
  const FeatureImageType * GetFeatureImage() const;

  /** Set Input1 (for backward compatibility) */
  void SetInput1(const TImage * input);

  /** Get Input1 (for backward compatibility) */
  const TImage * GetInput1() const;

  /** Set Input2 (for backward compatibility) */
  void SetInput2(const TFeatureImage * input);

  /** Get Input2 (for backward compatibility) */
  const TFeatureImage * GetInput2() const;

  /** Set/Get the red channel index */
  itkSetMacro(RedChannelIndex, unsigned int);
  itkGetMacro(RedChannelIndex, unsigned int);

  /** Set/Get the blue channel index */
  itkSetMacro(BlueChannelIndex, unsigned int);
  itkGetMacro(BlueChannelIndex, unsigned int);

  /** Set/Get the green channel index */
  itkSetMacro(GreenChannelIndex, unsigned int);
  itkGetMacro(GreenChannelIndex, unsigned int);

  /** Set/Get the near infra-red channel index */
  itkSetMacro(NIRChannelIndex, unsigned int);
  itkGetMacro(NIRChannelIndex, unsigned int);

  /** Set the reduced attribute set */
  void SetReducedAttributeSet(bool flag);

  /** Get the reduced attribute set */
  bool GetReducedAttributeSet() const;

  itkBooleanMacro(ReducedAttributeSet);

protected:
  /** Constructor */
  RadiometricAttributesLabelMapFilter();
  /** Destructor */
  ~RadiometricAttributesLabelMapFilter() {}

  /** Before threaded data generation */
  virtual void BeforeThreadedGenerateData();

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  RadiometricAttributesLabelMapFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** The red channel index */
  unsigned int m_RedChannelIndex;

  /** The green channel index */
  unsigned int m_GreenChannelIndex;

  /** The blue channel index */
  unsigned int m_BlueChannelIndex;

  /** The near infra-red channel index */
  unsigned int m_NIRChannelIndex;
}; // end of class

} // end namespace itk

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRadiometricAttributesLabelMapFilter.txx"
#endif

#endif
