//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
// 
// Author: Garrett Potts
//
// Description: Nitf support class for RPC00A -
// Rational Polynomial Coefficient extension.
//
//********************************************************************
// $Id: ossimNitfRpcBase.cpp 17206 2010-04-25 23:20:40Z dburken $

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>
#include <ossim/support_data/ossimNitfRpcBase.h>
#include <ossim/support_data/ossimNitfCommon.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimStringProperty.h>

static const ossimTrace traceDebug(ossimString("ossimNitfRpcBase:debug"));
static const ossimString ERR_BIAS_KW = "ERR_BIAS";
static const ossimString ERR_RAND_KW = "ERR_RAND";
static const ossimString LINE_OFF_KW = "LINE_OFF";
static const ossimString SAMP_OFF_KW = "SAMP_OFF";
static const ossimString LAT_OFF_KW = "LAT_OFF";
static const ossimString LONG_OFF_KW = "LONG_OFF";
static const ossimString HEIGHT_OFF_KW = "HEIGHT_OFF";
static const ossimString LINE_SCALE_KW = "LINE_SCALE";
static const ossimString SAMP_SCALE_KW = "SAMP_SCALE";
static const ossimString LAT_SCALE_KW = "LAT_SCALE";
static const ossimString LONG_SCALE_KW = "LONG_SCALE";
static const ossimString HEIGHT_SCALE_KW = "HEIGHT_SCALE";
static const ossimString LINE_NUM_COEFF_KW = "LINE_NUM_COEFF_";
static const ossimString LINE_DEN_COEFF_KW = "LINE_DEN_COEFF_";
static const ossimString SAMP_NUM_COEFF_KW = "SAMP_NUM_COEFF_";
static const ossimString SAMP_DEN_COEFF_KW = "SAMP_DEN_COEFF_";

RTTI_DEF1(ossimNitfRpcBase, "ossimNitfRpcBase", ossimNitfRegisteredTag);


ossimNitfRpcBase::ossimNitfRpcBase()
{
   clearFields();
}

void ossimNitfRpcBase::parseStream(std::istream& in)
{
   clearFields();
   
   in.read(theSuccess, SUCCESS_SIZE);
   in.read(theErrorBias, ERROR_BIAS_SIZE);
   in.read(theErrRand, ERR_RAND_SIZE);
   in.read(theLineOffset, LINE_OFFSET_SIZE);
   in.read(theSampleOffset, SAMPLE_OFFSET_SIZE);
   in.read(theGeodeticLatOffset, GEODETIC_LAT_OFFSET_SIZE);
   in.read(theGeodeticLonOffset, GEODETIC_LON_OFFSET_SIZE);
   in.read(theGeodeticHeightOffset, GEODETIC_HEIGHT_OFFSET_SIZE);
   in.read(theLineScale, LINE_SCALE_SIZE);
   in.read(theSampleScale, SAMPLE_SCALE_SIZE);
   in.read(theGeodeticLatScale, GEODETIC_LAT_SCALE_SIZE);
   in.read(theGeodeticLonScale, GEODETIC_LON_SCALE_SIZE);
   in.read(theGeodeticHeightScale, GEODETIC_HEIGHT_SCALE_SIZE);
   
   ossim_uint32 idx = 0;
   
   char temp1[LINE_NUMERATOR_COEFFICIENT_SIZE+1];
   temp1[LINE_NUMERATOR_COEFFICIENT_SIZE] = '\0';
   for(idx = 0; idx < LINE_NUMERATOR_COEFFICIENT_COUNT; ++idx)
   {
      in.read(temp1, LINE_NUMERATOR_COEFFICIENT_SIZE);
      theLineNumeratorCoefficient[idx] = temp1;
   }
   
   char temp2[LINE_DENOMINATOR_COEFFICIENT_SIZE+1];
   temp2[LINE_DENOMINATOR_COEFFICIENT_SIZE] = '\0';
   for(idx = 0; idx < LINE_DENOMINATOR_COEFFICIENT_COUNT; ++idx)
   {
      in.read(temp2, LINE_DENOMINATOR_COEFFICIENT_SIZE);
      theLineDenominatorCoefficient[idx] = temp2;
   }
   
   char temp3[SAMPLE_NUMERATOR_COEFFICIENT_SIZE+1];
   temp3[SAMPLE_NUMERATOR_COEFFICIENT_SIZE] = '\0';
   for(idx = 0; idx < SAMPLE_NUMERATOR_COEFFICIENT_COUNT; ++idx)
   {
      in.read(temp3, SAMPLE_NUMERATOR_COEFFICIENT_SIZE);
      theSampleNumeratorCoefficient[idx] = temp3;
   }
   
   char temp4[SAMPLE_DENOMINATOR_COEFFICIENT_SIZE+1];
   temp4[SAMPLE_DENOMINATOR_COEFFICIENT_SIZE] = '\0';
   for(idx = 0; idx < SAMPLE_DENOMINATOR_COEFFICIENT_COUNT; ++idx)
   {
      in.read(temp4, SAMPLE_DENOMINATOR_COEFFICIENT_SIZE);
      theSampleDenominatorCoefficient[idx] = temp4;
   }
}

