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
#ifndef __otbSharkRandomForestsMachineLearningModel_h
#define __otbSharkRandomForestsMachineLearningModel_h

#include "itkLightObject.h"
#include "otbMachineLearningModel.h"
#include "shark/Algorithms/Trainers/RFTrainer.h"

namespace otb
{
template <class TInputValue, class TTargetValue>
class ITK_EXPORT SharkRandomForestsMachineLearningModel
  : public MachineLearningModel <TInputValue, TTargetValue>
{
public:
  /** Standard class typedefs. */
  typedef SharkRandomForestsMachineLearningModel               Self;
  typedef MachineLearningModel<TInputValue, TTargetValue> Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  typedef typename Superclass::InputValueType             InputValueType;
  typedef typename Superclass::InputSampleType            InputSampleType;
  typedef typename Superclass::InputListSampleType        InputListSampleType;
  typedef typename Superclass::TargetValueType            TargetValueType;
  typedef typename Superclass::TargetSampleType           TargetSampleType;
  typedef typename Superclass::TargetListSampleType       TargetListSampleType;
  typedef typename Superclass::ConfidenceValueType        ConfidenceValueType;
  typedef itk::FixedArray<ConfidenceValueType,1>            ConfidenceSampleType;
  typedef itk::Statistics::ListSample<ConfidenceSampleType> ConfidenceListSampleType;
  
  /** Run-time type information (and related methods). */
  itkNewMacro(Self);
  itkTypeMacro(SharkRandomForestsMachineLearningModel, MachineLearningModel);

  /** Train the machine learning model */
  virtual void Train();
  /** Predict values using the model */
  virtual TargetSampleType Predict(const InputSampleType& input, ConfidenceValueType *quality=NULL) const;

  /** Save the model to file */
  virtual void Save(const std::string & filename, const std::string & name="");

  /** Load the model from file */
  virtual void Load(const std::string & filename, const std::string & name="");

  /** Classify all samples in InputListSample and fill TargetListSample with the associated label */
  virtual void PredictAll() override;
  /**\name Classification model file compatibility tests */
  //@{
  /** Is the input model file readable and compatible with the corresponding classifier ? */
  virtual bool CanReadFile(const std::string &);

  /** Is the input model file writable and compatible with the corresponding classifier ? */
  virtual bool CanWriteFile(const std::string &);
  //@}

  /**\name Confidence accessors for batch mode */
  //@{
  /** Set the confidence samples (to be used before PredictAll) */
  itkSetObjectMacro(ConfidenceListSample,ConfidenceListSampleType);
  /** Get the confidence values (to be used after PredictAll) */
  itkGetObjectMacro(ConfidenceListSample,ConfidenceListSampleType);
  //@}

  itkGetMacro(NumberOfTrees,unsigned int);
  itkSetMacro(NumberOfTrees,unsigned int);

  itkGetMacro(MTry, unsigned int);
  itkSetMacro(MTry, unsigned int);

  itkGetMacro(NodeSize, unsigned int);
  itkSetMacro(NodeSize, unsigned int);
  
  itkGetMacro(OobRatio, float);
  itkSetMacro(OobRatio, float);

  itkGetMacro(ComputeMargin, bool);
  itkSetMacro(ComputeMargin, bool);


protected:
  /** Constructor */
  SharkRandomForestsMachineLearningModel();

  /** Destructor */
  virtual ~SharkRandomForestsMachineLearningModel();

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  SharkRandomForestsMachineLearningModel(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  shark::RFClassifier m_RFModel;
  shark::RFTrainer m_RFTrainer;

  unsigned int m_NumberOfTrees;
  unsigned int m_MTry;
  unsigned int m_NodeSize;
  float m_OobRatio;
  bool m_ComputeMargin;

  /** Confidence list sample */
  typename ConfidenceListSampleType::Pointer m_ConfidenceListSample;

    };
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSharkRandomForestsMachineLearningModel.txx"
#endif

#endif
