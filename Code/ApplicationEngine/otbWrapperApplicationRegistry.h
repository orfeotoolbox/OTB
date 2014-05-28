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
#ifndef __otbWrapperApplicationRegistry_h
#define __otbWrapperApplicationRegistry_h

#include <string>
#include "otbMacro.h"
#include "itkObject.h"
#include "itkObjectFactory.h"

#include "otbWrapperApplication.h"

namespace otb
{
namespace Wrapper
{

/** \class Application
 *  \brief This class represent an application
 *  TODO
 *
 */
class ITK_ABI_EXPORT ApplicationRegistry : public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef ApplicationRegistry            Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Class Methods used to interface with the registered factories */

  /** Run-time type information (and related methods). */
  itkTypeMacro(ApplicationRegistry, Object);

  /** Convenient typedefs. */
  typedef otb::Wrapper::Application::Pointer ApplicationPointer;

  /** Set the specified path to the list of application search path. Reinit all previously set paths */
  static void SetApplicationPath(std::string path);

  /** Add the specified path to the list of application search path */
  static void AddApplicationPath(std::string path);

  /** Return the list of available applications */
  static std::vector<std::string> GetAvailableApplications();

  /** Create the specified Application */
  static Application::Pointer CreateApplication(const std::string& applicationName);


protected:
  ApplicationRegistry();
  virtual ~ApplicationRegistry();

private:
  ApplicationRegistry(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace Wrapper
} //end namespace otb

#endif // __otbWrapperApplication_h_
