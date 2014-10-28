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

#include "itkObject.h"
#include "itkVariableLengthVector.h"
#include "itkListSample.h"

namespace otb
{

/** \class MachineLearningModel
 * \brief MachineLearningModel is the base class for all classifier objects (SVM, KNN,
 *        Random Forests, Artificial Neural Network, ...) implemented in the supervised classification framework of the OTB.
 *
 * MachineLearningModel is an abstract object that specifies behavior and
 * interface of supervised classifiers (SVM, KNN, Random Forests, Artificial
 * Neural Network, ...) in the generic supervised classification framework of the OTB.
 * The main generic virtual methods specifically implemented in each classifier
 * derived from the MachineLearningModel class are two learning-related methods:
 * Train() and Save(), and two classification-related methods: Load() and Predict().
 *
 * Thus, each classifier derived from the MachineLearningModel class
 * computes its corresponding model with Train() and exports it with
 * the help of the Save() method.
 *
 * It is also possible to classify any input sample composed of several
 * features (or any number of bands in the case of a pixel extracted
 * from a multi-band image) with the help of the Predict() method which
 * needs a previous loading of the classification model with the Load() method.
 *
 * \sa MachineLearningModelFactory
 * \sa LibSVMMachineLearningModel
 * \sa SVMMachineLearningModel
 * \sa BoostMachineLearningModel
 * \sa KNearestNeighborsMachineLearningModel
 * \sa DecisionTreeMachineLearningModel
 * \sa RandomForestsMachineLearningModel
 * \sa GradientBoostedTreeMachineLearningModel
 * \sa NormalBayesMachineLearningModel
 * \sa NeuralNetworkMachineLearningModel
 * \sa ImageClassificationFilter
 *
 */
template <class TInputValue, class TTargetValue>
class ITK_EXPORT MachineLearningModel
  : public itk::Object
{
public:
  /**\name Standard ITK typedefs */
  //@{
  typedef MachineLearningModel                          Self;
  typedef itk::Object                                   Superclass;
  typedef itk::SmartPointer<Self>                       Pointer;
  typedef itk::SmartPointer<const Self>                 ConstPointer;
  //@}

  /**\name Input related typedefs */
  //@{
  typedef TInputValue                                   InputValueType;
  typedef itk::VariableLengthVector<InputValueType>     InputSampleType;
  typedef itk::Statistics::ListSample<InputSampleType>  InputListSampleType;
  //@}

  /**\name Target related typedefs */
  //@{
  typedef TTargetValue                                  TargetValueType;
  typedef itk::FixedArray<TargetValueType,1>            TargetSampleType;
  typedef itk::Statistics::ListSample<TargetSampleType> TargetListSampleType;
  //@}

  /**\name Standard macros */
  //@{
  /** Run-time type information (and related methods). */
  itkTypeMacro(MachineLearningModel, itk::Object);
  //@}

  /** Train the machine learning model */
  virtual void Train() = 0;

  /** Predict values using the model */
  virtual TargetSampleType Predict(const InputSampleType& input) const = 0;

  /** Classify all samples in InputListSample and fill TargetListSample with the associated label */
  void PredictAll();

  /**\name Classification model file manipulation */
  //@{
  /** Save the model to file */
  virtual void Save(const std::string & filename, const std::string & name="") = 0;

  /** Load the model from file */
  virtual void Load(const std::string & filename, const std::string & name="") = 0;
  //@}

  /**\name Classification model file compatibility tests */
  //@{
  /** Is the input model file readable and compatible with the corresponding classifier ? */
  virtual bool CanReadFile(const std::string &) = 0;

  /** Is the input model file writable and compatible with the corresponding classifier ? */
  virtual bool CanWriteFile(const std::string &)  = 0;
  //@}

  /**\name Input list of samples accessors */
  //@{
  itkSetObjectMacro(InputListSample,InputListSampleType);
  itkGetObjectMacro(InputListSample,InputListSampleType);
  //@}

  /**\name Classification output accessors */
  //@{
  /** Set the target labels (to be used before training) */
  itkSetObjectMacro(TargetListSample,TargetListSampleType);
  /** Get the target labels (to be used after PredictAll) */
  itkGetObjectMacro(TargetListSample,TargetListSampleType);
  //@}

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
