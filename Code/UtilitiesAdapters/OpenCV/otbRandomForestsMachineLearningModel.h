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
#ifndef __otbRandomForestsMachineLearningModel_h
#define __otbRandomForestsMachineLearningModel_h

#include "itkLightObject.h"
#include "itkVariableLengthVector.h"
#include "itkFixedArray.h"
#include "itkListSample.h"
#include "otbMachineLearningModel.h"

//include opencv
#include <cv.h>       // opencv general include file
#include <ml.h>		  // opencv machine learning include file

namespace otb
{
template <class TInputValue, class TTargetValue>
class ITK_EXPORT RandomForestsMachineLearningModel 
  : public MachineLearningModel <TInputValue, TTargetValue>
{
public:
  /** Standard class typedefs. */
  typedef RandomForestsMachineLearningModel               Self;
  typedef MachineLearningModel<TInputValue, TTargetValue> Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  // Input related typedefs
  typedef TInputValue                                   InputValueType;
  typedef itk::VariableLengthVector<InputValueType>     InputSampleType;
  typedef itk::Statistics::ListSample<InputSampleType>  InputListSampleType;

  // Target related typedefs
  typedef TTargetValue                                  TargetValueType;
  typedef itk::FixedArray<TargetValueType,1>            TargetSampleType;
  typedef itk::Statistics::ListSample<TargetSampleType> TargetListSampleType;

  //opencv typedef
  typedef CvRTrees RFType;

  /** Run-time type information (and related methods). */
  itkNewMacro(Self);
  itkTypeMacro(RandomForestsMachineLearningModel, itk::MachineLearningModel);

  /** Train the machine learning model */
  virtual void Train();

  /** Predict values using the model */
  virtual void Predict();

  /** Save the model to file */
  virtual void Save(char * filename, const char * name=0);

  /** Load the model from file */
  virtual void Load(char * filename, const char * name=0);

  /* /\** Input accessors *\/ */
  /* itkSetObjectMacro(InputListSample,InputListSampleType); */
  /* itkGetObjectMacro(InputListSample,InputListSampleType); */

  /* /\** Target accessors *\/ */
  /* itkSetObjectMacro(TargetListSample,TargetListSampleType); */
  /* itkGetObjectMacro(TargetListSample,TargetListSampleType); */
  
protected:
  /** Constructor */
  RandomForestsMachineLearningModel();

  /** Destructor */
  virtual ~RandomForestsMachineLearningModel();

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /* /\** Input list sample *\/ */
  /* typename InputListSampleType::Pointer m_InputListSample; */

  /* /\** Target list sample *\/ */
  /* typename TargetListSampleType::Pointer m_TargetListSample; */

private:
  RandomForestsMachineLearningModel(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  CvRTrees * m_RFModel;
  unsigned int m_MaxDepth;
  unsigned int m_MinSampleCount;
  double m_RegressionAccuracy;
  bool m_ComputeSurrogateSplit;
  unsigned int m_MaxNumberOfCategories;
  float * m_Priors;
  bool m_CalculateVariableImportance;
  unsigned int m_MaxNumberOfVariables;
  unsigned int m_MaxNumberOfTrees;
  double m_ForrestAccuracy;
  int m_TerminationCriteria;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRandomForestsMachineLearningModel.txx"
#endif

#endif
