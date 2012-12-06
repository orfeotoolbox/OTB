//*******************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: David Burken
//
// Description: Common file for global functions.
//
//*************************************************************************
// $Id: ossimCommon.cpp 20610 2012-02-27 12:19:25Z gpotts $

#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimPreferences.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimDpt3d.h>
#include <ossim/matrix/newmat.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimGpt.h>
#include <OpenThreads/Thread>
#include <sstream>

static ossimTrace traceDebug("ossimCommon:debug");

// stores a floating point nan value
const ossim::IntFloatBitCoercion ossim::nanValue(~ossim_int64(0));

std::istream& ossim::skipws(std::istream& in)
{
   int c = in.peek();
   while( !in.bad() && ossim::isWhiteSpace(c))
   {
      in.ignore();
      c = in.peek();
   }
   
   return in;
}
bool ossim::isWhiteSpace(int c)
{
   return ( (c == ' ') || (c == '\t') || (c == '\n')|| (c == '\r') ) ;
}

ossimByteOrder ossim::byteOrder()
{
   union
   {
      short s;
      char  c[sizeof(short)];
   } un;

   un.s = 0x0102;
   if (un.c[0] ==  2 && un.c[1] == 1)
   {
      return OSSIM_LITTLE_ENDIAN;
   }
   else
   {
      return OSSIM_BIG_ENDIAN;
   }
}

double ossim::defaultMin(ossimScalarType scalarType)
{
   switch(scalarType)
   {
      case OSSIM_UINT8:
      {
	return OSSIM_DEFAULT_MIN_PIX_UINT8;
      }
      case OSSIM_SINT8:
      {
	return OSSIM_DEFAULT_MIN_PIX_SINT8;
      }
      case OSSIM_UINT16:
      {
         return OSSIM_DEFAULT_MIN_PIX_UINT16;
      }
      case OSSIM_SINT16:
      {
         return OSSIM_DEFAULT_MIN_PIX_SINT16;
      }
      case OSSIM_USHORT11:
      {
         return OSSIM_DEFAULT_MIN_PIX_UINT11;
      }
      case OSSIM_UINT32:
      {
         return OSSIM_DEFAULT_MIN_PIX_UINT32;
      }
      case OSSIM_SINT32:
      {
         return OSSIM_DEFAULT_MIN_PIX_SINT32;
      }
      case OSSIM_FLOAT32:
      {
         return OSSIM_DEFAULT_MIN_PIX_FLOAT;
      }
      case OSSIM_NORMALIZED_FLOAT:
      {
         return OSSIM_DEFAULT_MIN_PIX_NORM_FLOAT;
      }
      case OSSIM_FLOAT64:
      {
         return OSSIM_DEFAULT_MIN_PIX_DOUBLE;
      }
      case OSSIM_NORMALIZED_DOUBLE:
      {
         return OSSIM_DEFAULT_MIN_PIX_NORM_DOUBLE;
      }
      case OSSIM_SCALAR_UNKNOWN:
      default:
      {
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << __FILE__ << ":" << __LINE__
               << "\nUnhandled scalar type:  " << scalarType << std::endl;
         }
         break;
      }
   }
   return 0.0; // Should never happen...
}

