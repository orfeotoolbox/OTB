#include <ossim/elevation/ossimGeneralRasterElevHandler.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimStreamFactoryRegistry.h>
#include <ossim/base/ossimDirectory.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimUnitTypeLut.h>
RTTI_DEF1(ossimGeneralRasterElevHandler, "ossimGeneralRasterElevHandler", ossimElevCellHandler);

ossimGeneralRasterElevHandler::ossimGeneralRasterElevHandler(const ossimFilename& file)
   :ossimElevCellHandler(file.c_str()),
    theBoundingRect(0,0,0,0,OSSIM_RIGHT_HANDED)
{
   theCurrentIdx = -1;
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
    theBoundingRectInfoList(src.theBoundingRectInfoList),
    theBoundingRect(src.theBoundingRect),
    theCurrentIdx(src.theCurrentIdx)// ,
//     theFileStr(0)
{
   theCurrentIdx = -1;
   setFilename(src.theFilename);
   
//    theFileStr.close();
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
   if(theBoundingRectInfoList.size() < 1)
   {
      return theNullHeightValue;
   }

   if(theCurrentIdx < 0)
   {
      open();
   }
   if(theCurrentIdx < 0)
   {
      return theNullHeightValue;
   }

   if(theCurrentIdx != 0)
   {
      if(!open((ossim_uint32)0))
      {
         return theNullHeightValue;
      }
   }
   ossim_uint32 idx = 0;
   ossim_uint32 idxMax = theBoundingRectInfoList.size();
   ossim_float64 result = theNullHeightValue;
   while(idx < idxMax)
   {
      const ossimGeneralRasterElevHandler::BoundingRectInfo& info = theBoundingRectInfoList[theCurrentIdx];
         
      switch(info.theScalarType)
      {
         case OSSIM_SINT8:
         {
            result = getHeightAboveMSLTemplate((ossim_sint8)0,
                                               info,
                                               gpt);
            break;
         }
         case OSSIM_UINT8:
         {
            result = getHeightAboveMSLTemplate((ossim_uint8)0,
                                               info,
                                               gpt);
            break;
         }
         case OSSIM_SINT16:
         {
            result = getHeightAboveMSLTemplate((ossim_sint16)0,
                                               info,
                                               gpt);
            break;
         }
         case OSSIM_UINT16:
         {
            result = getHeightAboveMSLTemplate((ossim_uint16)0,
                                               info,
                                               gpt);
            break;
         }
         case OSSIM_SINT32:
         {
            result = getHeightAboveMSLTemplate((ossim_sint32)0,
                                               info,
                                               gpt);
            break;
         }
         case OSSIM_UINT32:
         {
            result = getHeightAboveMSLTemplate((ossim_uint32)0,
                                               info,
                                               gpt);
            break;
         }
         case OSSIM_FLOAT32:
         {
            result =  getHeightAboveMSLTemplate((ossim_float32)0,
                                                info,
                                                gpt);
            break;
         }
         case OSSIM_FLOAT64:
         {
            result = getHeightAboveMSLTemplate((ossim_float64)0,
                                               info,
                                               gpt);
            break;
         }
         default:
         {
            break;
         }
         
      }
      if(result != info.theNullHeightValue)
      {
         return result;
      }
      if(!open((theCurrentIdx+1)%theBoundingRectInfoList.size()))
      {
         return theNullHeightValue;
      }
      ++idx;
   }

   return theNullHeightValue;
}

ossimIpt ossimGeneralRasterElevHandler::getSizeOfElevCell() const
{
   return ossimIpt(0,0);
}
   
double ossimGeneralRasterElevHandler::getPostValue(const ossimIpt& gridPt) const
{
   return theNullHeightValue;
}


bool ossimGeneralRasterElevHandler::open()
{
   return open((ossim_uint32)0);
}

bool ossimGeneralRasterElevHandler::open(ossim_uint32 idx)
{
   theCurrentIdx = -1;
   if(theBoundingRectInfoList.empty())
   {
      return false;
   }

   if(idx < theBoundingRectInfoList.size())
   {
      if(!theBoundingRectInfoList[idx].theInputStream.valid())
      {
         theBoundingRectInfoList[idx].theInputStream = ossimStreamFactoryRegistry::instance()->createNewInputStream(theBoundingRectInfoList[idx].theFilename,
                                                                                                                    ios::in | ios::binary);

         if(theBoundingRectInfoList[idx].theInputStream.valid()&&
            !theBoundingRectInfoList[idx].theInputStream->fail())
         {
            theCurrentIdx = idx;
            return true;
         }
      }
      else if(theBoundingRectInfoList[idx].theInputStream.valid())
      {
         theCurrentIdx = idx;
         return true;
      }
   }
//       close();
//       theFileStr.open(theBoundingRectInfoList[idx].theFilename.c_str(),
//                       ios::in|ios::binary);


//       theFileStr = 0;
//       theFileStr = ossimStreamFactoryRegistry::instance()->createNewInputStream(theBoundingRectInfoList[idx].theFilename,
//                                                                                 ios::in | ios::binary);

//       if(!theFileStr.fail())
//       {
//          theCurrentIdx = idx;
//          return true;
//       }
//   }

   return false;
}

/**
 * Closes the stream to the file.
 */
void ossimGeneralRasterElevHandler::close()
{
//    if(theFileStr.valid())
//    {
//       theFileStr->close();
//       theFileStr = 0;
//    }

#if 0   
   theFileStr.close();
   theFileStr.clear();
   theCurrentIdx = -1;
#endif
   theCurrentIdx = -1;
}

bool ossimGeneralRasterElevHandler::setFilename(const ossimFilename& file)
{
   theFilename = file;
   close();
   initializeList(file);   
   return open();
}

template <class T>
double ossimGeneralRasterElevHandler::getHeightAboveMSLTemplate(T dummy,
                                                                const ossimGeneralRasterElevHandler::BoundingRectInfo& info,
                                                                const ossimGpt& gpt)
{
   if(!info.theInputStream.valid())
   {
      return info.theNullHeightValue;
   }
//    if(theFileStr->fail())
//    {
//       theFileStr->clear();
//       theFileStr->seekg(0);
//    }
//    if(theFileStr.fail())
   if(info.theInputStream->fail())
   {
      info.theInputStream->clear();
      info.theInputStream->seekg(0);
   }
   ossimEndian endian;
   
   ossimGpt shiftedPoint = gpt;
   shiftedPoint.changeDatum(info.theUlGpt.datum());
   if(!info.theBounds.pointWithin(shiftedPoint))
   {
      return info.theNullHeightValue;
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
      return info.theNullHeightValue;
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

   info.theInputStream->seekg(offset, ios::beg);
   info.theInputStream->read((char*)p, sizeof(T));
   
   // Get the second post.
   info.theInputStream->read((char*)(p+1), sizeof(T));
   
//   offset += (bytesPerLine-2*sizeof(T));

   info.theInputStream->ignore(bytesPerLine-2*sizeof(T));
   // Get the third post.
   info.theInputStream->read((char*)(p+2), sizeof(T));
   
   // Get the fourth post.
   info.theInputStream->read((char*)(p+3), sizeof(T));
   
   if(info.theInputStream->fail())
   {
      info.theInputStream->clear();
      return info.theNullHeightValue;
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

   return info.theNullHeightValue;
}

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

bool ossimGeneralRasterElevHandler::initializeInfo(
   ossimGeneralRasterElevHandler::BoundingRectInfo& info,
   const ossimFilename& file )
{
   if(file.trim() == "")
   {
      return false;
   }
   ossimFilename hdrFile  = file;
   ossimFilename geomFile = file;
   info.theFilename = file;
   info.thePostSpacing.makeNan();
   info.theUlGpt.makeNan();
   info.theLrGpt.makeNan();
   info.theNumberOfSamples = 0;
   info.theNumberOfLines = 0;
   theNullHeightValue = OSSIM_DBL_NAN;
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
   
//    if(!info.theInfo.loadState(kwl))
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
      info.theDatum = 0;
      const char* datumCode = kwl.find(ossimKeywordNames::DATUM_KW);
      const char* dlat      = kwl.find(ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LAT);
      const char* dlon      = kwl.find(ossimKeywordNames::DECIMAL_DEGREES_PER_PIXEL_LON);
      const char* tieLat    = kwl.find(ossimKeywordNames::TIE_POINT_LAT_KW);
      const char* tieLon    = kwl.find(ossimKeywordNames::TIE_POINT_LON_KW);

      if(datumCode)
      {
         info.theDatum = ossimDatumFactory::instance()->create(datumCode);
      }
      if(!info.theDatum)
      {
         info.theDatum = ossimDatumFactory::instance()->wgs84();
      }
      if(!dlat||!dlon)
      {
         const char* scale     = kwl.find(ossimKeywordNames::PIXEL_SCALE_XY_KW);
         const char* scaleUnit = kwl.find(ossimKeywordNames::PIXEL_SCALE_UNITS_KW);
         if(scale&&scaleUnit)
         {
            if(ossimString(scaleUnit).trim() == ossimUnitTypeLut::instance()->getEntryString(OSSIM_DEGREES))
            {
               info.thePostSpacing.toPoint(scale);
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
//          return false;
      }
      else
      {
         info.thePostSpacing.x   = ossimString(dlon).toDouble();
         info.thePostSpacing.y   = ossimString(dlat).toDouble();
      }
      if(!tieLat||!tieLon)
      {
         const char* tie_point_xy = kwl.find(ossimKeywordNames::TIE_POINT_XY_KW);
         const char* tie_point_units = kwl.find(ossimKeywordNames::TIE_POINT_UNITS_KW);
         if(tie_point_xy&&(ossimString(tie_point_units).downcase() == "degrees"))
         {
            ossimDpt tie;
            tie.toPoint(tie_point_xy);
           
            info.theUlGpt = ossimGpt(tie.lat+(info.thePostSpacing.y*.5),
                                     tie.lon-(info.thePostSpacing.x*.5),
                                     0.0,
                                     info.theDatum);

         }
         else
         {
            return false;
         }
      }
      else
      {
         info.theUlGpt = ossimGpt(ossimString(tieLat).toDouble(),
                                  ossimString(tieLon).toDouble(),
                                  0.0,
                                  info.theDatum);
      }
      info.theLrGpt = info.theUlGpt;
      info.theNumberOfSamples = generalInfo.rawSamples();
      info.theNumberOfLines   = generalInfo.rawLines();
      if(info.theNumberOfLines && info.theNumberOfSamples)
      {
         info.theLrGpt.latd(info.theUlGpt.latd() - ((info.theNumberOfLines)*info.thePostSpacing.y));
         info.theLrGpt.lond(info.theUlGpt.lond() + ((info.theNumberOfSamples)*info.thePostSpacing.x));
      }
      info.theNullHeightValue = generalInfo.getNullPixelValue(0);
      info.theBounds = ossimDrect(info.theUlGpt,
                                  info.theUlGpt,
                                  info.theLrGpt,
                                  info.theLrGpt,
                                  OSSIM_RIGHT_HANDED);
      info.theByteOrder = generalInfo.getImageDataByteOrder();
      info.theScalarType = generalInfo.getScalarType();
      info.theBytesPerRawLine = generalInfo.bytesPerRawLine();
   }
   else
   {
      return false;
   }
   
   return true;
}

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
 
ossimDrect ossimGeneralRasterElevHandler::getBoundingRect()const
{
   return theBoundingRect;
}

bool ossimGeneralRasterElevHandler::pointHasCoverage(const ossimGpt& gpt) const
{
   ossimDpt pt = gpt;
   
   BoundingRectListType::const_iterator i = theBoundingRectInfoList.begin();
   while (i != theBoundingRectInfoList.end())
   {
      if ((*i).theBounds.pointWithin(pt))
      {
         return true;
      }
      ++i;
   }
   return false;
}
