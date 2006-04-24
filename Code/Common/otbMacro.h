

/**
 * otbMacro.h defines standard system-wide macros, constants, and other
 * parameters. One of its most important functions is to define macros used
 * to interface to instance variables in a standard fashion. 
 */
  
#ifndef __otbMacro_h
#define __otbMacro_h

#include "itkMacro.h"

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