double ossim::defaultMax(ossimScalarType scalarType)
{
   switch(scalarType)
   {
      case OSSIM_UINT8:
      {
         return OSSIM_DEFAULT_MAX_PIX_UINT8;
      }
      case OSSIM_SINT8:
      {
         return OSSIM_DEFAULT_MAX_PIX_SINT8;
      }
      case OSSIM_UINT16:
      {
	return OSSIM_DEFAULT_MAX_PIX_UINT16;
      }
      case OSSIM_SINT16:
      {
         return OSSIM_DEFAULT_MAX_PIX_SINT16;
      }
      case OSSIM_USHORT11:
      {
         return OSSIM_DEFAULT_MAX_PIX_UINT11;
      }
      case OSSIM_UINT32:
      {
	return OSSIM_DEFAULT_MAX_PIX_UINT32;
      }
      case OSSIM_SINT32:
      {
         return OSSIM_DEFAULT_MAX_PIX_SINT32;
      }
      case OSSIM_FLOAT32:
      {
         return OSSIM_DEFAULT_MAX_PIX_FLOAT;
      }
      case OSSIM_NORMALIZED_FLOAT:
      {
         return OSSIM_DEFAULT_MAX_PIX_NORM_FLOAT;
      }
      case OSSIM_FLOAT64:
      {
         return OSSIM_DEFAULT_MAX_PIX_DOUBLE;
      }
      case OSSIM_NORMALIZED_DOUBLE:
      {
         return OSSIM_DEFAULT_MAX_PIX_NORM_DOUBLE;
      }
      case OSSIM_SCALAR_UNKNOWN:
      default:
      {
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << __FILE__ << ":" << __LINE__
               << "\nUnhandled scalar type:  " << scalarType << std::endl;
         }
         break;
      }
   }
   return 0.0; // Should never happen...
}

double ossim::defaultNull(ossimScalarType scalarType)
{
   switch(scalarType)
   {
      case OSSIM_UINT8:
      {
         return OSSIM_DEFAULT_NULL_PIX_UINT8;
      }
      case OSSIM_SINT8:
      {
         return OSSIM_DEFAULT_NULL_PIX_SINT8;
      }
      case OSSIM_UINT16:
      {
         return OSSIM_DEFAULT_NULL_PIX_UINT16;
      }
      case OSSIM_SINT16:
      {
         return OSSIM_DEFAULT_NULL_PIX_SINT16;
      }
      case OSSIM_USHORT11:
      {
         return OSSIM_DEFAULT_NULL_PIX_UINT11;
      }
      case OSSIM_UINT32:
      {
         return OSSIM_DEFAULT_NULL_PIX_UINT32;
      }
      case OSSIM_SINT32:
      {
         return OSSIM_DEFAULT_NULL_PIX_SINT32;
      }
      case OSSIM_FLOAT32:
      {
         return OSSIM_DEFAULT_NULL_PIX_FLOAT;
      }
      case OSSIM_NORMALIZED_FLOAT:
      {
         return OSSIM_DEFAULT_NULL_PIX_NORM_FLOAT;
      }
      case OSSIM_FLOAT64:
      {
         return OSSIM_DEFAULT_NULL_PIX_DOUBLE;
      }
      case OSSIM_NORMALIZED_DOUBLE:
      {
         return OSSIM_DEFAULT_NULL_PIX_NORM_DOUBLE;
      }
      case OSSIM_SCALAR_UNKNOWN:
      default:
      {
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << __FILE__ << ":" << __LINE__
               << "\nUnhandled scalar type:  " << scalarType << std::endl;
         }
         break;
      }
    }

   return 0.0; // Should never happen...
}

ossim_uint32 ossim::scalarSizeInBytes(ossimScalarType scalarType)
{
   switch(scalarType)
   {
      case OSSIM_UINT8:
      {
         return sizeof(ossim_uint8);
      }
      case OSSIM_SINT8:
      {
         return sizeof(ossim_sint8);
      }
      case OSSIM_UINT16:
      case OSSIM_USHORT11:
      {
         return sizeof(ossim_uint16);
      }
      case OSSIM_SINT16:
      {
         return sizeof(ossim_sint16);
      }
      case OSSIM_UINT32:
      {
         return sizeof(ossim_uint32);
      }
      case OSSIM_SINT32:
      {
         return sizeof(ossim_sint32);
      }
      case OSSIM_FLOAT32:
      case OSSIM_NORMALIZED_FLOAT:
      {
         return sizeof(ossim_float32);
      }
      case OSSIM_FLOAT64:
      case OSSIM_NORMALIZED_DOUBLE:
      {
         return sizeof(ossim_float64);
      }
      case OSSIM_SCALAR_UNKNOWN:
      default:
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << __FILE__ << ":" << __LINE__
               << "\nUnhandled scalar type:  " << scalarType << std::endl;
         }
         break;
      }
  }
  
  return 1;
}

