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
#ifndef __otbConfusionMatrixToMassOfBelief_h
#define __otbConfusionMatrixToMassOfBelief_h

// First make sure that the configuration is available.
// This line can be removed once the optimized versions
// gets integrated into the main directories.
#include "itkConfigure.h"

#include "itkListSample.h"
#include "otbConfusionMatrixCalculator.h"


namespace otb
{
/** \class ConfusionMatrixToMassOfBelief
 *  \brief This class converts a confusion matrix into masses of belief for each label
 *
 *  This class converts a confusion matrix into a std::map<ClassLabelType, MassType> with MassType being
 *  the type of the Mass of Belief of each label to be used in the Dempster-Shafer theory.
 *
 *  TConfusionMatrixCalculator is the type of the confusion matrix calculator
 *  which generates the confusion matrix from reference and classified samples.
 *  N.B.: The confusion matrix indices are expected to be organized the following way:
 *  confusionMatrix[itReference][itClassified], with itClassified the indice
 *  over the columns representing the classified labels compared to the reference
 *  labels (i.e. the reference truth) organized over the lines of the confusion matrix.
 *
 *  The MassOfBeliefDefinitionMethod indicates which parameter of the confusion matrix will
 *  stand for the mass of belief of each label.
 *
 */

template < class TConfusionMatrixCalculator >
class ITK_EXPORT ConfusionMatrixToMassOfBelief : public itk::DataObject
{
public:
  /** Standard class typedefs */
  typedef ConfusionMatrixToMassOfBelief          Self;
  typedef itk::DataObject                        Superclass;
  typedef itk::SmartPointer<Self>                Pointer;
  typedef itk::SmartPointer<const Self>          ConstPointer;
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(ConfusionMatrixToMassOfBelief, DataObject);

  typedef TConfusionMatrixCalculator                            ConfusionMatrixCalculatorType;
  typedef typename TConfusionMatrixCalculator::Pointer          ConfusionMatrixCalculatorPointerType;
  typedef typename TConfusionMatrixCalculator::ClassLabelType   ClassLabelType;
  typedef std::map<int, ClassLabelType>                         MapOfIndicesType;

  typedef double                                                MassType;
  typedef std::map<ClassLabelType, MassType>                    SingleClassLabelMassMapType;

  enum MassOfBeliefDefinitionMethod {PRECISION, RECALL};

  virtual void Update();

  /** Accessors */
  itkSetMacro(ConfusionMatrixCalculator, ConfusionMatrixCalculatorPointerType);
  itkSetMacro(DefinitionMethod, MassOfBeliefDefinitionMethod);

  SingleClassLabelMassMapType GetMapMassOfBelief() const
  {
    return m_MapMassOfBelief;
  }


protected:
  /** Constructor */
  ConfusionMatrixToMassOfBelief();
  
  /** Destructor */
  virtual ~ConfusionMatrixToMassOfBelief() {};
  
  /** Triggers the computation of the confusion matrix */
  void GenerateData(void);

private:
  ConfusionMatrixToMassOfBelief(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  ConfusionMatrixCalculatorPointerType m_ConfusionMatrixCalculator;
  MassOfBeliefDefinitionMethod m_DefinitionMethod;
  SingleClassLabelMassMapType m_MapMassOfBelief;
};

} // end namespace otb


#ifndef OTB_MANUAL_INSTANTIATION
#include "otbConfusionMatrixToMassOfBelief.txx"
#endif

#endif

