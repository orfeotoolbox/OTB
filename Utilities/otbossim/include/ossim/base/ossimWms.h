// $Id$

#ifndef ossimWms_HEADER
#define ossimWms_HEADER

#include <ossim/base/ossimXmlDocument.h>
#include <ossim/base/ossimXmlNode.h>

#include <queue>
class ossimWmsStyle;
class ossimWmsLayer;

typedef std::vector<ossimString> ossimWmsStringListType;

typedef std::vector<ossimRefPtr<ossimWmsStyle> > ossimWmsStyleListType;
typedef const std::vector<ossimRefPtr<ossimWmsStyle> > ossimConstWmsStyleListType;
typedef std::vector<ossimRefPtr<ossimWmsLayer> > ossimWmsLayerListType;
typedef const std::vector<ossimRefPtr<ossimWmsLayer> > ossimConstWmsLayerListType;

class OSSIM_DLL ossimWmsGetMap : public ossimReferenced
{
public:
   ossimWmsGetMap()
   {
      clearFields();
   }
   virtual bool read(ossimRefPtr<ossimXmlNode> node);

   ossimWmsStringListType& getFormatTypes()
   {
      return theFormatTypes;
   }
   const ossimWmsStringListType& getFormatTypes()const
   {
      return theFormatTypes;
   }
   bool hasFormats()const
   {
      return theFormatTypes.size() > 0;
   }
   /**
    * Will do an exact compare on the input format.  You can also specify if you want case sensitive
    * compares
    */ 
   bool hasFormat(const ossimString& format, bool caseSensitive=true)const;

   /**
    * Will not do an exact compare but instead will test if the passed in string is contained within
    * the formats.  You can also specify case sensitive.
    */ 
   bool containsFormat(const ossimString& format, bool caseSensitive=true)const;
   void clearFields()
   {
      theUrl = "";
      thePostUrl = "";
      theFormatTypes.clear();
   }
   const ossimString& getUrl()const
   {
      return theUrl;
   }
   const ossimString& getPostUrl()const
   {
      return thePostUrl;
   }
protected:
   ossimString            theUrl; 
   ossimString            thePostUrl; 
   ossimWmsStringListType theFormatTypes;
};

class OSSIM_DLL ossimWmsGetCapabilities : public ossimReferenced
{
public:
   ossimWmsGetCapabilities()
   {
   }
   virtual bool read(ossimRefPtr<ossimXmlNode> node);

   const ossimString& getUrl()const
   {
      return theUrl;
   }
protected:
   ossimString        theUrl; 
  
};

class OSSIM_DLL ossimWmsRequest : public ossimReferenced
{
public:
   ossimWmsRequest()
   {
   }
   virtual bool read(ossimRefPtr<ossimXmlNode> node);

   ossimRefPtr<ossimWmsGetCapabilities> getCapabilities()
   {
      return theGetCapabilities;
   }
   const ossimRefPtr<ossimWmsGetCapabilities> getCapabilities()const
   {
      return theGetCapabilities;
   }
   ossimRefPtr<ossimWmsGetMap> getMap()
   {
      return theGetMap;
   }
   const ossimRefPtr<ossimWmsGetMap> getMap()const
   {
      return theGetMap;
   }
   
protected:
   ossimRefPtr<ossimWmsGetCapabilities> theGetCapabilities;
   ossimRefPtr<ossimWmsGetMap> theGetMap;
};
class OSSIM_DLL ossimWmsContactAdress : public ossimReferenced
{
public:
   ossimWmsContactAdress()
      {}
protected:
   ossimString theAddressType;
   ossimString theAddress;
   ossimString theCity;
   ossimString theStateOrProvince;
   ossimString thePostCode;
   ossimString theCountry;
};

class OSSIM_DLL ossimWmsContactInformation : public ossimReferenced
{
public:
   virtual bool read(ossimRefPtr<ossimXmlNode> /* node */ )
      {
         return true;
      }
   
protected:
   ossimString thePrimaryContactPerson;
   ossimString thePrimaryContactOrganization;
   ossimString theContactPosition;
   ossimRefPtr<ossimWmsContactAdress> theContactAddress;
   ossimString theContactVoiceTelephone;
   ossimString theContactEmailAddress;
};

