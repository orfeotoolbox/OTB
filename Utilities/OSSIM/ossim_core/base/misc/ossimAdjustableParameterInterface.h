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
// $Id: ossimAdjustableParameterInterface.h,v 1.29 2005/12/09 17:02:30 gpotts Exp $
#ifndef ossimAdjustableParameterInterface_HEADER
#define ossimAdjustableParameterInterface_HEADER
#include "base/common/ossimRtti.h"
#include <vector>
#include "ossimAdjustmentInfo.h"
#include "base/data_types/ossimKeywordlist.h"
#include "base/common/ossimObject.h"

class OSSIMDLLEXPORT ossimAdjustableParameterInterface
{
public:
   ossimAdjustableParameterInterface();
   virtual ~ossimAdjustableParameterInterface(){}
   void newAdjustment(ossim_uint32 numberOfParameters=0);
   void setAdjustmentDescription(const ossimString& description);
   ossimString getAdjustmentDescription()const;
   void setCurrentAdjustment(ossim_uint32 adjustmentIndex, bool notify=false);
   void eraseAdjustment(bool notify);
   void eraseAdjustment(ossim_uint32 idx, bool notify);
   virtual void initAdjustableParameters();
   void resetAdjustableParameters(bool notify=false);
   void copyAdjustment(ossim_uint32 idx, bool notify);
   void copyAdjustment(bool notify = false);

   virtual ossimObject* getBaseObject()=0;
   virtual const ossimObject* getBaseObject()const=0;
   
   /*!
    * Will copy the adjustment but will set the new center to the
    * applied current center plus the application of the adjustment
    *
    */
   void keepAdjustment(ossim_uint32 idx, bool createCopy);
   void keepAdjustment(bool createCopy=true);
   
   
   void removeAllAdjustments();
   ossim_uint32 getNumberOfAdjustableParameters()const;
   double       getAdjustableParameter(ossim_uint32 idx)const;
   void         setAdjustableParameter(ossim_uint32 idx, double value,
                                       bool notify=false);
   void         setAdjustableParameter(ossim_uint32 idx,
                                       double value,
                                       double sigma,
                                       bool notify=false);
   double       getParameterSigma(ossim_uint32 idx)const;
   void         setParameterSigma(ossim_uint32 idx,
                                  double value,
                                  bool notify=false);
   ossimUnitType getParameterUnit(ossim_uint32 idx)const;
   void          setParameterUnit(ossim_uint32 idx, ossimUnitType unit);
   void          setParameterUnit(ossim_uint32 idx, const ossimString& unit);

   void           setParameterCenter(ossim_uint32 idx,
                                     double center,
                                     bool notify = false);
   double        getParameterCenter(ossim_uint32 idx)const;
   double        computeParameterOffset(ossim_uint32 idx)const;
   void          setParameterOffset(ossim_uint32 idx,
                                    ossim_float64 value,
                                    bool notify = false);
   
   ossimString   getParameterDescription(ossim_uint32 idx)const;
   void          setParameterDescription(ossim_uint32 idx,
                                         const ossimString& descrption);
   
   void resizeAdjustableParameterArray(ossim_uint32 numberOfParameters);

   void setAdjustment(const ossimAdjustmentInfo& adj, bool notify=false);
   void setAdjustment(ossim_uint32 idx, const ossimAdjustmentInfo& adj, bool notify=false);
   
   void addAdjustment(const ossimAdjustmentInfo& adj, bool notify);
   void getAdjustment(ossimAdjustmentInfo& adj);
   void getAdjustment(ossim_uint32 idx, ossimAdjustmentInfo& adj);
   
   ossim_uint32 getNumberOfAdjustments()const;
   ossim_uint32 getCurrentAdjustmentIdx()const;


   
   void setDirtyFlag(bool flag=true);
   void setAllDirtyFlag(bool flag = true);
   bool hasDirtyAdjustments()const;
   
   bool saveAdjustments(ossimKeywordlist& kwl,
                        const ossimString& prefix=ossimString(""))const;
   bool loadAdjustments(const ossimKeywordlist& kwl,
                        const ossimString& prefix=ossimString(""));
private:
   std::vector<ossimAdjustmentInfo> theAdjustmentList;
   ossim_uint32                     theCurrentAdjustment;
   
public:
   virtual void adjustableParametersChanged();
   
TYPE_DATA
};

#endif
