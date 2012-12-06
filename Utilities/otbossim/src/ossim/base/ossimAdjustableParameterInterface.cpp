//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimAdjustableParameterInterface.cpp 20610 2012-02-27 12:19:25Z gpotts $
#include <algorithm>
#include <ossim/base/ossimAdjustableParameterInterface.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimUnitTypeLut.h>

RTTI_DEF(ossimAdjustableParameterInterface, "ossimAdjustableParameterInterface");

static const char* NUMBER_OF_ADJUSTMENTS_KW = "number_of_adjustments";
static const char* NUMBER_OF_ADJUSTMENTS_OLD_KW = "number_of_adjustements";
static const char* CURRENT_ADJUSTMENT_OLD_KW    = "current_adjustement";
static const char* CURRENT_ADJUSTMENT_KW    = "current_adjustment";
static const char* ADJUSTMENT_PREFIX        = "adjustment_";

ossimAdjustableParameterInterface::ossimAdjustableParameterInterface()
{
   theCurrentAdjustment = 0;
}

ossimAdjustableParameterInterface::ossimAdjustableParameterInterface(const ossimAdjustableParameterInterface& rhs)
   :theAdjustmentList(rhs.theAdjustmentList),
    theCurrentAdjustment(rhs.theCurrentAdjustment)
{
}

void ossimAdjustableParameterInterface::newAdjustment(ossim_uint32 numberOfParameters)
{
   theAdjustmentList.push_back(ossimAdjustmentInfo());
   if(numberOfParameters > 0)
   {
      theAdjustmentList[theAdjustmentList.size()-1].setNumberOfAdjustableParameters(numberOfParameters);
      theAdjustmentList[theAdjustmentList.size()-1].setDescription("Initial adjustment");
   }

   theCurrentAdjustment = (ossim_uint32)theAdjustmentList.size() - 1;

}

void ossimAdjustableParameterInterface::setAdjustmentDescription(const ossimString& description)
{
   if(!theAdjustmentList.size())
   {
      return;
   }
   theAdjustmentList[theCurrentAdjustment].setDescription(description);
}

ossimString ossimAdjustableParameterInterface::getAdjustmentDescription()const
{
   if(theAdjustmentList.size())
   {
      return theAdjustmentList[theCurrentAdjustment].getDescription();
   }

   return "";
}

void ossimAdjustableParameterInterface::setCurrentAdjustment(ossim_uint32 adjustmentIdx, bool notify)
{
   if(adjustmentIdx < theAdjustmentList.size())
   {
      theCurrentAdjustment = adjustmentIdx;
      if(notify)
      {
         adjustableParametersChanged();
      }
   }
}


void ossimAdjustableParameterInterface::initAdjustableParameters()
{
}

void ossimAdjustableParameterInterface::resetAdjustableParameters(bool notify)
{
    if(!theAdjustmentList.size())
    {
       return;
    }
    
    ossim_uint32 saveCurrent = theCurrentAdjustment;
    copyAdjustment();
    initAdjustableParameters();
    ossim_uint32 numberOfAdjustables = getNumberOfAdjustableParameters();
    ossim_uint32 idx = 0;
    
    for(idx = 0; idx < numberOfAdjustables; ++idx)
    {
       theAdjustmentList[saveCurrent].getParameterList()[idx].setParameter(theAdjustmentList[theAdjustmentList.size()-1].getParameterList()[idx].getParameter());
    }

    setCurrentAdjustment(saveCurrent);

    eraseAdjustment((ossim_uint32)theAdjustmentList.size()-1, false);
    
    if(notify)
    {
       adjustableParametersChanged();
    }
}

void ossimAdjustableParameterInterface::copyAdjustment(ossim_uint32 idx, bool notify)
{
    if(!theAdjustmentList.size())
    {
       return;
    }
    if(idx < theAdjustmentList.size())
    {
       theAdjustmentList.push_back(theAdjustmentList[idx]);

       if(idx == theCurrentAdjustment)
       {
          theCurrentAdjustment = (ossim_uint32)theAdjustmentList.size() - 1;
       }
       if(notify)
       {
          adjustableParametersChanged();
       }
    }
    
}

void ossimAdjustableParameterInterface::copyAdjustment(bool notify)
{
   copyAdjustment(theCurrentAdjustment, notify);
}

void ossimAdjustableParameterInterface::keepAdjustment(ossim_uint32 idx,
                                                       bool createCopy)
{
    if(!theAdjustmentList.size())
    {
       return;
    }
    if(idx < theAdjustmentList.size())
    {
       if(createCopy)
       {
          copyAdjustment(idx);
       }
       theAdjustmentList[theCurrentAdjustment].keep();
    }
}

