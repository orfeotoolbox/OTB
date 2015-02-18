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
#ifndef __otbVectorDataToRoadDescriptionFilter_h
#define __otbVectorDataToRoadDescriptionFilter_h

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
  virtual ~VectorDataToRoadDescriptionFilter() {}
  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;
  /** Triggers the Computation of the Descriptors */
  void GenerateData(void);

private:
  VectorDataToRoadDescriptionFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  typename ParserConditionFeatureFunctionType::Pointer  m_NDVIFeatureFunction;
  typename ParserConditionFeatureFunctionType::Pointer  m_SpectralAngleFeatureFunction;
  typename DBOverlapFeatureFunctionType::Pointer        m_DBOverlapFeatureFunction;
  DescriptorsListType m_DescriptorsList;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorDataToRoadDescriptionFilter.txx"
#endif

#endif
