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

#ifndef __otbHooverInstanceFilter_h
#define __otbHooverInstanceFilter_h

#include "itkInPlaceLabelMapFilter.h"
#include "itkVariableSizeMatrix.h"
#include "itkVariableLengthVector.h"
#include <set>

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
 * attributes are not reseted but removed before computing Hoover instances.
 * (see Hoover et al., "An experimental comparison of range image segmentation algorithms", IEEE PAMI vol. 18, no. 7, July 1996)
 * 
 * \sa HooverMatrixFilter
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
  typedef typename LabelMapType::LabelObjectContainerType   LabelObjectContainerType;
  typedef typename LabelObjectContainerType::const_iterator LabelObjectContainerTypeConstIterator;
  typedef typename LabelMapType::LabelObjectType        LabelObjectType;
  typedef typename LabelObjectType::AttributesValueType AttributesValueType;
  typedef typename LabelMapType::LabelVectorType        LabelVectorType;
  typedef typename LabelMapType::RegionType             ImageRegionType;

  typedef typename LabelObjectType::LineContainerType   LineContainerType;
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
  const LabelMapType* GetMachineSegmentationLabelMap();
  
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

  /** Attribute name for correct detection */
  static const std::string ATTRIBUTE_CD;
  
  /** Attribute name for over segmentation */
  static const std::string ATTRIBUTE_OS;
  
  /** Attribute name for under segmentation */
  static const std::string ATTRIBUTE_US;
  
  /** Attribute name for missing region */
  static const std::string ATTRIBUTE_M;
  
  /** Attribute name for noise region */
  static const std::string ATTRIBUTE_N;
  
  /** Attribute name for correct detection score */
  static const std::string ATTRIBUTE_RC;
  
  /** Attribute name for fragmentation score (over segmentation) */
  static const std::string ATTRIBUTE_RF;
  
  /** Attribute name for aggregation score (under segmentation) */
  static const std::string ATTRIBUTE_RA;
  
  /** Attribute name for missed score */
  static const std::string ATTRIBUTE_RM;
  
  /** Attribute name for noise score */
  static const std::string ATTRIBUTE_RN;

protected:
  HooverInstanceFilter();
  ~HooverInstanceFilter() {};
  
  /** Re implement the allocate output method to handle the second output correctly */
  virtual void AllocateOutputs();
  
  /** Re implement the release input method to handle the second input correctly */
  virtual void ReleaseInputs();
  
  /** Actions :
   *    - Fill cardinalities of GT regions
   */
  virtual void ThreadedProcessLabelObject( LabelObjectType * labelObject );
  
  /** Actions:
   *    - Check matrix size
   *    - Init cardinalities lists
   *    - Fill cardinalities list for MS (GT is done by ThreadedProcessLabelObject)
   */
  virtual void BeforeThreadedGenerateData();
  
  /** Actions :
   *    - Compute Hoover instances
   */
  virtual void AfterThreadedGenerateData();

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
#include "otbHooverInstanceFilter.txx"
#endif

#endif