void ossimAdjustableParameterInterface::keepAdjustment(bool createCopy)
{
   keepAdjustment(theCurrentAdjustment, createCopy);
}

const ossimAdjustableParameterInterface& ossimAdjustableParameterInterface::operator = (const ossimAdjustableParameterInterface& rhs)
{
   theAdjustmentList    = rhs.theAdjustmentList;
   theCurrentAdjustment = rhs.theCurrentAdjustment;

   return *this;
}

void ossimAdjustableParameterInterface::removeAllAdjustments()
{
   theAdjustmentList.clear();
   theCurrentAdjustment = 0;
}

ossim_uint32 ossimAdjustableParameterInterface::getNumberOfAdjustableParameters()const
{
   if(theAdjustmentList.size())
   {
      return theAdjustmentList[theCurrentAdjustment].getNumberOfAdjustableParameters();
   }

   return 0;
}

void ossimAdjustableParameterInterface::eraseAdjustment(bool notify)
{
   eraseAdjustment(theCurrentAdjustment, notify);
}

void ossimAdjustableParameterInterface::eraseAdjustment(ossim_uint32 idx, bool notify)
{
   if(!theAdjustmentList.size())
   {
      return;
   }
   
   if(theCurrentAdjustment == idx)
   {
      theAdjustmentList.erase(theAdjustmentList.begin() + theCurrentAdjustment);
      if(theCurrentAdjustment >= theAdjustmentList.size())
      {
         if(theAdjustmentList.size() < 1)
         {
            theCurrentAdjustment = 0;
         }
         else
         {
            theCurrentAdjustment = (ossim_uint32)theAdjustmentList.size() - 1;
         }
         
      }
      
      if(notify)
      {
         adjustableParametersChanged();
      }
   }
   else if(idx < theAdjustmentList.size())
   {
      theAdjustmentList.erase(theAdjustmentList.begin() + idx);
      if(theAdjustmentList.size() < 1)
      {
         theCurrentAdjustment = 0;
      }
      else
      {
         if(theCurrentAdjustment > idx)
         {
            --theCurrentAdjustment;
            if(notify)
            {
               adjustableParametersChanged();
            }
         }
      }
      if(notify)
      {
         adjustableParametersChanged();
      }
   }
}

double ossimAdjustableParameterInterface::getAdjustableParameter(ossim_uint32 idx)const
{
   if(theAdjustmentList.size())
   {
      if(idx < theAdjustmentList[theCurrentAdjustment].getNumberOfAdjustableParameters())
      {
         return theAdjustmentList[theCurrentAdjustment].getParameterList()[idx].getParameter();
      }
   }
   
   return 0.0;
}

void ossimAdjustableParameterInterface::setAdjustableParameter(ossim_uint32 idx, double value, double sigma, bool notify)
{
   if(!theAdjustmentList.size())
   {
      return;
   }
   if(idx < theAdjustmentList[theCurrentAdjustment].getNumberOfAdjustableParameters())
   {
      theAdjustmentList[theCurrentAdjustment].getParameterList()[idx].setParameter(value);
      theAdjustmentList[theCurrentAdjustment].getParameterList()[idx].setSigma(sigma);
      if(notify)
      {
         adjustableParametersChanged();
      }
   }
   
}

void ossimAdjustableParameterInterface::setAdjustableParameter(ossim_uint32 idx, double value, bool notify)
{
   if(!theAdjustmentList.size())
   {
      return;
   }
   if(idx < theAdjustmentList[theCurrentAdjustment].getNumberOfAdjustableParameters())
   {
      theAdjustmentList[theCurrentAdjustment].getParameterList()[idx].setParameter(value);

      if(notify)
      {
         adjustableParametersChanged();
      }
   }
}

double ossimAdjustableParameterInterface::getParameterSigma(ossim_uint32 idx)const
{
   if(theAdjustmentList.size())
   {
      if(idx < theAdjustmentList[theCurrentAdjustment].getNumberOfAdjustableParameters())
      {
         return theAdjustmentList[theCurrentAdjustment].getParameterList()[idx].getSigma();
      }
   }

   return 0.0;
}

void ossimAdjustableParameterInterface::setParameterSigma(ossim_uint32 idx, double value, bool notify)
{
   if(!theAdjustmentList.size())
   {
      return;
   }
   if(idx < theAdjustmentList[theCurrentAdjustment].getNumberOfAdjustableParameters())
   {
      theAdjustmentList[theCurrentAdjustment].getParameterList()[idx].setSigma(value);
      if(notify)
      {
         adjustableParametersChanged();
      }
   }
}

