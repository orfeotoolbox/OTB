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
#ifndef __otbExhaustiveExponentialOptimizer_h
#define __otbExhaustiveExponentialOptimizer_h

#include "itkSingleValuedNonLinearOptimizer.h"

namespace otb
{

/** \class ExhaustiveExponentialOptimizer
 * \brief This optimizer performs an exhaustive search on an exponential scale.
 *
 * This optimizer is equivalent to an exhaustive search in a discrete grid
 * defined over the parametric space. The grid is centered on the initial
 * position. The subdivisions of the grid along each one of the dimensions
 * of the parametric space is defined by a exponential progression with
 * scale factor m_ScaleFactor, with the exponant following an
 * arithmetic progression from 0 to m_NumberOfSteps * m_StepLength in
 * both positive and negative direction.
 *
 * This optimizer can be use to perform a preliminary coarse search on
 * the search space.
 *
 * \ingroup Numerics Optimizers
 *
 * \ingroup OTBSVMLearning
 */
class ITK_EXPORT ExhaustiveExponentialOptimizer :
  public itk::SingleValuedNonLinearOptimizer
{
public:
  /** Standard "Self" typedef. */
  typedef ExhaustiveExponentialOptimizer      Self;
  typedef itk::SingleValuedNonLinearOptimizer Superclass;
  typedef itk::SmartPointer<Self>             Pointer;
  typedef itk::SmartPointer<const Self>       ConstPointer;

  typedef itk::Array<unsigned long> StepsType;
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ExhaustiveExponentialOptimizer,           SingleValuedNonLinearOptimizer);

  void    StartOptimization(void) ITK_OVERRIDE;

  void StartWalking(void);
  void ResumeWalking(void);
  void StopWalking(void);

  itkSetMacro(GeometricProgression, double);
  itkSetMacro(NumberOfSteps, StepsType);
  itkSetMacro(StepLength, double);
  itkGetConstReferenceMacro(GeometricProgression, double);
  itkGetConstReferenceMacro(NumberOfSteps, StepsType);
  itkGetConstReferenceMacro(StepLength,   double);
  itkGetConstReferenceMacro(CurrentValue, MeasureType);
  itkGetConstReferenceMacro(MaximumMetricValue, MeasureType);
  itkGetConstReferenceMacro(MinimumMetricValue, MeasureType);
  itkGetConstReferenceMacro(MinimumMetricValuePosition, ParametersType);
  itkGetConstReferenceMacro(MaximumMetricValuePosition, ParametersType);
  itkGetConstReferenceMacro(CurrentIndex, ParametersType);
  itkGetConstReferenceMacro(MaximumNumberOfIterations, unsigned long);

protected:
  ExhaustiveExponentialOptimizer();
  ~ExhaustiveExponentialOptimizer() ITK_OVERRIDE {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

  /** Advance to the next grid position. */
  void AdvanceOneStep(void);
  void IncrementIndex(ParametersType& param);

protected:
  MeasureType    m_CurrentValue;
  StepsType      m_NumberOfSteps;
  double         m_StepLength;
  unsigned long  m_CurrentIteration;
  bool           m_Stop;
  unsigned int   m_CurrentParameter;
  double         m_GeometricProgression;
  ParametersType m_CurrentIndex;
  unsigned long  m_MaximumNumberOfIterations;
  MeasureType    m_MaximumMetricValue;
  MeasureType    m_MinimumMetricValue;
  ParametersType m_MinimumMetricValuePosition;
  ParametersType m_MaximumMetricValuePosition;

private:
  ExhaustiveExponentialOptimizer(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#endif
