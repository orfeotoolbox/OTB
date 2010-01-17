//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimValueAssignImageSourceFilter.cpp 15833 2009-10-29 01:41:53Z eshirschorn $
#include <ossim/imaging/ossimValueAssignImageSourceFilter.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageDataFactory.h>

RTTI_DEF1(ossimValueAssignImageSourceFilter,
          "ossimValueAssignImageSourceFilter",
          ossimImageSourceFilter);

ossimValueAssignImageSourceFilter::ossimValueAssignImageSourceFilter(ossimObject* owner)
   :ossimImageSourceFilter(owner),
    theAssignType(ossimValueAssignType_GROUP),
    theTile(NULL)
{
}

ossimValueAssignImageSourceFilter::~ossimValueAssignImageSourceFilter()
{
}

ossimRefPtr<ossimImageData> ossimValueAssignImageSourceFilter::getTile(
   const ossimIrect& tileRect,
   ossim_uint32 resLevel)
{
   if(!theInputConnection)
   {
      return NULL;  // This filter requires an input.
   }
   
   ossimRefPtr<ossimImageData> inputTile =
      theInputConnection->getTile(tileRect, resLevel);
   
   if(!isSourceEnabled() || !inputTile.valid())
   {
      return inputTile;
   }

   if(!theTile.valid())
   {
      allocate(); // First time through...
   }
   
   if (!theTile.valid()) // throw exeption...
   {
      return inputTile;
   }

   // Set the origin, resize if needed of the output tile.
   theTile->setImageRectangle(tileRect);

   if( inputTile->getDataObjectStatus() == OSSIM_NULL ||
       inputTile->getDataObjectStatus() == OSSIM_EMPTY )
   {
      theTile->makeBlank();
      return theTile;
   }

   if(!inputTile->getBuf())
   {
      theTile->makeBlank();
   }
   else
   {
      theTile->loadTile(inputTile.get());
   }

   switch(inputTile->getScalarType())
   {
      case OSSIM_UCHAR:
      {
         executeAssign(static_cast<ossim_uint8>(0),
                       theTile);
         break;
      }
      case OSSIM_FLOAT: 
      case OSSIM_NORMALIZED_FLOAT:
      {
         executeAssign(static_cast<float>(0),
                       theTile);
         break;
      }
      case OSSIM_USHORT16:
      case OSSIM_USHORT11:
      {
         executeAssign(static_cast<ossim_uint16>(0),
                       theTile);
         break;
      }
      case OSSIM_SSHORT16:
      {
         executeAssign(static_cast<ossim_sint16>(0),
                       theTile);
         break;
      }
      case OSSIM_DOUBLE:
      case OSSIM_NORMALIZED_DOUBLE:
      {
         executeAssign(static_cast<double>(0),
                       theTile);
         break;
      }
      default:
      {
         ossimNotify(ossimNotifyLevel_WARN) << "ossimValueAssignImageSourceFilter::getTile WARN: Scalar type = " << theTile->getScalarType()
                                            << " Not supported!" << std::endl;
         break;
      }
   }

   theTile->validate();
   
   return theTile;
}

void ossimValueAssignImageSourceFilter::setInputOutputValues(const vector<double>& inputValues,
                                                             const vector<double>& outputValues)
{
   theInputValueArray  = inputValues;
   theOutputValueArray = outputValues;

   validateArrays();
}

void ossimValueAssignImageSourceFilter::validateArrays()
{
   if(theOutputValueArray.size() != theInputValueArray.size())
   {
      ossim_uint32 index = std::min((ossim_uint32)theOutputValueArray.size(),
                                    (ossim_uint32)theInputValueArray.size());
      
      vector<double> copyVector(theOutputValueArray.begin(),
                                theOutputValueArray.begin() + index);

      theOutputValueArray = copyVector;

      for(ossim_uint32 index2 = index; index < theInputValueArray.size(); ++index)
      {
         theOutputValueArray.push_back(theInputValueArray[index2]);
      }
   }
}

template <class T> void ossimValueAssignImageSourceFilter::executeAssign(
   T,
   ossimRefPtr<ossimImageData>& data)
{
   if(!theInputValueArray.size()) return;
   
   if(theAssignType == ossimValueAssignType_SEPARATE)
   {
      executeAssignSeparate(static_cast<T>(0), data);
   }
   else
   {
      executeAssignGroup(static_cast<T>(0), data);      
   }
}

template <class T> void ossimValueAssignImageSourceFilter::executeAssignSeparate(
   T,
   ossimRefPtr<ossimImageData>& data)
{
   ossim_uint32 numberOfBands = std::min((ossim_uint32)data->getNumberOfBands(),
                                         (ossim_uint32)theInputValueArray.size());
   ossim_uint32 maxOffset     = data->getWidth()*data->getHeight();
   
   for(ossim_uint32 band = 0; band<numberOfBands; ++band)
   {
      T* buf      = static_cast<T*>(data->getBuf(band));
      T  inValue  = static_cast<T>(theInputValueArray[band]);
      T  outValue = static_cast<T>(theOutputValueArray[band]);
      for(ossim_uint32 count = 0; count < maxOffset; ++count)
      {
         if(*buf == inValue)
         {
            *buf = outValue;
         }
         ++buf;
      }
   }
}

