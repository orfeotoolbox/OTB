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

#include "otbFuzzyVariable.h"
#include "otbJointMassOfBeliefFilter.h"
#include "otbParser.h"

namespace otb
{
/** \class VectorDataToVectorDataFilter
  * \brief .
  * 
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

  /** Descriptor model accessors. */
  virtual void AddDescriptor(std::string key, std::vector<double> model);
  virtual void ClearDescriptors();

  /** Parameter accessors. */
  itkGetConstMacro(CriterionFormula, std::string);
  itkSetMacro(CriterionFormula, std::string);

  itkGetConstMacro(CriterionThreshold, double);
  itkSetMacro(CriterionThreshold, double);
  
/*
  itkGetConstMacro(Hypothesis, LabelSetType);
  itkSetMacro(Hypothesis, LabelSetType);
*/
  
  LabelSetType GetHypothesis()
  {
    return m_Hypothesis;
  }
  
  void SetHypothesis(LabelSetType hypothesis)
  {
    m_Hypothesis = hypothesis;
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


private:
  VectorDataToDSValidatedVectorDataFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  // Descriptor bench
  std::vector< std::pair<std::string, std::vector<double> > >
                                                  m_DescriptorModels;
  std::vector< typename FuzzyVarType::Pointer >   m_FuzzyVars;
  LabelSetType                                    m_Universe, m_Hypothesis;
  double                                          m_Bel, m_Plau;
  typename ParserType::Pointer                    m_Parser;

  std::string                                     m_CriterionFormula;
  double                                          m_CriterionThreshold;
  
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorDataToDSValidatedVectorDataFilter.txx"
#endif

#endif
