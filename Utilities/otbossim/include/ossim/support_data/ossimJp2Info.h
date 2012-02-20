//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: JP2 Info object.
// 
//----------------------------------------------------------------------------
// $Id: ossimJp2Info.h 20125 2011-10-11 19:47:19Z dburken $
#ifndef ossimJp2Info_HEADER
#define ossimJp2Info_HEADER 1

#include <ossim/base/ossimConstants.h>
#include <ossim/support_data/ossimInfoBase.h>
#include <ossim/base/ossimFilename.h>

/**
 * @brief JP2 info class.
 */
class OSSIM_DLL ossimJp2Info : public ossimInfoBase
{
public:

   /** Anonymous enumerations: */
   enum
   {
      SIGNATURE_BOX_SIZE = 12,
      GEOTIFF_UUID_SIZE  = 16
   };
   
   /** default constructor */
   ossimJp2Info();

   /** virtual destructor */
   virtual ~ossimJp2Info();

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

#endif /* End of "#ifndef ossimJp2Info_HEADER" */
