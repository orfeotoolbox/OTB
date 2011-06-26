//----------------------------------------------------------------------------
//
// File: ossimLibLasInfo.h
// 
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: LAS LIDAR info object.
// 
//----------------------------------------------------------------------------
// $Id$
#ifndef ossimLasInfo_HEADER
#define ossimLasInfo_HEADER 1

#include <ossim/base/ossimConstants.h>
#include <ossim/support_data/ossimInfoBase.h>
#include <ossim/base/ossimFilename.h>

#include <iosfwd>

/** @brief LAS LIDAR info object. */
class OSSIM_DLL ossimLasInfo : public ossimInfoBase
{
public:
   
   /** default constructor */
   ossimLasInfo();

   /** virtual destructor */
   virtual ~ossimLasInfo();

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

#endif /* End of "#ifndef ossimLasInfo_HEADER" */
