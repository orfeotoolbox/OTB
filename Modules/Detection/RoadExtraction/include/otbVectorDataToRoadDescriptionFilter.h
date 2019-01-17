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

#ifndef otbVectorDataToRoadDescriptionFilter_h
#define otbVectorDataToRoadDescriptionFilter_h

#include "otbVectorDataToSpecificDescriptionFilterBase.h"
#include "otbVectorData.h"

//#include "otbNDVIDataNodeFeatureFunction.h"
//#include "otbSpectralAngleDataNodeFeatureFunction.h"
#include "otbParserConditionDataNodeFeatureFunction.h"
#include "otbDBOverlapDataNodeFeatureFunction.h"

namespace otb
{
/** \class VectorDataToRoadDescriptionFilter
  * \brief VectorDataToRoadDescriptionFilter associates a score to each
  * adapted input VectorData DataNode.
  *
  * VectorDataToRoadDescriptionFilter is dedicated to road description.
  * Input and output are both VectorDatas in index coordinate.
  * Each DataNode of the output VectorData contains its scores
  * regarding to the associated descriptors.
  * The support is an Optical Image and the descriptor are:
  * - (NDVI >= threshold) per cent along the tested lines
  * - mean Spectral Angle regarding a reference pixel along the tested lines
  * - DB Overlap compute the number of building crossed by the data node in percent
  *   regarding all the buildings within a data node neighborhood
  *
  * \ingroup VectorDataFilter
  * \sa VectorDataToSpecificDescriptionFilterBase
  * \sa VectorDataToBuildingDescriptionFilter
 *
 * \ingroup OTBRoadExtraction
 */

template <class TVectorData, class TOpticalImage>
class ITK_EXPORT VectorDataToRoadDescriptionFilter :
public otb::VectorDataToSpecificDescriptionFilterBase<TVectorData>
{
public:
  /** Standard class typedefs. */
  typedef VectorDataToRoadDescriptionFilter            Self;
  typedef otb::VectorDataToSpecificDescriptionFilterBase<TVectorData>
                                                       Superclass;
  typedef itk::SmartPointer<Self>                      Pointer;
  typedef itk::SmartPointer<const Self>                ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorDataToRoadDescriptionFilter, VectorDataToSpecificDescriptionFilterBase);

  /** Some typedefs. */
  typedef TVectorData                             VectorDataType;
  typedef typename TVectorData::Pointer           VectorDataPointer;
  typedef typename VectorDataType::DataNodeType   DataNodeType;
  typedef itk::PreOrderTreeIterator<typename VectorDataType::DataTreeType>
                                                  TreeIteratorType;
  typedef typename DataNodeType::ValuePrecisionType
                                                  PrecisionType;
  typedef typename DataNodeType::PrecisionType
                                                  CoordRepType;

  typedef TOpticalImage                           OpticalImageType;
  typedef typename TOpticalImage::Pointer         OpticalImageTypePointer;

  typedef std::vector<std::string>                DescriptorsListType;

  /*
  typedef otb::NDVIDataNodeFeatureFunction<OpticalImageType, CoordRepType, PrecisionType>
                                                  NDVIFeatureFunctionType;
  typedef otb::SpectralAngleDataNodeFeatureFunction<OpticalImageType, CoordRepType, PrecisionType>
                                                  SpectralAngleFeatureFunctionType;
  */
  typedef otb::ParserConditionDataNodeFeatureFunction<OpticalImageType, CoordRepType, PrecisionType>
                                                  ParserConditionFeatureFunctionType;

  typedef otb::DBOverlapDataNodeFeatureFunction<CoordRepType, PrecisionType>
                                                  DBOverlapFeatureFunctionType;

  typedef itk::DataObject::Pointer DataObjectPointer;
  typedef itk::DataObject          DataObject;

  virtual void AddOpticalImage(const OpticalImageType * support);
  const OpticalImageType * GetOpticalImage();

  virtual void AddBuildingsDB(const VectorDataType * support);
  const VectorDataType * GetBuildingsDB();

  const DescriptorsListType& GetDescriptorsList() const
  {
    return m_DescriptorsList;
  }

protected:
  /** Constructor */
  VectorDataToRoadDescriptionFilter();
  /** Destructor */
  ~VectorDataToRoadDescriptionFilter() override {}
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;
  /** Triggers the Computation of the Descriptors */
  void GenerateData(void) override;

private:
  VectorDataToRoadDescriptionFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  typename ParserConditionFeatureFunctionType::Pointer  m_NDVIFeatureFunction;
  typename ParserConditionFeatureFunctionType::Pointer  m_SpectralAngleFeatureFunction;
  typename DBOverlapFeatureFunctionType::Pointer        m_DBOverlapFeatureFunction;
  DescriptorsListType m_DescriptorsList;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorDataToRoadDescriptionFilter.hxx"
#endif

#endif
