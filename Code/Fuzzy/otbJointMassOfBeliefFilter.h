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
#ifndef __otbJointMassOfBeliefFilter_h
#define __otbJointMassOfBeliefFilter_h

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
 */
template <class TMassFunction>
class ITK_EXPORT JointMassOfBeliefFilter : public itk::ProcessObject
{
public:
  /** Standard class typedefs */
  typedef JointMassOfBeliefFilter                                 Self;
  typedef itk::ProcessObject                           Superclass;
  typedef itk::SmartPointer<Self>                      Pointer;
  typedef itk::SmartPointer<const Self>                ConstPointer;

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
  virtual void PushBackInput(const MassFunctionType * input);

  /** Add an input to the front */
  virtual void PushFrontInput(const MassFunctionType * input);

  /** Remove the last input */
  virtual void PopBackInput();

  /** Remove the first input */
  virtual void PopFrontInput();

  /** Get the idx th input */
  const MassFunctionType * GetInput(unsigned int idx);

  /** Get the output joint mass */
  MassFunctionType * GetOutput();

protected:
  /** Constructor */
  JointMassOfBeliefFilter();

  /** Destructor */
  ~JointMassOfBeliefFilter() {}

  /** GenerateData */
  virtual void GenerateData();

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  JointMassOfBeliefFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Combine masses from input and output into output */
  void CombineMasses(const MassFunctionType * input, MassFunctionType * output);
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbJointMassOfBeliefFilter.txx"
#endif

#endif
