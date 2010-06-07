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
 *  \brief TODO
 *
 */
template < class TListLabel > 
class ITK_EXPORT ConfusionMatrixCalculator :
  public itk::ProcessObject
{
public:
  /** Standard class typedefs */
  typedef ConfusionMatrixCalculator     Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer< Self >     Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(ConfusionMatrixCalculator, itk::ProcessObject);
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** List to store the corresponding labels */
  typedef TListLabel                              ListLabelType;
  typedef typename ListLabelType::Pointer         ListLabelPointerType;

  /** Type for the confusion matrix */
  typedef itk::VariableSizeMatrix<double>         ConfusionMatrixType;
  
  /** Sets the reference labels (i.e. ground truth) */
  void SetReferenceLabels( const ListLabelType * );
  const ListLabelType * GetReferenceLabels() const;

  /** Sets the produced labels (i.e. output of a classifier) */
  void SetProducedLabels( const ListLabelType * );
  const ListLabelType * GetProducedLabels() const;

  virtual void Update();
  
  /** Accessors */

  itkGetConstMacro(KappaIndex, double);
  itkGetConstMacro(OverallAccuracy, double);
  itkGetConstMacro(NumberOfClasses, unsigned short);
  itkGetMacro(ConfusionMatrix, ConfusionMatrixType);
  
protected:
  ConfusionMatrixCalculator();
  virtual ~ConfusionMatrixCalculator() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;  
  
  /** Triggers the computation of the confusion matrix */
  void GenerateData( void );
  
private:
  ConfusionMatrixCalculator(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
  double                m_KappaIndex;
  double                m_OverallAccuracy;

  std::vector<int>      m_VecOfClasses;

  unsigned short        m_NumberOfClasses;

  ConfusionMatrixType   m_ConfusionMatrix;
  
};
}// end of namespace otb
  
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbConfusionMatrixCalculator.txx"
#endif

#endif
