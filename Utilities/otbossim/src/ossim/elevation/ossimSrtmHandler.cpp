//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:
// 
// Shuttle Radar Topography Mission (SRTM) elevation source.
//
//----------------------------------------------------------------------------
// $Id: ossimSrtmHandler.cpp 19444 2011-04-25 18:20:59Z dburken $

#include <ossim/elevation/ossimSrtmHandler.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimStreamFactoryRegistry.h>

RTTI_DEF1(ossimSrtmHandler, "ossimSrtmHandler" , ossimElevCellHandler)

//***
// Define Trace flags for use within this file:
//***
#include <ossim/base/ossimTrace.h>
static ossimTrace traceDebug ("ossimSrtmElevSource:debug");

ossimSrtmHandler::ossimSrtmHandler()
   :
      ossimElevCellHandler(),
      m_streamOpen(false),
      m_numberOfLines(0),
      m_numberOfSamples(0),
      m_srtmRecordSizeInBytes(0),
      m_latSpacing(0.0),
      m_lonSpacing(0.0),
      m_nwCornerPost(),
      m_swapper(0)
{
}

ossimSrtmHandler::~ossimSrtmHandler()
{
   if (m_swapper)
   {
      delete m_swapper;
      m_swapper = 0;
   }
   close();
}

double ossimSrtmHandler::getHeightAboveMSL(const ossimGpt& gpt)
{
   if(!isOpen()) return ossim::nan();
   if(!m_memoryMap.empty())
   {
      switch(m_scalarType)
      {
         case OSSIM_SINT16:
         {
            return getHeightAboveMSLMemoryTemplate((ossim_sint16)0, gpt);
         }
         case OSSIM_FLOAT32:
         {
            return getHeightAboveMSLMemoryTemplate((ossim_float32)0, gpt);
         }
         default:
         {
            break;
         }
      }
   }
   else
   {
      switch(m_scalarType)
      {
         case OSSIM_SINT16:
         {
            return getHeightAboveMSLFileTemplate((ossim_sint16)0, gpt);
         }
         case OSSIM_FLOAT32:
         {
            return getHeightAboveMSLFileTemplate((ossim_float32)0, gpt);
         }
         default:
         {
            break;
         }
      }
   }

   return ossim::nan();
}

template <class T>
double ossimSrtmHandler::getHeightAboveMSLFileTemplate(T /* dummy */, const ossimGpt& gpt)
{
   // Establish the grid indexes:
   double xi = (gpt.lon - m_nwCornerPost.lon) / m_lonSpacing;
   double yi = (m_nwCornerPost.lat - gpt.lat) / m_latSpacing;

   int x0 = static_cast<int>(xi);
   int y0 = static_cast<int>(yi);

   if(x0 == (m_numberOfSamples-1))
   {
      --x0; // Move over one post.
   }
   
   // Check for top edge.
   //    if (gpt.lat == theGroundRect.ul().lat)
   if(y0 == (m_numberOfLines-1))
   {
      --y0; // Move down one post.
   }


   // Do some error checking.
   if ( xi < 0.0 || yi < 0.0 ||
        x0 > (m_numberOfSamples  - 2.0) ||
        y0 > (m_numberOfLines    - 2.0) )
   {
      return ossim::nan();
   }

   T p[4];
   
   double p00;
   double p01;
   double p10;
   double p11;

   //---
   // Grab the four points from the srtm cell needed.
   //---
   std::streampos offset = y0 * m_srtmRecordSizeInBytes + x0 * sizeof(T);

   {
      OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_fileStrMutex);

      // lets a seek cur
      // 
      // Put the file pointer at the start of the first elevation post.
      m_fileStr.seekg(offset, std::ios::beg);
      // Get the first post.
      m_fileStr.read((char*)p, sizeof(T));
      
      // Get the second post.
      m_fileStr.read((char*)(p+1), sizeof(T));
      
      // Move over to the next column.
      offset += m_srtmRecordSizeInBytes;
      
      m_fileStr.seekg(offset, std::ios::beg);
      
      // Get the third post.
      m_fileStr.read((char*)(p+2), sizeof(T));
      
      // Get the fourth post.
      m_fileStr.read((char*)(p+3), sizeof(T));
      
      if(m_fileStr.fail())
      {
         return ossim::nan();
      }
   }
   if (m_swapper)
   {
      m_swapper->swap((T*)p, 4);
   }

   p00 = p[0];
   p01 = p[1];
   p10 = p[2];
   p11 = p[3];
   
   // Perform bilinear interpolation:

   double xt0 = xi - x0;
   double yt0 = yi - y0;
   double xt1 = 1-xt0;
   double yt1 = 1-yt0;

   double w00 = xt1*yt1;
   double w01 = xt0*yt1;
   double w10 = xt1*yt0;
   double w11 = xt0*yt0;

   //***
   // Test for null posts and set the corresponding weights to 0:
   //***
   if (p00 == theNullHeightValue)
      w00 = 0.0;
   if (p01 == theNullHeightValue)
      w01 = 0.0;
   if (p10 == theNullHeightValue)
      w10 = 0.0;
   if (p11 == theNullHeightValue)
      w11 = 0.0;

