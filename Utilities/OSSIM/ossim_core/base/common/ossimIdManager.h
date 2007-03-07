#ifndef ossimIdManager_HEADER
#define ossimIdManager_HEADER

#include "ossimId.h"

class OSSIMDLLEXPORT ossimIdManager
{
public:
   static ossimIdManager* instance();
   ossimId generateId();
   ossimId generateId(long customId);
   void releaseId(const ossimId& idToRelease);
   void setCurrentId(long customId);
   
protected:
   ossimIdManager(); // make sure this can't be constructed outside
   ossimIdManager(const ossimIdManager& rhs);
   ~ossimIdManager();
   void operator =(const ossimIdManager& rhs);

   static ossimIdManager* theInstance;
   static long            theCurrentId;
};

#endif
