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
 * The Hoover instances are computed from two label maps and a confusion matrix. It is intended to work with
 * AttributesMapLabelObject, where the instances will be stored.
 * The different instances are : correct detection, under segmentation, over segmentation, missed and noise.
 * (see Hoover et al., "An experimental comparison of range image segmentation algorithms", IEEE PAMI vol. 18, no. 7, July 1996)
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

protected:
  HooverInstanceFilter();
  ~HooverInstanceFilter() {};
  
  /** Re implement the allocate output method to handle the second output correctly */
  virtual void AllocateOutputs();
  
  /** Re implement the release input method to handle the second input correctly */
  virtual void ReleaseInputs();
  
  /** ThreadedProcessLabelObject :
   *    - Fill cardinalities of GT regions
   */
  virtual void ThreadedProcessLabelObject( LabelObjectType * labelObject );
  
  /** BeforeThreadedGenerateData:
   *    - Check matrix size
   *    - Init cardinalities lists
   *    - Fill cardinalities list for MS (GT is done by ThreadedProcessLabelObject)
   */
  virtual void BeforeThreadedGenerateData();
  
  /** AfterThreadedGenerateData :
   *    - Compute Hoover instances
   */
  virtual void AfterThreadedGenerateData();

private:
  
  /** number of label objects found in the input label maps */
  unsigned long     m_NumberOfRegionsGT;  // GT = Ground Truth
  unsigned long     m_NumberOfRegionsMS;  // MS = Machine Segmentation
  LabelVectorType   m_LabelsGT;
  MatrixType        m_HooverMatrix;
  CardinalVector    m_CardRegGT;  // cardinal of GT regions
  CardinalVector    m_CardRegMS;  // cardinal of MS regions
  double            m_Threshold;
};

}

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbHooverInstanceFilter.txx"
#endif

#endif