#if 0 /* Serious debug only... */
   cout << "\np00:  " << p00
        << "\np01:  " << p01
        << "\np10:  " << p10
        << "\np11:  " << p11
        << "\nw00:  " << w00
        << "\nw01:  " << w01
        << "\nw10:  " << w10
        << "\nw11:  " << w11
        << endl;
#endif

   double sum_weights = w00 + w01 + w10 + w11;

   if (sum_weights)
   {
      return (p00*w00 + p01*w01 + p10*w10 + p11*w11) / sum_weights;
   }

   return ossim::nan();
}


template <class T>
double ossimSrtmHandler::getHeightAboveMSLMemoryTemplate(T /* dummy */,
                                                         const ossimGpt& gpt)
{
   // Establish the grid indexes:
   double xi = (gpt.lon - m_nwCornerPost.lon) / m_lonSpacing;
   double yi = (m_nwCornerPost.lat - gpt.lat) / m_latSpacing;
   
   int x0 = static_cast<int>(xi);
   int y0 = static_cast<int>(yi);
   
   if(x0 == (m_numberOfSamples-1))
   {
      --x0; // Move over one post.
   }
   
   // Check for top edge.
   //    if (gpt.lat == theGroundRect.ul().lat)
   if(y0 == (m_numberOfLines-1))
   {
      --y0; // Move down one post.
   }
#if 0
   // Check for right edge.
   if (ossim::almostEqual(gpt.lon,theGroundRect.lr().lon))
   {
      --x0; // Move over one post to the left.
   }
   
   // Check for bottom edge.
   if (ossim::almostEqual(gpt.lat,theGroundRect.lr().lat))
   {
      --y0; // Move up one post.
   }
#endif
   // Do some error checking.
   if ( xi < 0.0 || yi < 0.0 ||
       x0 > (m_numberOfSamples  - 2.0) ||
       y0 > (m_numberOfLines    - 2.0) )
   {
      //       std::cout << "ossimSrtmHandler::getHeightAboveMSLTemplate: leaving 1" << std::endl;
      return ossim::nan();
   }

   // Grab the four points from the srtm cell needed.
   ossim_uint64 offset = y0 * m_srtmRecordSizeInBytes + x0 * sizeof(T);
   ossim_uint64 offset2 =offset+m_srtmRecordSizeInBytes;
   T v00 = *(reinterpret_cast<T*> (&m_memoryMap[offset]));
   T v01 = *(reinterpret_cast<T*> (&m_memoryMap[offset + sizeof(T)]));
   T v10 = *(reinterpret_cast<T*> (&m_memoryMap[offset2]));
   T v11 = *(reinterpret_cast<T*> (&m_memoryMap[offset2 + sizeof(T)]));
   if (m_swapper)
   {
      m_swapper->swap(v00);
      m_swapper->swap(v01);
      m_swapper->swap(v10);
      m_swapper->swap(v11);
   }
   double p00 = v00;
   double p01 = v01;
   double p10 = v10;
   double p11 = v11;
   
   double xt0 = xi - x0;
   double yt0 = yi - y0;
   double xt1 = 1-xt0;
   double yt1 = 1-yt0;
   
   double w00 = xt1*yt1;
   double w01 = xt0*yt1;
   double w10 = xt1*yt0;
   double w11 = xt0*yt0;
   
   //***
   // Test for null posts and set the corresponding weights to 0:
   //***
   if (p00 == theNullHeightValue)
      w00 = 0.0;
   if (p01 == theNullHeightValue)
      w01 = 0.0;
   if (p10 == theNullHeightValue)
      w10 = 0.0;
   if (p11 == theNullHeightValue)
      w11 = 0.0;
   
#if 0 /* Serious debug only... */
   cout << "\np00:  " << p00
   << "\np01:  " << p01
   << "\np10:  " << p10
   << "\np11:  " << p11
   << "\nw00:  " << w00
   << "\nw01:  " << w01
   << "\nw10:  " << w10
   << "\nw11:  " << w11
   << endl;
#endif
   
   double sum_weights = w00 + w01 + w10 + w11;
   
   if (sum_weights)
   {
      return (p00*w00 + p01*w01 + p10*w10 + p11*w11) / sum_weights;
   }
   
   return ossim::nan();
}

