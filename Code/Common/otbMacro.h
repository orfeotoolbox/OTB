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

/**
 * otbMacro.h defines standard system-wide macros, constants, and other
 * parameters. One of its most important functions is to define macros used
 * to interface to instance variables in a standard fashion.
 */

#ifndef __otbMacro_h
#define __otbMacro_h

#include "itkMacro.h"
#include "itkObject.h"
#include "otbConfigure.h"

/**
 * \namespace otb
 * \brief The "otb" namespace contains all Orfeo Toolbox (OTB) classes
 * There are several nested namespaces withing the otb:: namespace.
 */
namespace otb
{
} // end namespace otb - this is here for documentation purposes

/** This macro is used to print debug (or other information). They are
 * also used to catch errors, etc. Example usage looks like:
 * itkDebugMacro(<< "this is debug info" << this->SomeVariable); */
#define otbDebugMacro(x) itkDebugMacro(x)
/*  { if ( this->GetDebug() && *::itk::Object::GetGlobalWarningDisplay())   \
    { std::ostringstream itkmsg; \
      itkmsg << "Debug: In " __FILE__ ", line " << __LINE__ << "\n" \
             << this->GetNameOfClass() << " (" << this << "): " x  \
             << "\n\n"; \
      ::itk::OutputWindowDisplayDebugText(itkmsg.str().c_str()); } \
}*/

#define otbMsgDebugMacro(x) \
    { \
    if (this->GetDebug() && ::itk::Object::GetGlobalWarningDisplay())   \
      { \
      std::ostringstream itkmsg; \
      itkmsg << " Msg Debug: " x << "\n"; \
      ::itk::OutputWindowDisplayDebugText(itkmsg.str().c_str()); \
      } \
    }


#ifndef NDEBUG
#define otbGenericMsgDebugMacro(x) \
    {  \
    if (::itk::Object::GetGlobalWarningDisplay())   \
      { \
      std::ostringstream itkmsg; \
      itkmsg << " Generic Msg Debug: " x << "\n"; \
      ::itk::OutputWindowDisplayDebugText(itkmsg.str().c_str()); \
      } \
    }
#else
#define otbGenericMsgDebugMacro(x)
#endif

#define otbGenericMsgTestingMacro(x) \
    {  \
    std::cout x << std::endl; \
    }

#ifdef OTB_SHOW_ALL_MSG_DEBUG
#define otbMsgDevMacro(x) \
    { \
      { \
      std::ostringstream itkmsg; \
      itkmsg << " Msg Dev: (" << __FILE__ << ":" << __LINE__ << ") " x << "\n"; \
      ::itk::OutputWindowDisplayDebugText(itkmsg.str().c_str()); \
      } \
    }
#else
#define otbMsgDevMacro(x)
#endif

/** This macro is used to print warning information (i.e., unusual circumstance
 * but not necessarily fatal.) Example usage looks like:
 * itkWarningMacro(<< "this is warning info" << this->SomeVariable); */
#define otbWarningMacro(x) \
    { \
    if (itk::Object::GetGlobalWarningDisplay()) \
      { \
      std::ostringstream itkmsg; \
      itkmsg << "WARNING: In " __FILE__ ", line " << __LINE__ << "\n" \
             << this->GetNameOfClass() << " (" << this << "): " x  \
             << "\n\n"; \
      itk::OutputWindowDisplayWarningText(itkmsg.str().c_str()); \
      } \
    }

#define otbGenericWarningMacro(x) \
    { \
    if (itk::Object::GetGlobalWarningDisplay()) \
      { \
      std::ostringstream itkmsg; \
      itkmsg << "WARNING: In " __FILE__ ", line " << __LINE__ << ": " x <<"\n";\
      itk::OutputWindowDisplayWarningText(itkmsg.str().c_str()); \
      } \
    }

/** This macro is used to control condition. It use ONLY by the OTB developers
  *
  */
#define otbControlConditionTestMacro(condition, message) \
    { \
    if ((condition)) itkGenericExceptionMacro(<< message); \
    }

/** Set built-in type.  Creates member Set"name"() (e.g., SetVisibility()); */
#define otbSetObjectMemberMacro(object, name, type) \
  virtual void Set ## name (const type _arg) \
    { \
    itkDebugMacro("setting member " # name " to " << _arg); \
    this->m_ ## object->Set ## name(_arg); \
    this->Modified(); \
    }

/** Get built-in type.  Creates member Get"name"() (e.g., GetVisibility()); */
#define otbGetObjectMemberMacro(object, name, type) \
  virtual type Get ## name () \
    { \
    itkDebugMacro("returning " << # name " of " << this->m_ ## object->Get ## name()); \
    return this->m_ ## object->Get ## name(); \
    }

/** Get built-in type.  Creates member Get"name"() (e.g., GetVisibility());
 * This is the "const" form of the itkGetMacro.  It should be used unless
 * the member can be changed through the "Get" access routine. */
#define otbGetObjectMemberConstMacro(object, name, type) \
  virtual type Get ## name () const \
    { \
    itkDebugMacro("returning " << # name " of " << this->m_ ## object->Get ## name()); \
    return this->m_ ## object->Get ## name(); \
    }

/** Get built-in type.  Creates member Get"name"() (e.g., GetVisibility());
 * This is the "const" form of the itkGetMacro.  It should be used unless
 * the member can be changed through the "Get" access routine.
 * This versions returns a const reference to the variable. */
#define otbGetObjectMemberConstReferenceMacro(object, name, type) \
  virtual const type &Get ## name () const \
    { \
    itkDebugMacro("returning " << # name " of " << this->m_ ## object->Get ## name()); \
    return this->m_ ## object->Get ## name(); \
    }

/** Testing macro. This macro doesn't throw a exception if the called command
 * generate a itk::ExceptionObject object. For alls others use cases, the macro
 * generate a exception. */
#define otbTestingCheckValidCommand(command) \
    { \
    try \
      { \
      command; \
      } \
    catch (const std::exception&)   { \
      throw; } \
    catch (...) \
      { \
      std::ostringstream message; \
      message << "otb::ERROR Unknow error while running " << # command << " (catch(...) )"; \
      ::itk::ExceptionObject e_(__FILE__, __LINE__, message.str().c_str(), ITK_LOCATION); \
      throw e_; \
      } \
    std::cout << " Checking valid command " << # command " ok." << std::endl; \
    }

#define otbGenericExceptionMacro(T, x) \
{ \
  ::itk::OStringStream message; \
  message << "otb::ERROR: " x; \
  T e_(__FILE__, __LINE__, message.str(),ITK_LOCATION); \
  throw e_;\
}

#define otbTestingCheckNotValidCommand(command) \
    { \
    int result(1); \
    try \
      { \
      command; \
      } \
    catch (std::bad_alloc& err)     { \
      throw err; } \
    catch (itk::ExceptionObject&) { std::cout << "Checking not valid Command " << # command " ok." << std::endl; \
                                    result = 0; } \
    catch (const std::exception& stde)   { \
      throw stde; } \
    catch (...) \
      { \
      std::ostringstream message; \
      message << "otb::ERROR Unknow error while running " << # command << " (catch(...) )"; \
      ::itk::ExceptionObject e_(__FILE__, __LINE__, message.str().c_str(), ITK_LOCATION); \
      throw e_; \
      } \
    if (result == 1) \
      { \
      std::ostringstream message; \
      message << "otb::ERROR: " << # command << " should be throwing an exception."; \
      ::itk::ExceptionObject e_(__FILE__, __LINE__, message.str().c_str(), ITK_LOCATION); \
      throw e_; \
      } \
    }

#endif //end of otbMacro.h
