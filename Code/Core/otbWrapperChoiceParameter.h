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
  void AddChoice( std::string key, Parameter::Pointer param )
  {
    m_ChoiceList.push_back(std::make_pair(key, param));
  }

  Parameter::Pointer GetChoice( int i )
  {
    return m_ChoiceList[i].second;
  }

  unsigned int GetNbChoice( void )
  {
    return m_ChoiceList.size();
  }

  /** Set any value */
  virtual void SetAnyValue(boost::any v)
  {
    // Perform any cast
    m_CurrentChoice = boost::any_cast<bool>(v);

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
  {}

  /** Destructor */
  ~ChoiceParameter()
  {}

private:
  ChoiceParameter(const ChoiceParameter &); //purposely not implemented
  void operator =(const ChoiceParameter&); //purposely not implemented

  typedef std::pair<std::string, Parameter::Pointer> Choice;

  typedef std::vector<Choice> ChoiceList;
  ChoiceList m_ChoiceList;

  unsigned int m_CurrentChoice;

}; // End class Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
