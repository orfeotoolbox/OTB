//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: DTED Info object.
// 
//----------------------------------------------------------------------------
// $Id$
#ifndef ossimDtedInfo_HEADER
#define ossimDtedInfo_HEADER

#include <iosfwd>
#include <vector>
#include <ossim/base/ossimConstants.h>
#include <ossim/support_data/ossimInfoBase.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimRefPtr.h>

class ossimNitfFile;
class ossimProperty;

/**
 * @brief DTED Info class.
 *
 * Encapsulates the dtedInfo functionality.
 */
class OSSIM_DLL ossimDtedInfo : public ossimInfoBase
{
public:
   
   /** default constructor */
   ossimDtedInfo();

   /** virtual destructor */
   virtual ~ossimDtedInfo();

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

   /**
    * @brief Gets a property for name.
    * @param name Property name to get.
    * @return ossimRefPtr<ossimProperty> Note that this can be empty if
    * property for name was not found.
    */
   ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   
   /**
    * @brief Adds this class's properties to list.
    * @param propertyNames list to append to.
    */
   void getPropertyNames(std::vector<ossimString>& propertyNames)const;

private:
   ossimFilename theFile;
};

#endif /* End of "#ifndef ossimDtedInfo_HEADER" */
