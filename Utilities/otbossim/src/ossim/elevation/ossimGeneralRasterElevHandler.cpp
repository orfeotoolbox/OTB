#include <ossim/elevation/ossimGeneralRasterElevHandler.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimStreamFactoryRegistry.h>
#include <ossim/base/ossimDirectory.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimUnitTypeLut.h>
#include <OpenThreads/ScopedLock>

RTTI_DEF1(ossimGeneralRasterElevHandler, "ossimGeneralRasterElevHandler", ossimElevCellHandler);

ossimGeneralRasterElevHandler::ossimGeneralRasterElevHandler(const ossimFilename& file)
   :ossimElevCellHandler(file.c_str())
{
   if(file != "")
   {
      if(!setFilename(file))
      {
         theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      }
   }
}

ossimGeneralRasterElevHandler::ossimGeneralRasterElevHandler(const ossimGeneralRasterElevHandler& src)
   :ossimElevCellHandler(src),
    theGeneralRasterInfo(src.theGeneralRasterInfo)
{
   open();
}

ossimGeneralRasterElevHandler::ossimGeneralRasterElevHandler(const ossimGeneralRasterElevHandler::GeneralRasterInfo& generalRasterInfo)
{
   close();
   theGeneralRasterInfo = generalRasterInfo;
   if(!open())
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
   }
}

ossimObject* ossimGeneralRasterElevHandler::dup()const
{
   return new ossimGeneralRasterElevHandler(*this);
}

const ossimGeneralRasterElevHandler& ossimGeneralRasterElevHandler::operator=(const ossimGeneralRasterElevHandler& src)
{
   ossimElevCellHandler::operator=(*(const ossimElevCellHandler*)&src);

   return *this;
}

ossimGeneralRasterElevHandler::~ossimGeneralRasterElevHandler()
{
}

double ossimGeneralRasterElevHandler::getHeightAboveMSL(const ossimGpt& gpt)
{
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theMutex);
   if(!theInputStream.valid())
   {
      return ossim::nan();
   }

   ossim_float64 result = theGeneralRasterInfo.theNullHeightValue;
   switch(theGeneralRasterInfo.theScalarType)
   {
      case OSSIM_SINT8:
      {
         result = getHeightAboveMSLTemplate((ossim_sint8)0,
                                            theGeneralRasterInfo,
                                            gpt);
         break;
      }
      case OSSIM_UINT8:
      {
         result = getHeightAboveMSLTemplate((ossim_uint8)0,
                                            theGeneralRasterInfo,
                                            gpt);
         break;
      }
      case OSSIM_SINT16:
      {
         result = getHeightAboveMSLTemplate((ossim_sint16)0,
                                            theGeneralRasterInfo,
                                            gpt);
         break;
      }
      case OSSIM_UINT16:
      {
         result = getHeightAboveMSLTemplate((ossim_uint16)0,
                                            theGeneralRasterInfo,
                                            gpt);
         break;
      }
      case OSSIM_SINT32:
      {
         result = getHeightAboveMSLTemplate((ossim_sint32)0,
                                            theGeneralRasterInfo,
                                            gpt);
         break;
      }
      case OSSIM_UINT32:
      {
         result = getHeightAboveMSLTemplate((ossim_uint32)0,
                                            theGeneralRasterInfo,
                                            gpt);
         break;
      }
      case OSSIM_FLOAT32:
      {
         result =  getHeightAboveMSLTemplate((ossim_float32)0,
                                             theGeneralRasterInfo,
                                             gpt);
         break;
      }
      case OSSIM_FLOAT64:
      {
         result = getHeightAboveMSLTemplate((ossim_float64)0,
                                            theGeneralRasterInfo,
                                            gpt);
         break;
      }
      default:
      {
         break;
      }
      
   }

   return result;
}

ossimIpt ossimGeneralRasterElevHandler::getSizeOfElevCell() const
{
   return ossimIpt(theGeneralRasterInfo.theNumberOfSamples, theGeneralRasterInfo.theNumberOfLines);
}
   
double ossimGeneralRasterElevHandler::getPostValue(const ossimIpt& gridPt) const
{
   return ossim::nan();
}


bool ossimGeneralRasterElevHandler::open()
{
   close();
   theInputStream = ossimStreamFactoryRegistry::instance()->createNewIFStream(theGeneralRasterInfo.theFilename,
                                                                              ios::in | ios::binary);

   return theInputStream.valid();
}

/**
 * Closes the stream to the file.
 */
void ossimGeneralRasterElevHandler::close()
{
   if(theInputStream.valid())
   {
      theInputStream->close();
   }
   theInputStream = 0;
}

