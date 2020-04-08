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
#ifndef otbSOMModel_h
#define otbSOMModel_h

#include "otbSOMMap.h"

#include "itkEuclideanDistanceMetric.h" // the distance function

#include "otbCzihoSOMLearningBehaviorFunctor.h"
#include "otbCzihoSOMNeighborhoodBehaviorFunctor.h"

#include "otbMachineLearningModelTraits.h"
#include "otbMachineLearningModel.h"

namespace otb
{

/** \class SOMModel
 * MachineLearningModel for Self-Organizing Map
 *
 * \ingroup OTBDimensionalityReductionLearning
 */
template <class TInputValue, unsigned int MapDimension>
class ITK_EXPORT SOMModel : public MachineLearningModel<itk::VariableLengthVector<TInputValue>, itk::VariableLengthVector<TInputValue>>
{
public:
  typedef SOMModel Self;
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

  // Confidence map related typedefs
  typedef typename Superclass::ConfidenceValueType      ConfidenceValueType;
  typedef typename Superclass::ConfidenceSampleType     ConfidenceSampleType;
  typedef typename Superclass::ConfidenceListSampleType ConfidenceListSampleType;
  typedef typename Superclass::ProbaSampleType          ProbaSampleType;
  typedef typename Superclass::ProbaListSampleType      ProbaListSampleType;
  typedef SOMMap<itk::VariableLengthVector<TInputValue>, itk::Statistics::EuclideanDistanceMetric<itk::VariableLengthVector<TInputValue>>, MapDimension>
                                        MapType;
  typedef typename MapType::SizeType    SizeType;
  typedef typename MapType::SpacingType SpacingType;

  typedef Functor::CzihoSOMLearningBehaviorFunctor     SOMLearningBehaviorFunctorType;
  typedef Functor::CzihoSOMNeighborhoodBehaviorFunctor SOMNeighborhoodBehaviorFunctorType;

  itkNewMacro(Self);
  itkTypeMacro(SOMModel, DimensionalityReductionModel);

  /** Accessors */
  itkSetMacro(NumberOfIterations, unsigned int);
  itkGetMacro(NumberOfIterations, unsigned int);
  itkSetMacro(BetaInit, double);
  itkGetMacro(BetaInit, double);
  itkSetMacro(WriteMap, bool);
  itkGetMacro(WriteMap, bool);
  itkSetMacro(BetaEnd, double);
  itkGetMacro(BetaEnd, double);
  itkSetMacro(MinWeight, InputValueType);
  itkGetMacro(MinWeight, InputValueType);
  itkSetMacro(MaxWeight, InputValueType);
  itkGetMacro(MaxWeight, InputValueType);
  itkSetMacro(MapSize, SizeType);
  itkGetMacro(MapSize, SizeType);
  itkSetMacro(NeighborhoodSizeInit, SizeType);
  itkGetMacro(NeighborhoodSizeInit, SizeType);
  itkSetMacro(RandomInit, bool);
  itkGetMacro(RandomInit, bool);
  itkSetMacro(Seed, unsigned int);
  itkGetMacro(Seed, unsigned int);

  bool CanReadFile(const std::string& filename) override;
  bool CanWriteFile(const std::string& filename) override;

  void Save(const std::string& filename, const std::string& name = "") override;
  void Load(const std::string& filename, const std::string& name = "") override;

  void Train() override;

protected:
  SOMModel();
  ~SOMModel() override;

private:
  typename MapType::Pointer m_SOMMap;

  virtual TargetSampleType DoPredict(const InputSampleType& input, ConfidenceValueType* quality = nullptr, ProbaSampleType* proba = nullptr) const override;

  /** Map size (width, height) */
  SizeType m_MapSize;
  /** Number of iterations */
  unsigned int m_NumberOfIterations;
  /** Initial learning coefficient */
  double m_BetaInit;
  /** Final learning coefficient */
  double m_BetaEnd;
  /** Initial neighborhood size */
  SizeType m_NeighborhoodSizeInit;
  /** Minimum initial neuron weights */
  InputValueType m_MinWeight;
  /** Maximum initial neuron weights */
  InputValueType m_MaxWeight;
  /** Random initialization bool */
  bool m_RandomInit;
  /** Seed for random initialization */
  unsigned int m_Seed;
  /** Behavior of the Learning weightening (link to the beta coefficient) */
  SOMLearningBehaviorFunctorType m_BetaFunctor;
  /** Behavior of the Neighborhood extent */
  SOMNeighborhoodBehaviorFunctorType m_NeighborhoodSizeFunctor;
  /** Write the SOM Map vectors in a txt file */
  bool m_WriteMap;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSOMModel.hxx"
#endif

#endif