ossimUnitType ossimAdjustableParameterInterface::getParameterUnit(ossim_uint32 idx)const
{
   if(theAdjustmentList.size())
   {
      if(idx < theAdjustmentList[theCurrentAdjustment].getNumberOfAdjustableParameters())
      {
         return theAdjustmentList[theCurrentAdjustment].getParameterList()[idx].getUnit();
      }
   }

   return OSSIM_UNIT_UNKNOWN;
}

void ossimAdjustableParameterInterface::setParameterUnit(ossim_uint32 idx, ossimUnitType unit)
{
   if(theAdjustmentList.size())
   {
      if(idx < theAdjustmentList[theCurrentAdjustment].getNumberOfAdjustableParameters())
      {
         theAdjustmentList[theCurrentAdjustment].getParameterList()[idx].setUnit(unit);
      }
   }
}

void ossimAdjustableParameterInterface::setParameterUnit(ossim_uint32 idx, const ossimString& unit)
{
   if(theAdjustmentList.size())
   {
      if(idx < theAdjustmentList[theCurrentAdjustment].getNumberOfAdjustableParameters())
      {
         theAdjustmentList[theCurrentAdjustment].getParameterList()[idx].setUnit((ossimUnitType)ossimUnitTypeLut::instance()->getEntryNumber(unit));
      }
   }
}


ossimString ossimAdjustableParameterInterface::getParameterDescription(ossim_uint32 idx)const
{
   if(theAdjustmentList.size())
   {
      if(idx < theAdjustmentList[theCurrentAdjustment].getNumberOfAdjustableParameters())
      {
         return theAdjustmentList[theCurrentAdjustment].getParameterList()[idx].getDescription();
      }
   }

   return ossimString("");
}

void ossimAdjustableParameterInterface::setParameterDescription(ossim_uint32 idx,
                                                                const ossimString& description)
{
   if(!theAdjustmentList.size())
   {
      return;
   }
   if(idx < theAdjustmentList[theCurrentAdjustment].getNumberOfAdjustableParameters())
   {
      theAdjustmentList[theCurrentAdjustment].getParameterList()[idx].setDescription(description);
   }
}

void  ossimAdjustableParameterInterface::setParameterCenter(ossim_uint32 idx, double center, bool notify)
{
   if(!theAdjustmentList.size())
   {
      return;
   }

   if(idx < theAdjustmentList[theCurrentAdjustment].getNumberOfAdjustableParameters())
   {
     theAdjustmentList[theCurrentAdjustment].getParameterList()[idx].setCenter(center);

	 if(notify)
	 {
	   adjustableParametersChanged();
	 }
   }
}

double ossimAdjustableParameterInterface::getParameterCenter(ossim_uint32 idx)const
{
   if(!theAdjustmentList.size())
   {
      return 0.0;
   }
   if(idx < theAdjustmentList[theCurrentAdjustment].getNumberOfAdjustableParameters())
   {
      return theAdjustmentList[theCurrentAdjustment].getParameterList()[idx].getCenter();
   }

   return 0.0;
}

double   ossimAdjustableParameterInterface::computeParameterOffset(ossim_uint32 idx)const
{
   if(!theAdjustmentList.size())
   {
      return 0.0;
   }
   if(idx < theAdjustmentList[theCurrentAdjustment].getNumberOfAdjustableParameters())
   {
      return theAdjustmentList[theCurrentAdjustment].getParameterList()[idx].computeOffset();
   }

   return 0.0;
}

ossim_int32 ossimAdjustableParameterInterface::findParameterIdxGivenDescription(ossim_uint32 adjustmentIdx,
                                                                                const ossimString& name)const
{
   ossim_int32 result = -1;
   if(adjustmentIdx < getNumberOfAdjustments())
   {
      ossim_uint32 idx = 0;
      ossim_uint32 n = theAdjustmentList[adjustmentIdx].getNumberOfAdjustableParameters();
      bool found = false;
      for(idx = 0; ((idx < n)&&(!found)); ++idx)
      {
         if(theAdjustmentList[theCurrentAdjustment].getParameterList()[idx].getDescription() == name)
         {
            result = (ossim_int32) idx;
         }
      }
   }
   
   return result;
}

