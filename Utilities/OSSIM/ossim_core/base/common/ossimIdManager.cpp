#include "ossimIdManager.h"
#include <time.h>
#ifndef NULL
#include <stddef.h>
#endif


long ossimIdManager::theCurrentId           = ossimId::INVALID_ID;
ossimIdManager* ossimIdManager::theInstance = NULL;

ossimIdManager::ossimIdManager()
{
   theCurrentId=ossimId::INVALID_ID;
}

ossimIdManager::ossimIdManager(const ossimIdManager& /* rhs */)
{
   theCurrentId=ossimId::INVALID_ID;
}

ossimIdManager::~ossimIdManager()
{
   theCurrentId=ossimId::INVALID_ID;   
}

ossimIdManager* ossimIdManager::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimIdManager;
      theCurrentId = -1;
   }

   return theInstance;
}

ossimId ossimIdManager::generateId()
{
   return ossimId(++theCurrentId);
}

ossimId ossimIdManager::generateId(long customId)
{
   if(customId <= theCurrentId)
   {
      return ossimId(++theCurrentId);
   }
   theCurrentId = customId;
   return ossimId(theCurrentId);
}

void ossimIdManager::setCurrentId(long currentId)
{
   theCurrentId = (currentId >=0) ? currentId: theCurrentId;
}

void ossimIdManager::releaseId(const ossimId& idToRelease)
{
   // currently doesn't do anything but we will later
   // store id's and make sure that they are unique.
   //
}
void  ossimIdManager::operator=(const ossimIdManager& /* rhs */)
{}
