//----------------------------------------------------------------------------
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:  Generic OSSIM Exception that is a std::exception with a
// what() method implemented.
//----------------------------------------------------------------------------
// $Id: ossimException.h 10029 2006-12-04 23:23:11Z dburken $
#ifndef ossimException_HEADER
#define ossimException_HEADER

#include <exception>
#include <string>
#include <ossim/base/ossimConstants.h>

class OSSIM_DLL ossimException : public std::exception
{
public:

   /** @brief default construction */
   ossimException() throw();

   /**
    * @brief construction that takes an error string.
    * @param errorMessage The error message.
    */
   ossimException(const std::string& errorMessage) throw();

   /** @brief virtual destructor. */
   virtual ~ossimException() throw();

   /**
    * @brief Returns the error message.
    * @return The error message as a C-style character string.
    */
   virtual const char* what() const throw();

private:

   /** This is the error message returned by what(). */
   std::string theErrorMessage;
};

#endif /* End of #ifndef ossimException_HEADER */
