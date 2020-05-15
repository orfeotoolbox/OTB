/*
 * Copyright (C) 1999-2011 Insight Software Consortium
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbStatisticsAttributesLabelMapFilter_h
#define otbStatisticsAttributesLabelMapFilter_h

#include "otbLabelMapFeaturesFunctorImageFilter.h"
#include "itkMatrix.h"
#include "itkVector.h"
#include <string>

namespace otb
{
namespace Functor
{
/** \class StatisticsAttributesLabelObjectFunctor
*   \brief Functor to compute statistics attributes of one LabelObject.
*
*   \sa StatisticsAttributesLabelMapFilter
 *
 * \ingroup OTBLabelMap
*/
template <class TLabelObject, class TFeatureImage>
class StatisticsAttributesLabelObjectFunctor
{
public:
  // Self typedef
  typedef StatisticsAttributesLabelObjectFunctor Self;

  // Matrix typedef
  typedef typename itk::Matrix<double, TFeatureImage::ImageDimension, TFeatureImage::ImageDimension> MatrixType;
  // Vector typedef
  typedef typename itk::Vector<double, TFeatureImage::ImageDimension> VectorType;

  /// Typedef of the feature image type
  typedef typename TFeatureImage::PixelType FeatureType;

  /// Typedef of the label object
  typedef TLabelObject LabelObjectType;

  /** Const iterator over LabelObject lines */
  typedef typename LabelObjectType::ConstLineIterator ConstLineIteratorType;

  /** Constructor */
  StatisticsAttributesLabelObjectFunctor();

  /** Destructor */
  virtual ~StatisticsAttributesLabelObjectFunctor();

  /** The comparators */
  bool operator!=(const Self& self);
  bool operator==(const Self& self);

  /** This is the functor implementation
   *  Calling the functor on a label object
   *  will update its statistics attributes */
  inline void operator()(LabelObjectType* lo) const;

  /** Set the name of the feature */
  void SetFeatureName(const std::string& name);

  /** Get the feature name */
  const std::string& GetFeatureName() const;

  /** Set the feature image */
  void SetFeatureImage(const TFeatureImage* img);

  /** Get the feature image */
  const TFeatureImage* GetFeatureImage() const;

  /** Set the reduced attribute set */
  void SetReducedAttributeSet(bool flag);

  /** Get the reduced attribute set */
  bool GetReducedAttributeSet() const;

private:
  // The name of the feature
  std::string m_FeatureName;

  // The feature image
  typename TFeatureImage::ConstPointer m_FeatureImage;

  // True to compute only a reduced attribute set
  bool m_ReducedAttributeSet;
};
} // End namespace Functor

/** \class StatisticsAttributesLabelMapFilter
 * \brief This class is a fork of itk::StasticsLabelMapFilter to support AttributesMapLabelObject.
 *
 * To be able to support statistics attributes for multiple features
 * into the same AttributesMapLabelObject, a feature name can be
 * set. The key of the statistics features in the attributes map will
 * be "STATS::FeatureName::StatisticName".
 *
 * The flag ReducedAttributesSet allows computing only non-redundant
 * statistics and statistics useful for classification (not spatially
 * dependent statitistics). Reduced set contains only mean, variance,
 * skewness and kurtosis.
 *
 * \sa itk::StatisticsLabelMapFilter
 *
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 *
 * \ingroup OTBLabelMap
 */
template <class TImage, class TFeatureImage>
class ITK_EXPORT StatisticsAttributesLabelMapFilter
    : public LabelMapFeaturesFunctorImageFilter<TImage,
                                                typename Functor::StatisticsAttributesLabelObjectFunctor<typename TImage::LabelObjectType, TFeatureImage>>
{
public:
  /** Some convenient typedefs. */
  typedef TImage                              ImageType;
  typedef typename ImageType::LabelObjectType LabelObjectType;
  typedef TFeatureImage                       FeatureImageType;

  /** Functor typedef */
  typedef Functor::StatisticsAttributesLabelObjectFunctor<LabelObjectType, FeatureImageType> FunctorType;

  /** Standard class typedefs. */
  typedef StatisticsAttributesLabelMapFilter Self;
  typedef LabelMapFeaturesFunctorImageFilter<ImageType, FunctorType> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** ImageDimension constants */
  itkStaticConstMacro(ImageDimension, unsigned int, TImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(StatisticsAttributesLabelMapFilter, LabelMapFeaturesFunctorImageFilter);

  /** Set the feature image */
  void SetFeatureImage(const TFeatureImage* input);

  /** Get the feature image */
  const FeatureImageType* GetFeatureImage() const;

  /** Set Input1 (for backward compatibility) */
  void SetInput1(const TImage* input);

  /** Get Input1 (for backward compatibility) */
  const TImage* GetInput1() const;

  /** Set Input2 (for backward compatibility) */
  void SetInput2(const TFeatureImage* input);

  /** Get Input2 (for backward compatibility) */
  const TFeatureImage* GetInput2() const;

  /** Set the name of the feature */
  void SetFeatureName(const std::string& name);

  /** Get the feature name */
  const std::string& GetFeatureName() const;

  /** Set the reduced attribute set */
  void SetReducedAttributeSet(bool flag);

  /** Get the reduced attribute set */
  bool GetReducedAttributeSet() const;

  itkBooleanMacro(ReducedAttributeSet);

protected:
  /** Constructor */
  StatisticsAttributesLabelMapFilter();

  /** Destructor */
  ~StatisticsAttributesLabelMapFilter() override;

  /** Before threaded data generation */
  void BeforeThreadedGenerateData() override;

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  StatisticsAttributesLabelMapFilter(const Self&) = delete;
  void operator=(const Self&) = delete;
}; // end of class

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStatisticsAttributesLabelMapFilter.hxx"
#endif

#endif
