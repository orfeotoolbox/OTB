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
#ifndef __otbFuzzyVariable_h
#define __otbFuzzyVariable_h

#include "itkDataObject.h"
#include "itkFixedArray.h"
#include "itkObjectFactory.h"

namespace otb
{
/** \class FuzzyVariable
 *  \brief Class to represent a fuzzy N-valued variable.
 *
 *  A fuzzy variable is defined as a set of qualitative values for a
 *  magnitude. These values are ordered, as for instance Low, Medium,
 *  High. The semantics are not used here. This class holds a
 *  vector where each component is the membership value for each one
 *  of the qualitative values of the variable (MEDIUM = 0.9). Each
 *  membership function is modeled by a trapezoidal function for which
 *  4 values have to be provided.
 *  In addition, 2 more values can be provided to define the min and
 *  the max. By default, they are 0 and 1 respectively.
 *
 */
template <class TLabel = unsigned short, class TPrecision=double>
class ITK_EXPORT FuzzyVariable : public itk::DataObject
{
public:
  /** Standard class typedefs */
  typedef FuzzyVariable                 Self;
  typedef itk::DataObject               Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  /** Run-time type information (and related methods). */
  itkTypeMacro(FuzzyVariable, DataObject);

  /** Label typedef */
  typedef TLabel     LabelType;

  /** Precision typedef */
  typedef TPrecision PrecisionType;

  /** Type to hold the membership values */
  typedef std::map<LabelType, PrecisionType>        MembershipValueType;
  typedef itk::FixedArray< PrecisionType, 6>        ParametersType;
  typedef std::map<LabelType, ParametersType>       ParametersMapType;

  /** Get the membership related to one label */
  PrecisionType GetMembership(const LabelType & var, const PrecisionType & value) const;

  /** Get all the memberships */
  MembershipValueType GetMembership(const PrecisionType & value) const;

  /** Get the label with the highest membership */
  LabelType GetMaxVar(const PrecisionType & value) const;

  /** Set the membership for a given Label */
  void SetMembership(const LabelType & var,
                     const PrecisionType & v1,
                     const PrecisionType & v2,
                     const PrecisionType & v3,
                     const PrecisionType & v4);

  void SetMembership(const LabelType & var,
                     const PrecisionType & v1,
                     const PrecisionType & v2,
                     const PrecisionType & v3,
                     const PrecisionType & v4,
                     const PrecisionType & min,
                     const PrecisionType & max);

  /** Remove a given label from the membership table */
  void RemoveMembership(const LabelType & var);

  /** Clear all memberships */
  void Clear();

  /** Print a MembershipValueType */
  static std::ostream& PrintMembershipValueType(std::ostream & out,
                                                const MembershipValueType& membership);


protected:
  /** Constructor */
  FuzzyVariable();
  /** Destructor */
  virtual ~FuzzyVariable() {}
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  /** Map containing the membership functions */
  ParametersMapType m_MembershipFunctions;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbFuzzyVariable.txx"
#endif

#endif
