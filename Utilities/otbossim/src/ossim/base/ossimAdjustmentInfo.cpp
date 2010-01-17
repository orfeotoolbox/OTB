//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// LICENSE: See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimAdjustmentInfo.cpp 15833 2009-10-29 01:41:53Z eshirschorn $
#include <ossim/base/ossimAdjustmentInfo.h>
#include <ossim/base/ossimKeywordNames.h>

static const char* PARAM_PREFIX             = "adj_param_";
static const char* NUMBER_OF_PARAMS_KW      = "number_of_params";
static const char* DIRTY_FLAG_KW            = "dirty_flag";


std::ostream& operator <<(std::ostream& out, const ossimAdjustmentInfo& data)
{
   ossim_int32 idx = 0;

   out << "Description:      " << data.getDescription()
       << "\nNumber of Params: " << data.theParameterList.size()
       << "\nDirty flag:       " << data.theDirtyFlag << std::endl;

   for(idx = 0; idx < (int)data.getNumberOfAdjustableParameters(); ++idx)
   {
      out << "Param " << idx << std::endl;
      out << data.theParameterList[idx] << std::endl;
   }

   return out;
}


ossimAdjustmentInfo::ossimAdjustmentInfo(int numberOfAdjustableParameters)
   :theParameterList(numberOfAdjustableParameters),
    theDescription(""),
    theDirtyFlag(false)
{
}

ossimAdjustmentInfo::ossimAdjustmentInfo(const ossimAdjustmentInfo& rhs)
   :theParameterList(rhs.theParameterList),
    theDescription(rhs.theDescription),
    theDirtyFlag(rhs.theDirtyFlag)
{
}

void ossimAdjustmentInfo::setNumberOfAdjustableParameters(ossim_uint32 numberOfAdjustableParameters)
{
   std::vector<ossimAdjustableParameterInfo> temp = theParameterList;

   theParameterList.resize(numberOfAdjustableParameters);
   if(temp.size() < numberOfAdjustableParameters)
   {
      std::copy(temp.begin(),
                temp.end(),
                theParameterList.begin());
   }
   else if(temp.size() > numberOfAdjustableParameters)
   {
      if(numberOfAdjustableParameters > 0)
      {
         std::copy(temp.begin(),
                   temp.begin()+numberOfAdjustableParameters,
                   theParameterList.begin());
      }
   }
}

ossim_uint32 ossimAdjustmentInfo::getNumberOfAdjustableParameters()const
{
   return (ossim_uint32)theParameterList.size();
}

ossimString ossimAdjustmentInfo::getDescription()const
{
   return theDescription;
}

void ossimAdjustmentInfo::setDescription(const ossimString& description)
{
   theDescription = description;
}

bool ossimAdjustmentInfo::isDirty()const
{
   return theDirtyFlag;
}

void ossimAdjustmentInfo::setDirtyFlag(bool flag)
{
   theDirtyFlag = flag;
}

void ossimAdjustmentInfo::setLockFlag(bool flag,
                                      ossim_uint32 idx)
{
   if(idx < theParameterList.size())
   {
      theParameterList[idx].setLockFlag(flag);
   }
}

void ossimAdjustmentInfo::keep()
{
   ossim_uint32 idx = 0;

   for(idx = 0; idx < theParameterList.size();++idx)
   {
      double center = theParameterList[idx].computeOffset();
      theParameterList[idx].setParameter(0.0);
      theParameterList[idx].setCenter(center);
   }
}


std::vector<ossimAdjustableParameterInfo>& ossimAdjustmentInfo::getParameterList()
{
   return theParameterList;
}

const std::vector<ossimAdjustableParameterInfo>& ossimAdjustmentInfo::getParameterList()const
{
   return theParameterList;
}

bool ossimAdjustmentInfo::saveState(ossimKeywordlist& kwl,
                                    const ossimString& prefix)const
{
   kwl.add(prefix,
           ossimKeywordNames::DESCRIPTION_KW,
           getDescription(),
           true);
   kwl.add(prefix.c_str(),
           NUMBER_OF_PARAMS_KW,
           static_cast<ossim_uint32>(theParameterList.size()),
           true);

   kwl.add(prefix,
           DIRTY_FLAG_KW,
           (short)theDirtyFlag,
           true);

   ossimString value;
   for(ossim_uint32 idx = 0; idx < theParameterList.size();++idx)
   {
      ossimString newPrefix = ossimString(prefix) + (ossimString(PARAM_PREFIX) + ossimString::toString(idx)+".");
      theParameterList[idx].saveState(kwl, newPrefix.c_str());
   }

   return true;
}

bool ossimAdjustmentInfo::loadState(const ossimKeywordlist& kwl,
                                    const ossimString& prefix)
{
   setDescription(kwl.find(prefix, ossimKeywordNames::DESCRIPTION_KW));
   setNumberOfAdjustableParameters(ossimString(kwl.find(prefix, NUMBER_OF_PARAMS_KW)).toUInt32());
   const char *dirtyFlag = kwl.find(prefix, DIRTY_FLAG_KW);
   ossimString value;

   if(dirtyFlag)
   {
      theDirtyFlag = ossimString(dirtyFlag).toBool();
   }
   else
   {
      theDirtyFlag = false;
   }

   for(ossim_uint32 idx = 0; idx < theParameterList.size();++idx)
   {
      ossimString newPrefix = ossimString(prefix) + (ossimString(PARAM_PREFIX) + ossimString::toString(idx)+".");
      if(!theParameterList[idx].loadState(kwl, newPrefix.c_str()))
      {
         return false;
      }
   }

   return true;
}