void ossimNitfRpcBase::writeStream(std::ostream& out)
{
   ossim_uint32 idx = 0;
   
   out.write(theSuccess, SUCCESS_SIZE);
   out.write(theErrorBias, ERROR_BIAS_SIZE);
   out.write(theErrRand, ERR_RAND_SIZE);
   out.write(theLineOffset, LINE_OFFSET_SIZE);
   out.write(theSampleOffset, SAMPLE_OFFSET_SIZE);
   out.write(theGeodeticLatOffset, GEODETIC_LAT_OFFSET_SIZE);
   out.write(theGeodeticLonOffset, GEODETIC_LON_OFFSET_SIZE);
   out.write(theGeodeticHeightOffset, GEODETIC_HEIGHT_OFFSET_SIZE);
   out.write(theLineScale, LINE_SCALE_SIZE);
   out.write(theSampleScale, SAMPLE_SCALE_SIZE);
   out.write(theGeodeticLatScale, GEODETIC_LAT_SCALE_SIZE);
   out.write(theGeodeticLonScale, GEODETIC_LON_SCALE_SIZE);
   out.write(theGeodeticHeightScale, GEODETIC_HEIGHT_SCALE_SIZE);
   for(idx = 0; idx < LINE_NUMERATOR_COEFFICIENT_COUNT; ++idx)
   {
      out.write(theLineNumeratorCoefficient[idx].c_str(),
                LINE_NUMERATOR_COEFFICIENT_SIZE);
   }
   for(idx = 0; idx < LINE_DENOMINATOR_COEFFICIENT_COUNT; ++idx)
   {
      out.write(theLineDenominatorCoefficient[idx].c_str(),
                LINE_DENOMINATOR_COEFFICIENT_SIZE);
   }
   for(idx = 0; idx < SAMPLE_NUMERATOR_COEFFICIENT_COUNT; ++idx)
   {
      out.write(theSampleNumeratorCoefficient[idx].c_str(),
                SAMPLE_NUMERATOR_COEFFICIENT_SIZE);
   }
   for(idx = 0; idx < SAMPLE_DENOMINATOR_COEFFICIENT_COUNT; ++idx)
   {
      out.write(theSampleDenominatorCoefficient[idx].c_str(),
                SAMPLE_DENOMINATOR_COEFFICIENT_SIZE);
   }
}

ossim_uint32 ossimNitfRpcBase::getSizeInBytes()const
{
   return 1041;
}

void ossimNitfRpcBase::clearFields()
{
   theSuccess[0] = '1';
   memcpy(theErrorBias, "0000.00", 7);
   memcpy(theErrRand, "0000.00", 7);
   memcpy(theLineOffset, "000000", 6);
   memcpy(theSampleOffset, "00000", 5);
   memcpy(theGeodeticLatOffset, "+00.0000", 8);
   memcpy(theGeodeticLonOffset, "+000.0000", 9);
   memcpy(theGeodeticHeightOffset, "+0000", 5);
   memcpy(theLineScale, "000001", 6);
   memcpy(theSampleScale, "00001", 5);
   memcpy(theGeodeticLatScale, "+00.0000", 8);
   memcpy(theGeodeticLonScale, "+000.0000", 9);
   memcpy(theGeodeticHeightScale, "+0000", 5);
   
   theSuccess[1] = '\0';
   theErrorBias[7] = '\0';
   theErrRand[7] = '\0';
   theLineOffset[6] = '\0';
   theSampleOffset[5] = '\0';
   theGeodeticLatOffset[8]='\0';
   theGeodeticLonOffset[9]='\0';
   theGeodeticHeightOffset[5]='\0';
   theLineScale[6]='\0';
   theSampleScale[5]='\0';
   theGeodeticLatScale[8]='\0';
   theGeodeticLonScale[9]='\0';
   theGeodeticHeightScale[5]='\0';
   
   theLineNumeratorCoefficient.resize(20);
   theLineDenominatorCoefficient.resize(20);
   theSampleNumeratorCoefficient.resize(20);
   theSampleDenominatorCoefficient.resize(20);
   ossim_uint32 idx = 0;
   const char* zeroString = "+0.000000E+0";
   
   for(idx = 0; idx < theLineNumeratorCoefficient.size();++idx)
   {
      theLineNumeratorCoefficient[idx] = zeroString; 
      theLineDenominatorCoefficient[idx] = zeroString;;
      theSampleNumeratorCoefficient[idx] = zeroString;;
      theSampleDenominatorCoefficient[idx] = zeroString;;
   }
}