class OSSIM_DLL ossimWmsService : public ossimReferenced
{
public:
   ossimWmsService()
      {
      }

   virtual bool read(ossimRefPtr<ossimXmlNode> /* xml */ )
      {
         return true;
      }

protected:
   ossimString theName;
   ossimString theTitle;
   ossimString theAbstract;
   std::vector<ossimString> theKeywordList;
   ossimString theOnlineResource;
   ossimRefPtr<ossimWmsContactInformation> theContactInformation;
   ossimString theFees;
   ossimString theAccessConstraints;
};


class OSSIM_DLL ossimWmsTimeExtent : public ossimReferenced
{
public:
   ossimWmsTimeExtent()
      {
      }
   virtual bool read(ossimRefPtr<ossimXmlNode> node);

   void clearFields()
      {
         theDefaultValue = "";
         theTimes.clear();
      }
protected:
   ossimString theDefaultValue;
   std::vector<ossimString> theTimes;
};


class OSSIM_DLL ossimWmsMetadataUrl : public ossimReferenced
{
public:
   ossimWmsMetadataUrl()
      {
      }

protected:
   ossimString theFormat;
   ossimString theOnlineResourceHref;
};

class OSSIM_DLL ossimWmsDataUrl : public ossimReferenced
{
public:
protected:
   ossimString theFormat;
   ossimString theOnlineResourceHref;
};

class OSSIM_DLL ossimWmsStyle : public ossimReferenced
{
public:
   ossimWmsStyle()
      {}
   virtual bool read(ossimRefPtr<ossimXmlNode> node);
   void clearFields()
      {
         theName  = "";
         theTitle = "";
         theAbstract = "";
      }

   const ossimString& getName()const
      {
         return theName;
      }
   
   const ossimString& getTitle()const
      {
         return theTitle;
      }

   const ossimString& getAbstract()const
   {
      return theAbstract;
   }
protected:
   ossimString theName;
   ossimString theTitle;
   ossimString theAbstract;
};

class OSSIM_DLL ossimWmsScaleHint : public ossimReferenced
{
public:
   virtual bool read(ossimRefPtr<ossimXmlNode> node);

   void clearFields()
      {
         theMin = 0.0;
         theMax = 0.0;
      }
   void getMinMax(double& min, double& max)
   {
      min = theMin;
      max = theMax;
   }
   double getMin()const
   {
      return theMin;
   }
   double getMax()const
   {
      return theMax;
   }
protected:
   double theMin;
   double theMax;
};

class OSSIM_DLL ossimWmsBoundingBox : public ossimReferenced
{
public:
   ossimWmsBoundingBox()
      {
      }
   virtual bool read(ossimRefPtr<ossimXmlNode> node);
   void clearFields()
      {
         theSrs = "";
         theMinX = 0.0;
         theMinY = 0.0;
         theMaxX = 0.0;
         theMaxY = 0.0;
      }
   bool isGeographic()const
      {
         return theSrs.contains("4326");
      }
   const ossimString& getSrs()const
      {
         return theSrs;
      }
   double getMinX()const
      {
         return theMinX;
      }
   double getMinY()const
      {
         return theMinY;
      }
   double getMaxX()const
      {
         return theMaxX;
      }
   double getMaxY()const
      {
         return theMaxY;
      }
protected:
   ossimString theSrs;
   double theMinX;
   double theMinY;
   double theMaxX;
   double theMaxY;
};

class OSSIM_DLL ossimWmsLayer : public ossimReferenced
{
public:
   
   ossimWmsLayer()
      :theParent(0)
      {
         clearFields();
      }
   virtual bool read(ossimRefPtr<ossimXmlNode> node);

   void clearFields()
      {
         theName           = "";
         theTitle           = "";
         theAbstract       = "";
         theSrs            = "";
         theTimeExtent     = 0;
         theDimensionUnits = "";
         theDimensionName  = "";
         theBoundingBox    = 0;
         theScaleHint      = 0;
         theStyles.clear();
         theLayers.clear();
      }
   const ossimWmsLayer* getParent()const
      {
         return theParent;
      }