bool ossimGeneralRasterElevHandler::setFilename(const ossimFilename& file)
{
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theMutex);
   if(file.trim() == "")
   {
      return false;
   }
   ossimFilename hdrFile  = file;
   ossimFilename geomFile = file;
   theGeneralRasterInfo.theFilename = file;
   theGeneralRasterInfo.thePostSpacing.makeNan();
   theGeneralRasterInfo.theUlGpt.makeNan();
   theGeneralRasterInfo.theLrGpt.makeNan();
   theGeneralRasterInfo.theNumberOfSamples = 0;
   theGeneralRasterInfo.theNumberOfLines = 0;
   theNullHeightValue = ossim::nan();
   hdrFile = hdrFile.setExtension("omd");
   geomFile = geomFile.setExtension("geom");

   if(!hdrFile.exists()||
      !geomFile.exists())
   {
      return false;
   }
   ossimKeywordlist kwl(hdrFile);
   if (kwl.getErrorStatus() == ossimErrorCodes::OSSIM_ERROR)
   {
      return false;
   }
   
   kwl.add(ossimKeywordNames::FILENAME_KW,
           file.c_str(),
           true);
   ossimGeneralRasterInfo generalInfo;
   
   if(!generalInfo.loadState(kwl))
   {
      return false;
   }
    if(generalInfo.numberOfBands() != 1)
   {
      ossimNotify(ossimNotifyLevel_WARN) << "ossimGeneralRasterElevHandler::initializeInfo WARNING:The number of bands are not specified in the header file" << std::endl;
      return false;
   }

   kwl.clear();
   if(kwl.addFile(geomFile))
   {
      theGeneralRasterInfo.theDatum = 0;
      const char* datumCode = kwl.find(ossimKeywordNames::DATUM_KW);
      const char* dlat      = kwl.find(ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LAT);
      const char* dlon      = kwl.find(ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LON);
      const char* tieLat    = kwl.find(ossimKeywordNames::TIE_POINT_LAT_KW);
      const char* tieLon    = kwl.find(ossimKeywordNames::TIE_POINT_LON_KW);

      if(datumCode)
      {
         theGeneralRasterInfo.theDatum = ossimDatumFactory::instance()->create(datumCode);
      }
      if(!theGeneralRasterInfo.theDatum)
      {
         theGeneralRasterInfo.theDatum = ossimDatumFactory::instance()->wgs84();
      }
      if(!dlat||!dlon)
      {
         const char* scale     = kwl.find(ossimKeywordNames::PIXEL_SCALE_XY_KW);
         const char* scaleUnit = kwl.find(ossimKeywordNames::PIXEL_SCALE_UNITS_KW);
         if(scale&&scaleUnit)
         {
            if(ossimString(scaleUnit).trim() == ossimUnitTypeLut::instance()->getEntryString(OSSIM_DEGREES))
            {
               theGeneralRasterInfo.thePostSpacing.toPoint(scale);
            }
            else
            {
               ossimNotify(ossimNotifyLevel_WARN) << "ossimGeneralRasterElevHandler::initializeInfo WARNING: Decimal degrees per pixel for lat and lon are not specified" << std::endl;
               return false;
            }
         }
         else
         {
            ossimNotify(ossimNotifyLevel_WARN) << "ossimGeneralRasterElevHandler::initializeInfo WARNING: Decimal degrees per pixel for lat and lon are not specified" << std::endl;
            return false;
         }

      }
      else
      {
         theGeneralRasterInfo.thePostSpacing.x   = ossimString(dlon).toDouble();
         theGeneralRasterInfo.thePostSpacing.y   = ossimString(dlat).toDouble();
      }
      if(!tieLat||!tieLon)
      {
         const char* tie_point_xy = kwl.find(ossimKeywordNames::TIE_POINT_XY_KW);
         const char* tie_point_units = kwl.find(ossimKeywordNames::TIE_POINT_UNITS_KW);
         if(tie_point_xy&&(ossimString(tie_point_units).downcase() == "degrees"))
         {
            ossimDpt tie;
            tie.toPoint(tie_point_xy);
           
            theGeneralRasterInfo.theUlGpt = ossimGpt(tie.lat+(theGeneralRasterInfo.thePostSpacing.y*.5),
                                                     tie.lon-(theGeneralRasterInfo.thePostSpacing.x*.5),
                                                     0.0,
                                                     theGeneralRasterInfo.theDatum);

         }
         else
         {
            return false;
         }
      }
      else
      {
         theGeneralRasterInfo.theUlGpt = ossimGpt(ossimString(tieLat).toDouble(),
                                                  ossimString(tieLon).toDouble(),
                                                  0.0,
                                                  theGeneralRasterInfo.theDatum);
      }
      theGeneralRasterInfo.theLrGpt = theGeneralRasterInfo.theUlGpt;
      theGeneralRasterInfo.theNumberOfSamples = generalInfo.rawSamples();
      theGeneralRasterInfo.theNumberOfLines   = generalInfo.rawLines();
      if(theGeneralRasterInfo.theNumberOfLines && theGeneralRasterInfo.theNumberOfSamples)
      {
         theGeneralRasterInfo.theLrGpt.latd(theGeneralRasterInfo.theUlGpt.latd() - ((theGeneralRasterInfo.theNumberOfLines)*
                                                                                    theGeneralRasterInfo.thePostSpacing.y));
         theGeneralRasterInfo.theLrGpt.lond(theGeneralRasterInfo.theUlGpt.lond() + ((theGeneralRasterInfo.theNumberOfSamples)*
                                                                                    theGeneralRasterInfo.thePostSpacing.x));
      }
      theGeneralRasterInfo.theNullHeightValue = generalInfo.getNullPixelValue(0);
      theGeneralRasterInfo.theBounds = ossimDrect(theGeneralRasterInfo.theUlGpt,
                                                  theGeneralRasterInfo.theUlGpt,
                                                  theGeneralRasterInfo.theLrGpt,
                                                  theGeneralRasterInfo.theLrGpt,
                                                  OSSIM_RIGHT_HANDED);
      theGeneralRasterInfo.theByteOrder = generalInfo.getImageDataByteOrder();
      theGeneralRasterInfo.theScalarType = generalInfo.getScalarType();
      theGeneralRasterInfo.theBytesPerRawLine = generalInfo.bytesPerRawLine();
      theMeanSpacing = (theGeneralRasterInfo.thePostSpacing.lat +
                        theGeneralRasterInfo.thePostSpacing.lon)*ossimGpt().metersPerDegree().x / 2.0;
      theGroundRect = ossimGrect(theGeneralRasterInfo.theBounds.ul().lat,
                                 theGeneralRasterInfo.theBounds.ul().lon,
                                 theGeneralRasterInfo.theBounds.lr().lat,
                                 theGeneralRasterInfo.theBounds.lr().lon);
      theNullHeightValue = theGeneralRasterInfo.theNullHeightValue;
   }
   else
   {
      return false;
   }
   
   return true;
   
}

