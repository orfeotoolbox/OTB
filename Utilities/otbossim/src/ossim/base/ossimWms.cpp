// $Id$

#include <ossim/base/ossimWms.h>
#include <ossim/base/ossimXmlString.h>
#include <deque>
#include <iostream>
#include <sstream>
#include <ossim/base/ossimCommon.h>

bool ossimWmsGetMap::read(ossimRefPtr<ossimXmlNode> node)
{
   clearFields();
   const vector<ossimRefPtr<ossimXmlNode> >& childNodes = node->getChildNodes();
   ossim_uint32 idx = 0;
   
   for(idx = 0; idx < childNodes.size();++idx)
   {
      if(childNodes[idx]->getTag() == "Format")
      {
         ossimString text = childNodes[idx]->getText();
         text = text.trim();
         if(!text.empty())
         {
            theFormatTypes.push_back(text);
         }
         else
         {
            const vector<ossimRefPtr<ossimXmlNode> >& childFormatNodes =  childNodes[idx]->getChildNodes();
            if(childFormatNodes.size())
            {
               ossim_uint32 childIdx = 0;
               for(childIdx = 0; childIdx < childFormatNodes.size();++childIdx)
               {
                  text = childFormatNodes[childIdx]->getTag();
                  text = text.trim();
                  if(!text.empty())
                  {
                     theFormatTypes.push_back(text);
                  }
               }
            }
         }
      }
      else if(childNodes[idx]->getTag() == "DCPType")
      {
         ossimRefPtr<ossimXmlNode> node = childNodes[idx]->findFirstNode("HTTP/Get/OnlineResource");
         if(node.valid())
         {
            node->getAttributeValue(theUrl, "xlink:href");
         }
         else
         {
            node = childNodes[idx]->findFirstNode("HTTP/Get");
            if(node.valid())
            {
               node->getAttributeValue(theUrl, "onlineResource");
               theUrl = ossim::convertHtmlSpecialCharactersToNormalCharacter(theUrl);
            }
         }
      }
   }
   
   return true;
}

bool ossimWmsGetMap::hasFormat(const ossimString& format, bool caseSensitive)const
{
   if(theFormatTypes.empty()) return false;
   ossimString tempFormat = format;
   if(!caseSensitive) tempFormat = tempFormat.downcase();

   ossim_uint32 idx = 0;
   for(idx = 0; idx < theFormatTypes.size();++idx)
   {
      ossimString temp = theFormatTypes[idx];
      if(!caseSensitive)
      {
         temp = temp.downcase();
      }
      if(temp==tempFormat)
      {
         return true;
      }
   }

   return false;
}

bool ossimWmsGetMap::containsFormat(const ossimString& format, bool caseSensitive)const
{
   if(theFormatTypes.empty()) return false;
   ossimString tempFormat = format;
   if(!caseSensitive) tempFormat = tempFormat.downcase();

   ossim_uint32 idx = 0;
   for(idx = 0; idx < theFormatTypes.size();++idx)
   {
      ossimString temp = theFormatTypes[idx];
      if(!caseSensitive)
      {
         temp = temp.downcase();
      }
      if(temp.contains(tempFormat))
      {
         return true;
      }
   }

   return false;
}

bool ossimWmsGetCapabilities::read(ossimRefPtr<ossimXmlNode> node)
{
   ossimRefPtr<ossimXmlNode> childNode = node->findFirstNode("DCPType/HTTP/Get/OnlineResource");

   if(childNode.valid())
   {
      childNode->getAttributeValue(theUrl, "xlink:href");
   }

   return true;
}

bool ossimWmsRequest::read(ossimRefPtr<ossimXmlNode> node)
{
   const vector<ossimRefPtr<ossimXmlNode> >& childNodes = node->getChildNodes();
   ossim_uint32 idx = 0;
   
   for(idx = 0; idx < childNodes.size();++idx)
   {
      if((childNodes[idx]->getTag() == "GetCapabilities")||
         (childNodes[idx]->getTag() == "Capabilities"))
      {
         theGetCapabilities = new ossimWmsGetCapabilities;
         theGetCapabilities->read(childNodes[idx]);
      }
      else if((childNodes[idx]->getTag() == "GetMap")||
              (childNodes[idx]->getTag() == "Map"))
      {
         theGetMap = new ossimWmsGetMap;
         theGetMap->read(childNodes[idx]);
      }
   }
   
   return true;
}