bool ossimNitfRpcBase::getSuccess()const
{
   return (theSuccess[0] == '1');
}

void ossimNitfRpcBase::setSuccess(bool success)
{
   theSuccess[0] = success?'1':'0';
}

ossimString ossimNitfRpcBase::getErrorBias()const
{
   return theErrorBias;
}

void ossimNitfRpcBase::setErrorBias(const ossim_float64& errorBias)
{
   ossimString os = ossimNitfCommon::convertToDoubleString(errorBias,
                                                           2,
                                                           ERROR_BIAS_SIZE);
   setErrorBias(os);
}

void ossimNitfRpcBase::setErrorBias(const ossimString& errorBias)
{
   memset(theErrorBias, ' ', ERROR_BIAS_SIZE);
   memcpy(theErrorBias, errorBias.c_str(), std::min((size_t)ERROR_BIAS_SIZE, errorBias.length()));
}

ossimString ossimNitfRpcBase::getErrorRand()const
{
   return theErrRand;
}

void ossimNitfRpcBase::setErrorRand(const ossim_float64& errorRand)
{
   ossimString os = ossimNitfCommon::convertToDoubleString(errorRand,
                                                           2,
                                                           ERROR_BIAS_SIZE);
   setErrorRand(os);
}

void ossimNitfRpcBase::setErrorRand(const ossimString& errorRand)
{
   memset(theErrRand, ' ', ERR_RAND_SIZE);
   memcpy(theErrRand, errorRand.c_str(), std::min((size_t)ERR_RAND_SIZE, errorRand.length()));
}

ossimString ossimNitfRpcBase::getLineOffset()const
{
   return theLineOffset;
}

void ossimNitfRpcBase::setLineOffset(ossim_uint32 lineOffset)
{
   ossimString os = ossimNitfCommon::convertToUIntString(lineOffset,
                                                         LINE_OFFSET_SIZE);
   setLineOffset(os);
}

void ossimNitfRpcBase::setLineOffset(const ossimString& lineOffset)
{
   memset(theLineOffset, ' ', LINE_OFFSET_SIZE);
   memcpy(theLineOffset, lineOffset.c_str(), std::min((size_t)LINE_OFFSET_SIZE, lineOffset.length()));
}

ossimString ossimNitfRpcBase::getSampleOffset()const
{
   return theSampleOffset;
}

void ossimNitfRpcBase::setSampleOffset(ossim_uint32 sampleOffset)
{
   ossimString os = ossimNitfCommon::convertToUIntString(sampleOffset,
                                                         SAMPLE_OFFSET_SIZE);
   setSampleOffset(os);
}

void ossimNitfRpcBase::setSampleOffset(const ossimString& sampleOffset)
{
   memset(theSampleOffset, ' ', SAMPLE_OFFSET_SIZE);
   memcpy(theSampleOffset, sampleOffset.c_str(), std::min((size_t)SAMPLE_OFFSET_SIZE, sampleOffset.length()));
}

ossimString ossimNitfRpcBase::getGeodeticLatOffset()const
{
   return theGeodeticLatOffset;
}

void ossimNitfRpcBase::setGeodeticLatOffset(
   const ossim_float64& geodeticLatOffset)
{
   if ( (geodeticLatOffset >= -90.0) && (geodeticLatOffset <= 90.0) )
   {
      ossimString os =
         ossimNitfCommon::convertToDoubleString(geodeticLatOffset,
                                                4,
                                                GEODETIC_LAT_OFFSET_SIZE);
      setGeodeticLatOffset(os);
   }
}

void ossimNitfRpcBase::setGeodeticLatOffset(
   const ossimString& geodeticLatOffset)
{
   memset(theGeodeticLatOffset, ' ', GEODETIC_LAT_OFFSET_SIZE);
   memcpy(theGeodeticLatOffset, geodeticLatOffset.c_str(), std::min((size_t)GEODETIC_LAT_OFFSET_SIZE, geodeticLatOffset.length()));
}

ossimString ossimNitfRpcBase::getGeodeticLonOffset()const
{
   return theGeodeticLonOffset;
}