#if 0
void ossimGeneralRasterElevHandler::initializeList(const ossimFilename& file)
{
   close();
   theFilename = file;
   theBoundingRectInfoList.clear();
   if(theFilename.exists())
   {
      if(theFilename.isDir())
      {
         ossimDirectory dir(theFilename);

         ossimFilename file;
         ossim_uint32 maxCount = 10;
         ossim_uint32 count = 0;
         bool foundOne = false;
         if(dir.getFirst(file))
         {
            do
            {
               ++count;
               ossimString ext = file.ext();
               ext = ext.downcase();
               if(ext == "ras")
               {
                  ossimGeneralRasterElevHandler::BoundingRectInfo info;
                  if(initializeInfo(info, file))
                  {
                     foundOne = true;
                     addInfo(info);
                  }
               }
            } while(dir.getNext(file) &&
                   (!foundOne &&
                    (count < maxCount)));
         }
#if 0
         std::vector<ossimFilename> result;
         
         dir.findAllFilesThatMatch(result,
                                   ".*\\.ras");
      
         if(result.size())
         {
            ossimGeneralRasterElevHandler::BoundingRectInfo info;
            ossim_uint32 idx = 0;
            for(idx = 0; idx < result.size(); ++idx)
            {
               if(initializeInfo(info, ossimFilename(result[idx])))
               {
                  addInfo(info);
               }
            }
         }
#endif
         if(theBoundingRectInfoList.size()>0)
         {
            theNullHeightValue = theBoundingRectInfoList[0].theNullHeightValue;
         }
      }
      else
      {
         ossimGeneralRasterElevHandler::BoundingRectInfo info;
         if(initializeInfo(info, theFilename))
         {
            addInfo(info);
         }
      }
   }
}
#endif
template <class T>
double ossimGeneralRasterElevHandler::getHeightAboveMSLTemplate(
   T dummy,
   const ossimGeneralRasterElevHandler::GeneralRasterInfo& info,
   const ossimGpt& gpt)
{
   if(!theInputStream.valid())
   {
      return ossim::nan();
   }
//    if(theFileStr->fail())
//    {
//       theFileStr->clear();
//       theFileStr->seekg(0);
//    }
//    if(theFileStr.fail())
   if(theInputStream->fail())
   {
      theInputStream->clear();
      theInputStream->seekg(0);
   }
   ossimEndian endian;
   
   ossimGpt shiftedPoint = gpt;
   shiftedPoint.changeDatum(info.theUlGpt.datum());
   if(!info.theBounds.pointWithin(shiftedPoint))
   {
      return ossim::nan();
   }
   
   double xi = (shiftedPoint.lond() - info.theUlGpt.lond())/info.thePostSpacing.x;
   double yi = (info.theUlGpt.latd() -
                shiftedPoint.latd())/info.thePostSpacing.y;

   ossim_sint64 x0 = static_cast<ossim_sint64>(xi);
   ossim_sint64 y0 = static_cast<ossim_sint64>(yi);

   double xt0 = xi - x0;
   double yt0 = yi - y0;
   double xt1 = 1-xt0;
   double yt1 = 1-yt0;
   
   double w00 = xt1*yt1;
   double w01 = xt0*yt1;
   double w10 = xt1*yt0;
   double w11 = xt0*yt0;
   
   if ( xi < 0.0 || yi < 0.0 ||
        x0 > (info.theNumberOfSamples  - 1.0) ||
        y0 > (info.theNumberOfLines    - 1.0) )
   {
      return ossim::nan();
   }

   if(x0 == (info.theNumberOfSamples  - 1.0))
   {
      --x0;
   }
   if(y0 == (info.theNumberOfLines  - 1.0))
   {
      --y0;
   }
   T p[4];
   
   ossim_uint64 bytesPerLine  = info.theBytesPerRawLine;
   
   std::streampos offset = y0*bytesPerLine + x0*sizeof(T);

   theInputStream->seekg(offset, ios::beg);
   theInputStream->read((char*)p, sizeof(T));
   
   // Get the second post.
   theInputStream->read((char*)(p+1), sizeof(T));
   
//   offset += (bytesPerLine-2*sizeof(T));

   theInputStream->ignore(bytesPerLine-2*sizeof(T));
   // Get the third post.
   theInputStream->read((char*)(p+2), sizeof(T));
   
   // Get the fourth post.
   theInputStream->read((char*)(p+3), sizeof(T));
   
   if(theInputStream->fail())
   {
      theInputStream->clear();
      return ossim::nan();
   }
   if(endian.getSystemEndianType() != info.theByteOrder)
   {
      endian.swap((T*)p, (ossim_uint32)4);
   }
   double p00 = p[0];
   double p01 = p[1];
   double p10 = p[2];
   double p11 = p[3];

   if (p00 == info.theNullHeightValue)
      w00 = 0.0;
   if (p01 == info.theNullHeightValue)
      w01 = 0.0;
   if (p10 == info.theNullHeightValue)
      w10 = 0.0;
   if (p11 == info.theNullHeightValue)
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
#if 0
void ossimGeneralRasterElevHandler::addInfo(const ossimGeneralRasterElevHandler::BoundingRectInfo& info)
{
   if(theBoundingRectInfoList.empty())
   {
      theBoundingRectInfoList.push_back(info);
      theNullHeightValue = info.theNullHeightValue;
      theBoundingRect = info.theBounds;
   }
   else
   {
      BoundingRectListType::iterator iter = theBoundingRectInfoList.begin();
      bool found = false;
//       if(theNullHeightValue ==  info.theNullHeightValue)
//       {
      while((iter!=theBoundingRectInfoList.end())&&
            (!found))
      {
         if(info.thePostSpacing.length() <= iter->thePostSpacing.length())
         {
            theBoundingRectInfoList.insert(iter, info);
            theBoundingRect = theBoundingRect.combine(info.theBounds);
            found = true;
         }
         else
         {
            ++iter;
         }
      }
      if(!found)
      {
         theBoundingRectInfoList.push_back(info);
      }
   }
}
#endif
ossimDrect ossimGeneralRasterElevHandler::getBoundingRect()const
{
   return theGeneralRasterInfo.theBounds;
}

bool ossimGeneralRasterElevHandler::pointHasCoverage(const ossimGpt& gpt) const
{
   ossimDpt pt = gpt;

   return theGeneralRasterInfo.theBounds.pointWithin(pt);
//    BoundingRectListType::const_iterator i = theBoundingRectInfoList.begin();
//    while (i != theBoundingRectInfoList.end())
//    {
//       if ((*i).theBounds.pointWithin(pt))
//       {
//          return true;
//       }
//       ++i;
//    }
//    return false;
}
const ossimGeneralRasterElevHandler::GeneralRasterInfo& ossimGeneralRasterElevHandler::generalRasterInfo()const
{
   return theGeneralRasterInfo;
}
