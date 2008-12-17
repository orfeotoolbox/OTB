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
#include "otbConfigure.h"

/** This macro is used to print debug (or other information). They are
 * also used to catch errors, etc. Example usage looks like:
 * itkDebugMacro(<< "this is debug info" << this->SomeVariable); */
#if defined(OTB_LEAN_AND_MEAN) || defined(__BORLANDC__)
#define otbDebugMacro(x)
#else
#define otbDebugMacro(x) itkDebugMacro(x)
/*  { if ( this->GetDebug() && *::itk::Object::GetGlobalWarningDisplay())   \
    { ::itk::OStringStream itkmsg; \
      itkmsg << "Debug: In " __FILE__ ", line " << __LINE__ << "\n" \
             << this->GetNameOfClass() << " (" << this << "): " x  \
             << "\n\n"; \
      ::itk::OutputWindowDisplayDebugText(itkmsg.str().c_str());} \
}*/
#endif

#if defined(OTB_LEAN_AND_MEAN) || defined(__BORLANDC__)
#define otbMsgDebugMacro(x)
#else
#define otbMsgDebugMacro(x) \
  { if ( this->GetDebug() && ::itk::Object::GetGlobalWarningDisplay())   \
    { ::itk::OStringStream itkmsg; \
      itkmsg << " Msg Debug: " x << "\n"; \
      ::itk::OutputWindowDisplayDebugText(itkmsg.str().c_str());} \
}
#endif

#if defined(OTB_LEAN_AND_MEAN) || defined(__BORLANDC__)
#define otbGenericMsgDebugMacro(x)
#else
  #ifndef NDEBUG
    #define otbGenericMsgDebugMacro(x) \
    {  \
     	if ( ::itk::Object::GetGlobalWarningDisplay())   \
        { ::itk::OStringStream itkmsg; \
          itkmsg << " Generic Msg Debug: " x << "\n"; \
          ::itk::OutputWindowDisplayDebugText(itkmsg.str().c_str());} \
	}
  #else
    #define otbGenericMsgDebugMacro(x)
  #endif
#endif

#define otbGenericMsgTestingMacro(x) \
    {  \
     	std::cout x << std::endl; \
		}


#if defined(OTB_LEAN_AND_MEAN) || defined(__BORLANDC__)
#define otbMsgDevMacro(x)
#else
  #ifdef OTB_SHOW_ALL_MSG_DEBUG
    #define otbMsgDevMacro(x) \
    { { ::itk::OStringStream itkmsg; \
      itkmsg << " Msg Dev: " x << "\n"; \
      ::itk::OutputWindowDisplayDebugText(itkmsg.str().c_str());} \
     }
  #else
     #define otbMsgDevMacro(x)
  #endif
#endif


/** This macro is used to print warning information (i.e., unusual circumstance
 * but not necessarily fatal.) Example usage looks like:
 * itkWarningMacro(<< "this is warning info" << this->SomeVariable); */
#ifdef OTB_LEAN_AND_MEAN
#define otbWarningMacro(x)
#else
#define otbWarningMacro(x) \
{ if (itk::Object::GetGlobalWarningDisplay()) \
    { ::itk::OStringStream itkmsg; \
      itkmsg << "WARNING: In " __FILE__ ", line " << __LINE__ << "\n" \
             << this->GetNameOfClass() << " (" << this << "): " x  \
             << "\n\n"; \
      itk::OutputWindowDisplayWarningText(itkmsg.str().c_str());} \
}
#endif

/** This macro is used to control condition. It use ONLY by the OTB developpers
  *
  */
#define otbControlConditionTestMacro(condition, message) \
{       if( (condition) ) \
          itkGenericExceptionMacro(<<message);\
}



