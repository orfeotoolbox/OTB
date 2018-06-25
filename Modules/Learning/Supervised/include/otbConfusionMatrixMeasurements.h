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


#ifndef otbConfusionMatrixMeasurements_h
#define otbConfusionMatrixMeasurements_h

#include "itkObject.h"
#include "itkObjectFactory.h"
#include "itkVariableSizeMatrix.h"
#include "itkVariableLengthVector.h"

namespace otb
{
/** \class ConfusionMatrixMeasurements
 *  This class computes measurements on the input confusion matrix.
 *
 *  For a 2 classes problem, the confusion matrix is organized as follows:
 *  \f[ \left( \begin{array}{cc} True Positives & False Negatives \\ False Positives & True Negatives \end{array} \right) \f]
 *
 *  Please note that when accessing the confusion matrix values, the first index is the row index (reference samples),
 *  and the second is the column index (produced samples).
 *
 *  Some measurements are computed by this class:
 *  If we consider true positive (TP), true negative (TN), false positive (FP) and false negative (FP) rates, then in the 2 classes case:
 *  \f[ precision = \frac{TP}{\left( TP + FP \right) }  \f]
 *  \f[ recall = \frac{TP}{\left( TP + FN \right) }  \f]
 *  \f[ FScore = \frac{2 * precision * recall}{\left( precision + recall \right) }  \f]
 *
 *  Moreover overall accuracy and \f[ \kappa \f] index are computed.
 *
 *
 * \ingroup OTBSupervised
 */
template <class TConfusionMatrix = itk::VariableSizeMatrix<unsigned long>, class TLabel = int >
class ITK_EXPORT ConfusionMatrixMeasurements :
  public itk::Object
{
public:
  /** Standard class typedefs */
  typedef ConfusionMatrixMeasurements               Self;
  typedef itk::Object                               Superclass;
  typedef itk::SmartPointer<Self>                   Pointer;
  typedef itk::SmartPointer<const Self>             ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ConfusionMatrixMeasurements, itk::Object);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  typedef TLabel                                    ClassLabelType;
  typedef std::map<ClassLabelType, int>             MapOfClassesType;
  typedef std::map<int, ClassLabelType>             MapOfIndicesType;

  /** Type for the confusion matrix */
  typedef TConfusionMatrix                          ConfusionMatrixType;

  /** Type for the measurement */
  typedef itk::VariableLengthVector<double>         MeasurementType;


  /** Computes the measurements over m_ConfusionMatrix. */
  void Compute(void);

  /** Accessors */
  itkSetMacro(ConfusionMatrix, ConfusionMatrixType);

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

protected:
  ConfusionMatrixMeasurements();
  ~ConfusionMatrixMeasurements() override {}
  //void PrintSelf(std::ostream& os, itk::Indent indent) const;


private:
  ConfusionMatrixMeasurements(const Self &) = delete;
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

};
} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbConfusionMatrixMeasurements.hxx"
#endif

#endif
