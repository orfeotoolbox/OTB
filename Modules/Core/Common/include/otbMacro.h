/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2018 CS Systemes d'Information (CS SI)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


/**
 * otbMacro.h defines standard system-wide macros, constants, and other
 * parameters. One of its most important functions is to define macros used
 * to interface to instance variables in a standard fashion.
 */

#ifndef otbMacro_h
#define otbMacro_h

#include "itkMacro.h"
#include "itkObject.h"
#include "itkThreadSupport.h"
#include "otbConfigure.h"
#include "otbLogger.h"

/**
 * \namespace otb
 * \brief The "otb" namespace contains all Orfeo Toolbox (OTB) classes.
 *
 * There are several nested namespaces within this one.
 */
namespace otb
{
} // end namespace otb - this is here for documentation purposes
/* ITK 5.0 uses a different threading model compared to ITK 4.x.
 * This has a significant impact on OTB as we make heavy use of itk filters.
 * DynamicThreadedGenerateData() is the newer variant without threadId,
 * and is the preferred signature, which is called by default. This
 * variant can split the requested region into different number of
 * pieces depending on current multi-processing load, which allows
 * better load balancing. The non-dynamic (also known as classic)
 * ThreadedGenerateData() signature has threadId, and number of pieces
 * to be split into is known in advance. It is activated by calling
 * this->DynamicMultiThreadingOff(); in derived class constructor.
 *
 * OTB uses ThreadedGenerateData() with threadId and thus required
 * to call this->DynamicMultiThreadingOff(); in filter's constructor.
 *
 * OTB_DISABLE_DYNAMIC_MT is a simple macro that insert this
 * function call *IF* itk version used is 5.0 or above.
 * This macro expands to empty when OTB is built with ITK 4.x and
 * this ease from ITK 4.x to ITK 5.x. Ideally ThreadedGenerateData() function
 * everywhere in OTB must be replace with DynamicThreadedGenerateData().
 * This for sure is not a glorified sed command. Usage of function parameter
 * threadId (and progress reporter) must be removed as well.
 *
 * It also leads us into a situation that OTB will no longer work with ITK 4.x.
 * We cannot break that until ITK 5.0 is available on all platforms and is well tested.
 * As time of writing this patch ITK 5.0 is in alpha release.
 * Eventually OTB can move up in direction where it is requires ITK 5.0 or.
 * But until things are resolved this macro based compatibility must stay.
 *
 * Finally please note that OTB_DISABLE_DYNAMIC_MT does not disable multithreading.
 * Method or macro name can sometimes cause confusion when reading code.
 *
 * sample run-time exception:
 * =========================
 * 2018-06-04 15:30:09 (DEBUG): Caught itk::ExceptionObject during application execution:
 * 2018-06-04 15:30:09 (DEBUG): /usr/src/ports/itk/Modules/Core/Common/src/itkPoolMultiThreader.cxx:202:
 * itk::ERROR: PoolMultiThreader(0xabad1dea): Exception occurred during SingleMethodExecute
 * /home/rashad/local/include/ITK-5.0/itkImageSource.hxx:276:
 * itk::ERROR: ShiftScaleImageFilter(0xabad1deaff): Subclass should override this method!!! 
 * If old behavior is desired invoke this->DynamicMultiThreadingOff(); before Update() is called. The best place is in class constructor.
 */
#if ITK_VERSION_MAJOR < 5
#define OTB_DISABLE_DYNAMIC_MT
#else
#define OTB_DISABLE_DYNAMIC_MT this->DynamicMultiThreadingOff();
#endif

/*
//ITK_THREAD_RETURN_TYPE and ITK_THREAD_RETURN_VALUE bas been changed in ITK5 (master)
// we need to re-exposed to the global namespace to keep the
// use of these items consistent.
#if ITK_VERSION_MAJOR > 4
#define ITK_THREAD_RETURN_TYPE  itk::ITK_THREAD_RETURN_TYPE
#define ITK_THREAD_RETURN_VALUE itk::ITK_THREAD_RETURN_DEFAULT_VALUE
#endif
*/

#define otbFileContext(x) \
  << "file " __FILE__ ", line " << __LINE__<<", " x

#define otbClassContext(x) \
  << this->GetNameOfClass() << " (" << this << "): " x

// Beware that to log to CRITICAL level, level should be passed as "Error"
#define otbLogMacro(level,msg)                                    \
  {                                                               \
    std::ostringstream itkmsg;                                    \
    itkmsg msg << "\n";                                           \
    otb::Logger::Instance()->level(itkmsg.str().c_str());         \
  }


// Re-definition of old log macros to use the otbLogMacro
#define otbDebugMacro(x) otbLogMacro(Debug,otbFileContext(otbClassContext(x)))
#define otbMsgDebugMacro(x) otbLogMacro(Debug,otbFileContext(x))
#define otbGenericMsgDebugMacro(x) otbLogMacro(Debug,x)
#define otbMsgDevMacro(x) otbLogMacro(Debug,otbFileContext(x))
#define otbWarningMacro(x) otbLogMacro(Warning,otbFileContext(otbClassContext(x)))
#define otbGenericWarningMacro(x) otbLogMacro(Warning,otbFileContext(x))
#define otbGenericMsgTestingMAcro(x) otbLogMacro(Info,"[testing] "<<x)
  
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
      message << "otb::ERROR Unknown error while running " << # command << " (catch(...) )"; \
      ::itk::ExceptionObject e_(__FILE__, __LINE__, message.str(), ITK_LOCATION); \
      throw e_; \
      } \
    std::cout << " Checking valid command " << # command " ok." << std::endl; \
    }

#define otbGenericExceptionMacro(T, x) \
{ \
  std::ostringstream message; \
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
      message << "otb::ERROR Unknown error while running " << # command << " (catch(...) )"; \
      ::itk::ExceptionObject e_(__FILE__, __LINE__, message.str(), ITK_LOCATION); \
      throw e_; \
      } \
    if (result == 1) \
      { \
      std::ostringstream message; \
      message << "otb::ERROR: " << # command << " should be throwing an exception."; \
      ::itk::ExceptionObject e_(__FILE__, __LINE__, message.str(), ITK_LOCATION); \
      throw e_; \
      } \
    }

#  define otbUnusedMacro(x) do { (void)sizeof(x); } while(0)

//
// Unused variable warnings in Debug/Release management.
// see http://stackoverflow.com/questions/2290509/debug-vs-ndebug#2290616
#ifdef OTB_DEBUG
# define otbUseInDebug( name ) name
# define otbUseInRelease( name )
#else // NDEBUG
# define otbUseInDebug( name )
# define otbUseInRelease( name ) name
#endif // NDEBUG

#endif //end of otbMacro.h
