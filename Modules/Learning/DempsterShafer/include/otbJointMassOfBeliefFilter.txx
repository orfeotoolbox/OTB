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
#ifndef __otbJointMassOfBeliefFilter_txx
#define __otbJointMassOfBeliefFilter_txx

#include "otbJointMassOfBeliefFilter.h"

namespace otb
{
template <class TMassFunction>
JointMassOfBeliefFilter<TMassFunction>
::JointMassOfBeliefFilter()
{
  // Set the number of outputs
  this->SetNumberOfRequiredOutputs(1);

  // Build the output
  typename MassFunctionType::Pointer outputPtr = MassFunctionType::New();
  this->SetNthOutput(0, outputPtr.GetPointer());
}

template <class TMassFunction>
void
JointMassOfBeliefFilter<TMassFunction>
::PushBackInput(const MassFunctionType * input)
{
  this->itk::ProcessObject::PushBackInput(input);
}

template <class TMassFunction>
void
JointMassOfBeliefFilter<TMassFunction>
::PushFrontInput(const MassFunctionType * input)
{
  this->itk::ProcessObject::PushFrontInput(input);
}

template <class TMassFunction>
void
JointMassOfBeliefFilter<TMassFunction>
::PopBackInput()
{
  this->itk::ProcessObject::PopBackInput();
}

template <class TMassFunction>
void
JointMassOfBeliefFilter<TMassFunction>
::PopFrontInput()
{
  this->itk::ProcessObject::PopFrontInput();
}

template <class TMassFunction>
const typename JointMassOfBeliefFilter<TMassFunction>
::MassFunctionType *
JointMassOfBeliefFilter<TMassFunction>
::GetInput(unsigned int idx)
{
  return static_cast<const MassFunctionType *>(this->itk::ProcessObject::GetInput(idx));
}

template <class TMassFunction>
typename JointMassOfBeliefFilter<TMassFunction>
::MassFunctionType *
JointMassOfBeliefFilter<TMassFunction>
::GetOutput()
{
  if(this->GetNumberOfOutputs() < 1)
    {
    return 0;
    }
  return static_cast<MassFunctionType *>(this->itk::ProcessObject::GetOutput(0));
}

template <class TMassFunction>
void
JointMassOfBeliefFilter<TMassFunction>
::GenerateData()
{
  // Retrieve the output pointer
  typename MassFunctionType::Pointer outputPtr = this->GetOutput();

  // Walk the inputs
  for(unsigned int i = 0; i < this->GetNumberOfInputs(); ++i)
    {
    // Retrieve the ith input mass function
    typename MassFunctionType::ConstPointer inputPtr = this->GetInput(i);

    // Combine it with the current joint mass
    this->CombineMasses(inputPtr, outputPtr);
    }
}

template <class TMassFunction>
void
JointMassOfBeliefFilter<TMassFunction>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os, indent);
}

template <class TMassFunction>
void
JointMassOfBeliefFilter<TMassFunction>
::CombineMasses(const MassFunctionType * input, MassFunctionType * output)
{
  // Handle case where output is empty
  if(output->IsEmpty())
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
  LabelSetOfSetType inputSupportSet = input->GetSupport();
  LabelSetOfSetType currentSupportSet = output->GetSupport();

  // Then, Walk the two sets
  for(typename LabelSetOfSetType::const_iterator inputIt = inputSupportSet.begin();
      inputIt != inputSupportSet.end(); ++inputIt)
    {
    for(typename LabelSetOfSetType::const_iterator currentIt = currentSupportSet.begin();
      currentIt != currentSupportSet.end(); ++currentIt)
      {
      // Compute intersection
      LabelSetType intersectionSet;
      std::insert_iterator<LabelSetType> interIt(intersectionSet, intersectionSet.begin());

      // Perform set intersection
      std::set_intersection(inputIt->begin(), inputIt->end(), currentIt->begin(), currentIt->end(), interIt);

      // Compute mass product
      MassType massProduct = input->GetMass((*inputIt))*output->GetMass((*currentIt));

      // If intersection is null, update conflict
      if(intersectionSet.empty())
        {
        conflict+= massProduct;
        }
      // Else, update output mass
      else
        {
        // Retrieve current mass for this intersection set
        MassType intersectionMass = newJointMass->GetMass(intersectionSet);

        // Cumulate with product of masses
        intersectionMass+=massProduct;

        // Update new joint mass
        newJointMass->SetMass(intersectionSet, intersectionMass);

        // Store in joint support
        jointSupport.insert(intersectionSet);
        }
      }
    }
  // Normalize new joint mass with conflict
  MassType conflictCoefficient = 1/(1-conflict);

  // Retrieve support of joint mass
  for(typename LabelSetOfSetType::const_iterator it = jointSupport.begin();
      it!=jointSupport.end(); ++it)
    {
    // Retrieve joint mass
    MassType jointMass = newJointMass->GetMass((*it));

    // Normalize by conflict
    jointMass*=conflictCoefficient;

    // Update joint mass
    newJointMass->SetMass((*it), jointMass);
    }

  // Finally, swap output and newJointMass function
  output->Copy(newJointMass);
}

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbJointMassOfBeliefFilter.txx"
#endif

#endif
