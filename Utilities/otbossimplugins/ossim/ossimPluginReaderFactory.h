//----------------------------------------------------------------------------
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Image handler factory for ossim plugins plugin.
//----------------------------------------------------------------------------
// $Id$
#ifndef ossimPluginReaderFactory_HEADER
#define ossimPluginReaderFactory_HEADER 1

#include <ossimPluginConstants.h>
#include <ossim/imaging/ossimImageHandlerFactoryBase.h>

class ossimString;
class ossimFilename;
class ossimKeywordlist;

namespace ossimplugins
{


/** @brief Image handler factory for ossim plugins plugin. */
   class OSSIM_PLUGINS_DLL ossimPluginReaderFactory : public ossimImageHandlerFactoryBase
   {
   public:

      /** @brief virtual destructor */
      virtual ~ossimPluginReaderFactory();

      /**
       * @brief static method to return instance (the only one) of this class.
       * @return pointer to instance of this class.
       */
      static ossimPluginReaderFactory* instance();

      /**
       * @brief open that takes a file name.
       * @param file The file to open.
       * @return pointer to image handler on success, 0 on failure.
       */
      virtual ossimImageHandler* open(const ossimFilename& fileName,
                                      bool openOverview=true) const;

      /**
       * @brief open that takes a keyword list and prefix.
       * @param kwl The keyword list.
       * @param prefix the keyword list prefix.
       * @return pointer to image handler on success, 0 on failure.
       */
      virtual ossimImageHandler* open(const ossimKeywordlist& kwl,
                                      const char* prefix=0)const;

      /**
       * @brief createObject that takes a class name.
       * @param typeName The name of the class..
       * @return pointer to image writer on success, 0 on failure.
       */
      virtual ossimObject* createObject(const ossimString& typeName)const;

      /**
       * @brief Creates and object given a keyword list and prefix.
       * @param kwl The keyword list.
       * @param prefix the keyword list prefix.
       * @return pointer to image handler on success, 0 on failure.
       */
      virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                        const char* prefix=0)const;

      /**
       * @brief Adds ossimTerraSarTiffReader to the typeList.
       * @param typeList List to add to.
       */
      virtual void getTypeNameList(std::vector<ossimString>& typeList)const;

      /**
       * @brief Method to add supported extension to the list, like "png".
       *
       * @param extensionList The list to add to.
       */
      virtual void getSupportedExtensions(
         ossimImageHandlerFactoryBase::UniqueStringList& extensionList)const;

   protected:
      /** @brief hidden from use default constructor */
      ossimPluginReaderFactory();

      /** @brief hidden from use copy constructor */
      ossimPluginReaderFactory(const ossimPluginReaderFactory&);

      /** @brief hidden from use copy constructor */
      void operator=(const ossimPluginReaderFactory&);

      /** static instance of this class */
      static ossimPluginReaderFactory* theInstance;

      TYPE_DATA
         };
}

#endif /* end of #ifndef ossimPluginReaderFactory_HEADER */
