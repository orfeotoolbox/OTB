//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks)
// Description:
// Base class for any object that can be a source.  Derives from
// ossimConnectableObject since all source's should be connectable
//
//*************************************************************************
// $Id: ossimSource.h 15798 2009-10-23 19:15:20Z gpotts $

#ifndef ossimSource_HEADER
#define ossimSource_HEADER

#include <ossim/base/ossimConnectableObject.h>
#include <ossim/base/ossimErrorStatusInterface.h>
#include <ossim/base/ossimConstants.h>

class ossimDataObject;

class OSSIMDLLEXPORT ossimSource : public ossimConnectableObject,
				   public ossimErrorStatusInterface
{
public:
   ossimSource(ossimObject* owner=0);
   ossimSource(ossimObject* owner,
               ossim_uint32 inputListSize,
               ossim_uint32 outputListSize,
               bool inputListIsFixedFlag=true,
               bool outputListIsFixedFlag=true);
   virtual ~ossimSource();
   
   virtual bool isSourceEnabled()const;
   virtual void enableSource();
   virtual void disableSource();
   virtual bool getEnableFlag() const;
   virtual void setEnableFlag(bool flag);

   virtual bool isInitialized() const;
   virtual void setInitializedFlag(bool flag);
   virtual void initialize();

   /*---------------------- PROPERTY INTERFACE ---------------------------*/
   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;
   /*--------------------- PROPERTY INTERFACE END ------------------------*/

   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   virtual std::ostream& print(std::ostream& out) const;

protected:
   ossimSource(const ossimSource &rhs);
   const ossimSource& operator=(const ossimSource &rhs);

   bool   theEnableFlag;
   bool   theInitializedFlag;

TYPE_DATA
};

#endif
