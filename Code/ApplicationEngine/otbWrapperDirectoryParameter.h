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
#ifndef __otbWrapperDirectoryParameter_h
#define __otbWrapperDirectoryParameter_h

#include <string>
#include "otbWrapperStringParameter.h"

namespace otb
{
namespace Wrapper
{

/** \class DirectoryParameter
 *  \brief This class represent a string parameter for the wrapper framework
 */
class DirectoryParameter
  : public StringParameter
{
public:
  /** Standard class typedef */
  typedef DirectoryParameter            Self;
  typedef StringParameter               Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(DirectoryParameter, Parameter);

protected:
  /** Constructor */
  DirectoryParameter()
  {}

  /** Destructor */
  virtual ~DirectoryParameter()
  {}

private:
  DirectoryParameter(const DirectoryParameter &); //purposely not implemented
  void operator =(const DirectoryParameter&); //purposely not implemented

}; // End class Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