   ossimWmsLayer* getParent()
      {
         return theParent;
      }
   
    void setParent(ossimWmsLayer* parent)
      {
         theParent = parent;
      }
   const ossimRefPtr<ossimWmsBoundingBox> findBoundingBox()const
      {
         const ossimWmsLayer* currentLayer = this;

         while(currentLayer)
         {
            if(currentLayer->theBoundingBox.valid())
            {
               return currentLayer->theBoundingBox;
            }
            currentLayer = currentLayer->theParent;
         }
         
         return 0;
      }

   void getNamedLayers(ossimWmsLayerListType& namedLayers);

   const ossimString& getName()const
      {
         return theName;
      }
   const ossimString& getTitle()const
      {
         return theTitle;
      }
   const ossimString& getAbstract()const
      {
         return theAbstract;
      }
   const ossimString& getSrs()const
      {
         return theSrs;
      }
   const ossimRefPtr<ossimWmsTimeExtent> getTimeExtent()
      {
         return theTimeExtent;
      }
   const ossimString& getDimensionUnits()const
      {
         return theDimensionUnits;
      }
   const ossimString& getDimensionName()const
      {
         return theDimensionName;
      }
   const ossimRefPtr<ossimWmsBoundingBox> getBoundingBox()const
      {
         return theBoundingBox;
      }
   const ossimRefPtr<ossimWmsScaleHint> getScaleHint()const
      {
         return theScaleHint;
      }
   const ossimWmsStyleListType& getStyles()const
      {
         return theStyles;
      }
   ossimWmsStyleListType& getStyles()
      {
         return theStyles;
      }
   const ossimWmsLayerListType& getLayers()const
      {
         return theLayers;
      }
   ossimWmsLayerListType& getLayers()
   {
      return theLayers;
   }

   ossim_uint32 getNumberOfChildren()const
   {
      return (ossim_uint32)theLayers.size();
   }
   ossim_uint32 getNumberOfStyles()const
   {
      return (ossim_uint32)theStyles.size();
   }
protected:
   ossimWmsLayer* theParent;
   ossimString theName;
   ossimString theTitle;
   ossimString theAbstract;
   ossimString theSrs;
   ossimRefPtr<ossimWmsTimeExtent> theTimeExtent;
   ossimString theDimensionUnits;
   ossimString theDimensionName;
   ossimRefPtr<ossimWmsBoundingBox> theBoundingBox;
   ossimRefPtr<ossimWmsScaleHint>          theScaleHint;
   ossimWmsStyleListType theStyles;
   ossimWmsLayerListType theLayers;
};

class OSSIM_DLL ossimWmsCapability : public ossimReferenced
{
public:
   virtual bool read(const ossimRefPtr<ossimXmlNode> node);
   void getNamedLayers(ossimWmsLayerListType&  layers);
   ossimRefPtr<ossimWmsRequest> getRequest()
   {
      return theRequest;
   }
   const ossimRefPtr<ossimWmsRequest> getRequest()const
   {
      return theRequest;
   }
protected:
   ossimRefPtr<ossimWmsRequest>             theRequest;
   std::vector<ossimRefPtr<ossimWmsLayer> > theLayers;
};

class OSSIM_DLL ossimWmsCapabilitiesDocument : public ossimReferenced
{
public:
   ossimWmsCapabilitiesDocument()
      {
         
      }

   bool read(const std::string& inString);
   virtual bool read(ossimRefPtr<ossimXmlNode> node);
   void clearFields()
      {
         theVersion = "";
         theCapability=0;
      }
   ossimRefPtr<ossimWmsCapability> getCapability()
   {
      return theCapability;
   }
   const ossimRefPtr<ossimWmsCapability> getCapability()const
   {
      return theCapability;
   }

   ossimRefPtr<ossimWmsGetMap> getRequestGetMap();
   const ossimRefPtr<ossimWmsGetMap> getRequestGetMap()const;

   ossimRefPtr<ossimWmsGetCapabilities> getRequestGetCapabilities();
   const ossimRefPtr<ossimWmsGetCapabilities> getRequestGetCapabilities()const;

protected:
   ossimString theVersion;
   ossimRefPtr<ossimWmsCapability> theCapability;
};

#endif
