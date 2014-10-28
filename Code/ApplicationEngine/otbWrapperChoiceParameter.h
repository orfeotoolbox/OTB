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
#ifndef __otbWrapperChoiceParameter_h
#define __otbWrapperChoiceParameter_h

#include "otbWrapperParameterGroup.h"

namespace otb
{
namespace Wrapper
{

/** \class ChoiceParameter
 *  \brief This class represent a multiple choice parameter for the wrapper framework
 *
 *  Represents an enumerated parameter.
 *  The different values of the enumeration can be associated with a parameter
 */
class ITK_ABI_EXPORT ChoiceParameter
  : public Parameter
{
public:
  /** Standard class typedef */
  typedef ChoiceParameter               Self;
  typedef Parameter                     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(ChoiceParameter, Parameter);

  /** Add a value to the choice */
  void AddChoice( std::string choicekey, std::string choiceName );

  /** Get the key of a specific choice value */
  std::string GetChoiceKey( int i );

  /** Get the list of the different choice keys */
  std::vector<std::string> GetChoiceKeys();

  /** Get the long name of a specific choice value */
  std::string GetChoiceName( int i );

  /** Get the list of the different choice keys */
  std::vector<std::string> GetChoiceNames();

  /** Get the ParameterGroup associated to a choice value */
  ParameterGroup::Pointer GetChoiceParameterGroupByIndex( int i );

  /** Get the ParameterGroup associated to a choice value */
  ParameterGroup::Pointer GetChoiceParameterGroupByKey( std::string choiceKey );

  /** Get all parameters that are child of this choice parameter */
  std::vector<std::string> GetParametersKeys();

  /** Get the number of available choice */
  unsigned int GetNbChoices( void );

  /** Set choice value */
  virtual void SetValue(unsigned int v);

  /** Set choice value by its key */
  virtual void SetValue(std::string choiceKey);

  /** Return any value */
  virtual unsigned int GetValue();

  bool HasValue() const
  {
    // a choice parameter always has a value
    return true;
  }

  void ClearValue()
  {
    // nothing to do : a choice parameter always has a value
  }

protected:
  /** Constructor */
  ChoiceParameter();

  /** Destructor */
  virtual ~ChoiceParameter();

  struct Choice
  {
    Choice() {}

    std::string             m_Key;
    std::string             m_Name;
    ParameterGroup::Pointer m_AssociatedParameter;
  };

  typedef std::vector<Choice> ChoiceList;
  ChoiceList m_ChoiceList;

  unsigned int m_CurrentChoice;

private:
  ChoiceParameter(const ChoiceParameter &); //purposely not implemented
  void operator =(const ChoiceParameter&); //purposely not implemented

}; // End class Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
