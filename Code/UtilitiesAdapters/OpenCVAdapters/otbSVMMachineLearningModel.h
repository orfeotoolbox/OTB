
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
#ifndef __otbSVMMachineLearningModel_h
#define __otbSVMMachineLearningModel_h

#include "itkLightObject.h"
#include "itkVariableLengthVector.h"
#include "itkFixedArray.h"
#include "itkListSample.h"
#include "otbMachineLearningModel.h"

class CvSVM;

namespace otb
{
template <class TInputValue, class TTargetValue>
class ITK_EXPORT SVMMachineLearningModel
  : public MachineLearningModel <TInputValue, TTargetValue>
{
public:
  /** Standard class typedefs. */
  typedef SVMMachineLearningModel           Self;
  typedef MachineLearningModel<TInputValue, TTargetValue> Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  // Input related typedefs
  typedef TInputValue                                     InputValueType;
  typedef itk::VariableLengthVector<InputValueType>       InputSampleType;
  typedef itk::Statistics::ListSample<InputSampleType>    InputListSampleType;

  // Target related typedefs
  typedef TTargetValue                                    TargetValueType;
  typedef itk::FixedArray<TargetValueType,1>              TargetSampleType;
  typedef itk::Statistics::ListSample<TargetSampleType>   TargetListSampleType;

  /** Run-time type information (and related methods). */
  itkNewMacro(Self);
  itkTypeMacro(SVMMachineLearningModel, MachineLearningModel);

  /** Train the machine learning model */
  virtual void Train();

  /** Predict values using the model */
  virtual TargetSampleType Predict(const InputSampleType & input) const;

  /** Save the model to file */
  virtual void Save(const std::string & filename, const std::string & name="");

  /** Load the model from file */
  virtual void Load(const std::string & filename, const std::string & name="");

  /**\name Classification model file compatibility tests */
  //@{
  /** Is the input model file readable and compatible with the corresponding classifier ? */
  virtual bool CanReadFile(const std::string &) = 0;

  /** Is the input model file writable and compatible with the corresponding classifier ? */
  virtual bool CanWriteFile(const std::string &)  = 0;
  //@}

  //Setters/Getters to SVM model
  itkGetMacro(SVMType, int);
  itkSetMacro(SVMType, int);

  itkGetMacro(KernelType, int);
  itkSetMacro(KernelType, int);

  //CV_TERMCRIT_ITER or CV_TERMCRIT_EPS
  itkGetMacro(TermCriteriaType, int);
  itkSetMacro(TermCriteriaType, int);

  itkGetMacro(MaxIter, int);
  itkSetMacro(MaxIter, int);

  itkGetMacro(Epsilon, double);
  itkSetMacro(Epsilon, double);

  // for poly
  itkGetMacro(Degree, double);
  itkSetMacro(Degree, double);
  itkGetMacro(OutputDegree, double);

  // for poly/rbf/sigmoid
  itkGetMacro(Gamma, double);
  itkSetMacro(Gamma, double);
  itkGetMacro(OutputGamma, double);

  // for poly/sigmoid
  itkGetMacro(Coef0, double);
  itkSetMacro(Coef0, double);
  itkGetMacro(OutputCoef0, double);

  // for CV_SVM_C_SVC, CV_SVM_EPS_SVR and CV_SVM_NU_SVR
  itkGetMacro(C, double);
  itkSetMacro(C, double);
  itkGetMacro(OutputC, double);

  // for CV_SVM_NU_SVC, CV_SVM_ONE_CLASS, and CV_SVM_NU_SVR
  itkGetMacro(Nu, double);
  itkSetMacro(Nu, double);
  itkGetMacro(OutputNu, double);

  // for CV_SVM_EPS_SVR
  itkGetMacro(P, double);
  itkSetMacro(P, double);
  itkGetMacro(OutputP, double);

  itkGetMacro(ParameterOptimization, bool);
  itkSetMacro(ParameterOptimization, bool);

protected:
  /** Constructor */
  SVMMachineLearningModel();

  /** Destructor */
  virtual ~SVMMachineLearningModel();

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  SVMMachineLearningModel(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  CvSVM * m_SVMModel;
  int m_SVMType;
  int m_KernelType;
  double m_Degree;
  double m_Gamma;
  double m_Coef0;
  double m_C;
  double m_Nu;
  double m_P;
  int m_TermCriteriaType;
  int m_MaxIter;
  double m_Epsilon;
  bool m_ParameterOptimization;
  //Output parameters
  double m_OutputDegree;
  double m_OutputGamma;
  double m_OutputCoef0;
  double m_OutputC;
  double m_OutputNu;
  double m_OutputP;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSVMMachineLearningModel.txx"
#endif

#endif
