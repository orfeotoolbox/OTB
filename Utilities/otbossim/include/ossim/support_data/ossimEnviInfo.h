//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: ENVI Info object.
// 
//----------------------------------------------------------------------------
// $Id$
#ifndef ossimEnviInfo_HEADER
#define ossimEnviInfo_HEADER 1

#include <ossim/base/ossimConstants.h>
#include <ossim/support_data/ossimInfoBase.h>
#include <ossim/base/ossimFilename.h>

/** @brief ENVI info class. */
class OSSIM_DLL ossimEnviInfo : public ossimInfoBase
{
public:
   
   /** default constructor */
   ossimEnviInfo();

   /** virtual destructor */
   virtual ~ossimEnviInfo();

   /**
    * @brief open method.
    *
    * @param file File name to open.
    *
    * @return true on success false on error.
    */
   virtual bool open(const ossimFilename& file);

   /**
    * Print method.
    *
    * @param out Stream to print to.
    * 
    * @return std::ostream&
    */
   virtual std::ostream& print(std::ostream& out) const;

private:

   ossimFilename m_file;
};

#endif /* End of "#ifndef ossimEnviInfo_HEADER" */