void ossimNitfRpcBase::setGeodeticLonOffset(
   const ossim_float64& geodeticLonOffset)
{
   if ( (geodeticLonOffset >= -180.0) && (geodeticLonOffset <= 180.0) )
   {
      ossimString os =
         ossimNitfCommon::convertToDoubleString(geodeticLonOffset,
                                                4,
                                                GEODETIC_LON_OFFSET_SIZE);
      setGeodeticLonOffset(os);
   }
}

void ossimNitfRpcBase::setGeodeticLonOffset(const ossimString& geodeticLonOffset)
{
   memset(theGeodeticLonOffset, ' ', GEODETIC_LON_OFFSET_SIZE);
   memcpy(theGeodeticLonOffset, geodeticLonOffset.c_str(), std::min((size_t)GEODETIC_LON_OFFSET_SIZE, geodeticLonOffset.length()));
}

ossimString ossimNitfRpcBase::getGeodeticHeightOffset()const
{
   return theGeodeticHeightOffset;
}

void ossimNitfRpcBase::setGeodeticHeightOffset(
   ossim_int32 geodeticHeightOffset)
{
   ossimString os =
      ossimNitfCommon::convertToIntString(geodeticHeightOffset,
                                          GEODETIC_HEIGHT_OFFSET_SIZE);
   setGeodeticHeightOffset(os);
}

void ossimNitfRpcBase::setGeodeticHeightOffset(const ossimString& geodeticHeightOffset)
{
   memset(theGeodeticHeightOffset, ' ', GEODETIC_HEIGHT_OFFSET_SIZE);
   memcpy(theGeodeticHeightOffset, geodeticHeightOffset.c_str(), std::min((size_t)GEODETIC_HEIGHT_OFFSET_SIZE, geodeticHeightOffset.length()));
}

ossimString ossimNitfRpcBase::getLineScale()const
{
   return theLineScale;
}

void ossimNitfRpcBase::setLineScale(ossim_uint32 lineScale)
{
   ossimString os = ossimNitfCommon::convertToUIntString(lineScale,
                                                         LINE_SCALE_SIZE);
   setLineScale(os);
}

void ossimNitfRpcBase::setLineScale(const ossimString& lineScale)
{
   memset(theLineScale, ' ', LINE_SCALE_SIZE);
   memcpy(theLineScale, lineScale.c_str(), std::min((size_t)LINE_SCALE_SIZE, lineScale.length()));
}

ossimString ossimNitfRpcBase::getSampleScale()const
{
   return theSampleScale;
}

void ossimNitfRpcBase::setSampleScale(ossim_uint32 sampleScale)
{
   ossimString os = ossimNitfCommon::convertToUIntString(sampleScale,
                                       SAMPLE_SCALE_SIZE);
   setSampleScale(os);
}

void ossimNitfRpcBase::setSampleScale(const ossimString& sampleScale)
{
   memset(theSampleScale, ' ', SAMPLE_SCALE_SIZE);
   memcpy(theSampleScale, sampleScale.c_str(), std::min((size_t)SAMPLE_SCALE_SIZE, sampleScale.length()));
}

ossimString ossimNitfRpcBase::getGeodeticLatScale()const
{
   return theGeodeticLatScale;
}

void ossimNitfRpcBase::setGeodeticLatScale(
   const ossim_float64& geodeticLatScale)
{
   if (geodeticLatScale == 0.0)
   {
      // Error scale cannot be zero.
      return;
   }
   
   if ( (geodeticLatScale >= -90.0) && (geodeticLatScale <= 90.0) )
   {
      ossimString os = ossimNitfCommon::convertToDoubleString(geodeticLatScale,
                                             4,
                                             GEODETIC_LAT_SCALE_SIZE);
      setGeodeticLatScale(os);
   }
}

void ossimNitfRpcBase::setGeodeticLatScale(const ossimString& geodeticLatScale)
{
   memset(theGeodeticLatScale, ' ', GEODETIC_LAT_SCALE_SIZE);
   memcpy(theGeodeticLatScale, geodeticLatScale.c_str(), std::min((size_t)GEODETIC_LAT_SCALE_SIZE, geodeticLatScale.length()));
}

ossimString ossimNitfRpcBase::getGeodeticLonScale()const
{
   return theGeodeticLonScale;
}

void ossimNitfRpcBase::setGeodeticLonScale(
   const ossim_float64& geodeticLonScale)
{
   if (geodeticLonScale == 0.0)
   {
      // Error scale cannot be zero.
      return;
   }
   
   if ( (geodeticLonScale >= -180.0) && (geodeticLonScale <= 180.0) )
   {
      ossimString os = ossimNitfCommon::convertToDoubleString(geodeticLonScale,
                                             4,
                                             GEODETIC_LON_SCALE_SIZE);
      setGeodeticLonScale(os);
   }
}

