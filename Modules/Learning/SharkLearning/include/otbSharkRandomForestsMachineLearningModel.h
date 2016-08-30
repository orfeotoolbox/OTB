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

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
#endif
#include "shark/Algorithms/Trainers/RFTrainer.h"
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif

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
  typedef typename Superclass::ConfidenceSampleType       ConfidenceSampleType;
  typedef typename Superclass::ConfidenceListSampleType   ConfidenceListSampleType;
  
  /** Run-time type information (and related methods). */
  itkNewMacro(Self);
  itkTypeMacro(SharkRandomForestsMachineLearningModel, MachineLearningModel);

  /** Train the machine learning model */
  virtual void Train() ITK_OVERRIDE;

  /** Save the model to file */
  virtual void Save(const std::string & filename, const std::string & name="") ITK_OVERRIDE;

  /** Load the model from file */
  virtual void Load(const std::string & filename, const std::string & name="") ITK_OVERRIDE;

  /**\name Classification model file compatibility tests */
  //@{
  /** Is the input model file readable and compatible with the corresponding classifier ? */
  virtual bool CanReadFile(const std::string &) ITK_OVERRIDE;

  /** Is the input model file writable and compatible with the corresponding classifier ? */
  virtual bool CanWriteFile(const std::string &) ITK_OVERRIDE;
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

  /** Predict values using the model */
  virtual TargetSampleType DoPredict(const InputSampleType& input, ConfidenceValueType *quality=ITK_NULLPTR) const ITK_OVERRIDE;

  
  virtual void DoPredictBatch(const InputListSampleType *, const unsigned int & startIndex, const unsigned int & size, TargetListSampleType *, ConfidenceListSampleType * = ITK_NULLPTR) const ITK_OVERRIDE;
  
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
  ConfidenceValueType ComputeConfidence(shark::RealVector & probas, 
                                        bool computeMargin) const;

};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSharkRandomForestsMachineLearningModel.txx"
#endif

#endif
