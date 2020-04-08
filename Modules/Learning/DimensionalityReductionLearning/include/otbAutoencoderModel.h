/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef otbAutoencoderModel_h
#define otbAutoencoderModel_h

#include "otbMachineLearningModelTraits.h"
#include "otbMachineLearningModel.h"
#include <string>

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wheader-guard"
#pragma clang diagnostic ignored "-Wdivision-by-zero"
#pragma clang diagnostic ignored "-Wexpansion-to-defined"
#else
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif
#endif
#include "otb_shark.h"
#include <shark/Algorithms/StoppingCriteria/AbstractStoppingCriterion.h>
#include <shark/Models/LinearModel.h>
#include <shark/Models/ConcatenatedModel.h>
#include <shark/Models/NeuronLayers.h>
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif

namespace otb
{
/**
 * \class AutoencoderModel
 *
 * Autoencoder model wrapper class
 *
 * \ingroup OTBDimensionalityReductionLearning
 */
template <class TInputValue, class NeuronType>
class ITK_EXPORT AutoencoderModel : public MachineLearningModel<itk::VariableLengthVector<TInputValue>, itk::VariableLengthVector<TInputValue>>
{
public:
  typedef AutoencoderModel Self;
  typedef MachineLearningModel<itk::VariableLengthVector<TInputValue>, itk::VariableLengthVector<TInputValue>> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef typename Superclass::InputValueType       InputValueType;
  typedef typename Superclass::InputSampleType      InputSampleType;
  typedef typename Superclass::InputListSampleType  InputListSampleType;
  typedef typename InputListSampleType::Pointer     ListSamplePointerType;
  typedef typename Superclass::TargetValueType      TargetValueType;
  typedef typename Superclass::TargetSampleType     TargetSampleType;
  typedef typename Superclass::TargetListSampleType TargetListSampleType;

  /// Confidence map related typedefs
  typedef typename Superclass::ConfidenceValueType      ConfidenceValueType;
  typedef typename Superclass::ConfidenceSampleType     ConfidenceSampleType;
  typedef typename Superclass::ConfidenceListSampleType ConfidenceListSampleType;

  typedef typename Superclass::ProbaSampleType     ProbaSampleType;
  typedef typename Superclass::ProbaListSampleType ProbaListSampleType;
  /// Neural network related typedefs
  typedef shark::ConcatenatedModel<shark::RealVector> ModelType;
  typedef shark::LinearModel<shark::RealVector, NeuronType>          LayerType;
  typedef shark::LinearModel<shark::RealVector, shark::LinearNeuron> OutLayerType;

  itkNewMacro(Self);
  itkTypeMacro(AutoencoderModel, DimensionalityReductionModel);

  itkGetMacro(NumberOfHiddenNeurons, itk::Array<unsigned int>);
  itkSetMacro(NumberOfHiddenNeurons, itk::Array<unsigned int>);

  itkGetMacro(NumberOfIterations, unsigned int);
  itkSetMacro(NumberOfIterations, unsigned int);

  itkGetMacro(NumberOfIterationsFineTuning, unsigned int);
  itkSetMacro(NumberOfIterationsFineTuning, unsigned int);

  itkGetMacro(Epsilon, double);
  itkSetMacro(Epsilon, double);

  itkGetMacro(InitFactor, double);
  itkSetMacro(InitFactor, double);

  itkGetMacro(Regularization, itk::Array<double>);
  itkSetMacro(Regularization, itk::Array<double>);

  itkGetMacro(Noise, itk::Array<double>);
  itkSetMacro(Noise, itk::Array<double>);

  itkGetMacro(Rho, itk::Array<double>);
  itkSetMacro(Rho, itk::Array<double>);

  itkGetMacro(Beta, itk::Array<double>);
  itkSetMacro(Beta, itk::Array<double>);

  itkGetMacro(WriteLearningCurve, bool);
  itkSetMacro(WriteLearningCurve, bool);

  itkSetMacro(WriteWeights, bool);
  itkGetMacro(WriteWeights, bool);

  itkGetMacro(LearningCurveFileName, std::string);
  itkSetMacro(LearningCurveFileName, std::string);

  bool CanReadFile(const std::string& filename) override;
  bool CanWriteFile(const std::string& filename) override;

  void Save(const std::string& filename, const std::string& name = "") override;
  void Load(const std::string& filename, const std::string& name = "") override;

  void Train() override;

  template <class T>
  void TrainOneLayer(shark::AbstractStoppingCriterion<T>& criterion, unsigned int, shark::Data<shark::RealVector>&, std::ostream&);

  template <class T>
  void TrainOneSparseLayer(shark::AbstractStoppingCriterion<T>& criterion, unsigned int, shark::Data<shark::RealVector>&, std::ostream&);

  template <class T>
  void TrainNetwork(shark::AbstractStoppingCriterion<T>& criterion, shark::Data<shark::RealVector>&, std::ostream&);

protected:
  AutoencoderModel();
  ~AutoencoderModel() override;

  virtual TargetSampleType DoPredict(const InputSampleType& input, ConfidenceValueType* quality = nullptr, ProbaSampleType* proba = nullptr) const override;

  virtual void DoPredictBatch(const InputListSampleType*, const unsigned int& startIndex, const unsigned int& size, TargetListSampleType*,
                              ConfidenceListSampleType* quality = nullptr, ProbaListSampleType* proba = nullptr) const override;

private:
  /** Internal Network */
  ModelType                m_Encoder;
  std::vector<LayerType>   m_InLayers;
  OutLayerType             m_OutLayer;
  itk::Array<unsigned int> m_NumberOfHiddenNeurons;
  /** Training parameters */
  unsigned int       m_NumberOfIterations;           // stop the training after a fixed number of iterations
  unsigned int       m_NumberOfIterationsFineTuning; // stop the fine tuning after a fixed number of iterations
  double             m_Epsilon;                      // Stops the training when the training error seems to converge
  itk::Array<double> m_Regularization;               // L2 Regularization parameter
  itk::Array<double> m_Noise;                        // probability for an input to be set to 0 (denosing autoencoder)
  itk::Array<double> m_Rho;                          // Sparsity parameter
  itk::Array<double> m_Beta;                         // Sparsity regularization parameter
  double             m_InitFactor;                   // Weight initialization factor (the weights are intialized at m_initfactor/sqrt(inputDimension)  )

  bool        m_WriteLearningCurve;    // Flag for writing the learning curve into a txt file
  std::string m_LearningCurveFileName; // Name of the output learning curve printed after training
  bool        m_WriteWeights;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbAutoencoderModel.hxx"
#endif

#endif
