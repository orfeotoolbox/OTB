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
#ifndef __otbWrapperFilenameParameter_h
#define __otbWrapperFilenameParameter_h

#include <string>
#include "otbWrapperParameter.h"

namespace otb
{
namespace Wrapper
{

/** \class FilenameParameter
 *  \brief This class represent a string parameter for the wrapper framework
 */
class FilenameParameter
  : public Parameter
{
public:
  /** Standard class typedef */
  typedef FilenameParameter               Self;
  typedef Parameter                     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(FilenameParameter, Parameter);

  /** Set any value */
  virtual void SetAnyValue(boost::any v)
  {
    // Perform any cast
    m_Value = boost::any_cast<std::string>(v);

    // Call Modified();
    this->Modified();
  }

  /** Return any value */
  virtual boost::any GetAnyValue()
  {
    return boost::any(m_Value);
  }

  /** Set the value */
  itkSetMacro(Value,std::string);

  /** Get the value */
  itkGetMacro(Value,std::string);

protected:
  /** Constructor */
  FilenameParameter()
  {}

  /** Destructor */
  virtual ~FilenameParameter()
  {}

  std::string m_Value;

private:
  FilenameParameter(const FilenameParameter &); //purposely not implemented
  void operator =(const FilenameParameter&); //purposely not implemented

}; // End class Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