bool ossimWmsTimeExtent::read(ossimRefPtr<ossimXmlNode> node)
{
   clearFields();
   node->getAttributeValue(theDefaultValue, "default");
   ossimString timeValues = node->getText();

   timeValues.split(theTimes, "/");

   return true;
}

bool ossimWmsStyle::read(ossimRefPtr<ossimXmlNode> node)
{
   const vector<ossimRefPtr<ossimXmlNode> >& childNodes = node->getChildNodes();
   ossim_uint32 idx = 0;
   for(idx = 0; idx < childNodes.size();++idx)
   {
      if(childNodes[idx]->getTag() == "Name")
      {
         theName = childNodes[idx]->getText();
      }
      else if(childNodes[idx]->getTag() == "Title")
      {
         theTitle = childNodes[idx]->getText();
      }
      else if(childNodes[idx]->getTag() == "Abstract")
      {
         theAbstract = childNodes[idx]->getText();
      }
   }

   return true;
}

bool ossimWmsScaleHint::read(ossimRefPtr<ossimXmlNode> node)
{
   ossimString minScale, maxScale;

   if(node->getAttributeValue(minScale, "min")&&
      node->getAttributeValue(maxScale, "max"))
   {

      theMin = minScale.toDouble();
      theMax = maxScale.toDouble();
      
      return true;
   }
   
   return false;
}

bool ossimWmsBoundingBox::read(ossimRefPtr<ossimXmlNode> node)
{
   ossimString minx, miny, maxx, maxy;

   bool result = (node->getAttributeValue(minx, "minx")&&
                  node->getAttributeValue(miny, "miny")&&
                  node->getAttributeValue(maxx, "maxx")&&
                  node->getAttributeValue(maxy, "maxy"));
   
   node->getAttributeValue(theSrs, "SRS");
   
   if(node->getTag() == "LatLonBoundingBox")
   {
      theSrs = "EPSG:4326";
   }

   theMinX = minx.toDouble();
   theMinY = miny.toDouble();
   theMaxX = maxx.toDouble();
   theMaxY = maxy.toDouble();

   return result;
}

bool ossimWmsLayer::read(ossimRefPtr<ossimXmlNode> node)
{
   clearFields();
   const vector<ossimRefPtr<ossimXmlNode> >& childNodes = node->getChildNodes();
   ossim_uint32 idx = 0;
   for(idx = 0; idx < childNodes.size();++idx)
   {
      if(childNodes[idx]->getTag() == "Name")
      {
         theName = childNodes[idx]->getText();
      }
      else if(childNodes[idx]->getTag() == "Title")
      {
         theTitle = childNodes[idx]->getText();
      }
      else if(childNodes[idx]->getTag() == "Abstract")
      {
         theAbstract = childNodes[idx]->getText();
      }
      else if(childNodes[idx]->getTag() == "Extent")
      {
         ossimString name;
         if(childNodes[idx]->getAttributeValue(name, "name"))
         {
            if(name == "time")
            {
               theTimeExtent = new ossimWmsTimeExtent;
               if(!theTimeExtent->read(childNodes[idx]))
               {
                  theTimeExtent = 0;
               }
            }
         }
      }
      else if(childNodes[idx]->getTag() == "SRS")
      {
         theSrs = childNodes[idx]->getText();
      }
      else if(childNodes[idx]->getTag() == "Dimension")
      {
         childNodes[idx]->getAttributeValue(theDimensionUnits, "units");
         childNodes[idx]->getAttributeValue(theDimensionName,  "name");
      }
      else if(childNodes[idx]->getTag() == "Style")
      {
         ossimRefPtr<ossimWmsStyle> style = new ossimWmsStyle;
         style->read(childNodes[idx]);
         theStyles.push_back(style);
      }
      else if(childNodes[idx]->getTag() == "ScaleHint")
      {
         theScaleHint = new ossimWmsScaleHint;
         if(!theScaleHint->read(childNodes[idx]))
         {
            theScaleHint = 0;
         }
      }
      else if(childNodes[idx]->getTag() == "LatLonBoundingBox"||
              childNodes[idx]->getTag() == "BoundingBox")
      {
         theBoundingBox = new ossimWmsBoundingBox;
         if(!theBoundingBox->read(childNodes[idx]))
         {
            theBoundingBox = 0;
         }
      }
      else if(childNodes[idx]->getTag() == "Layer")
      {
         ossimRefPtr<ossimWmsLayer> layer = new ossimWmsLayer;
         if(!layer->read(childNodes[idx]))
         {
            return false;
         }
         layer->setParent(this);
         theLayers.push_back(layer.get());
      }
   }

   return true;
}

