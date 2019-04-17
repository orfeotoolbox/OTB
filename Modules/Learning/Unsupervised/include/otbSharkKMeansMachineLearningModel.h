/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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
#ifndef otbSharkKMeansMachineLearningModel_h
#define otbSharkKMeansMachineLearningModel_h

#include "boost/shared_ptr.hpp"
#include "itkLightObject.h"
#include "otbMachineLearningModel.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wcast-align"
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wheader-guard"
#pragma clang diagnostic ignored "-Wexpansion-to-defined"
#else
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif
#endif

#include "otb_shark.h"
#include "shark/Models/Clustering/HardClusteringModel.h"
#include "shark/Models/Clustering/SoftClusteringModel.h"
#include "shark/Models/Clustering/Centroids.h"
#include "shark/Models/Clustering/ClusteringModel.h"
#include "shark/Algorithms/KMeans.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif

/** \class SharkKMeansMachineLearningModel
 *  \brief Shark version of Random Forests algorithm
 *
 *  This is a specialization of MachineLearningModel class allowing to
 *  use Shark implementation of the Random Forests algorithm.
 *
 *  It is noteworthy that training step is parallel.
 *
 *  For more information, see
 *  http://image.diku.dk/shark/sphinx_pages/build/html/rest_sources/tutorials/algorithms/kmeans.html
 *
 *  \ingroup OTBUnsupervised
 */
namespace otb
{
template<class TInputValue, class TTargetValue>
class ITK_EXPORT SharkKMeansMachineLearningModel : public MachineLearningModel<TInputValue, TTargetValue>
{
public:
  /** Standard class typedefs. */
  typedef SharkKMeansMachineLearningModel                 Self;
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
  typedef typename Superclass::ProbaSampleType             ProbaSampleType;
  typedef typename Superclass::ProbaListSampleType        ProbaListSampleType;
  typedef shark::HardClusteringModel<shark::RealVector>   ClusteringModelType;
  typedef ClusteringModelType::OutputType                 ClusteringOutputType;

  /** Run-time type information (and related methods). */
  itkNewMacro( Self );
  itkTypeMacro( SharkKMeansMachineLearningModel, MachineLearningModel );

  /** Train the machine learning model */
  virtual void Train() override;

  /** Save the model to file */
  virtual void Save(const std::string &filename, const std::string &name = "") override;

  /** Load the model from file */
  virtual void Load(const std::string &filename, const std::string &name = "") override;

  /**\name Classification model file compatibility tests */
  //@{
  /** Is the input model file readable and compatible with the corresponding classifier ? */
  virtual bool CanReadFile(const std::string &) override;

  /** Is the input model file writable and compatible with the corresponding classifier ? */
  virtual bool CanWriteFile(const std::string &) override;
  //@}

  /** Get the maximum number of iteration for the kMeans algorithm.*/
  itkGetMacro( MaximumNumberOfIterations, unsigned );
  /** Set the maximum number of iteration for the kMeans algorithm.*/
  itkSetMacro( MaximumNumberOfIterations, unsigned );

  /** Get the number of class for the kMeans algorithm.*/
  itkGetMacro( K, unsigned );
  /** Set the number of class for the kMeans algorithm.*/
  itkSetMacro( K, unsigned );

  /** Initialize the centroids for the kmeans algorithm */
  void SetCentroidsFromData(const shark::Data<shark::RealVector> & data)
  {
    m_Centroids.setCentroids(data);
    this->Modified();
  }
  
protected:
  /** Constructor */
  SharkKMeansMachineLearningModel();

  /** Destructor */
  virtual ~SharkKMeansMachineLearningModel();

  /** Predict values using the model */
  virtual TargetSampleType
  DoPredict(const InputSampleType &input, ConfidenceValueType *quality = nullptr, ProbaSampleType *proba=nullptr) const override;

  virtual void DoPredictBatch(const InputListSampleType *, const unsigned int &startIndex, const unsigned int &size,
                              TargetListSampleType *, ConfidenceListSampleType * = nullptr, ProbaListSampleType * = nullptr) const override;

  /** PrintSelf method */
  void PrintSelf(std::ostream &os, itk::Indent indent) const override;

private:
  SharkKMeansMachineLearningModel(const Self &) = delete;
  void operator=(const Self &) = delete;

  // Parameters set by the user
  unsigned int m_K;
  unsigned int m_MaximumNumberOfIterations;
  bool m_CanRead;

  /** Centroids results form kMeans */
  shark::Centroids m_Centroids;

  /** shark Model could be SoftClusteringModel or HardClusteringModel */
  boost::shared_ptr<ClusteringModelType> m_ClusteringModel;

};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION

#include "otbSharkKMeansMachineLearningModel.hxx"

#endif

#endif
