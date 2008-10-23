#ifndef ossimIdManager_HEADER
#define ossimIdManager_HEADER

#include <ossim/base/ossimId.h>

class OSSIMDLLEXPORT ossimIdManager
{
public:
   static ossimIdManager* instance();
   ossimId generateId();
   ossimId generateId(ossim_int64 customId);
   void setCurrentId(ossim_int64 customId);
   
protected:
   ossimIdManager(); // make sure this can't be constructed outside
   ossimIdManager(const ossimIdManager& rhs);
   ~ossimIdManager();
   void operator =(const ossimIdManager& rhs);

   static ossimIdManager* theInstance;
   static ossim_int64            theCurrentId;
};

#endif
