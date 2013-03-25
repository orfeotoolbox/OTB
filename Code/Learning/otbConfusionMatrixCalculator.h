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

#ifndef __otbConfusionMatrixCalculator_h
#define __otbConfusionMatrixCalculator_h

#include "otbMacro.h"
#include "itkObject.h"
#include "itkObjectFactory.h"
#include "itkVariableSizeMatrix.h"
#include "otbConfusionMatrixMeasurements.h"

namespace otb
{
/** \class ConfusionMatrixCalculator
 *  This class computes a confusion matrix from 2 lists of labels. It
 *  assumes that the 2 lists have the same length and uses the
 *  position of the labels in the lists to build the pairs
 *  reference/produced labels.
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
  typedef TRefListLabel                             RefListLabelType;
  typedef typename RefListLabelType::Pointer        RefListLabelPointerType;

  typedef TProdListLabel                            ProdListLabelType;
  typedef typename ProdListLabelType::Pointer       ProdListLabelPointerType;

  typedef int                                       ClassLabelType;

  /** Type for the confusion matrix */
  typedef itk::VariableSizeMatrix<double>           ConfusionMatrixType;

  /** Type for the confusion matrix measurements calculator*/
  typedef otb::ConfusionMatrixMeasurements<ConfusionMatrixType> ConfusionMatrixMeasurementsType;

  /** Type for the measurement */
  typedef itk::VariableLengthVector<double>         MeasurementType;


  /**
   * \deprecated in OTB 3.16, please use void Compute(void) instead.
   */
  itkLegacyMacro( virtual void Update() );

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
  std::map<ClassLabelType, int> GetMapOfClasses() const
  {
    return m_MapOfClasses;
  }

  /* Gives the correspondence between an index in the
   * confusion matrix and the class label
   */
  std::map<int, ClassLabelType> GetMapOfIndices() const
  {
    return m_MapOfIndices;
  }

protected:
  ConfusionMatrixCalculator();
  virtual ~ConfusionMatrixCalculator() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;


private:
  ConfusionMatrixCalculator(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

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

  std::map<ClassLabelType, int> m_MapOfClasses;
  std::map<int, ClassLabelType> m_MapOfIndices;

  unsigned short m_NumberOfClasses;
  unsigned long  m_NumberOfSamples;

  ConfusionMatrixType m_ConfusionMatrix;
  ConfusionMatrixMeasurementsType::Pointer m_ConfMatMeasurements;

  RefListLabelPointerType  m_ReferenceLabels;
  ProdListLabelPointerType m_ProducedLabels;

};
} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbConfusionMatrixCalculator.txx"
#endif

#endif
