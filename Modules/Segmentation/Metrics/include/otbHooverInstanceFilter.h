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


#ifndef otbHooverInstanceFilter_h
#define otbHooverInstanceFilter_h

#include <set>
#include "itkInPlaceLabelMapFilter.h"
#include "itkVariableSizeMatrix.h"
#include "itkVariableLengthVector.h"

namespace otb
{
/** \class HooverInstanceFilter
 *
 * \brief This class computes the Hoover instances.
 *
 * The Hoover instances are computed from two segmentations : ground truth (GT) and machine segmentation (MS); and
 * a Hoover confusion matrix. It is intended to work with AttributesMapLabelObject, where the Hoover scores will be stored.
 * The different instances are :
 *    - correct detection (a GT region paired with a MS region)
 *    - under segmentation (a MS region paired with a set of GT regions)
 *    - over segmentation (a GT region paired with a set of MS regions)
 *    - missed (a GT region that doesn't belong to any of the previous instances)
 *    - noise (a MS region that doesn't belong to any of the previous instances)
 *
 * The corresponding scores are :
 *    - RC : for correct detection
 *    - RF : for over segmentation
 *    - RA : for under segmentation
 *    - RM : for missed regions (only for GT)
 *    - RN : for noise regions (only for MS)
 *
 * These Hoover scores that are stored in the label maps are computed between 0 (if the region doesn't belong to this kind of instance) and 1.
 *
 * If the user wants to which region labels have been paired, he can set the flag UseExtendedAttributes. The extended attributes contain the
 * labels that have been paired in Hoover instances:
 *    - ATTRIBUTE_CD (correct detection) : label of the corresponding region in the other segmentation
 *    - ATTRIBUTE_OS[_x] (over segmentation) : label of the opposite over segmented region in GT / labels of the corresponding regions of MS
 *    - ATTRIBUTE_US[_x] (under segmentation) : labels of the corresponding regions of GT / label of the under segmentated region of MS
 *    - ATTRIBUTE_M (missed) : missed region own label (in GT)
 *    - ATTRIBUTE_N (noise) : noise region own label (in MS)
 *
 * These attributes are handled in a different way than the Hoover scores. The simple presence of an extended attribute in a given region has a
 * meaning, regardless of its value. It is assumed that its value always corresponds to an existing region label. This is why these extended
 * attributes are not reset but removed before computing Hoover instances.
 * (see Hoover et al., "An experimental comparison of range image segmentation algorithms", IEEE PAMI vol. 18, no. 7, July 1996)
 *
 * \sa HooverMatrixFilter
 *
 * \ingroup OTBMetrics
 */

template< class TLabelMap >
class ITK_EXPORT HooverInstanceFilter :
    public itk::InPlaceLabelMapFilter< TLabelMap >
{
public:
  /** Standard class typedefs. */
  typedef HooverInstanceFilter   Self;
  typedef itk::InPlaceLabelMapFilter< TLabelMap >      Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard New method. */
  itkNewMacro(Self);
  /** Run-time type information (and related methods). */
  itkTypeMacro(HooverInstanceFilter, InPlaceLabelMapFilter);

  /** Some convenient typedefs. */
  typedef TLabelMap                                     LabelMapType;
  typedef typename LabelMapType::Pointer                LabelMapPointer;
  typedef typename LabelMapType::ConstIterator          ConstIteratorType;
  typedef typename LabelMapType::Iterator               IteratorType;
  typedef typename LabelMapType::LabelObjectType        LabelObjectType;
  typedef typename LabelObjectType::AttributeType       AttributeType;
  typedef typename LabelObjectType::AttributesValueType AttributesValueType;
  typedef typename LabelMapType::LabelVectorType        LabelVectorType;
  typedef typename LabelMapType::RegionType             ImageRegionType;

  //typedef typename LabelObjectType::LineContainerType   LineContainerType;
  typedef typename LabelObjectType::IndexType           IndexType;
  typedef typename LabelObjectType::LabelType           LabelType;

  typedef unsigned long                                 CoefficientType;
  typedef itk::VariableSizeMatrix<CoefficientType>      MatrixType;

  typedef itk::VariableLengthVector<CoefficientType>    CardinalVector;
  typedef std::set<CoefficientType>                     RegionSetType;
  typedef std::vector<LabelObjectType*>                 ObjectVectorType;

  void SetGroundTruthLabelMap(const LabelMapType *gt);
  void SetMachineSegmentationLabelMap(const LabelMapType *ms);

  const LabelMapType* GetGroundTruthLabelMap();
  LabelMapType* GetMachineSegmentationLabelMap();

  LabelMapType* GetOutputGroundTruthLabelMap();
  LabelMapType* GetOutputMachineSegmentationLabelMap();

  itkSetMacro(HooverMatrix, MatrixType);
  itkGetMacro(HooverMatrix, MatrixType);

  itkSetMacro(Threshold, double);
  itkGetMacro(Threshold, double);

  itkSetMacro(UseExtendedAttributes, bool);
  itkGetMacro(UseExtendedAttributes, bool);

  /** Get the average scores (after filter update) */
  itkGetMacro(MeanRC, AttributesValueType);
  itkGetMacro(MeanRF, AttributesValueType);
  itkGetMacro(MeanRA, AttributesValueType);
  itkGetMacro(MeanRM, AttributesValueType);
  itkGetMacro(MeanRN, AttributesValueType);

  enum AttributeTypes {
    ATTRIBUTE_CD=100,
    ATTRIBUTE_OS,
    ATTRIBUTE_US,
    ATTRIBUTE_M,
    ATTRIBUTE_N,
    ATTRIBUTE_RC,
    ATTRIBUTE_RF,
    ATTRIBUTE_RA,
    ATTRIBUTE_RM,
    ATTRIBUTE_RN
  };

  static std::string GetNameFromAttribute( const AttributeType & a )
    {
    std::string name;
    switch( a )
      {
      case ATTRIBUTE_CD: name = "HooverInstance_Ext_CD"; break;
      case ATTRIBUTE_OS: name = "HooverInstance_Ext_OS"; break;
      case ATTRIBUTE_US: name = "HooverInstance_Ext_US"; break;
      case ATTRIBUTE_M:  name = "HooverInstance_Ext_M";  break;
      case ATTRIBUTE_N:  name = "HooverInstance_Ext_N";  break;
      case ATTRIBUTE_RC: name = "HooverInstance_RC";     break;
      case ATTRIBUTE_RF: name = "HooverInstance_RF";     break;
      case ATTRIBUTE_RA: name = "HooverInstance_RA";     break;
      case ATTRIBUTE_RM: name = "HooverInstance_RM";     break;
      case ATTRIBUTE_RN: name = "HooverInstance_RN";     break;
      }
    return name;
    }

  static AttributeType GetAttributeFromName( const std::string & name )
    {
    if ( name == "HooverInstance_Ext_CD" )
      {
      return ATTRIBUTE_CD;
      }
    else if ( name == "HooverInstance_Ext_OS" )
      {
      return ATTRIBUTE_OS;
      }
    else if ( name == "HooverInstance_Ext_US" )
      {
      return ATTRIBUTE_US;
      }
    else if ( name == "HooverInstance_Ext_M" )
      {
      return ATTRIBUTE_M;
      }
    else if ( name == "HooverInstance_Ext_N" )
      {
      return ATTRIBUTE_N;
      }
    else if ( name == "HooverInstance_RC" )
      {
      return ATTRIBUTE_RC;
      }
    else if ( name == "HooverInstance_RF" )
      {
      return ATTRIBUTE_RF;
      }
    else if ( name == "HooverInstance_RA" )
      {
      return ATTRIBUTE_RA;
      }
    else if ( name == "HooverInstance_RM" )
      {
      return ATTRIBUTE_RM;
      }
    else if ( name == "HooverInstance_RN" )
      {
      return ATTRIBUTE_RN;
      }
    }

protected:
  HooverInstanceFilter();
  ~HooverInstanceFilter() override {};

  /** Re implement the allocate output method to handle the second output correctly */
  void AllocateOutputs() override;

  /** Re implement the release input method to handle the second input correctly */
  void ReleaseInputs() override;

  /** Actions :
   *    - Fill cardinalities of GT regions
   */
  void ThreadedProcessLabelObject( LabelObjectType * labelObject ) override;

  /** Actions:
   *    - Check matrix size
   *    - Init cardinalities lists
   *    - Fill cardinalities list for MS (GT is done by ThreadedProcessLabelObject)
   */
  void BeforeThreadedGenerateData() override;

  /** Actions :
   *    - Compute Hoover instances
   */
  void AfterThreadedGenerateData() override;

private:

  /** number of regions (label objects) found in ground truth (GT) */
  unsigned long     m_NumberOfRegionsGT;

  /** number of regions (label objects) found in machine segmentation (MS) */
  unsigned long     m_NumberOfRegionsMS;

  /** List of labels in GT segmentation */
  LabelVectorType   m_LabelsGT;

  /** Hoover confusion matrix computed between GT and MS*/
  MatrixType        m_HooverMatrix;

  /** List of region sizes in GT */
  CardinalVector    m_CardRegGT;

  /** List of region sizes in MS */
  CardinalVector    m_CardRegMS;

  /** Threshold used to compute Hoover instances */
  double            m_Threshold;

  /** Flag to output additional information in label maps */
  bool m_UseExtendedAttributes;

  /** Average score for correct detection on the whole segmentation */
  AttributesValueType m_MeanRC;

  /** Average score for over segmentation on the whole segmentation */
  AttributesValueType m_MeanRF;

  /** Average score for under segmentation on the whole segmentation */
  AttributesValueType m_MeanRA;

  /** Average score for missed detection on the whole segmentation */
  AttributesValueType m_MeanRM;

  /** Average score for noise on the whole segmentation */
  AttributesValueType m_MeanRN;

};

}

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbHooverInstanceFilter.hxx"
#endif

#endif
