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
#ifndef __otbMassOfBelief_h
#define __otbMassOfBelief_h

#include "itkDataObject.h"
#include "itkObjectFactory.h"
#include "itkNumericTraits.h"

#include <set>
#include <map>
#include <ostream>


namespace otb
{

/** \class MassOfBelief
 *  \brief This class represent a mass of belief function
 *
 *  This class represent a Mass of Belief function to be used in
 *  Dempster-Shafer theory.
 *
 *  Template parameter TLabel is the type of the elements in the
 *  universe set, while TMass is the type of the mass. TLabel can be
 *  any type implementing ordering operator, while TMass should be a
 *  floating point type.
 *
 *  An element of the power-set is represented by the sub-type
 *  LabelSetType, which is a std::set<TLabel>.
 *
 *  A mass can be associated with a given element from the power set
 *  by calling the SetMass method. It is not necessary to associate
 *  masses with each element of the power set: mass is supposed to be
 *  null otherwise declared.
 *
 *  A mass associated with an element from the power-set can be
 *  removed using the RemoveMass method, and all masses can be cleared
 *  using the Clear() function.
 *
 *  The mass associated with a given element from the power-set can be
 *  retrieved using the GetMass method. If no mass has been associated
 *  with this element, null is returned.
 *
 *  The Normalize() method allows normalizing all masses so that they
 *  sum to 1. The EstimateUncertainty() method allows associating to
 *  the uncertain set (i.e. the universe set) a mass so that all
 *  masses sum to 1.
 *
 *  The GetUniverse() method allows retrieving the set of all symbols
 *  (i.e. the universal set), while the GetSupport() method allows
 *  retrieving the set of all elements of the power-set for which a mass
 *  has been associated. This method returns a LabelSetOfSetType,
 *  which is a std::set<std::set<TLabel> >.
 *
 *  The mass function can be optionnaly initialized from a given
 *  universe set by building the power-set and associating a null mass
 *  with every elements of this power-set. This allows ensuring that
 *  two mass functions will share the exact same universe and
 *  power-set, but is not mandatory, since mass is supposed to be null
 *  for any set if not stated otherwise.
 *
 *  Once masses are set, the GetPlausibility() and GetBelief()
 *  function allow computing the belief and plausibility for an
 *  element of the power-set, or directly for a set of elements of the
 *  power-set intersecting (resp. contained in) a given hypothesis.
 *
 *
 * \ingroup OTBDempsterShafer
 */
template <class TLabel = unsigned short, class TMass = double>
class ITK_EXPORT MassOfBelief : public itk::DataObject
{
public:
  /** Standard class typedefs */
  typedef MassOfBelief                  Self;
  typedef itk::DataObject               Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MassOfBelief, DataObject);

  typedef TLabel                          LabelType;
  typedef TMass                           MassType;
  typedef std::set<LabelType>             LabelSetType;
  typedef std::map<LabelSetType, MassType> MassMapType;
  typedef std::set<LabelSetType>          LabelSetOfSetType;

  /** Register a mass associated with an element of the power-set */
  void SetMass(const LabelSetType & labelSet, const MassType & mass);

  /** Retrieve the mass associated with the given element of the power
   *  set */
  MassType GetMass(const LabelSetType & labelSet) const;

  /** Remove mass from the support */
  void RemoveMass(const LabelSetType & labelSet);

  /** Retrieve the support of the mass of belief (i.e. a vector of
   * element of the power set for which the mass of belief is not
   * null) */
  LabelSetOfSetType GetSupport() const;

  /** Get the set of universe symbols for this mass function */
  LabelSetType GetUniverse() const;

  /** Normalize mass set sot that the sum of masses equals 1 */
  void Normalize();

  /** Associate to uncertainty a mass such that the sum of masses
   * equals 1 */
  void EstimateUncertainty();

  /** Clear all masses */
  void Clear();

  /** Initialize with all elements from the power set with null
   *  masses. Please note that this is not mandatory and only allows
   *  to ensure that two mass functions share the same universe, even
   *  if most of masses are null.
   */
  void InitializePowerSetMasses(const LabelSetType & universe);

  /** Get the belief of a given element of the power-set */
  MassType GetBelief(const LabelSetType & labelSet) const;

  /** Get the plausibility of a given element of the power-set */
  MassType GetPlausibility(const LabelSetType & labelSet) const;

  /** Get the belief for an hypothesis containing a given set of
   *  elements from the power-set */
  MassType GetBelief(const LabelSetOfSetType & containedLabelSet) const;

  /** Get the belief for an hypothesis intersecting a given set of
   *  elements from the power set.*/
  MassType GetPlausibility(const LabelSetOfSetType & intersectedLabelSet) const;

  /** Copy masses of two mass functions */
  void Copy(const Self * massOfBelief);

  /** Return true if the support set is null */
  bool IsEmpty() const;

  /** Define a Print static method for label sets
   *  Do not overload << for std::set since it causes
   *  namespace issues. */
  static std::ostream & PrintLabelSet(std::ostream & out,
                                      const LabelSetType & labelSet);

  /** Define a Print static method for label sets
   *  Do not overload << for std::set since it causes
   *  namespace issues. */
  static std::ostream & PrintLabelSetOfSet(std::ostream & out,
                                           const LabelSetOfSetType & labelSet);

protected:
  /** Constructor */
  MassOfBelief() {}

  /** Desctructor */
  virtual ~MassOfBelief() {}

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  MassOfBelief(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** The masses map */
  MassMapType  m_MassesMap;
};

} // end namespace otb


#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMassOfBelief.txx"
#endif

#endif