bool ossim::isSigned(ossimScalarType scalarType)
{
   bool result = false;
   switch(scalarType)
   {
      case OSSIM_SINT8:
      case OSSIM_SINT16:
      case OSSIM_SINT32:
      case OSSIM_FLOAT32:
      case OSSIM_FLOAT64:
      {
         result = true;
         break;
      }
      default:
      {
         break;
      }
  }
  return result;
}

ossim_uint32 ossim::getActualBitsPerPixel(ossimScalarType scalarType)
{
   ossim_uint32 actualBitsPerPixel = 0;
   switch(scalarType)
   {
      case OSSIM_UINT8:
      case OSSIM_SINT8:
      {
         actualBitsPerPixel = 8;
         break;
      }
      case OSSIM_USHORT11:
      {
         actualBitsPerPixel = 11;
         break;
      }
      case OSSIM_UINT16:
      case OSSIM_SINT16:
      {
         actualBitsPerPixel = 16;
         break;
      }
      case OSSIM_UINT32:
      case OSSIM_SINT32:
      case OSSIM_FLOAT32:
      case OSSIM_CINT16:
      case OSSIM_NORMALIZED_FLOAT:
      {
         actualBitsPerPixel = 32;
         break;
      }
      case OSSIM_FLOAT64:
      case OSSIM_CFLOAT32:
      case OSSIM_CINT32:
      case OSSIM_NORMALIZED_DOUBLE:
      {
         actualBitsPerPixel = 64;
         break;
      }
      case OSSIM_CFLOAT64:
      {
         actualBitsPerPixel = 128;
         
         break;
      }
      default:
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << __FILE__ << ":" << __LINE__
               << "\nUnhandled scalar type:  " << scalarType << std::endl;
         }
         break;
      }
   }
   return actualBitsPerPixel;
}

ossim_uint32 ossim::getBitsPerPixel(ossimScalarType scalarType)
{
   ossim_uint32 bitsPerPixel = 0;
   switch(scalarType)
   {
      case OSSIM_UINT8:
      case OSSIM_SINT8:
      {
         bitsPerPixel = 8;
         break;
      }
      case OSSIM_USHORT11:
      {
         bitsPerPixel = 16;
         break;
      }
      case OSSIM_UINT16:
      case OSSIM_SINT16:
      {
         bitsPerPixel = 16;
         break;
      }
      case OSSIM_UINT32:
      case OSSIM_SINT32:
      case OSSIM_FLOAT32:
      case OSSIM_NORMALIZED_FLOAT:
      {
         bitsPerPixel = 32;
         break;
      }
      case OSSIM_FLOAT64:
      case OSSIM_NORMALIZED_DOUBLE:
      {
         bitsPerPixel = 64;
         break;
      }
      default:
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << __FILE__ << ":" << __LINE__
               << "\nUnhandled scalar type:  " << scalarType << std::endl;
         }
         break;
      }
   }
   return bitsPerPixel;
}

void ossim::defaultTileSize(ossimIpt& tileSize)
{
   const char* tileSizeKw = ossimPreferences::instance()->
      findPreference("tile_size");

   if(tileSizeKw)
   {
      std::vector<ossimString> splitArray;
      ossimString tempString(tileSizeKw);
      tempString.split(splitArray, " ");
      bool hasX = true;
    if(splitArray.size() == 2)
      {
         tileSize.x = splitArray[0].toInt32();
         tileSize.y = splitArray[1].toInt32();
      }
      else if(splitArray.size() == 1)
      {
         tileSize.x = splitArray[0].toInt32();
         tileSize.y = splitArray[0].toInt32();
      }
      else
      {
         tileSize = ossimIpt(0,0);
      }
      if(tileSize.x < 1)
      {
         tileSize.x = OSSIM_DEFAULT_TILE_WIDTH;
         hasX = false;
      }
      if(tileSize.y < 1)
      {
         if(!hasX)
         {
            tileSize.y = OSSIM_DEFAULT_TILE_HEIGHT;
         }
         else
         {
            tileSize.y = tileSize.x;
         }
      }
   }
   else
   {
      tileSize.x = OSSIM_DEFAULT_TILE_WIDTH;
      tileSize.y = OSSIM_DEFAULT_TILE_HEIGHT;
   }
}

