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
#ifndef __otbWrapperCommandLineLauncher_h
#define __otbWrapperCommandLineLauncher_h

#include <string>
#include "otbMacro.h"
#include "itkObject.h"
#include "itkObjectFactory.h"

#include "otbWrapperTypes.h"
#include "otbWrapperParameterGroup.h"
#include "otbWrapperApplication.h"

//#include "otbWrapperEventsSender.h"
//#include "otbWrapperEvent.h"

namespace otb
{
namespace Wrapper
{
  
/** \class CommandLineLauncher
 *  \brief This class represent an application
 *  TODO
 *
 */
  class ITK_EXPORT CommandLineLauncher: public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef CommandLineLauncher Self;
  typedef itk::Object Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(CommandLineLauncher,itk::Object);

  
protected:
  /** Constructor */
  CommandLineLauncher();

   /** Constructor */
  CommandLineLauncher( Application::Pointer app );

  /** Destructor */
  virtual ~CommandLineLauncher();

private:
  CommandLineLauncher(const CommandLineLauncher &); //purposely not implemented
  void operator =(const CommandLineLauncher&); //purposely not implemented

  Application::Pointer m_Application;

}; //end class

} // end namespace Wrapper
} //end namespace otb

#endif // __otbWrapperCommandLineLauncher_h_
