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
 *  High. The semantics are not used here. This class it holds a
 *  vector where each component is the membership value for each one
 *  of the qualitative values of the variable (MEDIUM = 0.9). Each
 *  membership function is modeled by a trapezoidal function for which
 *  4 values have to be provided.
 */
template <unsigned int TNValues = 2, class TPrecision=double>
class ITK_EXPORT FuzzyVariable : public itk::DataObject
{
public:
  /** Standard class typedefs */
  typedef FuzzyVariable                      Self;
  typedef itk::DataObject               Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  /** Run-time type information (and related methods). */
  itkTypeMacro(FuzzyVariable, DataObject);
  /** Precision typedef */
  typedef TPrecision PrecisionType;
  /** Type to hold the membership functions */
  typedef itk::FixedArray< PrecisionType, TNValues > ValueVectorType;
  /** Membership value accessors */
  itkGetMacro(Memberships, ValueVectorType);


protected:
  /** Constructor */
  FuzzyVariable();
  /** Desctructor */
  ~FuzzyVariable() {}
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  /** Vector containing the membership functions */
  ValueVectorType m_Memberships;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbFuzzyVariable.txx"
#endif

#endif
