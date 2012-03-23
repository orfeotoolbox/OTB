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
#ifndef __otbMachineLearningModel_h
#define __otbMachineLearningModel_h

#include "itkLightObject.h"
#include "itkVariableLengthVector.h"
#include "itkFixedArray.h"
#include "itkListSample.h"

namespace otb
{
template <class TInputValue, class TTargetValue>
class ITK_EXPORT MachineLearningModel 
  : public itk::LightObject
{
public:
  /** Standard class typedefs. */
  typedef SVMModel                                      Self;
  typedef itk::DataObject                               Superclass;
  typedef itk::SmartPointer<Self>                       Pointer;
  typedef itk::SmartPointer<const Self>                 ConstPointer;

  // Input related typedefs
  typedef TInputValue                                   InputValueType;
  typedef itk::VariableLengthVector<InputValueType>     InputSampleType;
  typedef itk::Statistics::ListSample<InputSampleType>  InputListSampleType;

  // Target related typedefs
  typedef TTargetValue                                  TargetValueType;
  typedef itk::FixedArray<TargetValueType,1>            TargetSampleType;
  typedef itk::Statistics::ListSample<TargetSampleType> TargetListSampleType;
  
  /** Run-time type information (and related methods). */
  itkNewMacro(Self);
  itkTypeMacro(MachineLearningModel, itk::DataObject);

  /** Train the machine learning model */
  virtual void Train() = 0;

  /** Predict values using the model */
  virtual void Predict()= 0;

  /** Save the model to file */
  virtual void Save() = 0;

  /** Load the model from file */
  virtual void Load() = 0;

  /** Input accessors */
  itkSetObjectMacro(InputListSample,InputListSampleType);
  itkGetObjectMacro(InputListSample,InputListSampleType);

  /** Target accessors */
  itkSetObjectMacro(TargetListSample,TargetListSampleType);
  itkGetObjectMacro(TargetListSample,TargetListSampleType);
  
protected:
  /** Constructor */
  MachineLearningModel();

  /** Destructor */
  virtual ~MachineLearningModel();

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Input list sample */
  typename InputListSampleType::Pointer m_InputListSample;

  /** Target list sample */
  typename TargetListSampleType::Pointer m_TargetListSample;

private:
  MachineLearningModel(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMachineLearningModel.txx"
#endif

#endif
