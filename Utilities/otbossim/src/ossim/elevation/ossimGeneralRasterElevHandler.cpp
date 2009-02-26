#include <ossim/elevation/ossimGeneralRasterElevHandler.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimStreamFactoryRegistry.h>
#include <ossim/base/ossimDirectory.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimUnitTypeLut.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimImageSource.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimImageViewTransform.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimGpt.h>

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
   return ossimIpt(theGeneralRasterInfo.theWidth, theGeneralRasterInfo.theHeight);
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
   if(file.trim() == "")
   {
      return false;
   }
   ossimFilename hdrFile  = file;
   ossimFilename geomFile = file;
   theGeneralRasterInfo.theFilename = file;
   theGeneralRasterInfo.theWidth = 0;
   theGeneralRasterInfo.theHeight = 0;
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
      theGeneralRasterInfo.theNullHeightValue = generalInfo.getNullPixelValue(0);
      theGeneralRasterInfo.theImageRect       = generalInfo.imageRect();
      theGeneralRasterInfo.theUl              = theGeneralRasterInfo.theImageRect.ul();
      theGeneralRasterInfo.theLr              = theGeneralRasterInfo.theImageRect.lr();
      theGeneralRasterInfo.theWidth           = theGeneralRasterInfo.theImageRect.width();
      theGeneralRasterInfo.theHeight          = theGeneralRasterInfo.theImageRect.height();
      theGeneralRasterInfo.theImageRect       = generalInfo.imageRect();
      theGeneralRasterInfo.theByteOrder       = generalInfo.getImageDataByteOrder();
      theGeneralRasterInfo.theScalarType      = generalInfo.getScalarType();
      theGeneralRasterInfo.theBytesPerRawLine = generalInfo.bytesPerRawLine();

	  //add  by simbla
      theGeneralRasterInfo.theProjection = ossimProjectionFactoryRegistry::instance()->createProjection(kwl);
      
      if(!theGeneralRasterInfo.theProjection.valid())
      {
         return false;
      }
      ossimGpt defaultDatum;
      ossimGpt ulGpt;
      ossimGpt urGpt;
      ossimGpt lrGpt;
      ossimGpt llGpt;
      theGeneralRasterInfo.theDatum = defaultDatum.datum();
      theGeneralRasterInfo.theProjection->lineSampleToWorld(theGeneralRasterInfo.theImageRect.ul(), ulGpt);
      theGeneralRasterInfo.theProjection->lineSampleToWorld(theGeneralRasterInfo.theImageRect.ur(), urGpt);
      theGeneralRasterInfo.theProjection->lineSampleToWorld(theGeneralRasterInfo.theImageRect.lr(), lrGpt);
      theGeneralRasterInfo.theProjection->lineSampleToWorld(theGeneralRasterInfo.theImageRect.ll(), llGpt);
      
      ulGpt.changeDatum(theGeneralRasterInfo.theDatum);
      urGpt.changeDatum(theGeneralRasterInfo.theDatum);
      lrGpt.changeDatum(theGeneralRasterInfo.theDatum);
      llGpt.changeDatum(theGeneralRasterInfo.theDatum);
      theMeanSpacing = theGeneralRasterInfo.theProjection->getMetersPerPixel().y;
      theGroundRect = ossimGrect(ulGpt, urGpt, lrGpt, llGpt);
      theGeneralRasterInfo.theWgs84GroundRect = ossimDrect(ulGpt, urGpt, lrGpt, llGpt, OSSIM_RIGHT_HANDED);
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
   shiftedPoint.changeDatum(info.theDatum);
   if(!info.theWgs84GroundRect.pointWithin(shiftedPoint))
   {
      return ossim::nan();
   }
   
   ossimDpt pt;
   info.theProjection->worldToLineSample(shiftedPoint,pt);
   double xi = pt.x;
   double yi = pt.y;
   
   xi -= info.theUl.x;
   yi -= info.theUl.y;
   
  //modifed by simbla  2008 7.17
   //double xi = (shiftedPoint.lond() - info.theUlGpt.lond())/info.thePostSpacing.x;
   //double yi = (info.theUlGpt.latd() -
   //             shiftedPoint.latd())/info.thePostSpacing.y;
   
   
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
   
   
   if ( xi < 0 || yi < 0 ||
        x0 > (info.theWidth  - 1.0) ||
        y0 > (info.theHeight  - 1.0) )
   {
      return ossim::nan();
   }

   if(x0 == (info.theWidth  - 1.0))
   {
      --x0;
   }
   if(y0 == (info.theHeight  - 1.0))
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
   return theGeneralRasterInfo.theWgs84GroundRect;
}

bool ossimGeneralRasterElevHandler::pointHasCoverage(const ossimGpt& gpt) const
{
   ossimDpt pt = gpt;

   return theGeneralRasterInfo.theWgs84GroundRect.pointWithin(pt);
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
