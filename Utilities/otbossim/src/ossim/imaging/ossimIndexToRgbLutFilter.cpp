//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimIndexToRgbLutFilter.cpp 17206 2010-04-25 23:20:40Z dburken $
#include <ossim/imaging/ossimIndexToRgbLutFilter.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/base/ossimRgbVector.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimFilenameProperty.h>
#include <ossim/base/ossimStringProperty.h>

static const ossimTrace traceDebug("ossimIndexToRgbLutFilter:debug");

RTTI_DEF1(ossimIndexToRgbLutFilter, "ossimIndexToRgbLutFilter", ossimImageSourceFilter);

static const char* MIN_VALUE_KW          = "min_value";
static const char* MAX_VALUE_KW          = "max_value";
static const char* INTERPOLATION_TYPE_KW = "interpolation_type";

ossimIndexToRgbLutFilter::ossimIndexToRgbLutFilter()
   :ossimImageSourceFilter(),
    theLut(new ossimRgbLutDataObject()),
    theMinValue(ossim::nan()),
    theMaxValue(ossim::nan()),
    theMinMaxDeltaLength(ossim::nan()),
    theMinValueOverride(false),
    theMaxValueOverride(false),
    theInterpolationType(ossimIndexToRgbLutFilter_NEAREST),
    theTile(NULL),
    theLutFile("")
{
}

ossimIndexToRgbLutFilter::ossimIndexToRgbLutFilter(ossimImageSource* inputSource,
                                                 const ossimRgbLutDataObject& lut,
                                                 double minValue,
                                                 double maxValue,
                                                 ossimIndexToRgbLutFilterInterpolationType interpolationType)
   :ossimImageSourceFilter(inputSource),
    theLut((ossimRgbLutDataObject*)lut.dup()),
    theMinValue(minValue),
    theMaxValue(maxValue),
    theMinValueOverride(false),
    theMaxValueOverride(false),
    theInterpolationType(interpolationType),
    theTile(NULL),
    theLutFile("")
{ 
   if(theMinValue > theMaxValue)
   {
      double temp = theMinValue;
      theMinValue = theMaxValue;
      theMaxValue = temp;
   }
   theMinMaxDeltaLength = theMaxValue - theMinValue;
   
}

ossimIndexToRgbLutFilter::~ossimIndexToRgbLutFilter()
{
}

ossimRefPtr<ossimImageData> ossimIndexToRgbLutFilter::getTile(
   const ossimIrect& tileRect,
   ossim_uint32 resLevel)
{
   if(!theInputConnection)
   {
      return ossimRefPtr<ossimImageData>();
   }
   ossimRefPtr<ossimImageData> tile = theInputConnection->getTile(tileRect,
                                                                  resLevel);
   if(!tile)
   {
      return 0;
   }
   
   if(!theTile)
   {
      allocate();
      if (!theTile)
      {
         return theTile;
      }
   }

   theTile->setImageRectangle(tileRect);

   theTile->setDataObjectStatus(OSSIM_FULL);
   theTile->makeBlank();

   if(tile->getBuf()&&
      tile->getDataObjectStatus() != OSSIM_EMPTY)
   {
      return convertInputTile(tile);
   }

   return theTile;
   
//    if ( theTile->getDataObjectStatus() != OSSIM_EMPTY )
//    {
//       theTile->makeBlank();
//    }

}

