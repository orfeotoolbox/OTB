#include <ossim/base/ossimIdManager.h>
#include <time.h>
#ifndef NULL
#include <stddef.h>
#endif


ossim_int64 ossimIdManager::theCurrentId           = ossimId::INVALID_ID;
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

ossimId ossimIdManager::generateId(ossim_int64 customId)
{
   if(customId <= theCurrentId)
   {
      return ossimId(++theCurrentId);
   }
   theCurrentId = customId;
   return ossimId(theCurrentId);
}

void ossimIdManager::setCurrentId(ossim_int64 currentId)
{
   theCurrentId = (currentId >=0) ? currentId: theCurrentId;
}

void  ossimIdManager::operator=(const ossimIdManager& /* rhs */)
{}
