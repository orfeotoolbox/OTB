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
#ifndef __otbBandsStatisticsAttributesLabelMapFilter_h
#define __otbBandsStatisticsAttributesLabelMapFilter_h

#include "otbStatisticsAttributesLabelMapFilter.h"
#include "otbMultiToMonoChannelExtractROI.h"

namespace otb
{
namespace Functor
{
/** \class BandStatsAttributesLabelObjectFunctor
*   \brief Functor to compute bands statistics attributes.
*
* For one label object, this functors applies the
* StatisticsAttributesLabelObjectFunctor one each feature image
* provided through AddFeature()
*
* As such, it allows computing in one pass statistics related to
* multiple features. It is used in the
* BandsStatisticsAttributesLabelMapFilter.
*
* Features can be added, removed or cleared via the appropriate
* methods.
*
*   \sa BandsStatisticsAttributesLabelMapFilter
*   \sa StatisticsAttributesLabelObjectFunctor
 *
 * \ingroup OTBLabelMap
*/
template <class TLabelObject, class TFeatureImage>
class BandStatsAttributesLabelObjectFunctor
{
public:
  // Self typedef
  typedef BandStatsAttributesLabelObjectFunctor Self;

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
  BandStatsAttributesLabelObjectFunctor();

  /** Destructor */
  virtual ~BandStatsAttributesLabelObjectFunctor();

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
  const TFeatureImage * GetFeatureImage(const std::string& name) const;

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

/** \class BandsStatisticsAttributesLabelMapFilter
 *  \brief This filter computes band statistics attributes for each object.
 *
 * Images are supposed to be compatible with otb::VectorImage
 *
 * This filter internally applies the
 * StatisticsAttributesLabelMapFilter on each channel independently
 *
 * The feature name is constructed as:
 * 'STATS' + '::' + 'Band' + band_index + '::' + statistic_name
 *
 * The ReducedAttributesSet flag allows telling the internal
 * statistics filter to compute only the main attributes (mean, variance, skewness and kurtosis).
 *
 * \sa MultiStatsAttributesLabelObjectFunctor AttributesMapLabelObject
 *
 * \ingroup ImageEnhancement MathematicalMorphologyImageFilters
 *
 * \ingroup OTBLabelMap
 */
template<class TImage, class TFeatureImage>
class ITK_EXPORT BandsStatisticsAttributesLabelMapFilter
  : public LabelMapFeaturesFunctorImageFilter
  <TImage,
      typename Functor::BandStatsAttributesLabelObjectFunctor
      <typename TImage::LabelObjectType, otb::Image<double, 2> > >
{
public:
  /** Some convenient typedefs. */
  typedef TImage                                       ImageType;
  typedef typename ImageType::RegionType     InputImageRegionType;
  typedef typename ImageType::LabelObjectType          LabelObjectType;
  typedef TFeatureImage                                FeatureImageType;
  typedef typename FeatureImageType::InternalPixelType FeatureInternalPixelType;
  typedef double                                       InternalPrecisionType;
  typedef Image<InternalPrecisionType, 2>              InternalImageType;

  /** Functor typedef */
  typedef Functor::BandStatsAttributesLabelObjectFunctor
  <LabelObjectType, InternalImageType>                    FunctorType;

  /** Standard class typedefs. */
  typedef BandsStatisticsAttributesLabelMapFilter Self;
  typedef LabelMapFeaturesFunctorImageFilter
  <ImageType, FunctorType>                                Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  typedef typename ImageType::Pointer         ImagePointer;

  /** ImageDimension constants */
  itkStaticConstMacro(ImageDimension, unsigned int, TImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(BandsStatisticsAttributesLabelMapFilter, LabelMapFeaturesFunctorImageFilter);

  /** Set the feature image */
  void SetFeatureImage(const TFeatureImage *input);

  /** Get the feature image */
  const FeatureImageType * GetFeatureImage() const;

  /** Set the reduced attribute set */
  void SetReducedAttributeSet(bool flag);

  /** Get the reduced attribute set */
  bool GetReducedAttributeSet() const;

  itkBooleanMacro(ReducedAttributeSet);

protected:
  /** Constructor */
  BandsStatisticsAttributesLabelMapFilter();
  /** Destructor */
  ~BandsStatisticsAttributesLabelMapFilter() {}

  virtual void AllocateOutputs();

  void GenerateInputRequestedRegion();

  void EnlargeOutputRequestedRegion(itk::DataObject *){};

  /** Before threaded data generation */
  virtual void BeforeThreadedGenerateData();

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  BandsStatisticsAttributesLabelMapFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

}; // end of class

} // end namespace itk

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbBandsStatisticsAttributesLabelMapFilter.txx"
#endif

#endif
