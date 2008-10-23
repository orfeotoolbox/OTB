//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
//
//*************************************************************************
// $Id: ossimAdjustmentInfo.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimAdjustmentInfo_HEADER
#define ossimAdjustmentInfo_HEADER
#include <vector>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimAdjustableParameterInfo.h>

class ossimAdjustmentInfo
{
public:
   friend std::ostream& operator <<(std::ostream& out, const ossimAdjustmentInfo& data);
   
   
   ossimAdjustmentInfo(int numberOfAdjustableParameters=0);
   ossimAdjustmentInfo(const ossimAdjustmentInfo& rhs);
   
   void setNumberOfAdjustableParameters(ossim_uint32 numberOfAdjustableParameters);
   ossim_uint32 getNumberOfAdjustableParameters()const;
   ossimString getDescription()const;
   void setDescription(const ossimString& description);
   bool isDirty()const;
   void setDirtyFlag(bool flag=true);
   void setLockParameterFlag(bool flag,
                             ossim_uint32 idx);
   void keep();
   
   std::vector<ossimAdjustableParameterInfo>& getParameterList();
   const std::vector<ossimAdjustableParameterInfo>& getParameterList()const;
   void setLockFlag(bool flag,ossim_uint32 idx);
   
   bool saveState(ossimKeywordlist& kwl,
                  const ossimString& prefix=ossimString(""))const;
   bool loadState(const ossimKeywordlist& kwl,
                  const ossimString& prefix=ossimString(""));
   
private:
   std::vector<ossimAdjustableParameterInfo> theParameterList;
   ossimString                               theDescription;
   mutable bool                              theDirtyFlag;
};

#endif
