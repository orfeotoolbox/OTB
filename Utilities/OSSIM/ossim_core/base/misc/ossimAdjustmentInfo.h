//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
//
//*************************************************************************
// $Id: ossimAdjustmentInfo.h,v 1.2 2004/01/09 19:10:09 gpotts Exp $
#ifndef ossimAdjustmentInfo_HEADER
#define ossimAdjustmentInfo_HEADER
#include <vector>
#include "base/common/ossimConstants.h"
#include "base/data_types/ossimString.h"
#include "ossimAdjustableParameterInfo.h"

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

   void keep();
   
   std::vector<ossimAdjustableParameterInfo>& getParameterList();
   const std::vector<ossimAdjustableParameterInfo>& getParameterList()const;
   
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