void ossimNitfRpcBase::setGeodeticLonScale(const ossimString& geodeticLonScale)
{
   memset(theGeodeticLonScale, ' ', GEODETIC_LON_SCALE_SIZE);
   memcpy(theGeodeticLonScale, geodeticLonScale.c_str(), std::min((size_t)GEODETIC_LON_SCALE_SIZE, geodeticLonScale.length()));
}

ossimString ossimNitfRpcBase::getGeodeticHeightScale()const
{
   return theGeodeticHeightScale;
}

void ossimNitfRpcBase::setGeodeticHeightScale(ossim_int32 geodeticHeightScale)
{
   ossimString os =
      ossimNitfCommon::convertToIntString(geodeticHeightScale,
                                          GEODETIC_HEIGHT_SCALE_SIZE);
   setGeodeticHeightScale(os);
}

void ossimNitfRpcBase::setGeodeticHeightScale(
   const ossimString& geodeticHeightScale)
{
   memset(theGeodeticHeightScale, ' ', GEODETIC_HEIGHT_SCALE_SIZE);
   memcpy(theGeodeticHeightScale, geodeticHeightScale.c_str(), std::min((size_t)GEODETIC_HEIGHT_SCALE_SIZE, geodeticHeightScale.length()));
}

/* Temp comment out until range error handling is decided.*/
#if 0
// The assert here will be ignored when compiled with -DNDEBUG so ossimSetError and fix the index
#define ASSERT_ARRAY_CONSTRAINT(expression, idx, correctedValue, module) \
   assert(expression);                                                  \
   ossimSetError(getClassName().c_str(), ossimErrorCodes::OSSIM_ERROR,  \
                 "File %s line %d Module %s Error:\nArray index constraint error.\n", \
                 __FILE__, __LINE__, module);                           \
   if (! (expression) ) idx = correctedValue;

#endif
   
ossimString ossimNitfRpcBase::getLineNumeratorCoeff(ossim_uint32 idx)const
{
//ASSERT_ARRAY_CONSTRAINT(idx >= 0 && idx < LINE_NUMERATOR_COEFFICIENT_COUNT,idx,0,"ossimNitfRpcBase::getLineNumeratorCoeff");

   if (idx >= LINE_NUMERATOR_COEFFICIENT_COUNT)
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimNitfRpcBase::getLineNumeratorCoeff range error!"
            << std::endl;
      }
      return ossimString();
   }
   
   return theLineNumeratorCoefficient[idx];
}

void ossimNitfRpcBase::setLineNumeratorCoeff(
   const std::vector<ossim_float64>& coeff)
{
   if (coeff.size() != LINE_NUMERATOR_COEFFICIENT_COUNT)
   {
      // Error...
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimNitfRpcBase::setLineNumeratorCoeff WARNING:"
            << "\nArray not of correct size!"
            << std::endl;
      }
      return;
   }

   for (ossim_uint32 i = 0; i < LINE_NUMERATOR_COEFFICIENT_COUNT; ++i)
   {
      ossimString os = ossimNitfCommon::convertToScientificString(
         coeff[i], LINE_NUMERATOR_COEFFICIENT_SIZE);
      setLineNumeratorCoeff(i, os);
   }
}

void ossimNitfRpcBase::setLineNumeratorCoeff(ossim_uint32 idx,
                                             const ossimString& coeff)
{
   // ASSERT_ARRAY_CONSTRAINT(idx >= 0 && idx < LINE_NUMERATOR_COEFFICIENT_COUNT,idx,0,"ossimNitfRpcBase::setLineNumeratorCoeff");

   if (idx >= LINE_NUMERATOR_COEFFICIENT_COUNT)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimNitfRpcBase::setLineNumeratorCoeff range error!"
         << std::endl;
      return;
   } 

   std::string s(coeff, 0, LINE_NUMERATOR_COEFFICIENT_SIZE);
   theLineNumeratorCoefficient[idx] = s;  
}

ossimString ossimNitfRpcBase::getLineDenominatorCoeff(ossim_uint32 idx)const
{
   // ASSERT_ARRAY_CONSTRAINT(idx >= 0 && idx < LINE_DENOMINATOR_COEFFICIENT_COUNT,idx,0,"ossimNitfRpcBase::getLineDenominatorCoeff");

   if (idx >= LINE_DENOMINATOR_COEFFICIENT_COUNT)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimNitfRpcBase::getLineDenominatorCoeff range error!"
         << std::endl;
      return ossimString();
   }  
   
   return theLineDenominatorCoefficient[idx];
}