std::string ossim::convertHtmlSpecialCharactersToNormalCharacter(const std::string& src)
{
   ossimString result = src;
   std::string::size_type pos = 0;
   pos = result.find("&");
   
   while(pos != std::string::npos)
   {
      std::string::size_type size = result.size();
      std::string test1(&result[pos], ossim::min(6, (int)(size-pos)));
      std::string test2(&result[pos], ossim::min(5, (int)(size-pos)));
      std::string test3(&result[pos], ossim::min(4, (int)(size-pos)));
      
      if(test1 == "&apos;")
      {
         result = result.substitute(test1, "'");
      }
      else if(test1 == "&quot;")
      {
         result = result.substitute(test1, "\"");
      }
      else if(test2 == "&amp;")
      {
         result = result.substitute(test2, "&");
      }
      else if(test3 == "&gt;")
      {
         result = result.substitute(test3, ">");
      }
      else if(test3 == "&lt;")
      {
         result = result.substitute(test3, "<");
      }
      pos = result.find("&", pos+1);
   }
   
   return result;
}

bool ossim::matrixToHpr( ossim_float64 hpr[3], const NEWMAT::Matrix& rotation )
{
    //implementation converted from plib's sg.cxx
    //PLIB - A Suite of Portable Game Libraries
    //Copyright (C) 1998,2002  Steve Baker
    //For further information visit http://plib.sourceforge.net
    
   NEWMAT::Matrix mat(rotation);
    
   ossimDpt3d col1(rotation[0][0], rotation[1][0], rotation[2][0]);
   double s = col1.length();
   
   hpr[0] = 0.0;
   hpr[1] = 0.0;
   hpr[2] = 0.0;

   if ( s <= 0.00001 )
   {
      return true;
   }
   
   
   double oneOverS = 1.0f / s;
   for( int i = 0; i < 3; i++ )
      for( int j = 0; j < 3; j++ )
         mat[i][j] = rotation[j][i] * oneOverS;
   
   
   hpr[1] = ossim::asind(ossim::clamp(mat[1][2], -1.0, 1.0));
   
   double cp = ossim::cosd(hpr[1]);
   
   if ( cp > -0.00001 && cp < 0.00001 )
   {
      double cr = ossim::clamp(mat[0][1], -1.0, 1.0);
      double sr = ossim::clamp(-mat[2][1], -1.0, 1.0);
      
      hpr[0] = 0.0f;
      hpr[2] = ossim::atan2d(sr,cr);
   }
   else
   {
      cp = 1.0 / cp;
      double sr = ossim::clamp((-mat[0][2] * cp), -1.0,1.0);
      double cr = ossim::clamp((mat[2][2] * cp), -1.0, 1.0);
      double sh = ossim::clamp((-mat[1][0] * cp), -1.0, 1.0);
      double ch = ossim::clamp((mat[1][1] * cp), -1.0, 1.0);
      
      if ( (sh == 0.0f && ch == 0.0f) || (sr == 0.0f && cr == 0.0f) )
      {
         cr = ossim::clamp(mat[0][1], -1.0, 1.0);
         sr = ossim::clamp(-mat[2][1], -1.0, 1.0);
         
         hpr[0] = 0.0f;
      }
      else
      {
         hpr[0] = ossim::atan2d(sh, ch);
      }
      
      hpr[2] = ossim::atan2d(sr, cr);
   }
   
   hpr[0] *= -1.0;
   return true;
}