template <class T> void ossimValueAssignImageSourceFilter::executeAssignGroup(
   T,
   ossimRefPtr<ossimImageData>& data)
{
   ossim_uint32 numberOfBands = std::min((ossim_uint32)data->getNumberOfBands(),
                                         (ossim_uint32)theInputValueArray.size());
   ossim_uint32 maxOffset     = data->getWidth()*data->getHeight();
   ossim_uint32 band = 0;
   bool equalFlag = false;

   T** bufArray = new T*[data->getNumberOfBands()];
   for(band = 0; band < data->getNumberOfBands(); ++band)
   {
      bufArray[band] = static_cast<T*>(data->getBuf(band));
   }
   
   for(ossim_uint32 offset = 0; offset < maxOffset; ++offset)
   {
      equalFlag = true;
      
      for(band = 0; band<numberOfBands; ++band)
      {
         if(bufArray[band][offset] != theInputValueArray[band])
         {
            equalFlag = false;
         }
      }
      if(equalFlag)
      {
         for(band = 0; band<numberOfBands; ++band)
         {
            bufArray[band][offset] = static_cast<T>(theOutputValueArray[band]);
         }
      }
   }

   delete [] bufArray;
}

void ossimValueAssignImageSourceFilter::initialize()
{
   // Base class will recapture "theInputConnection".
   ossimImageSourceFilter::initialize();
}

void ossimValueAssignImageSourceFilter::allocate()
{   
   theTile = NULL;

   if(theInputConnection)
   {
      ossimImageDataFactory* idf = ossimImageDataFactory::instance();
      
      theTile = idf->create(this,
                            this);
      
      theTile->initialize();
   }
}

bool ossimValueAssignImageSourceFilter::saveState(ossimKeywordlist& kwl,
                                                  const char* prefix)const
{
   ossim_uint32 index = 0;
   for(index =0; index < theInputValueArray.size(); ++index)
   {
      ossimString in  = ("input_band" + ossimString::toString(index));
      ossimString out = ("output_band" + ossimString::toString(index));

      kwl.add(prefix,
              in.c_str(),
              theInputValueArray[index],
              true);
      kwl.add(prefix,
              out.c_str(),
              theOutputValueArray[index],
              true);
   }
   if(theAssignType ==  ossimValueAssignType_SEPARATE)
   {
      kwl.add(prefix,
              "assign_type",
              "separate",
              true);
   }
   else
   {
      kwl.add(prefix,
              "assign_type",
              "group",
              true);
   }
           
   return ossimImageSourceFilter::saveState(kwl, prefix);
}

/*!
 * Method to the load (recreate) the state of an object from a keyword
 * list.  Return true if ok or false on error.
 */
bool ossimValueAssignImageSourceFilter::loadState(const ossimKeywordlist& kwl,
                                                  const char* prefix)
{
   
   ossimString inRegExpression =  ossimString("^(") +
                                  ossimString(prefix) +
                                  "input_band[0-9]+)";
   ossimString outRegExpression =  ossimString("^(") +
                                   ossimString(prefix) +
                                   "output_band[0-9]+)";
   ossim_uint32 numberOfInputs  = kwl.getNumberOfSubstringKeys(inRegExpression);
   ossim_uint32 numberOfOutputs = kwl.getNumberOfSubstringKeys(outRegExpression);
   ossim_uint32 index = 0;
   ossim_uint32 currentIndex = 0;
   
   theInputValueArray.clear();
   theOutputValueArray.clear();

   while(index < numberOfInputs)
   {
      const char* value = kwl.find(prefix,
                                   (ossimString("input_band") +
                                    ossimString::toString(currentIndex)).c_str()
                                   );
      if(value)
      {
         theInputValueArray.push_back(ossimString(value).toDouble());
         ++index;
      }
      ++currentIndex;
   }
   index = 0;
   currentIndex = 0;
   while(index < numberOfOutputs)
   {
      const char* value = kwl.find(prefix,
                                   (ossimString("output_band") +
                                    ossimString::toString(currentIndex)).c_str()
                                   );
      if(value)
      {
         theOutputValueArray.push_back(ossimString(value).toDouble());
         ++index;
      }
      ++currentIndex;
   }
   validateArrays();
   
   const char* assignType = kwl.find(prefix, "assign_type");
   if(assignType)
   {
      if(ossimString(assignType).trim().downcase() == "separate")
      {
         theAssignType = ossimValueAssignType_SEPARATE;
      }
      else
      {
         theAssignType = ossimValueAssignType_GROUP;
      }
   }
   else
   {
      theAssignType = ossimValueAssignType_GROUP;
   }
   
   return ossimImageSourceFilter::loadState(kwl, prefix);
}

const vector<double>& ossimValueAssignImageSourceFilter::getInputValues()const
{
   return theInputValueArray;
}
const vector<double>& ossimValueAssignImageSourceFilter::getOutputValues()const
{
   return theOutputValueArray;
}

ossimValueAssignImageSourceFilter::ossimValueAssignType ossimValueAssignImageSourceFilter::getValueAssignType()const
{
   return theAssignType;
}
void ossimValueAssignImageSourceFilter::setValueAssignType(ossimValueAssignType type)
{
   theAssignType = type;
}