double ossimSrtmHandler::getPostValue(const ossimIpt& /* gridPt */) const
{
   ossimNotify(ossimNotifyLevel_WARN)
      << "ossimSrtmHandler::getPostValue(const ossimIpt& gridPt):  NEED TO IMPLEMENT TO NEW INTERFACE\n";
   
   return theNullHeightValue;
#if 0
   static const char MODULE[] = "ossimSrtmHandler::getPostValue";
   
   if(!theFileStr.valid()) return ossim::nan();
   
   // Do some error checking.
   if ( gridPt.x < 0.0 || gridPt.y < 0.0 ||
        gridPt.x > (theNumberOfSamples  - 1) ||
        gridPt.y > (theNumberOfLines - 1) )
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "WARNING " << MODULE << ": No intersection..." << std::endl;
      }
      return ossim::nan();
   }

   if (!isOpen())
   {
      return ossim::nan();
   }

   std::streampos offset = gridPt.y * theSrtmRecordSizeInBytes + gridPt.x * 2;

   // Put the file pointer at the start of the first elevation post.
   m_fileStr.seekg(offset, std::ios::beg);

   ossim_sint16 p;

   // Get the post.
   m_fileStr.read((char*)&p, 2);
   if (theSwapper)
   {
      theSwapper->swap(p);
   }
   return p;
#endif
}

ossimIpt ossimSrtmHandler::getSizeOfElevCell() const
{
   return ossimIpt(m_numberOfSamples, m_numberOfLines);
}

const ossimSrtmHandler&
ossimSrtmHandler::operator=(const ossimSrtmHandler& rhs)
{
   m_supportData = rhs.m_supportData;
   m_streamOpen = rhs.m_streamOpen,
   m_numberOfLines = rhs.m_numberOfLines;
   m_numberOfSamples = rhs.m_numberOfSamples;
   m_srtmRecordSizeInBytes = rhs.m_srtmRecordSizeInBytes;
   m_latSpacing = rhs.m_latSpacing;
   m_lonSpacing = rhs.m_lonSpacing;
   m_nwCornerPost = rhs.m_nwCornerPost;
   m_swapper = rhs.m_swapper?new ossimEndian:0;
   m_scalarType = rhs.m_scalarType;
   m_memoryMap = rhs.m_memoryMap;
   
   ossimElevCellHandler::operator = (rhs);
   return *this;
}