bool ossim::matrixToHpr( ossim_float64 hpr[3],
                  const NEWMAT::Matrix& lsrMatrix,
                  const NEWMAT::Matrix& rotationalMatrix)
{
    bool result = false;
    NEWMAT::Matrix invertLsr(lsrMatrix.i());
   
    hpr[0] = 0.0;
    hpr[1] = 0.0;
    hpr[2] = 0.0;
    result = matrixToHpr(hpr, invertLsr*rotationalMatrix);
    if(std::abs(hpr[0]) < FLT_EPSILON)
    {
       hpr[0] = 0.0;
    }
    if(std::abs(hpr[1]) < FLT_EPSILON)
    {
       hpr[1] = 0.0;
    }
    if(std::abs(hpr[2]) < FLT_EPSILON)
    {
       hpr[2] = 0.0;
    }
    
    return result;
}


void ossim::lexQuotedTokens(const std::string& str,
                            ossim_uint32 start,
                            const char* whitespace,
                            const char* quotes,
                            std::vector<std::string>& tokens,
                            bool& unbalancedQuotes)
{
   ossimREQUIRE(whitespace != NULL);
   ossimREQUIRE(quotes != NULL);
   ossimREQUIRE(tokens != NULL);
   
   const char openQuote(quotes[0]), closeQuote(quotes[1]);
   
   tokens.clear();
   unbalancedQuotes = false;
   
   int end=0;
   while (start < str.length())
   {
      if (str[start] == openQuote)
      {
         int openBraceCount = 1;
         
         if (start+1 < str.length())
         {
            start++;
            if (str[start] != closeQuote)
            {
               //               end = start+1;
               end = start;
               while (static_cast<ossim_uint32>(end) < str.length() &&
                      openBraceCount > 0)
               {
                  if (str[end] == openQuote)
                     openBraceCount++;
                  else if (str[end] == closeQuote)
                     openBraceCount--;
                  end++;
               }
            }
            else
            {
               openBraceCount = 0;
               start++;
               end = start+1;
            }
         }
         if (openBraceCount == 0)
         {
            tokens.push_back(str.substr(start, end-1-start));
         }
         else
         {
            unbalancedQuotes = true;
            end = (int)str.length();
         }
      }
      else if (str[start] == closeQuote)
      {
         unbalancedQuotes = true;
         end = (int)str.length();
	 
      }
      else
      {
         end = (int)str.find_first_of(whitespace, start);
         tokens.push_back(str.substr(start, end-start));
      }
      
      start = (ossim_uint32)str.find_first_not_of(whitespace, end);
   }
}

void ossim::toStringList(ossimString& resultStringOfPoints,
                         const std::vector<ossimDpt>& pointList, 
                         char separator)
{
   ossim_uint32 idx = 0;
   for(;idx < pointList.size();++idx)
   {
      ossimString pt = pointList[idx].toString();
      if(resultStringOfPoints.empty())
      {
         resultStringOfPoints = pt;
      }
      else
      {
         resultStringOfPoints += (separator + pt);
      }
   }
}

void ossim::toStringList(ossimString& resultStringOfPoints,
                         const std::vector<ossimIpt>& pointList, 
                         char separator)
{
   ossim_uint32 idx = 0;
   for(;idx < pointList.size();++idx)
   {
      ossimString pt = pointList[idx].toString();
      if(resultStringOfPoints.empty())
      {
         resultStringOfPoints = pt;
      }
      else
      {
         resultStringOfPoints += (separator + pt);
      }
   }
}

void ossim::toStringList(ossimString& resultStringOfPoints,
                         const std::vector<ossimGpt>& pointList, 
                         char separator)
{
   ossim_uint32 idx = 0;
   for(;idx < pointList.size();++idx)
   {
      ossimString pt = pointList[idx].toString();
      if(resultStringOfPoints.empty())
      {
         resultStringOfPoints = pt;
      }
      else
      {
         resultStringOfPoints += (separator + pt);
      }
   }
}

