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

#ifndef otbConfusionMatrixToMassOfBelief_h
#define otbConfusionMatrixToMassOfBelief_h

// First make sure that the configuration is available.
// This line can be removed once the optimized versions
// gets integrated into the main directories.
#include "itkConfigure.h"

#include "itkProcessObject.h"
#include "otbConfusionMatrixMeasurements.h"


namespace otb
{
/** \class ConfusionMatrixToMassOfBelief
 *  \brief This class converts a confusion matrix into masses of belief for each label
 *
 *  This class converts a confusion matrix into a std::map<ClassLabelType, MassType> with MassType being
 *  the type of the Mass of Belief of each label to be used in the Dempster-Shafer theory.
 *
 *  TConfusionMatrix is the type of the confusion matrix from which the measurements will be computed.
 *  N.B.: The confusion matrix indices are expected to be organized the following way:
 *  confusionMatrix[itReference][itClassified], with itClassified the indice
 *  over the columns representing the classified labels compared to the reference
 *  labels (i.e. the reference truth) organized over the lines of the confusion matrix.
 *
 *  The MassOfBeliefDefinitionMethod indicates which parameter of the confusion matrix will
 *  stand for the mass of belief of each label (PRECISION, RECALL, ACCURACY, KAPPA).
 *
 *
 * \ingroup OTBDempsterShafer
 */

template <class TConfusionMatrix = itk::VariableSizeMatrix<double>, class TLabel = int >
class ITK_EXPORT ConfusionMatrixToMassOfBelief :
  public itk::ProcessObject
{
public:
  /** Standard class typedefs */
  typedef ConfusionMatrixToMassOfBelief             Self;
  typedef itk::ProcessObject                        Superclass;
  typedef itk::SmartPointer<Self>                   Pointer;
  typedef itk::SmartPointer<const Self>             ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ConfusionMatrixToMassOfBelief, itk::ProcessObject);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);


  /** Type for the confusion matrix */
  typedef TConfusionMatrix                                                      ConfusionMatrixType;

  /** Type for the labels */
  typedef TLabel                                                                ClassLabelType;

  /** Type for the confusion matrix measurements calculator*/
  typedef otb::ConfusionMatrixMeasurements<ConfusionMatrixType, ClassLabelType> ConfusionMatrixMeasurementsType;
  typedef typename ConfusionMatrixMeasurementsType::MapOfClassesType            MapOfClassesType;
  typedef typename ConfusionMatrixMeasurementsType::MapOfIndicesType            MapOfIndicesType;

  typedef double                                                                MassType;
  typedef std::map<ClassLabelType, MassType>                                    LabelMassMapType;

  enum MassOfBeliefDefinitionMethod {PRECISION, RECALL, ACCURACY, KAPPA};

  void Update() override;

  /** Accessors */
  itkSetMacro(ConfusionMatrix, ConfusionMatrixType);
  itkSetMacro(DefinitionMethod, MassOfBeliefDefinitionMethod);

  /* Gives the correspondence between a class label
   * and its index in the confusion matrix
   */
  virtual void SetMapOfClasses(const MapOfClassesType _arg)
  {
    m_MapOfClasses = _arg;
    typename MapOfClassesType::iterator itMapOfClasses;
    m_MapOfIndices.clear();
    for (itMapOfClasses = m_MapOfClasses.begin(); itMapOfClasses != m_MapOfClasses.end(); ++itMapOfClasses)
      {
      m_MapOfIndices[itMapOfClasses->second] = itMapOfClasses->first;
      }
  }

  MapOfClassesType GetMapOfClasses() const
  {
    return m_MapOfClasses;
  }

  /* Gives the correspondence between an index in the
   * confusion matrix and the class label
   */
  virtual void SetMapOfIndices(const MapOfIndicesType _arg)
  {
    m_MapOfIndices = _arg;
    typename MapOfIndicesType::iterator itMapOfIndices;
    m_MapOfClasses.clear();
    for (itMapOfIndices = m_MapOfIndices.begin(); itMapOfIndices != m_MapOfIndices.end(); ++itMapOfIndices)
      {
      m_MapOfClasses[itMapOfIndices->second] = itMapOfIndices->first;
      }
  }

  MapOfIndicesType GetMapOfIndices() const
  {
    return m_MapOfIndices;
  }


  LabelMassMapType GetMapMassOfBelief() const
  {
    return m_MapMassOfBelief;
  }


protected:
  /** Constructor */
  ConfusionMatrixToMassOfBelief();

  /** Destructor */
  ~ConfusionMatrixToMassOfBelief() override {};

  /** Triggers the computation of the confusion matrix */
  void GenerateData(void) override;

private:
  ConfusionMatrixToMassOfBelief(const Self&) = delete;
  void operator=(const Self&) = delete;

  ConfusionMatrixType m_ConfusionMatrix;
  typename ConfusionMatrixMeasurementsType::Pointer m_ConfMatMeasurements;

  MapOfClassesType m_MapOfClasses;
  MapOfIndicesType m_MapOfIndices;

  MassOfBeliefDefinitionMethod m_DefinitionMethod;
  LabelMassMapType m_MapMassOfBelief;
};

} // end namespace otb


#ifndef OTB_MANUAL_INSTANTIATION
#include "otbConfusionMatrixToMassOfBelief.hxx"
#endif

#endif

