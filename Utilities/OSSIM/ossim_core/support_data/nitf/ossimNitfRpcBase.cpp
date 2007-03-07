//*******************************************************************
// Copyright (C) 2000 Intelligence Data Systems. 
//
// LICENSE: LGPL
//
// see top level LICENSE.txt
// 
// Author: Garrett Potts
//
// Description: Nitf support class for RPC00A -
// Rational Polynomial Coefficient extension.
//
//********************************************************************
// $Id: ossimNitfRpcBase.cpp,v 1.6 2005/10/12 10:47:03 dburken Exp $

#include <support_data/nitf/ossimNitfRpcBase.h>
#include <support_data/nitf/ossimNitfCommon.h>
#include <base/context/ossimNotifyContext.h>
#include <base/common/ossimTrace.h>

static const ossimTrace traceDebug(ossimString("ossimNitfRpcBase:debug"));

RTTI_DEF1(ossimNitfRpcBase, "ossimNitfRpcBase", ossimNitfRegisteredTag);

#ifndef MIN
#  define MIN(a,b) ((a)<(b)?(a):(b))
#endif

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
   temp1[LINE_NUMERATOR_COEFFICIENT_SIZE+1] = '\0';
   for(idx = 0; idx < LINE_NUMERATOR_COEFFICIENT_COUNT; ++idx)
   {
      in.read(temp1, LINE_NUMERATOR_COEFFICIENT_SIZE);
      theLineNumeratorCoefficient[idx] = temp1;
   }
   
   char temp2[LINE_DENOMINATOR_COEFFICIENT_SIZE+1];
   temp2[LINE_DENOMINATOR_COEFFICIENT_SIZE+1] = '\0';
   for(idx = 0; idx < LINE_DENOMINATOR_COEFFICIENT_COUNT; ++idx)
   {
      in.read(temp2, LINE_DENOMINATOR_COEFFICIENT_SIZE);
      theLineDenominatorCoefficient[idx] = temp2;
   }
   
   char temp3[SAMPLE_NUMERATOR_COEFFICIENT_SIZE+1];
   temp3[SAMPLE_NUMERATOR_COEFFICIENT_SIZE+1] = '\0';
   for(idx = 0; idx < SAMPLE_NUMERATOR_COEFFICIENT_COUNT; ++idx)
   {
      in.read(temp3, SAMPLE_NUMERATOR_COEFFICIENT_SIZE);
      theSampleNumeratorCoefficient[idx] = temp3;
   }
   
   char temp4[SAMPLE_DENOMINATOR_COEFFICIENT_SIZE+1];
   temp4[SAMPLE_DENOMINATOR_COEFFICIENT_SIZE+1] = '\0';
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
   
   std::fill(theLineNumeratorCoefficient.begin(),
             theLineNumeratorCoefficient.end(),
             '0');
   std::fill(theLineDenominatorCoefficient.begin(),
             theLineDenominatorCoefficient.end(),
             '0');
   std::fill(theSampleNumeratorCoefficient.begin(),
             theSampleNumeratorCoefficient.end(),
             '0');
   std::fill(theSampleDenominatorCoefficient.begin(),
             theSampleDenominatorCoefficient.end(),
             '0');
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
   memcpy(theErrorBias, errorBias.c_str(), MIN(ERROR_BIAS_SIZE, errorBias.length()));
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
   memcpy(theErrRand, errorRand.c_str(), MIN(ERR_RAND_SIZE, errorRand.length()));
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
   memcpy(theLineOffset, lineOffset.c_str(), MIN(LINE_OFFSET_SIZE, lineOffset.length()));
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
   memcpy(theSampleOffset, sampleOffset.c_str(), MIN(SAMPLE_OFFSET_SIZE, sampleOffset.length()));
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
   memcpy(theGeodeticLatOffset, geodeticLatOffset.c_str(), MIN(GEODETIC_LAT_OFFSET_SIZE, geodeticLatOffset.length()));
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
   memcpy(theGeodeticLonOffset, geodeticLonOffset.c_str(), MIN(GEODETIC_LON_OFFSET_SIZE, geodeticLonOffset.length()));
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
   memcpy(theGeodeticHeightOffset, geodeticHeightOffset.c_str(), MIN(GEODETIC_HEIGHT_OFFSET_SIZE, geodeticHeightOffset.length()));
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
   memcpy(theLineScale, lineScale.c_str(), MIN(LINE_SCALE_SIZE, lineScale.length()));
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
   memcpy(theSampleScale, sampleScale.c_str(), MIN(SAMPLE_SCALE_SIZE, sampleScale.length()));
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
   memcpy(theGeodeticLatScale, geodeticLatScale.c_str(), MIN(GEODETIC_LAT_SCALE_SIZE, geodeticLatScale.length()));
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
   memcpy(theGeodeticLonScale, geodeticLonScale.c_str(), MIN(GEODETIC_LON_SCALE_SIZE, geodeticLonScale.length()));
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
   memcpy(theGeodeticHeightScale, geodeticHeightScale.c_str(), MIN(GEODETIC_HEIGHT_SCALE_SIZE, geodeticHeightScale.length()));
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
            << endl;
      }
      return ossimString();
   }
   
   return theLineNumeratorCoefficient[idx];
}

