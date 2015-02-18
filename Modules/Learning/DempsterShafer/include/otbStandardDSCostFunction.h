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
#ifndef __otbStandardDSCostFunction_h
#define __otbStandardDSCostFunction_h

#include "itkSingleValuedCostFunction.h"

#include "otbVectorDataToDSValidatedVectorDataFilter.h"

namespace otb
{
/** \class StandardDSCostFunction
  * \brief Standard Cost Function used to estimate the fuzzy model parameters
  * in the Dempster-Shafer framework
  *
  * This class has been developed to estimate, with the help of the Amoeba
  * optimizer, the fuzzy model parameters to be used in the class
  * otb::VectorDataToDSValidatedVectorDataFilter. The cost value compute the
  * cost according to:
  * - an enriched ground truth vector data (using VectorDataToRoadDescription)
  * - an enriched negative sample VectorData or at least random samples
  * - an hypothesis (the same as the considered DSValidationFilter)
  *      (by default (NONDVI, ROADSA, NOBUIL))
  * - a weight between 0 and 1 (0.5 by default) corresponding to the situation
  *      policy regarding under detection/false detection (1 no under detection
  *      0 no false detection)
  * For now the cost function use the NONDVI Feature, ROADSA Feature and and
  * the NOBUIL Feature.
  * For each evolution of the VectorDataToDSValidatedVectorDataFilter,
  * this cost function must be adapted.
  *
  * \ingroup CostFunction
  * \sa VectorDataToDSValidatedVectorDataFilter
  * \sa AmoebaOptimizer
 */

template <class TDSValidationFilter>
class ITK_EXPORT StandardDSCostFunction :
public itk::SingleValuedCostFunction
{
public:
  /** Standard class typedefs. */
  typedef StandardDSCostFunction                       Self;
  typedef itk::SingleValuedCostFunction                Superclass;
  typedef itk::SmartPointer<Self>                      Pointer;
  typedef itk::SmartPointer<const Self>                ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(StandardDSCostFunction, itk::SingleValuedCostFunction);

  typedef Superclass::MeasureType                      MeasureType;   //double
  typedef Superclass::DerivativeType                   DerivativeType; //Array<double>
  typedef Superclass::ParametersType                   ParametersType; //Array<double>

  typedef TDSValidationFilter                          DSValidationFilterType;
  typedef typename DSValidationFilterType::VectorDataType
                                                       VectorDataType;
  typedef typename DSValidationFilterType::TreeIteratorType
                                                       TreeIteratorType;
  typedef typename DSValidationFilterType::LabelSetType
                                                       LabelSetType;

  typedef FuzzyDescriptorsModelManager                 FuzzyDescriptorsModelManagerType;

  typedef FuzzyDescriptorsModelManager::DescriptorListType
                                                       DescriptorListType;
  typedef FuzzyDescriptorsModelManager::DescriptorsModelType
                                                       DescriptorsModelType;
  typedef FuzzyDescriptorsModelManager::PairType       PairType;

  typedef Parser                                       ParserType;

  /** This method returns the value of the cost function corresponding
    * to the specified parameters.    */
  virtual MeasureType GetValue( const ParametersType & parameters ) const;

  /** This method returns the derivative of the cost function corresponding
    * to the specified parameters.   */
  virtual void GetDerivative( const ParametersType & parameters,
                               DerivativeType & derivative ) const;

  virtual unsigned int GetNumberOfParameters(void) const;

  itkSetMacro(Weight, double);
  itkGetConstMacro(Weight, double);

  itkSetMacro(CriterionFormula, std::string);
  itkGetConstMacro(CriterionFormula, std::string);

  void SetDescriptorList(DescriptorListType list)
  {
    m_DescriptorList = list;
  }

  DescriptorListType GetDescriptorList()
  {
    return m_DescriptorList;
  }

  itkSetObjectMacro(GTVectorData, VectorDataType);
  itkGetConstObjectMacro(GTVectorData, VectorDataType);

  itkSetObjectMacro(NSVectorData, VectorDataType);
  itkGetConstObjectMacro(NSVectorData, VectorDataType);


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

protected:
  /** Constructor */
  StandardDSCostFunction();
  /** Destructor */
  virtual ~StandardDSCostFunction() {}
  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  StandardDSCostFunction(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented


  typename VectorDataType::Pointer            m_GTVectorData; //Ground Truth
  typename VectorDataType::Pointer            m_NSVectorData; //Negative Samples

  typename ParserType::Pointer       m_Parser;
  std::string                        m_CriterionFormula;

  double                             m_Weight; //range ]0; 1[

  LabelSetType                       m_BeliefHypothesis;
  LabelSetType                       m_PlausibilityHypothesis;

  DescriptorListType                 m_DescriptorList;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStandardDSCostFunction.txx"
#endif

#endif
