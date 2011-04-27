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

#include "otbWrapperParameter.h"
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
class ChoiceParameter
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
  void AddChoice( std::string key, std::string name, Parameter* param )
  {
    ParameterGroup* paramAsGroup = dynamic_cast<ParameterGroup*>(param);
    if ( paramAsGroup != 0 )
      {
      Choice choice;
      choice.m_Key = key;
      choice.m_Name = name;
      choice.m_AssociatedParameter = paramAsGroup;
      m_ChoiceList.push_back(choice);
      }
    else
      {
      // wrap in  group first
      ParameterGroup::Pointer group = ParameterGroup::New();
      group->AddParameter(param);
      AddChoice(key, name, group.GetPointer());
      }
  }

  std::string GetChoiceKey( int i )
  {
    return m_ChoiceList[i].m_Key;
  }

  std::string GetChoiceName( int i )
  {
    return m_ChoiceList[i].m_Name;
  }

  ParameterGroup::Pointer GetChoiceAssociatedParameter( int i )
  {
    return m_ChoiceList[i].m_AssociatedParameter;
  }

  unsigned int GetNbChoices( void )
  {
    return m_ChoiceList.size();
  }

  /** Set any value */
  virtual void SetValue(unsigned int v)
  {
    // Perform any cast
    m_CurrentChoice = v;
    // Call Modified();
    this->Modified();
  }

  /** Return any value */
  virtual unsigned int GetValue()
  {
    return m_CurrentChoice;
  }

  /** Set any value */
  virtual void SetAnyValue(boost::any v)
  {
    // Perform any cast
    unsigned int val = boost::any_cast<unsigned int>(v);

    if ( val >= GetNbChoices() )
      {
      itkExceptionMacro(<< "Invalid choice value : " << val)
      }

    m_CurrentChoice = val;

    // Call Modified();
    this->Modified();
  }

  /** Return any value */
  virtual boost::any GetAnyValue()
  {
    return boost::any(m_CurrentChoice);
  }


protected:
  /** Constructor */
  ChoiceParameter()
    : m_CurrentChoice(0)
  {}

  /** Destructor */
  virtual ~ChoiceParameter()
  {}

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
