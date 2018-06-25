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

#ifndef otbVectorDataToDSValidatedVectorDataFilter_h
#define otbVectorDataToDSValidatedVectorDataFilter_h

#include "otbMassOfBelief.h"

#include "otbVectorDataToVectorDataFilter.h"

#include "itkPreOrderTreeIterator.h"
#include "otbVectorData.h"
#include "otbFuzzyDescriptorsModelManager.h"

#include "otbFuzzyVariable.h"
#include "otbJointMassOfBeliefFilter.h"
#include "otbParser.h"

namespace otb
{
/** \class VectorDataToDSValidatedVectorDataFilter
  * \brief VectorData filter validating data nodes using
  * Dempster-Shafer theory.
  *
  * This filter takes as input a vector data containing
  * features score and validate (or not) each data node
  * according to these features score, a fuzzy model corresponding
  * to each feature and a criterion according to the Dempster-Shafer
  * theory. By default the filter is (for now) parametrized to process
  * VectorDatas providing by the VectorDataToRoadDescriptionFilter.
  * The hypothesis must be set using the SetHypothesis method.
  * The convention is for each feature Fe, a mass of Belief is defined
  * with two variables named Fe and Fe_. Thus, an hypothesis must declared:
  * VectorDataToVectorDataFilter::LabelSetType hyp;
  * hyp.insert("Fe(i)");
  * hyp.insert("Fe(j)_");
  * hyp.insert("Fe(k)_");
  *
  * Fe(i), Fe(j) and Fe(k) being descriptors.
  *
  * The criterion can be set through SetCriterionFormula() as a
  * string referring to "Belief" and "Plausibility" from the
  * Dempster-Shafer theory. By default the criterion is:
  * "((Belief + Plausibility)/2) >= 0.5"
  *
  * The considered features will be the intersection between
  * the features embedded in this filter and the features embedded
  * in the input vector data. For now, the filter uses "NONDVI" and
  * "ROADSA" features.
  *
  *
  * \ingroup VectorDataFilter
  * \sa VectorDataToRoadDescriptionFilter
  * \sa VectorDataToBuildingDescriptionFilter
 *
 * \ingroup OTBDempsterShafer
 */

template <class TVectorData, class TPrecision=float>
class ITK_EXPORT VectorDataToDSValidatedVectorDataFilter :
public otb::VectorDataToVectorDataFilter<TVectorData, TVectorData>
{
public:
  /** Standard class typedefs. */
  typedef VectorDataToDSValidatedVectorDataFilter      Self;
  typedef VectorDataToVectorDataFilter<TVectorData,
    TVectorData>                                       Superclass;
  typedef itk::SmartPointer<Self>                      Pointer;
  typedef itk::SmartPointer<const Self>                ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorDataToDSValidatedVectorDataFilter, VectorDataToVectorDataFilter);

  /** Some typedefs. */
  typedef TVectorData                               VectorDataType;
  typedef typename VectorDataType::Pointer          VectorDataPointer;
  typedef typename VectorDataType::DataNodeType     DataNodeType;
  typedef itk::PreOrderTreeIterator<typename VectorDataType::DataTreeType>
                                                    TreeIteratorType;

  typedef TPrecision                                PrecisionType;
  typedef MassOfBelief<std::string, PrecisionType>
                                                    MassOfBeliefFunctionType;
  typedef typename MassOfBeliefFunctionType::LabelSetType
                                                    LabelSetType;
  typedef JointMassOfBeliefFilter<MassOfBeliefFunctionType>
                                                    JointMassOfBeliefFilterType;
  typedef Parser                                    ParserType;

  typedef itk::DataObject::Pointer DataObjectPointer;
  typedef itk::DataObject          DataObject;


  typedef FuzzyVariable<std::string, PrecisionType>           FuzzyVarType;

  typedef FuzzyDescriptorsModelManager::PairType              PairType;
  typedef FuzzyDescriptorsModelManager::DescriptorsModelType  DescriptorModelsType;

  /** Descriptor model accessors. */
  virtual void AddDescriptor(const std::string& key, std::vector<double> model);
  virtual void ClearDescriptors();

  void SetDescriptorModels( DescriptorModelsType model );
  DescriptorModelsType GetDescriptorModels()
  {
    return m_DescriptorModels;
  }

  /** Parameter accessors. */
  itkGetConstMacro(CriterionFormula, std::string);
  itkSetMacro(CriterionFormula, std::string);

  itkGetMacro(CriterionThreshold, double);
  itkSetMacro(CriterionThreshold, double);

  LabelSetType GetBeliefHypothesis()
  {
    return m_BeliefHypothesis;
  }

  void SetBeliefHypothesis(LabelSetType hypothesis)
  {
    m_BeliefHypothesis = hypothesis;
  }

  LabelSetType GetPlausibilityHypothesis()
  {
    return m_PlausibilityHypothesis;
  }

  void SetPlausibilityHypothesis(LabelSetType hypothesis)
  {
    m_PlausibilityHypothesis = hypothesis;
  }

  unsigned int GetNumberOfParameters()
  {
    return (m_DescriptorModels.size() * 4);
  }


protected:
  /** Triggers the Computation */
  void GenerateData(void) override;
  /** Constructor */
  VectorDataToDSValidatedVectorDataFilter();
  /** Destructor */
  ~VectorDataToDSValidatedVectorDataFilter() override {}
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  std::string GetNextID()
    {
      std::ostringstream oss;
      oss << m_CurrentID++;
      return oss.str();
    }

private:
  VectorDataToDSValidatedVectorDataFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  // Descriptor bench
  DescriptorModelsType                            m_DescriptorModels;
  std::vector< typename FuzzyVarType::Pointer >   m_FuzzyVars;
  LabelSetType                                    m_Universe,
                                                  m_BeliefHypothesis,
                                                  m_PlausibilityHypothesis;
  double                                          m_Bel, m_Plau;
  typename ParserType::Pointer                    m_Parser;

  std::string                                     m_CriterionFormula;
  double                                          m_CriterionThreshold;
  unsigned int                                    m_CurrentID;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorDataToDSValidatedVectorDataFilter.hxx"
#endif

#endif
