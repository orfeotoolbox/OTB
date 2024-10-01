/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbJointMassOfBeliefFilter_h
#define otbJointMassOfBeliefFilter_h

#include "itkProcessObject.h"

namespace otb
{
/** \class JointMassOfBeliefFilter
 *  \brief Performs Dempster-Shafer combination of n masses function.
 *
 *  This filter computes the joint mass of n input masses using
 *  Dempster-Shafer rule of combination.
 *
 *  Input masses can be added by using the PushBackInput() method.
 *
 *  \sa MassOfBelief
 *
 * \ingroup OTBDempsterShafer
 */
template <class TMassFunction>
class ITK_EXPORT JointMassOfBeliefFilter : public itk::ProcessObject
{
public:
  /** Standard class typedefs */
  typedef JointMassOfBeliefFilter       Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(JointMassOfBeliefFilter, ProcessObject);

  /** Standard class typedefs */
  typedef TMassFunction                                MassFunctionType;
  typedef typename MassFunctionType::Pointer           MassFunctionPointerType;
  typedef typename MassFunctionType::LabelType         LabelType;
  typedef typename MassFunctionType::MassType          MassType;
  typedef typename MassFunctionType::LabelSetType      LabelSetType;
  typedef typename MassFunctionType::LabelSetOfSetType LabelSetOfSetType;

  /** Add an input to the end */
  using Superclass::PushBackInput;
  virtual void PushBackInput(const MassFunctionType* input);

  /** Add an input to the front */
  using Superclass::PushFrontInput;
  virtual void PushFrontInput(const MassFunctionType* input);

  /** Remove the last input */
  using Superclass::PopBackInput;
  void PopBackInput() override;

  /** Remove the first input */
  using Superclass::PopFrontInput;
  void PopFrontInput() override;

  /** Get the idx th input */
  const MassFunctionType* GetInput(unsigned int idx);

  /** Get the output joint mass */
  MassFunctionType* GetOutput();

protected:
  /** Constructor */
  JointMassOfBeliefFilter();

  /** Destructor */
  ~JointMassOfBeliefFilter() override
  {
  }

  /** GenerateData */
  void GenerateData() override;

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  JointMassOfBeliefFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** Combine masses from input and output into output */
  void CombineMasses(const MassFunctionType* input, MassFunctionType* output);
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbJointMassOfBeliefFilter.hxx"
#endif

#endif
