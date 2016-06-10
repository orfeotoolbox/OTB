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
#ifndef __otbNormalBayesMachineLearningModel_h
#define __otbNormalBayesMachineLearningModel_h

#include "otbRequiresOpenCVCheck.h"

#include "itkLightObject.h"
#include "itkFixedArray.h"
#include "otbMachineLearningModel.h"


class CvNormalBayesClassifier;

namespace otb
{
template <class TInputValue, class TTargetValue>
class ITK_EXPORT NormalBayesMachineLearningModel
  : public MachineLearningModel <TInputValue, TTargetValue>
{
public:
  /** Standard class typedefs. */
  typedef NormalBayesMachineLearningModel           Self;
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

  /** Run-time type information (and related methods). */
  itkNewMacro(Self);
  itkTypeMacro(NormalBayesMachineLearningModel, MachineLearningModel);

  /** Train the machine learning model */
  void Train() ITK_OVERRIDE;
  /** Predict values using the model */
  TargetSampleType Predict(const InputSampleType& input, ConfidenceValueType *quality=NULL) const ITK_OVERRIDE;

  /** Save the model to file */
  void Save(const std::string & filename, const std::string & name="") ITK_OVERRIDE;

  /** Load the model from file */
  void Load(const std::string & filename, const std::string & name="") ITK_OVERRIDE;

  /**\name Classification model file compatibility tests */
  //@{
  /** Is the input model file readable and compatible with the corresponding classifier ? */
  bool CanReadFile(const std::string &) ITK_OVERRIDE;

  /** Is the input model file writable and compatible with the corresponding classifier ? */
  bool CanWriteFile(const std::string &) ITK_OVERRIDE;
  //@}

protected:
  /** Constructor */
  NormalBayesMachineLearningModel();

  /** Destructor */
  ~NormalBayesMachineLearningModel() ITK_OVERRIDE;

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

private:
  NormalBayesMachineLearningModel(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  CvNormalBayesClassifier * m_NormalBayesModel;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbNormalBayesMachineLearningModel.txx"
#endif

#endif