ossimSrtmHandler::ossimSrtmHandler(const ossimSrtmHandler& src)
:ossimElevCellHandler(src),
m_supportData(src.m_supportData),
m_streamOpen(src.m_streamOpen),
m_numberOfLines(src.m_numberOfLines),
m_numberOfSamples(src.m_numberOfSamples),
m_srtmRecordSizeInBytes(src.m_srtmRecordSizeInBytes),
m_latSpacing(src.m_latSpacing),
m_lonSpacing(src.m_lonSpacing),
m_nwCornerPost(src.m_nwCornerPost),
m_swapper(src.m_swapper?new ossimEndian:0),
m_scalarType(src.m_scalarType),
m_memoryMap(src.m_memoryMap)
{
   if(m_memoryMap.empty()&&src.isOpen())
   {
      m_fileStr.open(src.getFilename().c_str(),
                     std::ios::binary|std::ios::in);
   }
}

bool ossimSrtmHandler::isOpen()const
{
   if(!m_memoryMap.empty()) return true;
   
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_fileStrMutex);
   return m_streamOpen;
   // return (m_fileStr.is_open());
}


bool ossimSrtmHandler::open(const ossimFilename& file, bool memoryMapFlag)
{
   theFilename = file;
   clearErrorStatus();
   if (!m_supportData.setFilename(file, false) )
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      return false;
   }
   if(m_swapper) {delete m_swapper;m_swapper=0;}
   m_scalarType = m_supportData.getScalarType();
   // SRTM is stored in big endian.
   if ( ossim::byteOrder() == OSSIM_LITTLE_ENDIAN)
   {
      m_swapper = new ossimEndian();
   }
   m_streamOpen = false;
   m_numberOfLines         = m_supportData.getNumberOfLines();
   m_numberOfSamples       = m_supportData.getNumberOfSamples();
   m_srtmRecordSizeInBytes = m_numberOfSamples * ossim::scalarSizeInBytes(m_scalarType);
   m_latSpacing            = m_supportData.getLatitudeSpacing();
   m_lonSpacing            = m_supportData.getLongitudeSpacing();
   m_nwCornerPost.lon      = m_supportData.getSouthwestLongitude();
   m_nwCornerPost.lat      = m_supportData.getSouthwestLatitude()+1.0;
   m_scalarType            = m_supportData.getScalarType();
   theGroundRect = ossimGrect(ossimGpt(m_nwCornerPost.lat,
                                       m_nwCornerPost.lon,
                                       0.0),
                              ossimGpt(m_nwCornerPost.lat-1.0,
                                       m_nwCornerPost.lon+1.0,
                                       0.0));
   theMeanSpacing = (m_latSpacing*ossimGpt().metersPerDegree().y);
   
   // Set the base class null height value.
   theNullHeightValue = -32768.0;

   m_fileStr.clear();
   m_fileStr.open(theFilename.c_str(), std::ios::in | std::ios::binary);
   if(!m_fileStr)
   {
      return false;
   }
   
   if(memoryMapFlag)
   {
      m_memoryMap.resize(theFilename.fileSize());
      m_fileStr.read((char*)&m_memoryMap.front(), (streamsize)m_memoryMap.size());
      m_fileStr.close();
   }
   m_streamOpen = true;
   // Capture the stream state for non-const is_open on old compiler.
   
   return m_streamOpen;
#if 0
   // theMinHeightAboveMSL;
   // theMaxHeightAboveMSL;
   if(theFileStr.valid())
   {
      if(!m_fileStr.fail())
      {
         return true;
      }
   }
   theFileStr = 0;

   
   theFileStr = ossimStreamFactoryRegistry::instance()->createNewIFStream(theFilename,
                                                                          std::ios::in | std::ios::binary);   

   if(theFileStr.valid())
   {
      return (!m_fileStr.fail());
   }
   return false;
#endif
}

void ossimSrtmHandler::close()
{
   m_fileStr.close();
   m_memoryMap.clear();
   m_streamOpen = false;
}
