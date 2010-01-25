//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: USGS DEM Info object.
// 
//----------------------------------------------------------------------------
// $Id$
#ifndef ossimDemInfo_HEADER
#define ossimDemInfo_HEADER

#include <iosfwd>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/support_data/ossimInfoBase.h>

/**
 * @brief USGS DEM info class.
 *
 * Encapsulates the dumpnitf functionality.
 */
class OSSIM_DLL ossimDemInfo : public ossimInfoBase
{
public:
   
   /** default constructor */
   ossimDemInfo();

   /** virtual destructor */
   virtual ~ossimDemInfo();

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
   ossimFilename theFile;
};

#endif /* End of "#ifndef ossimDemInfo_HEADER" */
