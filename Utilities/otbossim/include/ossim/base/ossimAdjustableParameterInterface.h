//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
//
//*************************************************************************
// $Id: ossimAdjustableParameterInterface.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimAdjustableParameterInterface_HEADER
#define ossimAdjustableParameterInterface_HEADER
#include <ossim/base/ossimRtti.h>
#include <vector>
#include <ossim/base/ossimAdjustmentInfo.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimObject.h>

class OSSIMDLLEXPORT ossimAdjustableParameterInterface
{
public:
   ossimAdjustableParameterInterface();
   ossimAdjustableParameterInterface(const ossimAdjustableParameterInterface& rhs);
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
   

   const ossimAdjustableParameterInterface& operator = (const ossimAdjustableParameterInterface& rhs);
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

   ossim_int32 findParameterIdxGivenDescription(ossim_uint32 adjustmentIdx,
                                                const ossimString& name)const;
   ossim_int32 findParameterIdxContainingDescription(ossim_uint32 adjustmentIdx,
                                                     const ossimString& name)const;
   
   bool isParameterLocked(ossim_uint32 idx)const;

   void setParameterLockFlag(ossim_uint32 idxParam, bool flag);
   bool getParameterLockFlag(ossim_uint32 idx)const;

   void lockAllParametersCurrentAdjustment();
   void unlockAllParametersCurrentAdjustment();

   void lockAllParameters(ossim_uint32 idxAdjustment);
   void unlockAllParameters(ossim_uint32 idxAdjustment);
   
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
