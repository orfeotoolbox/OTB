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
#ifndef __otbVectorDataToDSValidatedVectorDataFilter_h
#define __otbVectorDataToDSValidatedVectorDataFilter_h

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
  * string refering to "Belief" and "Plausibility" from the
  * Dempster-Shafer theory. By default the criterion is:
  * "((Belief + Plausibility)/2) >= 0.5"
  *
  * The concidered features will be the intersection between
  * the features embedded in this filter and the features embedded
  * in the input vector data. For now, the filter uses "NDVI" and
  * "RADIOM" features.
  *
  *
  * \ingroup VectorDataFilter
  * \sa VectorDataToRoadDescriptionFilter
  * \sa VectorDataToBuildingDescriptionFilter
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
  typedef FuzzyVariable<std::string, PrecisionType> FuzzyVarType;
  typedef MassOfBelief<std::string, PrecisionType>
                                                    MassOfBeliefFunctionType;
  typedef typename MassOfBeliefFunctionType::LabelSetType
                                                    LabelSetType;
  typedef JointMassOfBeliefFilter<MassOfBeliefFunctionType>
                                                    JointMassOfBeliefFilterType;
  typedef Parser                                    ParserType;

  typedef itk::DataObject::Pointer DataObjectPointer;
  typedef itk::DataObject          DataObject;

  typedef std::pair< std::string, std::vector<double> > PairType;
  typedef std::vector< PairType >                       DescriptorModelsType; ;

  /** Descriptor model accessors. */
  virtual void AddDescriptor(std::string key, std::vector<double> model);
  virtual void ClearDescriptors();

  void SetDescriptorModels( DescriptorModelsType model )
  {
    m_DescriptorModels = model;
    this->Modified();
  }

  DescriptorModelsType GetDescriptorModels()
  {
    return m_DescriptorModels;
  }


  /** Parameter accessors. */
  itkGetConstMacro(CriterionFormula, std::string);
  itkSetMacro(CriterionFormula, std::string);

  /** Fuzzy Models */
  void SetFuzzyModel(std::string key, std::vector<double> model)
  {
    unsigned int nbDescriptor = m_DescriptorModels.size();

    if (model.size() != 4)
      {
      itkExceptionMacro(<< "Wrong model! Size(="
                        << model.size()
                        << ") > 4" )
      }
    else if (model[0]<0 || model[1]<model[0] || model[2]<model[1] || model[2]>1)
    {
      itkExceptionMacro(<< "Wrong model! Values have to be 0<=v1<=v2<=v3<=1" )
    }
    else if (model[3]<.75 || model[3]>1)
    {
      itkExceptionMacro(<< "Wrong model! Values have to be 0.75<=v4<=1" )
    }
    else
      {
       for (unsigned j=0; j<nbDescriptor; j++)
         {
         if (m_DescriptorModels[j].first.compare(key) == 0)
           {
           for (unsigned int i=0; i<4; i++)
             {
             if ((model[i]<=1.) && (model[i]>=0.))
               {
               m_DescriptorModels[j].second.at(i) = model[i];
               }
             else
               {
               itkExceptionMacro(<< "Wrong model! Value must be between 0 and 1")
               }
             }
           }
         }
      }
  }


  LabelSetType GetHypothesis()
  {
    return m_Hypothesis;
  }
  
  void SetHypothesis(LabelSetType hypothesis)
  {
    m_Hypothesis = hypothesis;
  }
  
  unsigned int GetNumberOfParameters()
  {
    return (m_DescriptorModels.size() * 4);
  }


protected:
  /** Triggers the Computation */
  void GenerateData(void);
  /** Constructor */
  VectorDataToDSValidatedVectorDataFilter();
  /** Destructor */
  virtual ~VectorDataToDSValidatedVectorDataFilter() {}
  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

  std::string GetNextID()
    {
      std::ostringstream oss;
      oss << m_CurrentID++;
      return oss.str();
    }

private:
  VectorDataToDSValidatedVectorDataFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  // Descriptor bench
  DescriptorModelsType                            m_DescriptorModels;
  std::vector< typename FuzzyVarType::Pointer >   m_FuzzyVars;
  LabelSetType                                    m_Universe, m_Hypothesis;
  double                                          m_Bel, m_Plau;
  typename ParserType::Pointer                    m_Parser;

  std::string                                     m_CriterionFormula;
  unsigned int                                    m_CurrentID;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorDataToDSValidatedVectorDataFilter.txx"
#endif

#endif