ossimRefPtr<ossimImageData> ossimIndexToRgbLutFilter::convertInputTile(const ossimRefPtr<ossimImageData>& tile)
{
   ossim_uint8* outBuf[3];
   outBuf[0] = (ossim_uint8*)(theTile->getBuf(0));
   outBuf[1] = (ossim_uint8*)(theTile->getBuf(1));
   outBuf[2] = (ossim_uint8*)(theTile->getBuf(2));
   long numberOfEntries = (long)theLut->getNumberOfEntries();

   if(!numberOfEntries)
   {
      return ossimRefPtr<ossimImageData>();
   }
   
   long maxLength = tile->getWidth()*tile->getHeight();
   ossimRgbVector color;

   switch(tile->getScalarType())
   {
      case OSSIM_SSHORT16:
      {      
         ossim_sint16* buf = (ossim_sint16*)(tile->getBuf());

         for(long index = 0; index < maxLength; ++index)
         {
            if(!tile->isNull(index))
            {
               double colorIndex;
               normalizeValue(*buf, colorIndex);            
               getColorNormIndex(colorIndex, color);
               outBuf[0][index]  = color.getR();
               outBuf[1][index]  = color.getG();
               outBuf[2][index]  = color.getB();
            }
            else
            {
               outBuf[0][index] = 0;
               outBuf[1][index] = 0;
               outBuf[2][index] = 0;
            }
            ++buf;
         }
         break;
      }
      case OSSIM_FLOAT:
      case OSSIM_NORMALIZED_FLOAT:
      {      
         float* buf = (float*)(tile->getBuf());
       
         for(long index = 0; index < maxLength; ++index)
	 {
            if(!tile->isNull(index))
            {
	       double colorIndex;
	       normalizeValue(*buf, colorIndex);            
	       getColorNormIndex(colorIndex, color);
	       outBuf[0][index]  = color.getR();
	       outBuf[1][index]  = color.getG();
	       outBuf[2][index]  = color.getB();
            }
            else
            {
	       outBuf[0][index] = 0;
	       outBuf[1][index] = 0;
	       outBuf[2][index] = 0;
            }
            ++buf;
	 }
         break;
      }
      case OSSIM_UCHAR:
      {
         ossim_uint8* buf = (ossim_uint8*)(tile->getBuf());

         for(long index = 0; index < maxLength; ++index)
         {
            if(!tile->isNull(index))
            {
               double colorIndex;
               normalizeValue(*buf, colorIndex);
               getColorNormIndex(colorIndex, color);
	    
               outBuf[0][index]  = color.getR();
               outBuf[1][index]  = color.getG();
               outBuf[2][index]  = color.getB();
            }
            else
            {
               outBuf[0][index] = 0;
               outBuf[1][index] = 0;
               outBuf[2][index] = 0;
            }
            ++buf;
         }
         break;
      }
      case OSSIM_USHORT16:
      case OSSIM_USHORT11:
      {
         ossim_uint16* buf = (ossim_uint16*)(tile->getBuf());

         for(long index = 0; index < maxLength; ++index)
         {
            if(!tile->isNull(index))
            {
               double colorIndex;
               normalizeValue(*buf, colorIndex);            
               getColorNormIndex(colorIndex, color);
               outBuf[0][index]  = color.getR();
               outBuf[1][index]  = color.getG();
               outBuf[2][index]  = color.getB();
            }
            else
            {
               outBuf[0][index] = 0;
               outBuf[1][index] = 0;
               outBuf[2][index] = 0;
            }
            ++buf;
         }
         break;
      }
      default:
         break;
   }
   theTile->validate();
   return theTile;
}

void ossimIndexToRgbLutFilter::allocate()
{
   if(!theInputConnection) return;

   theTile = ossimImageDataFactory::instance()->create(this, 3, this);
   if(theTile.valid())
   {
      theTile->initialize();
   }
}

void ossimIndexToRgbLutFilter::initialize()
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimIndexToRgbLutFilter::initialize() DEBUG: Entered .... " << std::endl;
   }
   ossimImageSourceFilter::initialize();
   
   theTile      = NULL;
   if(!theInputConnection)
   {
      return;
   }
   if(!theMinValueOverride)
   {
      theMinValue = theInputConnection->getMinPixelValue(0);
   }
   if(!theMaxValueOverride)
   {
      theMaxValue = theInputConnection->getMaxPixelValue(0);
   }
   if(theMinValue > theMaxValue)
   {
      swap(theMinValue, theMaxValue);
   }
   theMinMaxDeltaLength = theMaxValue - theMinValue;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "theInputConnection Pointer = " << theInputConnection
         << "\nossimIndexToRgbLutFilter::initialize() DEBUG: Leaving .... "
         << std::endl;
   }

}

void ossimIndexToRgbLutFilter::setProperty(ossimRefPtr<ossimProperty> property)
{
   if(property.valid())
   {
      ossimString value = property->valueToString();
      value = value.trim();
      
      if(property->getName() == "Lut file")
      {
         setLut(ossimFilename(property->valueToString()));
      }
      else if(property->getName() == "Interpolation type")
      {
         value=value.downcase();
         if(value == "linear")
         {
            setInterpolationType(ossimIndexToRgbLutFilter_LINEAR);
         }
         else
         {
            setInterpolationType(ossimIndexToRgbLutFilter_NEAREST);
         }
      }
      else
      {
         ossimImageSourceFilter::setProperty(property);
      }
   }
}