void ossimNitfRpcBase::setLineNumeratorCoeff(
   const vector<ossim_float64>& coeff)
{
   if (coeff.size() != LINE_NUMERATOR_COEFFICIENT_COUNT)
   {
      // Error...
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimNitfRpcBase::setLineNumeratorCoeff WARNING:"
            << "\nArray not of correct size!"
            << endl;
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
         << endl;
      return;
   } 

   string s(coeff, 0, LINE_NUMERATOR_COEFFICIENT_SIZE);
   theLineNumeratorCoefficient[idx] = s;  
}

ossimString ossimNitfRpcBase::getLineDenominatorCoeff(ossim_uint32 idx)const
{
   // ASSERT_ARRAY_CONSTRAINT(idx >= 0 && idx < LINE_DENOMINATOR_COEFFICIENT_COUNT,idx,0,"ossimNitfRpcBase::getLineDenominatorCoeff");

   if (idx >= LINE_DENOMINATOR_COEFFICIENT_COUNT)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimNitfRpcBase::getLineDenominatorCoeff range error!"
         << endl;
      return ossimString();
   }  
   
   return theLineDenominatorCoefficient[idx];
}

void ossimNitfRpcBase::setLineDenominatorCoeff(
   const vector<ossim_float64>& coeff)
{
   if (coeff.size() != LINE_DENOMINATOR_COEFFICIENT_COUNT)
   {
      // Error...
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimNitfRpcBase::setLineDenominatorCoeff WARNING:"
            << "\nArray not of correct size!"
            << endl;
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
         << endl;
      return;
   } 
   
   string s(coeff, 0, LINE_DENOMINATOR_COEFFICIENT_SIZE);
   theLineDenominatorCoefficient[idx] = s;  
}

ossimString ossimNitfRpcBase::getSampleNumeratorCoeff(ossim_uint32 idx)const
{
   // ASSERT_ARRAY_CONSTRAINT(idx >= 0 && idx < SAMPLE_NUMERATOR_COEFFICIENT_COUNT,idx,0,"ossimNitfRpcBase::getSampleNumeratorCoeff");

   if (idx >= SAMPLE_NUMERATOR_COEFFICIENT_COUNT)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimNitfRpcBase::getSampleNumeratorCoeff range error!"
         << endl;
      return ossimString();
   } 
   return theSampleNumeratorCoefficient[idx];
}