/** Set built-in type.  Creates member Set"name"() (e.g., SetVisibility()); */
#define otbSetObjectMemberMacro(object,name,type) \
  virtual void Set##name (const type _arg) \
{ \
    itkDebugMacro("setting member " #name " to " << _arg); \
    this->m_##object->Set##name(_arg); \
    this->Modified(); \
}

/** Get built-in type.  Creates member Get"name"() (e.g., GetVisibility()); */
#define otbGetObjectMemberMacro(object,name,type) \
  virtual type Get##name () \
{ \
    itkDebugMacro("returning " << #name " of " << this->m_##object->Get##name() ); \
    return this->m_##object->Get##name(); \
}

/** Get built-in type.  Creates member Get"name"() (e.g., GetVisibility());
 * This is the "const" form of the itkGetMacro.  It should be used unless
 * the member can be changed through the "Get" access routine. */
#define otbGetObjectMemberConstMacro(object,name,type) \
  virtual type Get##name () const \
{ \
    itkDebugMacro("returning " << #name " of " << this->m_##object->Get##name() ); \
    return this->m_##object->Get##name(); \
}

/** Get built-in type.  Creates member Get"name"() (e.g., GetVisibility());
 * This is the "const" form of the itkGetMacro.  It should be used unless
 * the member can be changed through the "Get" access routine.
 * This versions returns a const reference to the variable. */
#define otbGetObjectMemberConstReferenceMacro(object,name,type) \
  virtual const type & Get##name () const \
{ \
    itkDebugMacro("returning " << #name " of " << this->m_##object->Get##name() ); \
    return this->m_##object->Get##name(); \
}

/** Testing macro. This macro doesn't throw a exception if the called command
 * generate a itk::ExceptionObject object. For alls others use cases, the macro
 * generate a exception. */
#define otbTestingCheckValidCommand(command) \
  { \
        try \
        { \
                command;\
        } \
        catch( std::bad_alloc & err )     { throw err; } \
        catch( itk::ExceptionObject & e ) { throw e ; } \
        catch( const std::exception & stde)   { throw stde; } \
        catch( ... ) \
        { \
                ::itk::OStringStream message; \
                message << "otb::ERROR Unknow error while running "<<#command<<" (catch(...) )"; \
                ::itk::ExceptionObject e_(__FILE__, __LINE__, message.str().c_str(),ITK_LOCATION); \
                throw e_; \
        } \
        std::cout << " Checking valid command "<< #command " ok."<<std::endl; \
   }

#define otbTestingCheckNotValidCommand(command) \
  { \
        int result(1); \
        try \
        { \
                command;\
        } \
        catch( std::bad_alloc & err )     { throw err; } \
        catch( itk::ExceptionObject & e ) { std::cout << "Checking not valid Command "<< #command " ok."<<std::endl; result = 0; } \
        catch( const std::exception & stde)   { throw stde; } \
        catch( ... ) \
        { \
                ::itk::OStringStream message; \
                message << "otb::ERROR Unknow error while running "<<#command<<" (catch(...) )"; \
                ::itk::ExceptionObject e_(__FILE__, __LINE__, message.str().c_str(),ITK_LOCATION); \
                throw e_; \
        } \
        if(result == 1) \
        { \
                ::itk::OStringStream message; \
                message << "otb::ERROR: "<<#command<<" should be throwing an exception."; \
                ::itk::ExceptionObject e_(__FILE__, __LINE__, message.str().c_str(),ITK_LOCATION); \
                throw e_; \
        } \
   }


#ifdef OTB_USE_VISU_GUI
#include <FL/Fl.H>

// Replace th Fl::run method
#define otbFlRunMacro(condition) { while(condition) { Fl::wait(1e20); } }

// Check if application running or will be stopped
#define otbRunningMacro() \
     private: \
        virtual void StartRunning() {this->m_Running = true;  } \
        virtual void StopRunning()  {this->m_Running = false; } \
        bool m_Running;\
        public: \
        virtual bool IsRunning() { return this->m_Running; }

#define otbTestCallbackWithValue(view,button,button_value)\
  { \
        std::cout<<"Line "<<__LINE__<<", testing widget "<<#button<<" of view "<<view->GetNameOfClass()<<" with value "<<#button_value<<": ";\
        if(view->button->active() && (view->button->callback()))\
	{\
          std::cout<<" active, triggering callback."<<std::endl;\
	  view->button->value(button_value);\
	  view->button->do_callback(view->button);\
	  Fl::check();\
	}\
	else \
        {\
	  std::cout<<"inactive."<<std::endl; \
	}\
  }

#define otbTestCallback(view,button)\
  { \
        std::cout<<"Line "<<__LINE__<<", testing widget "<<#button<<" of view "<<view->GetNameOfClass()<<": ";\
        if(view->button->active() && (view->button->callback()))\
	{\
          std::cout<<" active, triggering callback."<<std::endl;\
	  view->button->do_callback(view->button);\
	  Fl::check();\
	}\
	else \
        {\
	  std::cout<<"inactive."<<std::endl; \
	}\
  }

#define otbTestMenuItemCallback(view,menubar,button)\
  { \
         std::cout<<"Line "<<__LINE__<<", testing menu item "<<#button<<" from menu bar "<<#menubar<<" of view "<< view->GetNameOfClass()<<": ";\
        if(view->button->active() && (view->button->callback()))\
	{\
          std::cout<<" active, triggering callback."<<std::endl;\
	  view->button->do_callback(view->menubar);\
	  Fl::check();\
	}\
	else \
        {\
       	  std::cout<<"inactive."<<std::endl; \
	}\
  }

#else

#define otbFlRunMacro(condition) 
#define otbRunningMacro() 
#define otbTestCallbackWithValue(view,button,button_value)
#define otbTestCallback(view,button)
#define otbTestMenuItemCallback(view,menubar,button)



#endif



namespace otb
{

/**
 * otb::StringStream wrapper to hide differences between
 * std::stringstream and the old ostrstream.  Necessary for
 * portability.
 */
#if !defined(ITK_NO_ANSI_STRING_STREAM)
class StringStream: public std::stringstream
{
public:
  StringStream() {}
private:
  StringStream(const StringStream&);
  void operator=(const StringStream&);
};
#else
namespace StringStreamDetail
{
  class Cleanup
  {
  public:
    Cleanup(std::strstream& ostr): m_StrStream(ostr) {}
    ~Cleanup() { m_StrStream.rdbuf()->freeze(0); }
    static void IgnoreUnusedVariable(const Cleanup&) {}
  protected:
    std::strstream& m_StrStream;
  };
}//namespace OStringStreamDetail

class StringStream: public std::strstream
{
public:
  typedef std::strstream Superclass;
  StringStream() {}
  std::string str()
    {
      StringStreamDetail::Cleanup cleanup(*this);
      StringStreamDetail::Cleanup::IgnoreUnusedVariable(cleanup);
      int pcount = this->pcount();
      const char* ptr = this->Superclass::str();
      return std::string(ptr?ptr:"", pcount);
    }
private:
  StringStream(const StringStream&);
  void operator=(const StringStream&);
};
#endif


}//namespace otb

#endif //end of otbMacro.h