void ossimNitfRpcBase::setLineDenominatorCoeff(
   const std::vector<ossim_float64>& coeff)
{
   if (coeff.size() != LINE_DENOMINATOR_COEFFICIENT_COUNT)
   {
      // Error...
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimNitfRpcBase::setLineDenominatorCoeff WARNING:"
            << "\nArray not of correct size!"
            << std::endl;
      }
      return;
   }

   for (ossim_uint32 i = 0; i < LINE_DENOMINATOR_COEFFICIENT_COUNT; ++i)
   {
      ossimString os = ossimNitfCommon::convertToScientificString(
         coeff[i], LINE_DENOMINATOR_COEFFICIENT_SIZE);
      setLineDenominatorCoeff(i, os);
   }
}

void ossimNitfRpcBase::setLineDenominatorCoeff(ossim_uint32 idx,
                                               const ossimString& coeff)
{
   // ASSERT_ARRAY_CONSTRAINT(idx >= 0 && idx < LINE_DENOMINATOR_COEFFICIENT_COUNT,idx,0,"ossimNitfRpcBase::setLineDenominatorCoeff");

   if (idx >= LINE_DENOMINATOR_COEFFICIENT_COUNT)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimNitfRpcBase::setLineDenominatorCoeff range error!"
         << std::endl;
      return;
   } 
   
   std::string s(coeff, 0, LINE_DENOMINATOR_COEFFICIENT_SIZE);
   theLineDenominatorCoefficient[idx] = s;  
}

ossimString ossimNitfRpcBase::getSampleNumeratorCoeff(ossim_uint32 idx)const
{
   // ASSERT_ARRAY_CONSTRAINT(idx >= 0 && idx < SAMPLE_NUMERATOR_COEFFICIENT_COUNT,idx,0,"ossimNitfRpcBase::getSampleNumeratorCoeff");

   if (idx >= SAMPLE_NUMERATOR_COEFFICIENT_COUNT)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimNitfRpcBase::getSampleNumeratorCoeff range error!"
         << std::endl;
      return ossimString();
   } 
   return theSampleNumeratorCoefficient[idx];
}

void ossimNitfRpcBase::setSampleNumeratorCoeff(
   const std::vector<ossim_float64>& coeff)
{
   if (coeff.size() != SAMPLE_NUMERATOR_COEFFICIENT_COUNT)
   {
      // Error...
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimNitfRpcBase::setSampleNumeratorCoeff WARNING:"
            << "\nArray not of correct size!"
            << std::endl;
      }
      return;
   }

   for (ossim_uint32 i = 0; i < SAMPLE_NUMERATOR_COEFFICIENT_COUNT; ++i)
   {
      ossimString os = ossimNitfCommon::convertToScientificString(
         coeff[i], SAMPLE_NUMERATOR_COEFFICIENT_SIZE);
      setSampleNumeratorCoeff(i, os);
   }
}

void ossimNitfRpcBase::setSampleNumeratorCoeff(ossim_uint32 idx,
                                               const ossimString& coeff)
{
   // ASSERT_ARRAY_CONSTRAINT(idx >= 0 && idx < SAMPLE_NUMERATOR_COEFFICIENT_COUNT,idx,0,"ossimNitfRpcBase::setSampleNumeratorCoeff");

   if (idx >= SAMPLE_NUMERATOR_COEFFICIENT_COUNT)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimNitfRpcBase::setSampleNumeratorCoeff range error!"
         << std::endl;
      return;
   } 
   
   std::string s(coeff, 0, SAMPLE_NUMERATOR_COEFFICIENT_SIZE);
   theSampleNumeratorCoefficient[idx] = s;  
}

ossimString ossimNitfRpcBase::getSampleDenominatorCoeff(ossim_uint32 idx)const
{
   // ASSERT_ARRAY_CONSTRAINT(idx >= 0 && idx < SAMPLE_DENOMINATOR_COEFFICIENT_COUNT,idx,0,"ossimNitfRpcBase::getSampleDenominatorCoeff");

   if (idx >= SAMPLE_DENOMINATOR_COEFFICIENT_COUNT)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimNitfRpcBase::getSampleDenominatorCoeff range error!"
         << std::endl;
      return ossimString();
   } 
   
   return theSampleDenominatorCoefficient[idx];
}

void ossimNitfRpcBase::setSampleDenominatorCoeff(
   const std::vector<ossim_float64>& coeff)
{
   if (coeff.size() != SAMPLE_DENOMINATOR_COEFFICIENT_COUNT)
   {
      // Error...
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimNitfRpcBase::setSampleDenominatorCoeff WARNING:"
            << "\nArray not of correct size!"
            << std::endl;
      }
      return;
   }

   for (ossim_uint32 i = 0; i < SAMPLE_DENOMINATOR_COEFFICIENT_COUNT; ++i)
   {
      ossimString os = ossimNitfCommon::convertToScientificString(
         coeff[i], SAMPLE_DENOMINATOR_COEFFICIENT_SIZE);
      setSampleDenominatorCoeff(i, os);
   }
}

