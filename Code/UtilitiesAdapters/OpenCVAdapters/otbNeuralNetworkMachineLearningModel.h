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
#ifndef __otbNeuralNetworkMachineLearningModel_h
#define __otbNeuralNetworkMachineLearningModel_h

#include "itkLightObject.h"
#include "itkFixedArray.h"
#include "otbMachineLearningModel.h"

namespace otb
{
template <class TInputValue, class TTargetValue>
class ITK_EXPORT NeuralNetworkMachineLearningModel
  : public MachineLearningModel <TInputValue, TTargetValue>
{
public:
  /** Standard class typedefs. */
  typedef NeuralNetworkMachineLearningModel           Self;
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

  typedef std::map<TargetValueType, unsigned int>         MapOfLabelsType;

  /** Run-time type information (and related methods). */
  itkNewMacro(Self);
  itkTypeMacro(NeuralNetworkMachineLearningModel, itk::MachineLearningModel);

  /** Setters/Getters to the train method
   *  2 methods are available:
   *   - CvANN_MLP_TrainParams::BACKPROP The back-propagation algorithm.
   *   - CvANN_MLP_TrainParams::RPROP The RPROP algorithm.
   *  Default is CvANN_MLP_TrainParams::RPROP.
   *  \see http://docs.opencv.org/modules/ml/doc/neural_networks.html
   */
  itkGetMacro(TrainMethod, int);
  itkSetMacro(TrainMethod, int);

  /**
   * Set the number of neurons in each layer (including input and output layers).
   * The number of neuron in the first layer (input layer) must be equal
   * to the number of samples in the \c InputListSample
   */
  void SetLayerSizes (const std::vector<unsigned int> layers);


  /** Setters/Getters to the neuron activation function
   *  3 methods are available:
   *   - CvANN_MLP::IDENTITY
   *   - CvANN_MLP::SIGMOID_SYM
   *   - CvANN_MLP::GAUSSIAN
   *  Default is CvANN_MLP::SIGMOID_SYM
   *  \see http://docs.opencv.org/modules/ml/doc/neural_networks.html
   */
  itkGetMacro(ActivateFunction, int);
  itkSetMacro(ActivateFunction, int);

  /** Setters/Getters to the alpha parameter of the activation function
   *  Default is 0.
   *  \see http://docs.opencv.org/modules/ml/doc/neural_networks.html
   */
  itkGetMacro(Alpha, double);
  itkSetMacro(Alpha, double);

  /** Setters/Getters to the beta parameter of the activation function
   *  Default is 0.
   *  \see http://docs.opencv.org/modules/ml/doc/neural_networks.html
   */
  itkGetMacro(Beta, double);
  itkSetMacro(Beta, double);

  /** Strength of the weight gradient term in the BACKPROP method.
   *  The recommended value is about 0.1
   *  Default is 0.1
   *  \see http://docs.opencv.org/modules/ml/doc/neural_networks.html
   */
  itkGetMacro(BackPropDWScale, double);
  itkSetMacro(BackPropDWScale, double);

  /** Strength of the momentum term (the difference between weights on the 2 previous iterations).
   *  This parameter provides some inertia to smooth the random fluctuations of the weights.
   *  It can vary from 0 (the feature is disabled) to 1 and beyond.
   *  The value 0.1 or so is good enough
   *  Default is 0.1
   *  \see http://docs.opencv.org/modules/ml/doc/neural_networks.html
   */
  itkGetMacro(BackPropMomentScale, double);
  itkSetMacro(BackPropMomentScale, double);

  /** Initial value \Delta_0 of update-values \Delta_{ij} in RPROP method.
   *  Default is 0.1
   *  \see http://docs.opencv.org/modules/ml/doc/neural_networks.html
   */
  itkGetMacro(RegPropDW0, double);
  itkSetMacro(RegPropDW0, double);

  /** Update-values lower limit \Delta_{min} in RPROP method.
   * It must be positive. Default is FLT_EPSILON
   *  \see http://docs.opencv.org/modules/ml/doc/neural_networks.html
   */
  itkGetMacro(RegPropDWMin, double);
  itkSetMacro(RegPropDWMin, double);

  /** Termination criteria.
   * It can be CV_TERMCRIT_ITER or CV_TERMCRIT_EPS or CV_TERMCRIT_ITER+CV_TERMCRIT_EPS
   * default is CV_TERMCRIT_ITER+CV_TERMCRIT_EPS.
   * \see http://docs.opencv.org/modules/ml/doc/neural_networks.html
   */
  itkGetMacro(TermCriteriaType, int);
  itkSetMacro(TermCriteriaType, int);

  /** Maximum number of iteration used in the Termination criteria.
   * default is 1000
   * \see http://docs.opencv.org/modules/ml/doc/neural_networks.html
   */
  itkGetMacro(MaxIter, int);
  itkSetMacro(MaxIter, int);

  /** Epsilon value used in the Termination criteria.
   * default is 0.01
   * \see http://docs.opencv.org/modules/ml/doc/neural_networks.html
   */
  itkGetMacro(Epsilon, double);
  itkSetMacro(Epsilon, double);

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
  virtual bool CanReadFile(const std::string &);

  /** Is the input model file writable and compatible with the corresponding classifier ? */
  virtual bool CanWriteFile(const std::string &);
  //@}

protected:
  /** Constructor */
  NeuralNetworkMachineLearningModel();

  /** Destructor */
  virtual ~NeuralNetworkMachineLearningModel();

  void LabelsToMat(const TargetListSampleType * listSample, cv::Mat & output);

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  NeuralNetworkMachineLearningModel(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  CvANN_MLP * m_ANNModel;
  int m_TrainMethod;
  int m_ActivateFunction;
  std::vector<unsigned int> m_LayerSizes;
  double m_Alpha;
  double m_Beta;
  double m_BackPropDWScale;
  double m_BackPropMomentScale;
  double m_RegPropDW0;
  double m_RegPropDWMin;
  int m_TermCriteriaType;
  int m_MaxIter;
  double m_Epsilon;

  CvMat*             m_CvMatOfLabels;
  MapOfLabelsType    m_MapOfLabels;

};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbNeuralNetworkMachineLearningModel.txx"
#endif

#endif