ossimRefPtr<ossimProperty> ossimIndexToRgbLutFilter::getProperty(const ossimString& name)const
{
   if(name == "Lut file")
   {
      ossimFilenameProperty* filenameProperty = new ossimFilenameProperty(name, theLutFile);
      if(filenameProperty)
      {
         filenameProperty->setIoType(ossimFilenameProperty::ossimFilenamePropertyIoType_INPUT);
         filenameProperty->clearChangeType();
         filenameProperty->setCacheRefreshBit();
         filenameProperty->setReadOnlyFlag(false);

         return filenameProperty;
      }
   }
   else if(name == "Interpolation type")
   {
      std::vector<ossimString> options;
      options.push_back("nearest");
      options.push_back("linear");
      ossimString value = "nearest";
      if(theInterpolationType == ossimIndexToRgbLutFilter_LINEAR)
      {
         value = "linear";
      }
      
      ossimStringProperty* stringProperty = new ossimStringProperty(name,
                                                                    value,
                                                                    false,
                                                                    options);
      
      stringProperty->clearChangeType();
      stringProperty->setCacheRefreshBit();
      stringProperty->setReadOnlyFlag(false);

      return stringProperty;
   }

   return ossimImageSourceFilter::getProperty(name);
}

void ossimIndexToRgbLutFilter::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimImageSourceFilter::getPropertyNames(propertyNames);
   propertyNames.push_back("Lut file");
   propertyNames.push_back("Interpolation type");
}

void ossimIndexToRgbLutFilter::normalizeValue(double value,
                                              double& normalizedValue)
{
   // map to normalized elevation within our range of values
   normalizedValue = (value - theMinValue)/theMinMaxDeltaLength;
   if(normalizedValue > 1) normalizedValue = 1;
   if(normalizedValue < 0) normalizedValue = 0;   
}

void ossimIndexToRgbLutFilter::getColorNormIndex(double index,
                                                 ossimRgbVector& result)
{
   long numberOfEntries = theLut->getNumberOfEntries();
   index*=numberOfEntries;
   if(theInterpolationType == ossimIndexToRgbLutFilter_LINEAR)
   {
      int lutIndex = (long)index;
      
      double lutT   = index - lutIndex;
      
      int lutIndex2 = lutIndex+1;
      if(lutIndex2>=numberOfEntries) lutIndex2 = numberOfEntries-1;
      
      result = (*theLut)[lutIndex]*(1.0-lutT) +
               (*theLut)[lutIndex2]*(lutT);
      
   }
   else
   {
      int i = ossim::round<int>(index);
      i = i < 0?0:i;
      i = i >numberOfEntries?numberOfEntries:i;
      result = (*theLut)[i];
   }
}

void ossimIndexToRgbLutFilter::getColor(double index,
                                        ossimRgbVector& result)
{
   if(theInterpolationType == ossimIndexToRgbLutFilter_LINEAR)
   {
      long numberOfEntries = theLut->getNumberOfEntries();
      int lutIndex = (int)index;
      
      double lutT   = index - lutIndex;
      int lutIndex2 = lutIndex+1;
      
      if(lutIndex2>=numberOfEntries) lutIndex2 = numberOfEntries-1;
      
      result = (*theLut)[lutIndex]*(1.0-lutT) +
               (*theLut)[lutIndex2]*(lutT);
   }
   else
   {
      result = (*theLut)[ossim::round<int>(index)];
   }
}

bool ossimIndexToRgbLutFilter::saveState(ossimKeywordlist& kwl,
                                         const char* prefix)const
{
   kwl.add(prefix,
           MIN_VALUE_KW,
           theMinValue,
           true);
   
   kwl.add(prefix,
           MAX_VALUE_KW,
           theMaxValue,
           true);
   
   ossimString interpolationType = "nearest";
   switch(theInterpolationType)
   {
      case ossimIndexToRgbLutFilter_LINEAR:
      {
         interpolationType = "linear";
      }
      default:
         break;
   }
   kwl.add(prefix,
           INTERPOLATION_TYPE_KW,
           interpolationType.c_str(),
           true);
   
   ossimString newPrefix = ossimString(prefix)+ "lut.";
   if(theLutFile != "")
   {
      kwl.add(newPrefix.c_str(), "lut_file", theLutFile.c_str(), true);
      ossimKeywordlist kwl2;
      theLut->saveState(kwl2);
   }
   else
   {
      theLut->saveState(kwl, newPrefix.c_str());
   }

   return ossimImageSourceFilter::saveState(kwl, prefix);
}