void ossim::toVector(std::vector<ossimDpt>& result,
                     const ossimString& stringOfPoints)
{
   std::vector<ossimString> splitResult;
  
   // let grab each point string that is surrounded by ()
   std::istringstream in(stringOfPoints.trim());
   ossimString currentPoint = "";
   ossimDpt tempPoint;
   while(!in.bad()&&!in.eof())
   {
      skipws(in);
      if(in.peek() == '(')
      {
         currentPoint += (char)in.get();
         skipws(in);
         if(in.peek() == '(') in.ignore();
         
         while(!in.bad()&&!in.eof()&&(in.peek() != ')'))
         {
            currentPoint += (char)in.get();
         }
         if(in.good())
         {
            currentPoint += (char)in.get();
            tempPoint.toPoint(currentPoint);
            result.push_back(tempPoint);
         }
         currentPoint = "";
      }
      else 
      {
         in.ignore();
      }
   }   
}

void ossim::toVector(std::vector<ossimIpt>& result,
                     const ossimString& stringOfPoints)
{
   std::vector<ossimString> splitResult;
   
   // let grab each point string that is surrounded by ()
   std::istringstream in(stringOfPoints.trim());
   ossimString currentPoint = "";
   ossimIpt tempPoint;
   while(!in.bad()&&!in.eof())
   {
      skipws(in);
      if(in.peek() == '(')
      {
         currentPoint += (char)in.get();
         skipws(in);
         if(in.peek() == '(') in.ignore();
         
         while(!in.bad()&&!in.eof()&&(in.peek() != ')'))
         {
            currentPoint += (char)in.get();
         }
         if(in.good())
         {
            currentPoint += (char)in.get();
            tempPoint.toPoint(currentPoint);
            result.push_back(tempPoint);
         }
         currentPoint = "";
      }
      else 
      {
         in.ignore();
      }
   }   
}
void ossim::toVector(std::vector<ossimGpt>& result,
                     const ossimString& stringOfPoints)
{
   std::vector<ossimString> splitResult;
   
   // let grab each point string that is surrounded by ()
   std::istringstream in(stringOfPoints.trim());
   ossimString currentPoint = "";
   ossimGpt tempPoint;
   while(!in.bad()&&!in.eof())
   {
      skipws(in);
      if(in.peek() == '(')
      {
         currentPoint += (char)in.get();
         skipws(in);
         if(in.peek() == '(') in.ignore();
         
         while(!in.bad()&&!in.eof()&&(in.peek() != ')'))
         {
            currentPoint += (char)in.get();
         }
         if(in.good())
         {
            currentPoint += (char)in.get();
            tempPoint.toPoint(currentPoint);
            result.push_back(tempPoint);
         }
         currentPoint = "";
      }
      else 
      {
         in.ignore();
      }
   }   
}

bool ossim::extractSimpleValues(std::vector<ossimString>& values,
                                const ossimString& stringOfPoints)
{
   std::istringstream in(stringOfPoints);
   ossim::skipws(in);
   bool result = true;
   if(stringOfPoints.empty()) return result;
   char c = in.get();
   ossimString value = "";
   if(c == '(')
   {
      c = (char)in.get();
      while((c!=')')&&
            (c!= '\n')&&
            (in.good()))
      {
         if(c!= ',')
         {
            value += ossimString(c);
         }
         else
         {
            values.push_back(value);
            value = "";
         }
         c = in.get();
      }
   }
   if(c!= ')')
   {
      result = false;
   }
   else
   {
      if(!value.empty())
      {
         values.push_back(value);
      }
   }
   
   return result;
}

bool ossim::toSimpleVector(std::vector<ossim_uint32>& result,
                           const ossimString& stringOfPoints)
{
   std::vector<ossimString> extractedValues;
   bool resultFlag = extractSimpleValues(extractedValues, stringOfPoints);
   if(resultFlag)
   {
      ossim_uint32 idx = 0;
      ossim_uint32 size = (ossim_uint32) extractedValues.size();
      for(idx = 0; idx < size; ++idx)
      {
         result.push_back(extractedValues[idx].toUInt32());
      }
   }
   return resultFlag;
}

