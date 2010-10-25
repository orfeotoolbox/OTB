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

#include "itkProcessObject.h"
#include "itkVariableSizeMatrix.h"

namespace otb
{
/** \class ConfusionMatrixCalculator
 *  This class computes a confusion matrix from 2 lists of labels. It
 *  assumes that the 2 lists have the same length and uses the
 *  position of the labels in the lists to build the pairs
 *  reference/produced labels.
 *
 *  For a 2 classes problem, the confusion matrix is organized as follows:
 *  \f[ \left( \begin{array}{cc} True positives & False positives \\ False negatives & true Negatives \end{array} \right) \f]
 *
 *  Please note that when accessing the confusion matrix values, the first index is the row index, and the second is the column index,
 *  so that accessing the false positive rate is done by calling GetConfusionMatrix()[1,0].
 *
 */
template <class TRefListLabel, class TProdListLabel>
class ITK_EXPORT ConfusionMatrixCalculator :
  public itk::ProcessObject
{
public:
  /** Standard class typedefs */
  typedef ConfusionMatrixCalculator     Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ConfusionMatrixCalculator, itk::ProcessObject);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** List to store the corresponding labels */
  typedef TRefListLabel                      RefListLabelType;
  typedef typename RefListLabelType::Pointer RefListLabelPointerType;

  typedef TProdListLabel                      ProdListLabelType;
  typedef typename ProdListLabelType::Pointer ProdListLabelPointerType;

  typedef int ClassLabelType;

  /** Type for the confusion matrix */
  typedef itk::VariableSizeMatrix<double> ConfusionMatrixType;

  virtual void Update();

  /** Accessors */

  itkSetObjectMacro(ReferenceLabels, RefListLabelType);
  itkGetConstObjectMacro(ReferenceLabels, RefListLabelType);
  itkSetObjectMacro(ProducedLabels, ProdListLabelType);
  itkGetConstObjectMacro(ProducedLabels, ProdListLabelType);
  itkGetMacro(KappaIndex, double);
  itkGetMacro(OverallAccuracy, double);
  itkGetMacro(NumberOfClasses, unsigned short);
  itkGetMacro(NumberOfSamples, unsigned long);
  itkGetMacro(ConfusionMatrix, ConfusionMatrixType);

  std::map<ClassLabelType, int> GetMapOfClasses() const
  {
    return m_MapOfClasses;
  }

protected:
  ConfusionMatrixCalculator();
  virtual ~ConfusionMatrixCalculator() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Triggers the computation of the confusion matrix */
  void GenerateData(void);

private:
  ConfusionMatrixCalculator(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  double m_KappaIndex;
  double m_OverallAccuracy;

  std::map<ClassLabelType, int> m_MapOfClasses;

  unsigned short m_NumberOfClasses;
  unsigned long  m_NumberOfSamples;

  ConfusionMatrixType m_ConfusionMatrix;

  RefListLabelPointerType  m_ReferenceLabels;
  ProdListLabelPointerType m_ProducedLabels;

};
} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbConfusionMatrixCalculator.txx"
#endif

#endif