void ossimNitfRpcBase::setSampleDenominatorCoeff(
   ossim_uint32 idx, const ossimString& coeff)
{
   // ASSERT_ARRAY_CONSTRAINT(idx >= 0 && idx < SAMPLE_DENOMINATOR_COEFFICIENT_COUNT,idx,0,"ossimNitfRpcBase::setSampleDenominatorCoeff");

   if (idx >= SAMPLE_DENOMINATOR_COEFFICIENT_COUNT)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimNitfRpcBase::setSampleDenominatorCoeff range error!"
         << std::endl;
      return;
   }
   
  std::string s(coeff, 0, SAMPLE_DENOMINATOR_COEFFICIENT_SIZE);
  s.resize(SAMPLE_DENOMINATOR_COEFFICIENT_SIZE, ' ');
  theSampleDenominatorCoefficient[idx] = s;  
}

std::ostream& ossimNitfRpcBase::print(std::ostream& out,
                                      const std::string& prefix) const
{
   std::string pfx = prefix;
   pfx += getRegisterTagName();
   pfx += ".";

   out << setiosflags(std::ios::left)
       << pfx << std::setw(24) << "CETAG:"
       << getRegisterTagName() << "\n"
       << pfx << std::setw(24) << "CEL:"
       << getSizeInBytes() << "\n"
       << pfx << std::setw(24) << "SUCCESS:"
       << theSuccess << "\n" 
       << pfx << std::setw(24) << "ERR_BIAS:"
       << theErrorBias << "\n"
       << pfx << std::setw(24) << "ERR_RAND:"
       <<theErrRand  << "\n" 
       << pfx << std::setw(24) << "LINE_OFF:"
       << theLineOffset << "\n"
       << pfx << std::setw(24) << "SAMP_OFF:"
       << theSampleOffset << "\n" 
       << pfx << std::setw(24) << "LAT_OFF:"
       << theGeodeticLatOffset << "\n"
       << pfx << std::setw(24) << "LONG_OFF:"
       << theGeodeticLonOffset << "\n" 
       << pfx << std::setw(24) << "HEIGHT_OFF:"
       << theGeodeticHeightOffset << "\n"
       << pfx << std::setw(24) << "LINE_SCALE:"
       << theLineScale  << "\n" 
       << pfx << std::setw(24)
       << "SAMP_SCALE:" << theSampleScale << "\n"
       << pfx << std::setw(24) << "LAT_SCALE:"
       << theGeodeticLatScale << "\n" 
       << pfx << std::setw(24) << "LONG_SCALE:"
       << theGeodeticLonScale << "\n"
       << pfx << std::setw(24) << "HEIGHT_SCALE:"
       << theGeodeticHeightScale << "\n";

   ossim_int32 i;
   ossimString s;
   
   for (i=0; i<LINE_NUMERATOR_COEFFICIENT_COUNT; ++i)
   {
      s = "LINE_NUM_COEFF_";
      s += ossimString::toString(i);
      s += ":";
      out << pfx << std::setw(24) << s
          << theLineNumeratorCoefficient[i] << "\n";
   }

   for (i=0; i<LINE_DENOMINATOR_COEFFICIENT_COUNT; ++i)
   {
      s = "LINE_DEN_COEFF_";
      s += ossimString::toString(i);
      s += ":";
      out << pfx << std::setw(24) << s
          << theLineDenominatorCoefficient[i] << "\n";
   }

   for (i=0; i<LINE_NUMERATOR_COEFFICIENT_COUNT; ++i)
   {
      s = "SAMP_NUM_COEFF_";
      s += ossimString::toString(i);
      s += ":";
      out << pfx << std::setw(24) << s
          << theSampleNumeratorCoefficient[i] << "\n";
   }

   for (i=0; i<LINE_DENOMINATOR_COEFFICIENT_COUNT; ++i)
   {
      s = "SAMP_DEN_COEFF_";
      s += ossimString::toString(i);
      s += ":";
      out << pfx << std::setw(24) << s
          << theSampleDenominatorCoefficient[i] << "\n";
   }

   out.flush();
   
   return out;
}

void ossimNitfRpcBase::setProperty(ossimRefPtr<ossimProperty> /* property */)
{
   
}