void ossimWmsLayer::getNamedLayers(ossimWmsLayerListType& namedLayers)
{
   if(theLayers.empty()) return;
   
   std::deque<ossimRefPtr<ossimWmsLayer> > layers(theLayers.begin(), theLayers.end());
   
   while(!layers.empty())
   {
      ossimRefPtr<ossimWmsLayer> layer = layers.front();
      layers.pop_front();
      if(!layer->getName().empty())
      {
         namedLayers.push_back(layer);
      }
      if(layer->getNumberOfChildren()>0)
      {
         ossimWmsLayerListType& layerList = layer->getLayers();
         layers.insert(layers.end(), layerList.begin(), layerList.end());
      }
   }
}

bool ossimWmsCapability::read(const ossimRefPtr<ossimXmlNode> node)
{
   const vector<ossimRefPtr<ossimXmlNode> >& childNodes = node->getChildNodes();
   
   ossim_uint32 idx = 0;
   for(idx = 0; idx < childNodes.size();++idx)
   {
      if(childNodes[idx]->getTag() == "Layer")
      {
         ossimRefPtr<ossimWmsLayer> layer = new ossimWmsLayer;
         if(!layer->read(childNodes[idx]))
         {
            return false;
         }
         theLayers.push_back(layer.get());
      }
      else if(childNodes[idx]->getTag() == "Request")
      {
         theRequest = new ossimWmsRequest;
         theRequest->read(childNodes[idx]);
      }
   }
   
   return true;
}

void ossimWmsCapability::getNamedLayers(ossimWmsLayerListType&  layers)
{
   ossim_uint32 idx = 0;

   for(idx = 0; idx < theLayers.size(); ++idx)
   {
      if(!theLayers[idx]->getName().empty())
      {
         layers.push_back(theLayers[idx]);
      }
      theLayers[idx]->getNamedLayers(layers);
   }
   
}

bool ossimWmsCapabilitiesDocument:: read(const std::string& inString)
{
   std::istringstream in(inString);
   ossimRefPtr<ossimXmlDocument> document = new ossimXmlDocument;
   if(!document->read(in))
   {
//       std::cout << "Couldn't parse XML!!!!!" << std::endl;
      return false;
   }
   if(!read(document->getRoot()))
   {
      return false;
   }
   
//   std::cout << *document << std::endl;
   return true;
}

bool ossimWmsCapabilitiesDocument::read(ossimRefPtr<ossimXmlNode> node)
{
   const vector<ossimRefPtr<ossimXmlNode> >& childNodes = node->getChildNodes();
   clearFields();
   if(!node.valid()) return false;
   node->getAttributeValue(theVersion, "version");
   ossim_uint32 idx = 0;
   for(idx = 0; idx < childNodes.size();++idx)
   {
      if(childNodes[idx]->getTag() == "Capability")
      {
         theCapability = new ossimWmsCapability;
         if(!theCapability->read(childNodes[idx]))
         {
            return false;
         }
      }
   }

   return theCapability.valid();
}

ossimRefPtr<ossimWmsGetMap> ossimWmsCapabilitiesDocument::getRequestGetMap()
{
   ossimRefPtr<ossimWmsGetMap> result;

   if(theCapability.valid())
   {
      ossimRefPtr<ossimWmsRequest> request = theCapability->getRequest();
      if(request.valid())
      {
         result = request->getMap();
      }
   }
   
   return result;
}

const ossimRefPtr<ossimWmsGetMap> ossimWmsCapabilitiesDocument::getRequestGetMap()const
{

   if(theCapability.valid())
   {
      const ossimRefPtr<ossimWmsRequest> request = theCapability->getRequest();
      if(request.valid())
      {
         return request->getMap();
      }
   }
   
   return 0;
}

ossimRefPtr<ossimWmsGetCapabilities> ossimWmsCapabilitiesDocument::getRequestGetCapabilities()
{
   ossimRefPtr<ossimWmsGetCapabilities> result;

   if(theCapability.valid())
   {
      ossimRefPtr<ossimWmsRequest> request = theCapability->getRequest();
      if(request.valid())
      {
         result = request->getCapabilities();
      }
   }
   
   return result;
}

const ossimRefPtr<ossimWmsGetCapabilities> ossimWmsCapabilitiesDocument::getRequestGetCapabilities()const
{
   if(theCapability.valid())
   {
      const ossimRefPtr<ossimWmsRequest> request = theCapability->getRequest();
      if(request.valid())
      {
         return request->getCapabilities();
      }
   }
   
   return 0;
}
