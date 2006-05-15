/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P.Imbo
  Language  :   C++
  Date      :   25 avril 2006
  Version   :   
  Role      :   otbMacro defines standard system-wide macros
  $Id$

=========================================================================*/


/**
 * otbMacro.h defines standard system-wide macros, constants, and other
 * parameters. One of its most important functions is to define macros used
 * to interface to instance variables in a standard fashion. 
 */
  
#ifndef __otbMacro_h
#define __otbMacro_h

#include "itkMacro.h"

/** This macro is used to print debug (or other information). They are
 * also used to catch errors, etc. Example usage looks like:
 * itkDebugMacro(<< "this is debug info" << this->SomeVariable); */
#if defined(OTB_LEAN_AND_MEAN) || defined(__BORLANDC__)
#define otbDebugMacro(x)
#else
#define otbDebugMacro(x) \
  { if ( /*this->GetDebug() && */ ::itk::Object::GetGlobalWarningDisplay())   \
    { ::itk::OStringStream itkmsg; \
      itkmsg << "Debug: In " __FILE__ ", line " << __LINE__ << "\n" \
             << this->GetNameOfClass() << " (" << this << "): " x  \
             << "\n\n"; \
      ::itk::OutputWindowDisplayDebugText(itkmsg.str().c_str());} \
}
#endif

#if defined(OTB_LEAN_AND_MEAN) || defined(__BORLANDC__)
#define otbMsgDebugMacro(x)
#else
#define otbMsgDebugMacro(x) \
  { if ( /*this->GetDebug() && */ ::itk::Object::GetGlobalWarningDisplay())   \
    { ::itk::OStringStream itkmsg; \
      itkmsg << " Msg Debug: " x << "\n"; \
      ::itk::OutputWindowDisplayDebugText(itkmsg.str().c_str());} \
}
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