ossim_int32 ossimAdjustableParameterInterface::findParameterIdxContainingDescription(ossim_uint32 adjustmentIdx,
                                                                                     const ossimString& name)const
{
   ossim_int32 result = -1;
   if(adjustmentIdx < getNumberOfAdjustments())
   {
      ossim_uint32 idx = 0;
      ossim_uint32 n = theAdjustmentList[adjustmentIdx].getNumberOfAdjustableParameters();
      bool found = false;
      for(idx = 0; ((idx < n)&&(!found)); ++idx)
      {
         if(theAdjustmentList[theCurrentAdjustment].getParameterList()[idx].getDescription().contains(name))
         {
            result = (ossim_int32) idx;
         }
      }
   }

   return result;
}


bool ossimAdjustableParameterInterface::isParameterLocked(ossim_uint32 idx)const
{
   if(!theAdjustmentList.size())
   {
      return false;
   }
   if(idx < theAdjustmentList[theCurrentAdjustment].getNumberOfAdjustableParameters())
   {
      return theAdjustmentList[theCurrentAdjustment].getParameterList()[idx].getLockFlag();
   }

   return false;
   
}

void ossimAdjustableParameterInterface::setParameterLockFlag(ossim_uint32 idxParam, bool flag)
{
   if(!theAdjustmentList.size())
   {
      return;
   }
   if(idxParam < theAdjustmentList[theCurrentAdjustment].getNumberOfAdjustableParameters())
   {
      theAdjustmentList[theCurrentAdjustment].getParameterList()[idxParam].setLockFlag(flag);
   }
}

bool ossimAdjustableParameterInterface::getParameterLockFlag(ossim_uint32 idx)const
{
   if(!theAdjustmentList.size())
   {
      return false;
   }
   if(idx < theAdjustmentList[theCurrentAdjustment].getNumberOfAdjustableParameters())
   {
      return theAdjustmentList[theCurrentAdjustment].getParameterList()[idx].getLockFlag();
   }

   return false;
}

void ossimAdjustableParameterInterface::lockAllParametersCurrentAdjustment()
{
   lockAllParameters(theCurrentAdjustment);
}

void ossimAdjustableParameterInterface::unlockAllParametersCurrentAdjustment()
{
   unlockAllParameters(theCurrentAdjustment);
}

void ossimAdjustableParameterInterface::lockAllParameters(ossim_uint32 idxAdjustment)
{
   if(idxAdjustment < getNumberOfAdjustments())
   {
      ossim_uint32 idx = 0;
      ossim_uint32 n   = theAdjustmentList[theCurrentAdjustment].getNumberOfAdjustableParameters();
      
      for(idx = 0; idx < n; ++idx)
      {
         theAdjustmentList[idxAdjustment].getParameterList()[idx].setLockFlag(true);
      }
   }
}

void ossimAdjustableParameterInterface::unlockAllParameters(ossim_uint32 idxAdjustment)
{
   if(idxAdjustment < getNumberOfAdjustments())
   {
      ossim_uint32 idx = 0;
      ossim_uint32 n   = theAdjustmentList[theCurrentAdjustment].getNumberOfAdjustableParameters();
      
      for(idx = 0; idx < n; ++idx)
      {
         theAdjustmentList[idxAdjustment].getParameterList()[idx].setLockFlag(false);
      }
   }
}


void ossimAdjustableParameterInterface::setParameterOffset(ossim_uint32 idx,
                                                           ossim_float64 value,
                                                           bool notify)
{
//    double center   = getParameterCenter(idx);
//    double sigma    = getParameterSigma(idx);
//    double minValue = center - sigma;
//    double maxValue = center + sigma;
//    double x = 0.0;
   
//    if(sigma != 0.0)
//    {
//       x = (value - center)/sigma;
      
//       value = center + x*sigma;
      
//       if(value < minValue)
//       {
//          x = -1;
//       }
//       else if(value >maxValue)
//       {
//          x = 1.0;
//       }
//       setAdjustableParameter(idx, x, false);
//    }
   
   if(!theAdjustmentList.size())
   {
      return;
   }
   if(idx < theAdjustmentList[theCurrentAdjustment].getNumberOfAdjustableParameters())
   {
      theAdjustmentList[theCurrentAdjustment].getParameterList()[idx].setOffset(value);
      if(notify)
      {
         adjustableParametersChanged();
      }
   }
}

void ossimAdjustableParameterInterface::resizeAdjustableParameterArray(ossim_uint32 numberOfParameters)
{
   if(!theAdjustmentList.size())
   {
      newAdjustment(numberOfParameters);
      return;
   }

   theAdjustmentList[theCurrentAdjustment].setNumberOfAdjustableParameters(numberOfParameters);
}

void ossimAdjustableParameterInterface::setAdjustment(const ossimAdjustmentInfo& adj, bool notify)
{
   setAdjustment(theCurrentAdjustment, adj, notify);
}

