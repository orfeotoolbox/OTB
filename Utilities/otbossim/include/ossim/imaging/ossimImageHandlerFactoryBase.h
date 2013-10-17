//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimImageHandlerFactoryBase.h 22228 2013-04-12 14:11:45Z dburken $

#ifndef ossimImageHandlerFactoryBase_HEADER
#define ossimImageHandlerFactoryBase_HEADER 1

#include <algorithm>

#include <ossim/base/ossimString.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/base/ossimObjectFactory.h>

class ossimFilename;
class ossimKeywordlist;

class OSSIM_DLL ossimImageHandlerFactoryBase : public ossimObjectFactory
{
public:
   template <class T> class UniqueList
   {
   public:
      
      void push_back(const T& value)
      {
         if(std::find(theList.begin(),theList.end(), value) == theList.end())
         {
            theList.push_back(value);
         }
      }

      const std::vector<T>& getList()const
      {
         return theList;
      }
      ossim_uint32 size()const
      {
         return theList.size();
      }
      const T& operator[](ossim_uint32 idx)const
      {
         return theList[idx];
      }
      T& operator[](ossim_uint32 idx)
      {
         return theList[idx];
      }
   protected:
      std::vector<T> theList;
   };
   
   typedef UniqueList<ossimString> UniqueStringList;
   typedef std::vector<ossimRefPtr<ossimImageHandler> > ImageHandlerList;
   
   virtual ossimImageHandler* open(const ossimFilename& fileName,
                                   bool openOverview=true)const = 0;
   virtual ossimImageHandler* open(const ossimKeywordlist& kwl,
                                   const char* prefix=0)const = 0;

   /**
    * @brief Open overview that takes a file name.
    *
    * This default implementation returns a null ref pointer.
    * Derived factories that have overview readers should override.
    * 
    * @param file File to open.
    *
    * @return This default implementation returns a null ref pointer.
    */
   virtual ossimRefPtr<ossimImageHandler> openOverview(
      const ossimFilename& file ) const;

   virtual void getImageHandlersBySuffix(ImageHandlerList& result,
                                         const ossimString& ext)const;

   virtual void getImageHandlersByMimeType(ImageHandlerList& result,
                                           const ossimString& mimeType)const;

   virtual void getSupportedExtensions(ossimImageHandlerFactoryBase::UniqueStringList& extensionList)const=0;
   
TYPE_DATA
};

#endif
