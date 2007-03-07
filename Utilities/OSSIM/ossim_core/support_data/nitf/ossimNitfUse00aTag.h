//*******************************************************************
//
// LICENSE: LGPL
//
// see top level LICENSE.txt
// 
// Author: Walt Bunch
//
// Description: Nitf support class for USE00A -
// Exploitation Usability extension.
// 
//********************************************************************
// $Id: ossimNitfUse00aTag.h,v 1.4 2005/08/09 11:32:21 gpotts Exp $
#ifndef ossimNitfUse00aTag_HEADER
#define ossimNitfUse00aTag_HEADER
#include <support_data/nitf/ossimNitfRegisteredTag.h>

class OSSIM_DLL ossimNitfUse00aTag : public ossimNitfRegisteredTag
{
public:
   
   enum
   {
      ANGLE_TO_NORTH_SIZE  = 3,
      MEAN_GSD_SIZE        = 5,
      FIELD3_SIZE          = 1,
      DYNAMIC_RANGE_SIZE   = 5,
      FIELD5_SIZE          = 3,
      FIELD6_SIZE          = 1,
      FIELD7_SIZE          = 3,
      OBL_ANG_SIZE         = 5,
      ROLL_ANG_SIZE        = 6,
      FIELD10_SIZE         = 12,
      FIELD11_SIZE         = 15,
      FIELD12_SIZE         = 4,
      FIELD13_SIZE         = 1,
      FIELD14_SIZE         = 3,
      FIELD15_SIZE         = 1,
      FIELD16_SIZE         = 1,
      N_REF_SIZE           = 2,
      REV_NUM_SIZE         = 5,
      N_SEG_SIZE           = 3,
      MAX_LP_SEG_SIZE      = 6,
      FIELD20_SIZE         = 6,
      FIELD21_SIZE         = 6,
      SUN_EL_SIZE          = 5,
      SUN_AZ_SIZE          = 5
      //                  -----
      //                  107 bytes
   };
   
   ossimNitfUse00aTag();
   virtual ~ossimNitfUse00aTag();
   virtual ossimString getRegisterTagName()const;
   virtual void parseStream(std::istream& in);
   virtual void writeStream(std::ostream& out);
   virtual ossim_uint32 getSizeInBytes()const;
   virtual void clearFields();
   
  // The set methods below taking ossimString args will truncate and
  // pad with spaces, as necessary, to match enumed size
   void setAngleToNorth(ossimString angleToNorth);
   ossimString getAngleToNorth()const;
   void setMeanGsd(ossimString meanGsd);
   ossimString getMeanGsd()const;
   void setField3(ossimString field3);
   ossimString getField3()const;
   void setDynamicRange(ossimString dynamicRange);
   ossimString getDynamicRange()const;
   void setField5(ossimString field5);
   ossimString getField5()const;
   void setField6(ossimString field6);
   ossimString getField6()const;
   void setField7(ossimString field7);
   ossimString getField7()const;
   void setOblAng(ossimString oblAng);
   ossimString getOblAng()const;
   void setRollAng(ossimString rollAng);
   ossimString getRollAng()const;
   void setField10(ossimString field10);
   ossimString getField10()const;
   void setField11(ossimString field11);
   ossimString getField11()const;
   void setField12(ossimString field12);
   ossimString getField12()const;
   void setField13(ossimString field13);
   ossimString getField13()const;
   void setField14(ossimString field14);
   ossimString getField14()const;
   void setField15(ossimString field15);
   ossimString getField15()const;
   void setField16(ossimString field16);
   ossimString getField16()const;
   void setNRef(ossimString nRef);
   ossimString getNRef()const;
   void setRevNum(ossimString revNum);
   ossimString getRevNum()const;
   void setNSeg(ossimString nSeg);
   ossimString getNSeg()const;
   void setMaxLpSeg(ossimString maxLpSeg);
   ossimString getMaxLpSeg()const;
   void setField20(ossimString field20);
   ossimString getField20()const;
   void setField21(ossimString field21);
   ossimString getField21()const;
   void setSunEl(ossimString sunEl);
   ossimString getSunEl()const;
   void setSunAz(ossimString sunAz);
   ossimString getSunAz()const;
   
protected:
   
   /**
    * FIELD: ANGLETONORTH
    *
    */
   char theAngleToNorth[ANGLE_TO_NORTH_SIZE+1];
   
   /**
    * FIELD: MEANGSD
    *
    */
   char theMeanGsd[MEAN_GSD_SIZE+1];
   
   /**
    * FIELD: FIELD3
    *
    */
   char theField3[FIELD3_SIZE+1];

   /**
    * FIELD: DYNAMICRANGE
    *
    */
   char theDynamicRange[DYNAMIC_RANGE_SIZE+1];

   /**
    * FIELD: FIELD5
    *
    */
   char theField5[FIELD5_SIZE+1];

   /**
    * FIELD: FIELD6
    *
    */
   char theField6[FIELD6_SIZE+1];

   /**
    * FIELD: FIELD7
    *
    */
   char theField7[FIELD7_SIZE+1];

   /**
    * FIELD: OBLANG
    *
    */
   char theOblAng[OBL_ANG_SIZE+1];

   /**
    * FIELD: ROLLANG
    *
    */
   char theRollAng[ROLL_ANG_SIZE+1];

   /**
    * FIELD: FIELD10
    *
    */
   char theField10[FIELD10_SIZE+1];

   /**
    * FIELD: FIELD11
    *
    */
   char theField11[FIELD11_SIZE+1];

   /**
    * FIELD: FIELD12
    *
    */
   char theField12[FIELD12_SIZE+1];

   /**
    * FIELD: FIELD13
    *
    */
   char theField13[FIELD13_SIZE+1];

   /**
    * FIELD: FIELD14
    *
    */
   char theField14[FIELD14_SIZE+1];

   /**
    * FIELD: FIELD15
    *
    */
   char theField15[FIELD15_SIZE+1];

   /**
    * FIELD: FIELD16
    *
    */
   char theField16[FIELD16_SIZE+1];

   /**
    * FIELD: NREF
    *
    */
   char theNRef[N_REF_SIZE+1];

   /**
    * FIELD: REVNUM
    *
    */
   char theRevNum[REV_NUM_SIZE+1];

   /**
    * FIELD: NSEG
    *
    */
   char theNSeg[N_SEG_SIZE+1];

   /**
    * FIELD: MAXLPSEG
    *
    */
   char theMaxLpSeg[MAX_LP_SEG_SIZE+1];

   /**
    * FIELD: FIELD20
    *
    */
   char theField20[FIELD20_SIZE+1];

   /**
    * FIELD: FIELD21
    *
    */
   char theField21[FIELD21_SIZE+1];

   /**
    * FIELD: SUNEL
    *
    */
   char theSunEl[SUN_EL_SIZE+1];

   /**
    * FIELD: SUNAZ
    *
    */
   char theSunAz[SUN_AZ_SIZE+1];

TYPE_DATA   
};

#endif
