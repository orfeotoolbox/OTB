//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Mingjie Su
//
// Description: Rpf Info object.
// 
//----------------------------------------------------------------------------
// $Id: ossimRpfInfo.h 1236 2010-08-05 19:45:44Z ming.su $
#ifndef ossimRpfInfo_HEADER
#define ossimRpfInfo_HEADER

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/support_data/ossimInfoBase.h>
#include <ossim/base/ossimFilename.h>

class ossimKeywordlist;
/**
 * @brief HDF info class.
 *
 * Encapsulates the HDF functionality.
 */
class ossimRpfInfo : public ossimInfoBase
{
public:

   /** default constructor */
   ossimRpfInfo();

   /** virtual destructor */
   virtual ~ossimRpfInfo();

   /**
    * @brief open method.
    *
    * @param file File name to open.
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

   bool isOpen();

   private: 
   
     ossimFilename                       theFile;
     ossimFilename                       m_infoFile;
};

#endif /* End of "#ifndef ossimRpfInfo_HEADER" */