bool ossimIndexToRgbLutFilter::loadState(const ossimKeywordlist& kwl,
                                         const char* prefix)
{
   const char* minValue = kwl.find(prefix,
                                   MIN_VALUE_KW);
   const char* maxValue = kwl.find(prefix,
                                   MAX_VALUE_KW);

   const char* interpolationType =  kwl.find(prefix,
                                             INTERPOLATION_TYPE_KW);
   
   theInterpolationType = ossimIndexToRgbLutFilter_NEAREST;
   if(interpolationType)
   {
      ossimString interp = ossimString(interpolationType).trim().upcase();
      if(interp == "LINEAR")
      {
         theInterpolationType = ossimIndexToRgbLutFilter_LINEAR;
      }
   }
   if(minValue)
   {
      theMinValue = ossimString(minValue).toDouble();
      theMinValueOverride = true;
      
   }
   else
   {
      theMinValueOverride = false;
   }
   
   if(maxValue)
   {
      theMaxValue = ossimString(maxValue).toDouble();
      theMaxValueOverride = true;
   }
   else
   {
      theMaxValueOverride = false;
   }
   if( (ossim::isnan(theMinValue) == false) &&
       (ossim::isnan(theMaxValue) == false) )
   {
      if(theMinValue > theMaxValue)
      {
         double temp = theMinValue;
         theMinValue = theMaxValue;
         theMaxValue = temp;
      }
      theMinMaxDeltaLength = theMaxValue - theMinValue;
   }
   else
   {
      theMinMaxDeltaLength = ossim::nan();
   }
   ossimString newPrefix = ossimString(prefix)+ "lut.";

   const char* file = kwl.find(newPrefix.c_str(), "lut_file");
   if(file)
   {
      theLutFile = file;
   }
   else
   {
      theLutFile = "";
   }
   theLut->loadState(kwl, newPrefix.c_str());

   bool result = ossimImageSourceFilter::loadState(kwl, prefix);

   return result;
}

ossim_uint32 ossimIndexToRgbLutFilter::getNumberOfOutputBands() const
{
   if(isSourceEnabled())
   {
      return 3;
   }
   return ossimImageSourceFilter::getNumberOfOutputBands();
}

ossimScalarType ossimIndexToRgbLutFilter::getOutputScalarType() const
{
   if(isSourceEnabled())
   {
      return OSSIM_UCHAR;
   }

   return ossimImageSourceFilter::getOutputScalarType();
}

void ossimIndexToRgbLutFilter::setLut(ossimRgbLutDataObject& lut)
{
   theLut = new ossimRgbLutDataObject(lut);
}

void ossimIndexToRgbLutFilter::setLut(const ossimFilename& file)
{
   theLutFile = file;
   if(file.exists())
   {
      ossimKeywordlist kwl(file.c_str());
      theLut->loadState(kwl);
   }
}

ossimFilename ossimIndexToRgbLutFilter::getLutFile()const
{
   return theLutFile;
}

double ossimIndexToRgbLutFilter::getMinValue()const
{
   return theMinValue;
}

double ossimIndexToRgbLutFilter::getMaxValue()const
{
   return theMaxValue;
}

void ossimIndexToRgbLutFilter::setMinValue(double value)
{
   theMinValue = value;
}

void ossimIndexToRgbLutFilter::setMaxValue(double value)
{
   theMaxValue = value;
}

double ossimIndexToRgbLutFilter::getNullPixelValue()const
{
   return 0.0;
}

double ossimIndexToRgbLutFilter::getMinPixelValue(ossim_uint32 /* band */)const
{
   return 1.0;
}

double ossimIndexToRgbLutFilter::getMaxPixelValue(ossim_uint32 /* band */)const
{
   return 255.0;
}

ossimIndexToRgbLutFilter::ossimIndexToRgbLutFilterInterpolationType ossimIndexToRgbLutFilter::getInterpolationType()const
{
   return theInterpolationType;
}

void ossimIndexToRgbLutFilter::setInterpolationType(ossimIndexToRgbLutFilterInterpolationType type)
{
   theInterpolationType = type;
}
