#ifndef ossimImageHandlerSarFactory_HEADER
#define ossimImageHandlerSarFactory_HEADER
#include <ossim/imaging/ossimImageHandlerFactoryBase.h>

#include <ossim/base/ossimString.h>

class ossimImageHandler;
class ossimFilename;
class ossimKeywordlist;

/**
 * @ingroup ProcessingParametersRecord
 * @brief This class handle all the SAR file reader classes
 * @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 18-01-08
 */
class OSSIMDLLEXPORT ossimImageHandlerSarFactory : public ossimImageHandlerFactoryBase
{
public:
   virtual ~ossimImageHandlerSarFactory();
   static ossimImageHandlerSarFactory* instance();
   
   virtual ossimImageHandler* open(const ossimFilename& fileName)const;
   virtual ossimImageHandler* open(const ossimKeywordlist& kwl,
                                   const char* prefix=0)const;

   
   virtual ossimObject* createObject(const ossimString& typeName)const;
   
   /*!
    * Creates and object given a keyword list.
    */
   virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                     const char* prefix=0)const;
   
   /*!
    * This should return the type name of all objects in all factories.
    * This is the name used to construct the objects dynamially and this
    * name must be unique.
    */
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const;
   virtual void getSupportedExtensions(ossimImageHandlerFactoryBase::UniqueStringList& extensionList)const;
  
protected:
   ossimImageHandlerSarFactory(){}
   ossimImageHandlerSarFactory(const ossimImageHandlerSarFactory&){}
   void operator = (const ossimImageHandlerSarFactory&){}

   static ossimImageHandlerSarFactory* theInstance;

TYPE_DATA
};

#endif