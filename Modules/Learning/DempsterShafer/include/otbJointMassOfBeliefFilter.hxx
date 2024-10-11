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

#ifndef otbJointMassOfBeliefFilter_hxx
#define otbJointMassOfBeliefFilter_hxx

#include "otbJointMassOfBeliefFilter.h"

namespace otb
{
template <class TMassFunction>
JointMassOfBeliefFilter<TMassFunction>::JointMassOfBeliefFilter()
{
  // Set the number of outputs
  this->SetNumberOfRequiredOutputs(1);

  // Build the output
  typename MassFunctionType::Pointer outputPtr = MassFunctionType::New();
  this->SetNthOutput(0, outputPtr.GetPointer());
}

template <class TMassFunction>
void JointMassOfBeliefFilter<TMassFunction>::PushBackInput(const MassFunctionType* input)
{
  this->itk::ProcessObject::PushBackInput(input);
}

template <class TMassFunction>
void JointMassOfBeliefFilter<TMassFunction>::PushFrontInput(const MassFunctionType* input)
{
  this->itk::ProcessObject::PushFrontInput(input);
}

template <class TMassFunction>
void JointMassOfBeliefFilter<TMassFunction>::PopBackInput()
{
  this->itk::ProcessObject::PopBackInput();
}

template <class TMassFunction>
void JointMassOfBeliefFilter<TMassFunction>::PopFrontInput()
{
  this->itk::ProcessObject::PopFrontInput();
}

template <class TMassFunction>
const typename JointMassOfBeliefFilter<TMassFunction>::MassFunctionType* JointMassOfBeliefFilter<TMassFunction>::GetInput(unsigned int idx)
{
  return static_cast<const MassFunctionType*>(this->itk::ProcessObject::GetInput(idx));
}

template <class TMassFunction>
typename JointMassOfBeliefFilter<TMassFunction>::MassFunctionType* JointMassOfBeliefFilter<TMassFunction>::GetOutput()
{
  if (this->GetNumberOfOutputs() < 1)
  {
    return nullptr;
  }
  return static_cast<MassFunctionType*>(this->itk::ProcessObject::GetOutput(0));
}

template <class TMassFunction>
void JointMassOfBeliefFilter<TMassFunction>::GenerateData()
{
  // Retrieve the output pointer
  typename MassFunctionType::Pointer outputPtr = this->GetOutput();

  // Walk the inputs
  for (unsigned int i = 0; i < this->GetNumberOfInputs(); ++i)
  {
    // Retrieve the ith input mass function
    typename MassFunctionType::ConstPointer inputPtr = this->GetInput(i);

    // Combine it with the current joint mass
    this->CombineMasses(inputPtr, outputPtr);
  }
}

template <class TMassFunction>
void JointMassOfBeliefFilter<TMassFunction>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os, indent);
}

template <class TMassFunction>
void JointMassOfBeliefFilter<TMassFunction>::CombineMasses(const MassFunctionType* input, MassFunctionType* output)
{
  // Handle case where output is empty
  if (output->IsEmpty())
  {
    // In this case, copy the input mass function
    output->Copy(input);

    // And return
    return;
  }

  // Define a temporary mass function
  typename MassFunctionType::Pointer newJointMass = MassFunctionType::New();

  // Conflict sum will be used to wheight the final masses
  MassType conflict = itk::NumericTraits<MassType>::Zero;

  // Store joint mass support set so as to avoid computing it again
  // when normalizing by conflict
  LabelSetOfSetType jointSupport;

  // First, retrieve the support set of input and currentJointMap
  LabelSetOfSetType inputSupportSet   = input->GetSupport();
  LabelSetOfSetType currentSupportSet = output->GetSupport();

  // Then, Walk the two sets
  for (typename LabelSetOfSetType::const_iterator inputIt = inputSupportSet.begin(); inputIt != inputSupportSet.end(); ++inputIt)
  {
    for (typename LabelSetOfSetType::const_iterator currentIt = currentSupportSet.begin(); currentIt != currentSupportSet.end(); ++currentIt)
    {
      // Compute intersection
      LabelSetType                       intersectionSet;
      std::insert_iterator<LabelSetType> interIt(intersectionSet, intersectionSet.begin());

      // Perform set intersection
      std::set_intersection(inputIt->begin(), inputIt->end(), currentIt->begin(), currentIt->end(), interIt);

      // Compute mass product
      MassType massProduct = input->GetMass((*inputIt)) * output->GetMass((*currentIt));

      // If intersection is null, update conflict
      if (intersectionSet.empty())
      {
        conflict += massProduct;
      }
      // Else, update output mass
      else
      {
        // Retrieve current mass for this intersection set
        MassType intersectionMass = newJointMass->GetMass(intersectionSet);

        // Cumulate with product of masses
        intersectionMass += massProduct;

        // Update new joint mass
        newJointMass->SetMass(intersectionSet, intersectionMass);

        // Store in joint support
        jointSupport.insert(intersectionSet);
      }
    }
  }
  // Normalize new joint mass with conflict
  MassType conflictCoefficient = 1 / (1 - conflict);

  // Retrieve support of joint mass
  for (typename LabelSetOfSetType::const_iterator it = jointSupport.begin(); it != jointSupport.end(); ++it)
  {
    // Retrieve joint mass
    MassType jointMass = newJointMass->GetMass((*it));

    // Normalize by conflict
    jointMass *= conflictCoefficient;

    // Update joint mass
    newJointMass->SetMass((*it), jointMass);
  }

  // Finally, swap output and newJointMass function
  output->Copy(newJointMass);
}

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbJointMassOfBeliefFilter.hxx"
#endif

#endif
