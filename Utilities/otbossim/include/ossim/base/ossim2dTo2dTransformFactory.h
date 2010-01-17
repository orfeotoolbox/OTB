//**************************************************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Description: Class declaration of ossim2dTo2dTransformFactory.
//
//**************************************************************************************************
// $Id$
#ifndef ossim2dTo2dTransformFactory_HEADER
#define ossim2dTo2dTransformFactory_HEADER
#include <ossim/base/ossim2dTo2dTransformFactoryBase.h>

class ossim2dTo2dTransform;
class OSSIM_DLL ossim2dTo2dTransformFactory : public ossim2dTo2dTransformFactoryBase
{
public:
   ossim2dTo2dTransformFactory(){}
   static ossim2dTo2dTransformFactory* instance();
   
   /**
    * Take a transform type name.
    */
   virtual ossim2dTo2dTransform* createTransform(const ossimString& name)const;
   
   /**
    * Take a keywordlist.
    */
   virtual ossim2dTo2dTransform* createTransform(const ossimKeywordlist& kwl,
                                                 const char* prefix)const;
   
   /*!
    * This should return the type name of all objects in all factories.
    * This is the name used to construct the objects dynamially and this
    * name must be unique.
    */
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const;
  
protected:
   static ossim2dTo2dTransformFactory* m_instance;
};

#endif