ossimRefPtr<ossimProperty> ossimNitfRpcBase::getProperty(const ossimString& name)const
{
   ossimProperty* result = 0;
   if(name == ERR_BIAS_KW)
   {
      result = new ossimStringProperty(name, theErrorBias);
   }
   else if(name == ERR_RAND_KW)
   {
      result = new ossimStringProperty(name, theErrRand);
   }
   else if(name == LINE_OFF_KW)
   {
      result = new ossimStringProperty(name, theLineOffset);
   }
   else if(name == SAMP_OFF_KW)
   {
      result = new ossimStringProperty(name, theSampleOffset);
   }
   else if(name == LAT_OFF_KW)
   {
      result = new ossimStringProperty(name, theGeodeticLatOffset);
   }
   else if(name == LONG_OFF_KW)
   {
      result = new ossimStringProperty(name, theGeodeticLonOffset);
   }
   else if(name == HEIGHT_OFF_KW)
   {
      result = new ossimStringProperty(name, theGeodeticHeightOffset);
   }
   else if(name == LINE_SCALE_KW)
   {
      result = new ossimStringProperty(name, theLineScale);
   }
   else if(name == SAMP_SCALE_KW)
   {
      result = new ossimStringProperty(name,theSampleScale );
   }
   else if(name == LAT_SCALE_KW)
   {
      result = new ossimStringProperty(name, theGeodeticLatScale);
   }
   else if(name == LONG_SCALE_KW)
   {
      result = new ossimStringProperty(name, theGeodeticLonScale);
   }
   else if(name == HEIGHT_SCALE_KW)
   {
      result = new ossimStringProperty(name, theGeodeticHeightScale);
   }
   else if(name.contains(LINE_NUM_COEFF_KW))
   {
      ossim_uint32 idx = 0;

         idx = ossimString(name.begin()+LINE_NUM_COEFF_KW.length(),
                           name.end()).toUInt32();
      --idx;

      if(idx < theLineNumeratorCoefficient.size())
      {
         result = new ossimStringProperty(name, theLineNumeratorCoefficient[idx]);
      }
   }
   else if(name.contains(LINE_DEN_COEFF_KW))
   {
      ossim_uint32 idx = 0;

      idx = ossimString(name.begin()+LINE_DEN_COEFF_KW.length(),
                        name.end()).toUInt32();
      --idx;

      if(idx < theLineDenominatorCoefficient.size())
      {
         result = new ossimStringProperty(name, theLineDenominatorCoefficient[idx]);
      }
   }
   else if(name.contains(SAMP_NUM_COEFF_KW))
   {
      ossim_uint32 idx = 0;

      idx = ossimString(name.begin()+SAMP_NUM_COEFF_KW.length(),
                        name.end()).toUInt32(); 
      --idx;

      if(idx < theSampleNumeratorCoefficient.size())
      {
         result = new ossimStringProperty(name, theSampleNumeratorCoefficient[idx]);
      }
   }
   else if(name.contains(SAMP_DEN_COEFF_KW))
   {
      ossim_uint32 idx = 0;

      idx = ossimString(name.begin()+SAMP_DEN_COEFF_KW.length(),
                        name.end()).toUInt32();
      --idx;

      if(idx < theSampleDenominatorCoefficient.size())
      {
         result = new ossimStringProperty(name, theSampleDenominatorCoefficient[idx]);
      }
   }

   return result;
}

void ossimNitfRpcBase::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossim_uint32 idx = 0;
   
   propertyNames.push_back(ERR_BIAS_KW);
   propertyNames.push_back(ERR_RAND_KW);
   propertyNames.push_back(LINE_OFF_KW);
   propertyNames.push_back(SAMP_OFF_KW);
   propertyNames.push_back(LAT_OFF_KW);
   propertyNames.push_back(LONG_OFF_KW);
   propertyNames.push_back(HEIGHT_OFF_KW);
   propertyNames.push_back(LINE_SCALE_KW);
   propertyNames.push_back(SAMP_SCALE_KW);
   propertyNames.push_back(LAT_SCALE_KW);
   propertyNames.push_back(LONG_SCALE_KW);
   propertyNames.push_back(HEIGHT_SCALE_KW);

   for(idx = 1; idx <= 20; ++idx)
   {
      propertyNames.push_back(LINE_NUM_COEFF_KW+ossimString::toString(idx));
   }
   for(idx = 1; idx <= 20; ++idx)
   {
      propertyNames.push_back(LINE_DEN_COEFF_KW+ossimString::toString(idx));
   }
   for(idx = 1; idx <= 20; ++idx)
   {
      propertyNames.push_back(SAMP_NUM_COEFF_KW+ossimString::toString(idx));
   }
   for(idx = 1; idx <= 20; ++idx)
   {
      propertyNames.push_back(SAMP_DEN_COEFF_KW+ossimString::toString(idx));
   }
}