template <>
void ossim::toSimpleStringList(ossimString& result,
                               const std::vector<ossim_uint8>& valuesList)

{
   std::ostringstream out;
   
   if(!valuesList.empty())
   {
      ossim_uint32 idx = 0;
      ossim_uint32 size = (ossim_uint32)(valuesList.size()-1);
      for(idx = 0; idx < size; ++idx)
      {
         out << ((ossim_uint32)valuesList[idx]) << ",";
      }
      out << static_cast<ossim_uint32>(valuesList[size]);
   }
   
   result = "("+out.str()+")";
}

bool ossim::toSimpleVector(std::vector<ossim_int32>& result,
                           const ossimString& stringOfPoints)
{
   std::vector<ossimString> extractedValues;
   bool resultFlag = extractSimpleValues(extractedValues, stringOfPoints);
   if(resultFlag)
   {
      ossim_uint32 idx = 0;
      ossim_uint32 size = (ossim_int32) extractedValues.size();
      for(idx = 0; idx < size; ++idx)
      {
         result.push_back(extractedValues[idx].toInt32());
      }
   }
   return resultFlag;
}

bool ossim::toSimpleVector(std::vector<ossim_uint16>& result,
                           const ossimString& stringOfPoints)
{
   std::vector<ossimString> extractedValues;
   bool resultFlag = extractSimpleValues(extractedValues, stringOfPoints);
   if(resultFlag)
   {
      ossim_uint32 idx = 0;
      ossim_uint32 size = (ossim_int32) extractedValues.size();
      for(idx = 0; idx < size; ++idx)
      {
         result.push_back(extractedValues[idx].toUInt32());
      }
   }
   return resultFlag;
}

bool ossim::toSimpleVector(std::vector<ossim_int16>& result,
                           const ossimString& stringOfPoints)
{
   std::vector<ossimString> extractedValues;
   bool resultFlag = extractSimpleValues(extractedValues, stringOfPoints);
   if(resultFlag)
   {
      ossim_uint32 idx = 0;
      ossim_uint32 size = (ossim_uint32) extractedValues.size();
      for(idx = 0; idx < size; ++idx)
      {
         result.push_back(extractedValues[idx].toInt32());
      }
   }
   return resultFlag;
}

bool ossim::toSimpleVector(std::vector<ossim_uint8>& result,
                    const ossimString& stringOfPoints)
{
   std::vector<ossimString> extractedValues;
   bool resultFlag = extractSimpleValues(extractedValues, stringOfPoints);
   if(resultFlag)
   {
      ossim_uint32 idx = 0;
      ossim_uint32 size = (ossim_uint32) extractedValues.size();
      for(idx = 0; idx < size; ++idx)
      {
         result.push_back(extractedValues[idx].toUInt8());
      }
   }
   return resultFlag;
}

bool ossim::toSimpleVector(std::vector<ossim_int8>& result,
                    const ossimString& stringOfPoints)
{
   std::vector<ossimString> extractedValues;
   bool resultFlag = extractSimpleValues(extractedValues, stringOfPoints);
   if(resultFlag)
   {
      ossim_uint32 idx = 0;
      ossim_uint32 size = (ossim_uint32) extractedValues.size();
      for(idx = 0; idx < size; ++idx)
      {
         result.push_back(extractedValues[idx].toUInt8());
      }
   }
   return resultFlag;
}

ossim_uint32 ossim::getNumberOfThreads()
{
   ossim_uint32 result;
   const char* str = ossimPreferences::instance()->findPreference("ossim_threads");
   if ( str )
   {
      result = ossimString(str).toUInt32();
   }
   else
   {
      result = static_cast<ossim_uint32>( OpenThreads::GetNumberOfProcessors() );
   }
   if ( !result )
   {
      result = 1;
   }
   return result;
}
