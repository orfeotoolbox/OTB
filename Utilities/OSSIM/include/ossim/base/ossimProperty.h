//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimProperty.h 10174 2007-01-03 18:24:31Z gpotts $
#ifndef ossimProperty_HEADER
#define ossimProperty_HEADER
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimXmlNode.h>

class ossimContainerProperty;
class OSSIM_DLL ossimProperty : public ossimObject
{
public:
   /*!
    * This are bitwise and can be ored together
    * The change type is very important and
    * it determines how things are refreshed.
    */
   enum ossimPropertyChangeType
   {
      ossimPropertyChangeType_NOTSPECIFIED   = 0,
      ossimPropertyChangeType_CACHE_REFRESH  = 1,
      ossimPropertyChangeType_FULL_REFRESH   = 2
   };
   ossimProperty(const ossimString& name=ossimString(""));
   ossimProperty(const ossimProperty& rhs);
   virtual ~ossimProperty();
   
   const  ossimProperty& operator = (const ossimProperty& rhs);
   virtual const ossimProperty& assign(const ossimProperty& rhs);

   const ossimString& getName()const;
   void setName(const ossimString& name);

   virtual bool setValue(const ossimString& value)=0;
   virtual void valueToString(ossimString& valueResult)const=0;
   virtual ossimString valueToString()const;

   virtual const ossimContainerProperty* asContainer()const;
   virtual ossimContainerProperty* asContainer();
   
   void clearChangeType();
   void setChangeType(ossimPropertyChangeType type);
   void setFullRefreshBit();
   void setCacheRefreshBit();
   
   ossimPropertyChangeType getChangeType()const;
   bool isFullRefresh()const;
   bool isCacheRefresh()const;
   bool isChangeTypeSpecified()const;

   void setReadOnlyFlag(bool flag);
   bool getReadOnlyFlag()const;
   bool isReadOnly()const;

   void setModifiedFlag(bool flag);
   bool getModifiedFlag()const;
   bool isModified()const;
   void clearModifiedFlag();

   void setDescription(const ossimString& description);
   
   virtual ossimString getDescription()const;

   virtual ossimRefPtr<ossimXmlNode> toXml()const;
   
protected:
   ossimString             theName;
   ossimString             theDescription;
   bool                    theReadOnlyFlag;
   ossimPropertyChangeType theChangeType;
   bool                    theModifiedFlag;
   
TYPE_DATA
};

#endif
