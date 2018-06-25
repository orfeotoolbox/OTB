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


#ifndef otbConfusionMatrixCalculator_h
#define otbConfusionMatrixCalculator_h

#include "otbMacro.h"
#include "otbConfusionMatrixMeasurements.h"

namespace otb
{
/** \class ConfusionMatrixCalculator
 *  This class computes a confusion matrix from 2 lists of labels. It
 *  assumes that the 2 lists have the same length and uses the
 *  position of the labels in the lists to build the pairs
 *  reference/produced labels.
 *
 *  The rows and columns of the output confusion matrix are sorted according to increasing class labels.
 *
 *  For a 2 classes problem, the confusion matrix is organized as follows:
 *  \f[ \left( \begin{array}{cc} True Positives & False Negatives \\ False Positives & True Negatives \end{array} \right) \f]
 *
 *  Please note that when accessing the confusion matrix values, the first index is the row index (reference samples),
 *  and the second is the column index (produced samples), so that accessing the false positive rate is done by
 *  calling GetConfusionMatrix()[1, 0] for the case of a 2 classes problem.
 *
 *  Some measurements are computed by this class:
 *  If we consider true positive (TP), true negative (TN), false positive (FP) and false negative (FP) rates, then in the 2 classes case:
 *  \f[ precision = \frac{TP}{\left( TP + FP \right) }  \f]
 *  \f[ recall = \frac{TP}{\left( TP + FN \right) }  \f]
 *  \f[ FScore = \frac{2 * precision * recall}{\left( precision + recall \right) }  \f]
 *
 *  In case of multiclasses problem, these measurements are extended by considering one class versus others.
 *
 *  Moreover overall accuracy and \f[ \kappa \f] index are computed.
 *
 * \ingroup OTBSupervised
 */
template <class TRefListLabel, class TProdListLabel>
class ITK_EXPORT ConfusionMatrixCalculator :
  public itk::Object
{
public:
  /** Standard class typedefs */
  typedef ConfusionMatrixCalculator                 Self;
  typedef itk::Object                               Superclass;
  typedef itk::SmartPointer<Self>                   Pointer;
  typedef itk::SmartPointer<const Self>             ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ConfusionMatrixCalculator, itk::Object);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** List to store the corresponding labels */
  typedef TRefListLabel                                                         RefListLabelType;
  typedef typename RefListLabelType::Pointer                                    RefListLabelPointerType;

  typedef TProdListLabel                                                        ProdListLabelType;
  typedef typename ProdListLabelType::Pointer                                   ProdListLabelPointerType;

  typedef typename RefListLabelType::ValueType::ValueType                       ClassLabelType;
  typedef std::map<ClassLabelType, int>                                         MapOfClassesType;
  typedef std::map<int, ClassLabelType>                                         MapOfIndicesType;

  /** Type for the confusion matrix */
  typedef itk::VariableSizeMatrix<unsigned long>                                ConfusionMatrixType;

  /** Type for the confusion matrix measurements calculator*/
  typedef otb::ConfusionMatrixMeasurements<ConfusionMatrixType, ClassLabelType> ConfusionMatrixMeasurementsType;

  /** Type for the measurement */
  typedef itk::VariableLengthVector<double>                                     MeasurementType;

  /** Computes m_ConfusionMatrix and then the measurements over it. */
  void Compute(void);

  /** Accessors */
  itkSetObjectMacro(ReferenceLabels, RefListLabelType);
  itkGetConstObjectMacro(ReferenceLabels, RefListLabelType);
  itkSetObjectMacro(ProducedLabels, ProdListLabelType);
  itkGetConstObjectMacro(ProducedLabels, ProdListLabelType);
  itkGetMacro(TruePositiveValues, MeasurementType);
  itkGetMacro(TrueNegativeValues, MeasurementType);
  itkGetMacro(FalsePositiveValues, MeasurementType);
  itkGetMacro(FalseNegativeValues, MeasurementType);
  itkGetMacro(TruePositiveValue, double);
  itkGetMacro(TrueNegativeValue, double);
  itkGetMacro(FalsePositiveValue, double);
  itkGetMacro(FalseNegativeValue, double);
  itkGetMacro(KappaIndex, double);
  itkGetMacro(OverallAccuracy, double);
  itkGetMacro(Precisions, MeasurementType);
  itkGetMacro(Recalls, MeasurementType);
  itkGetMacro(FScores, MeasurementType);
  itkGetMacro(Precision, double);
  itkGetMacro(Recall, double);
  itkGetMacro(FScore, double);
  itkGetMacro(NumberOfClasses, unsigned short);
  itkGetMacro(NumberOfSamples, unsigned long);
  itkGetMacro(ConfusionMatrix, ConfusionMatrixType);

  /* Gives the correspondence between a class label
   * and its index in the confusion matrix
   */
  MapOfClassesType GetMapOfClasses() const
  {
    return m_MapOfClasses;
  }

  /* Gives the correspondence between an index in the
   * confusion matrix and the class label
   */
  MapOfIndicesType GetMapOfIndices() const
  {
    return m_MapOfIndices;
  }

protected:
  ConfusionMatrixCalculator();
  ~ConfusionMatrixCalculator() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;


private:
  ConfusionMatrixCalculator(const Self &) = delete;
  void operator =(const Self&) = delete;

  double m_KappaIndex;
  double m_OverallAccuracy;

  MeasurementType m_FalseNegativeValues;
  MeasurementType m_TrueNegativeValues;
  MeasurementType m_FalsePositiveValues;
  MeasurementType m_TruePositiveValues;

  MeasurementType m_Precisions;
  MeasurementType m_Recalls;
  MeasurementType m_FScores;

  double m_FalseNegativeValue;
  double m_TrueNegativeValue;
  double m_FalsePositiveValue;
  double m_TruePositiveValue;

  double m_Precision;
  double m_Recall;
  double m_FScore;

  MapOfClassesType m_MapOfClasses;
  MapOfIndicesType m_MapOfIndices;

  unsigned short m_NumberOfClasses;
  unsigned long  m_NumberOfSamples;

  ConfusionMatrixType m_ConfusionMatrix;
  typename ConfusionMatrixMeasurementsType::Pointer m_ConfMatMeasurements;

  RefListLabelPointerType  m_ReferenceLabels;
  ProdListLabelPointerType m_ProducedLabels;

};
} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbConfusionMatrixCalculator.hxx"
#endif

#endif