void ossimNitfRpcBase::setSampleNumeratorCoeff(
   const vector<ossim_float64>& coeff)
{
   if (coeff.size() != SAMPLE_NUMERATOR_COEFFICIENT_COUNT)
   {
      // Error...
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimNitfRpcBase::setSampleNumeratorCoeff WARNING:"
            << "\nArray not of correct size!"
            << endl;
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
         << endl;
      return;
   } 
   
   string s(coeff, 0, SAMPLE_NUMERATOR_COEFFICIENT_SIZE);
   theSampleNumeratorCoefficient[idx] = s;  
}

ossimString ossimNitfRpcBase::getSampleDenominatorCoeff(ossim_uint32 idx)const
{
   // ASSERT_ARRAY_CONSTRAINT(idx >= 0 && idx < SAMPLE_DENOMINATOR_COEFFICIENT_COUNT,idx,0,"ossimNitfRpcBase::getSampleDenominatorCoeff");

   if (idx >= SAMPLE_DENOMINATOR_COEFFICIENT_COUNT)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimNitfRpcBase::getSampleDenominatorCoeff range error!"
         << endl;
      return ossimString();
   } 
   
   return theSampleDenominatorCoefficient[idx];
}

void ossimNitfRpcBase::setSampleDenominatorCoeff(
   const vector<ossim_float64>& coeff)
{
   if (coeff.size() != SAMPLE_DENOMINATOR_COEFFICIENT_COUNT)
   {
      // Error...
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimNitfRpcBase::setSampleDenominatorCoeff WARNING:"
            << "\nArray not of correct size!"
            << endl;
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
         << endl;
      return;
   }
   
  string s(coeff, 0, SAMPLE_DENOMINATOR_COEFFICIENT_SIZE);
  s.resize(SAMPLE_DENOMINATOR_COEFFICIENT_SIZE, ' ');
  theSampleDenominatorCoefficient[idx] = s;  
}

std::ostream& ossimNitfRpcBase::print(std::ostream& out) const
{
   out << "ossimNitfRpcBase::print"
       << "\ntheSuccess:               " << theSuccess
       << "\ntheErrorBias:             " << theErrorBias
       << "\ntheErrRand:               " << theErrRand
       << "\ntheLineOffset:            " << theLineOffset
       << "\ntheSampleOffset:          " << theSampleOffset
       << "\ntheGeodeticLatOffset:     " << theGeodeticLatOffset
       << "\ntheGeodeticLonOffset:     " << theGeodeticLonOffset
       << "\ntheGeodeticHeightOffset:  " << theGeodeticHeightOffset
       << "\ntheLineScale:             " <<theLineScale
       << "\ntheSampleScale:           " << theSampleScale
       << "\ntheGeodeticLatScale:      " << theGeodeticLatScale
       << "\ntheGeodeticLonScale:      " << theGeodeticLonScale
       << "\ntheGeodeticHeightScale:   " << theGeodeticHeightScale
       << std::endl;

   ossim_int32 i;
   
   for (i=0; i<LINE_NUMERATOR_COEFFICIENT_COUNT; ++i)
   {
      out << "\ntheLineNumeratorCoefficient[" << i << "]: "
          << theLineNumeratorCoefficient[i];
   }

   out << std::endl;
   
   for (i=0; i<LINE_DENOMINATOR_COEFFICIENT_COUNT; ++i)
   {
      out << "\ntheLineDenominatorCoefficient[" << i << "]: "
          << theLineDenominatorCoefficient[i];
   }

   out << std::endl;
   

   for (i=0; i<SAMPLE_NUMERATOR_COEFFICIENT_COUNT; ++i)
   {
      out << "\ntheSampleNumeratorCoefficient[" << i << "]: "
          << theSampleNumeratorCoefficient[i];
   }

   out << std::endl;
   

   for (i=0; i<SAMPLE_DENOMINATOR_COEFFICIENT_COUNT; ++i)
   {
      out << "\ntheSampleDenominatorCoefficient[" << i << "]: "
          << theSampleDenominatorCoefficient[i];
   }

   out << std::endl;
      
   return out;
}
