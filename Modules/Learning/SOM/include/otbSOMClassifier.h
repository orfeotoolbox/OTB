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

#ifndef otbSOMClassifier_h
#define otbSOMClassifier_h

#include "itkProcessObject.h"
#include "itkMembershipSample.h"

namespace otb
{

/** \class SOMClassifier
 *  \brief This class implements a SOM-Based classifier.
 *
 * The classifier iterates on the input list sample, feeding
 * the output membership sample with the one-dimensionnal index
 * of the winner neuron.
 *
 * Since this classifier differs from the base framework of itk in that it
 * does not use DecisionRule and Memberships function, it derives from itk::ProcessObject
 * instead of itk::SampleClassifierBase.
 *
 * \sa SOM, SOMMap, SOMActivationBuilder.
 *
 * \ingroup OTBSOM
 */

template<class TSample, class TSOMMap, class TLabel>
class ITK_EXPORT SOMClassifier :
  public itk::ProcessObject
{
public:
  /** Standard class typedef*/
  typedef SOMClassifier                 Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macros */
  itkTypeMacro(SOMClassifier, itk::ProcessObject);
  itkNewMacro(Self);

  /** typedefs from TSample object */
  typedef TSample                                               SampleType;
  typedef typename SampleType::Pointer                          SamplePointerType;
  typedef typename SampleType::MeasurementType                  MeasurementType;
  typedef typename SampleType::MeasurementVectorType            MeasurementVectorType;
  typedef typename SampleType::MeasurementVectorType::ValueType InputPixelType;

  /** SOM Map typedefs */
  typedef TSOMMap                      SOMMapType;
  typedef typename SOMMapType::Pointer SOMMapPointerType;

  /** Output typedefs */
  typedef itk::Statistics::MembershipSample<SampleType> OutputType;
  typedef typename OutputType::Pointer                  OutputPointerType;

  /** Label type typedef */
  typedef TLabel ClassLabelType;

  /// Accessors
  void SetMap(SOMMapType * sommap);
  SOMMapType * GetMap(void);
  itkSetObjectMacro(Sample, SampleType);
  itkGetObjectMacro(Sample, SampleType);
  
  OutputType * GetOutput();

protected:
  /** Constructor */
  SOMClassifier();
  /** Destructor */
  ~SOMClassifier() override {}
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;
  /** Starts the classification process */
  void GenerateData() override;

private:
  /// The input sample
  SamplePointerType m_Sample;

}; // end of class
} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSOMClassifier.hxx"
#endif

#endif
