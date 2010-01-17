//----------------------------------------------------------------------------
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:  Generic OSSIM Exception that is a std::exception with a
// what() method implemented.
//----------------------------------------------------------------------------
// $Id: ossimException.cpp 10029 2006-12-04 23:23:11Z dburken $

#include <ossim/base/ossimException.h>

ossimException::ossimException() throw()
   : theErrorMessage()
{
}

ossimException::ossimException(const std::string& errorMessage) throw()
   : theErrorMessage(errorMessage)
{
}

ossimException::~ossimException() throw()
{
}

const char* ossimException::what() const throw()
{
   return theErrorMessage.c_str();
}
