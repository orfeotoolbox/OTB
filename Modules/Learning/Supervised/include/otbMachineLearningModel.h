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
#ifndef otbMachineLearningModel_h
#define otbMachineLearningModel_h

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
 * Train() and Save(), and three classification-related methods: Load(),
 * DoPredict() and optionnaly DoPredictBatch().
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
 * \sa SharkRandomForestsMachineLearningModel
 * \sa ImageClassificationFilter
 *
 *
 * \ingroup OTBSupervised
 */
template <class TInputValue, class TTargetValue, class TConfidenceValue = double >
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

  /**\name Confidence value typedef */
  typedef TConfidenceValue                              ConfidenceValueType;
  typedef itk::FixedArray<ConfidenceValueType,1>            ConfidenceSampleType;
  typedef itk::Statistics::ListSample<ConfidenceSampleType> ConfidenceListSampleType;

  /**\name Standard macros */
  //@{
  /** Run-time type information (and related methods). */
  itkTypeMacro(MachineLearningModel, itk::Object);
  //@}

  /** Train the machine learning model */
  virtual void Train() =0;

  /** Predict a single sample
    * \param input The sample
    * \param quality A pointer to the quality variable were to store
    * quality value, or NULL
    * \return The predicted label
     */
  TargetSampleType Predict(const InputSampleType& input, ConfidenceValueType *quality = ITK_NULLPTR) const;



  /** Predict a batch of samples (InputListSampleType)
    * \param input The batch of sample to predict
    * \param quality A pointer to the list were to store
    * quality value, or NULL
    * \return The predicted labels
    * Note that this method will be multi-threaded if OTB is built
    * with OpenMP.
     */
  typename TargetListSampleType::Pointer PredictBatch(const InputListSampleType * input, ConfidenceListSampleType * quality = ITK_NULLPTR) const;
  
  /** THIS METHOD IS DEPRECATED AND SHOULD NOT BE USED. */
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

  /** Query capacity to produce a confidence index */
  bool HasConfidenceIndex() const {return m_ConfidenceIndex;}

  /**\name Input list of samples accessors */
  //@{
  itkSetObjectMacro(InputListSample,InputListSampleType);
  itkGetObjectMacro(InputListSample,InputListSampleType);
  itkGetConstObjectMacro(InputListSample,InputListSampleType);
  //@}

  /**\name Classification output accessors */
  //@{
  /** Set the target labels (to be used before training) */
  itkSetObjectMacro(TargetListSample,TargetListSampleType);
  /** Get the target labels (to be used after PredictAll) */
  itkGetObjectMacro(TargetListSample,TargetListSampleType);
  //@}

  itkGetObjectMacro(ConfidenceListSample,ConfidenceListSampleType);
  
  /**\name Use model in regression mode */
  //@{
  itkGetMacro(RegressionMode,bool);
  void SetRegressionMode(bool flag);
  //@}

protected:
  /** Constructor */
  MachineLearningModel();

  /** Destructor */
  ~MachineLearningModel() ITK_OVERRIDE;
 
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

  /** Input list sample */
  typename InputListSampleType::Pointer m_InputListSample;

  /** Target list sample */
  typename TargetListSampleType::Pointer m_TargetListSample;

  typename ConfidenceListSampleType::Pointer m_ConfidenceListSample;
  
  /** flag to choose between classification and regression modes */
  bool m_RegressionMode;
  
  /** flag that indicates if the model supports regression, child
   *  classes should modify it in their constructor if they support
   *  regression mode */
  bool m_IsRegressionSupported;

  /** flag that tells if the model support confidence index output */
  bool m_ConfidenceIndex;

  /** Is DoPredictBatch multi-threaded ? */
  bool m_IsDoPredictBatchMultiThreaded;

private:
  /**  Actual implementation of BatchPredicition
    *  Default implementation will call DoPredict iteratively 
    *  \param input The input batch
    *  \param startIndex Index of the first sample to predict
    *  \param size Number of samples to predict
    *  \param target Pointer to the list of produced labels
    *  \param quality Pointer to the list of produced confidence
    *  values, or NULL
    * 
    * Override me if internal implementation allows for batch
    * prediction.
    * 
    * Also set m_IsDoPredictBatchMultiThreaded to true if internal
    * implementation allows for parallel batch prediction.
    */
  virtual void DoPredictBatch(const InputListSampleType * input, const unsigned int & startIndex, const unsigned int & size, TargetListSampleType * target, ConfidenceListSampleType * quality = ITK_NULLPTR) const;

  /** Actual implementation of single sample prediction
   *  \param input sample to predict
   *  \param quality Pointer to a variable to store confidence value,
   *  or NULL
   *  \return The predicted label
   */ 
  virtual TargetSampleType DoPredict(const InputSampleType& input, ConfidenceValueType * quality= ITK_NULLPTR) const = 0;  
 
  MachineLearningModel(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMachineLearningModel.txx"
#endif

#endif