void ossimAdjustableParameterInterface::setAdjustment(ossim_uint32 idx, const ossimAdjustmentInfo& adj, bool notify)
{
   if(idx < getNumberOfAdjustments())
   {
      theAdjustmentList[(int)idx] = adj;
      if(notify)
      {
         adjustableParametersChanged();
      }
   }
}


void ossimAdjustableParameterInterface::addAdjustment(const ossimAdjustmentInfo& adj, bool notify)
{
   theAdjustmentList.push_back(adj);
   if(notify)
   {
      adjustableParametersChanged();
   }
}

void ossimAdjustableParameterInterface::getAdjustment(ossimAdjustmentInfo& adj)
{
   getAdjustment(theCurrentAdjustment,  adj);
}

void ossimAdjustableParameterInterface::getAdjustment(ossim_uint32 idx, ossimAdjustmentInfo& adj)
{
   adj.setNumberOfAdjustableParameters(0);

   if(idx < getNumberOfAdjustments())
   {
      adj = theAdjustmentList[(int)idx];
   }
}

ossim_uint32 ossimAdjustableParameterInterface::getNumberOfAdjustments()const
{
   return (ossim_uint32)theAdjustmentList.size();
}

ossim_uint32 ossimAdjustableParameterInterface::getCurrentAdjustmentIdx()const
{
   return theCurrentAdjustment;
}

void ossimAdjustableParameterInterface::setDirtyFlag(bool flag)
{
   if(theAdjustmentList.size() > 0)
   {
      theAdjustmentList[theCurrentAdjustment].setDirtyFlag(flag);
   }
}

void ossimAdjustableParameterInterface::setAllDirtyFlag(bool flag)
{
   ossim_uint32 idx = 0;
   
   for(idx = 0; idx < theAdjustmentList.size(); ++idx)
   {
      theAdjustmentList[idx].setDirtyFlag(flag);
   }
}

bool ossimAdjustableParameterInterface::hasDirtyAdjustments()const
{
   ossim_uint32 idx = 0;
      
   for(idx = 0; idx < theAdjustmentList.size(); ++idx)
   {
      if(theAdjustmentList[idx].isDirty())
      {
         return true;
      }
   }

   return false;
}

bool ossimAdjustableParameterInterface::saveAdjustments(ossimKeywordlist& kwl,
                                                        const ossimString& prefix)const
{
   kwl.add(prefix,
           NUMBER_OF_ADJUSTMENTS_KW,
           static_cast<ossim_uint32>(theAdjustmentList.size()),
           true);
   kwl.add(prefix,
           CURRENT_ADJUSTMENT_KW,
           (int)theCurrentAdjustment,
           true);
   
   for(ossim_uint32 adjIdx = 0; adjIdx < theAdjustmentList.size(); ++adjIdx)
   {
      ossimString adjPrefix = prefix + (ossimString(ADJUSTMENT_PREFIX) + ossimString::toString(adjIdx)+".");

      theAdjustmentList[adjIdx].saveState(kwl,
                                          adjPrefix);
   }

   return true;
}

bool ossimAdjustableParameterInterface::loadAdjustments(const ossimKeywordlist& kwl,
                                                        const ossimString& prefix)
{
   theAdjustmentList.clear();
   theCurrentAdjustment = 0;
   
   const char* numberOfAdjustments = kwl.find(prefix, NUMBER_OF_ADJUSTMENTS_KW);
   const char* currentAdjustment   = kwl.find(prefix, CURRENT_ADJUSTMENT_KW);

   if(!numberOfAdjustments)
   {
      numberOfAdjustments = kwl.find(prefix, NUMBER_OF_ADJUSTMENTS_OLD_KW);
   }
   if(!currentAdjustment)
   {
      currentAdjustment = kwl.find(prefix, CURRENT_ADJUSTMENT_OLD_KW);
   }
   if(!numberOfAdjustments) return false;
   
   ossim_int32 maxdjustments = ossimString(numberOfAdjustments).toInt32();
   
   for(ossim_int32 adjIdx = 0; adjIdx <maxdjustments; ++adjIdx)
   {
      ossimAdjustmentInfo info;
      ossimString adjPrefix = prefix + (ossimString(ADJUSTMENT_PREFIX) + ossimString::toString(adjIdx)+".");

      if(!info.loadState(kwl, adjPrefix))
      {
         return false;
      }
      theAdjustmentList.push_back(info);
   }

   theCurrentAdjustment = ossimString(currentAdjustment).toUInt32();
   
   return true;
}

void ossimAdjustableParameterInterface::adjustableParametersChanged()
{
}
