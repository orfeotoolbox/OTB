//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*******************************************************************
//  $Id: ossimStreamFactoryRegistry.h 11176 2007-06-07 19:45:56Z dburken $
//
#ifndef ossimStreamFactoryRegistry_HEADER
#define ossimStreamFactoryRegistry_HEADER
#include <vector>

#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimIoStream.h>
#include <ossim/base/ossimStreamFactoryBase.h>


class OSSIM_DLL ossimStreamFactoryRegistry : public ossimStreamFactoryBase
{
public:
   static ossimStreamFactoryRegistry* instance();
   virtual ~ossimStreamFactoryRegistry();
   
   void registerFactory(ossimStreamFactoryBase* factory);
   
   virtual ossimRefPtr<ossimIFStream> createNewIFStream(
      const ossimFilename& file, std::ios_base::openmode openMode) const;
   
protected:
   ossimStreamFactoryRegistry();
private:
   /** @brief copy constructor hidden from use */
   ossimStreamFactoryRegistry(const ossimStreamFactoryRegistry&);
   
   std::vector<ossimStreamFactoryBase*> theFactoryList;
   static ossimStreamFactoryRegistry* theInstance;
};

#endif
