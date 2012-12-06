//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimLandsatTopoCorrectionFilter.cpp 21850 2012-10-21 20:09:55Z dburken $
#include <ossim/imaging/ossimLandsatTopoCorrectionFilter.h>
#include <ossim/imaging/ossimImageToPlaneNormalFilter.h>
#include <ossim/support_data/ossimFfL7.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/base/ossimDirectory.h>
#include <ossim/base/ossim2dLinearRegression.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimVisitor.h>
#include <iostream>

RTTI_DEF1(ossimLandsatTopoCorrectionFilter, "ossimLandsatTopoCorrectionFilter",ossimTopographicCorrectionFilter);
ossimLandsatTopoCorrectionFilter::ossimLandsatTopoCorrectionFilter()
   :ossimTopographicCorrectionFilter(),
    theLandsatHeader("")
{
}

ossimLandsatTopoCorrectionFilter::~ossimLandsatTopoCorrectionFilter()
{
}

ossimLandsatTopoCorrectionFilter::ossimLandsatTopoCorrectionFilter(ossimImageSource* colorSource,
                                                                   ossimImageSource* elevSource,
                                                                   const ossimFilename& landsatFileName)
   :ossimTopographicCorrectionFilter(colorSource,
                                     elevSource),
    theLandsatHeader(landsatFileName)
{
}

void ossimLandsatTopoCorrectionFilter::initialize()
{
   
   ossimTopographicCorrectionFilter::initialize();
   if(theLandsatHeader == "")
   {
      setLandsatHeader(findLandsatHeader());
   }
   else
   {
      setLandsatHeader(theLandsatHeader);
   }
}

void ossimLandsatTopoCorrectionFilter::setLandsatHeader(const ossimFilename& header)
{
   if(header != "")
   {
      theLandsatHeader = header;
      ossimRefPtr<ossimFfL7> headerL7 = new ossimFfL7(header.c_str());
      headerL7->getGain(theGain);
      headerL7->getBias(theBias);
      headerL7->getSunElevation(theLightSourceElevationAngle);
      headerL7->getSunAzimuth(theLightSourceAzimuthAngle);
      computeLightDirection();
      theJulianDay = headerL7->getJulianDay();
   }
}

ossimFilename ossimLandsatTopoCorrectionFilter::findLandsatHeader()
{
   ossimFilename result;

   if(!getInput(0))
   {
      return result;
   }

   ossimTypeNameVisitor visitor(ossimString("ossimImageHandler"),
                                true,
                                ossimVisitor::VISIT_CHILDREN|ossimVisitor::VISIT_INPUTS);
   getInput(0)->accept(visitor);
   
   // If there are multiple image handlers, e.g. a mosaic do not uses.
   ossimRefPtr<ossimImageHandler> handler = 0;   
   if ( visitor.getObjects().size() == 1 )
   {
      handler = visitor.getObjectAs<ossimImageHandler>( 0 );
   }

   if( handler.valid() )
   {
      ossimFilename imageFile = handler->getFilename();
      imageFile.setExtension("fst");
      if(imageFile.exists())
      {
         result = imageFile;
      }
      else
      {
         imageFile.setExtension("FST");
         if(imageFile.exists())
         {
            result = imageFile;
         }
         else
         {
            ossimDirectory dir;

            if(dir.open(imageFile.path()))
            {
               vector<ossimFilename> fileList;

               dir.findAllFilesThatMatch(fileList,
                                         ".*.fst");
               if(fileList.size() <1)
               {
                  dir.findAllFilesThatMatch(fileList,
                                            ".*.FST");
               }
               for(int i = 0; i < (int)fileList.size(); ++i)
               {
                  if(fileList[i].contains("_hpn")||
                     fileList[i].contains("_hrf")||
                     fileList[i].contains("_htm"))
                  {
                     result = fileList[i];
                     break;
                  }
               }
            }
         }
      }
   }
   return result;
}

// void ossimLandsatTopoCorrectionFilter::addRegressionPoints(std::vector<ossim2dLinearRegression>& regressionPoints,
//                                                            ossimImageData* colorData,
//                                                            ossimImageData* normalData)
// {
//    ossim_uint32 b = 0;
   
//    for(b = 0; b < colorData->getNumberOfBands(); ++b)
//    {
//       ossim_float64* normalBands[3];
//       normalBands[0] = normalData->getBuf(0);
//       normalBands[1] = normalData->getBuf(1);
//       normalBands[2] = normalData->getBuf(2);
//       ossim_float64* colorData = 
// }

#if 0
void ossimLandsatTopoCorrectionFilter::computeC()
{
   theCComputedFlag = false;
   ossim_uint32 totalNumberOfPixelsUsed = 0;
   if(getInput(0)&&getInput(1))
   {
      std::vector<ossim2dLinearRegression> linearRegression(6);
      
      ossimImageSource* colorSource = PTR_CAST(ossimImageSource, getInput(0));
      ossimImageSource* normalSource  = PTR_CAST(ossimImageSource, getInput(1));
      ossimFfL7 headerL7(theLandsatHeader.c_str());

      if(theLandsatHeader == "")
      {
         ossimNotify(ossimNotifyLevel_WARN) << "No landsat header found " << std::endl;
         return;
      }
      if(colorSource->getNumberOfOutputBands() < 6)
      {
         ossimNotify(ossimNotifyLevel_WARN) << "The color source needs all the vir bands from the landsat" << std::endl
                                            << "Not enough bands to process the request" << std::endl;
         return;
      }
      
      ossimIrect normalRect = normalSource->getBoundingRect();
      ossimIrect colorRect  = colorSource->getBoundingRect();
      ossimIrect clipRect   = normalRect.clipToRect(colorRect);
      ossimIpt ul = clipRect.ul();
      ossimIpt lr = clipRect.lr();
      long julianDay = headerL7.getJulianDay();
      double d2 =pow( (double)(1 - 0.01674 * ossim::cosd(0.9856*(julianDay-4)) ), 2.0);
      ossim_uint32 numberOfPixelsUsedForTile = 0;
      for(int y = ul.y; ((y < lr.y)&&(totalNumberOfPixelsUsed<1000));++y)
      {
         for(int x = ul.x; ((x < lr.x)&&(totalNumberOfPixelsUsed<1000)); ++x)
         {
            ossimIrect reqRect(x,
                               y,
                               x + 127,
                               y + 127);
            ossimIrect reqClip = reqRect.clipToRect(clipRect);
            ossimImageData* colorData  = colorSource->getTile(reqClip, 0);
            ossimImageData* normalData = normalSource->getTile(reqClip, 0);
            ossim_uint8* colorBands[6];
            double* normalBands[3];
            double normalBandsNp[3];
            ossim_uint8 colorBandsNp[6];
            colorBands[0] = (ossim_uint8*)colorData->getBuf(0);
            colorBands[1] = (ossim_uint8*)colorData->getBuf(1);
            colorBands[2] = (ossim_uint8*)colorData->getBuf(2);
            colorBands[3] = (ossim_uint8*)colorData->getBuf(3);
            colorBands[4] = (ossim_uint8*)colorData->getBuf(4);
            colorBands[5] = (ossim_uint8*)colorData->getBuf(5);
            colorBandsNp[0] = (ossim_uint8)colorData->getNullPix(0);
            colorBandsNp[1] = (ossim_uint8)colorData->getNullPix(1);
            colorBandsNp[2] = (ossim_uint8)colorData->getNullPix(2);
            colorBandsNp[3] = (ossim_uint8)colorData->getNullPix(3);
            colorBandsNp[4] = (ossim_uint8)colorData->getNullPix(4);
            colorBandsNp[5] = (ossim_uint8)colorData->getNullPix(5);
            normalBands[0] = (double*)normalData->getBuf(0);
            normalBands[1] = (double*)normalData->getBuf(1);
            normalBands[2] = (double*)normalData->getBuf(2);
            normalBandsNp[0] = normalData->getNullPix(0);
            normalBandsNp[1] = normalData->getNullPix(1);
            normalBandsNp[2] = normalData->getNullPix(2);
            ossim_uint32 offset = 0;
            ossim_uint32 numberOfPixels = reqClip.width()*reqClip.height();
            numberOfPixelsUsedForTile = 0;
            for(offset = 0; ((offset < numberOfPixels)&&(numberOfPixelsUsedForTile < 10)); ++offset)
            {
               if((*colorBands[0] != colorBandsNp[0])&&
                  (*colorBands[1] != colorBandsNp[1])&&
                  (*colorBands[2] != colorBandsNp[2])&&
                  (*colorBands[3] != colorBandsNp[3])&&
                  (*colorBands[4] != colorBandsNp[4])&&
                  (*colorBands[5] != colorBandsNp[5])&&
                  (*normalBands[0] != normalBandsNp[0])&&
                  (*normalBands[1] != normalBandsNp[1])&&
                  (*normalBands[2] != normalBandsNp[2]))
               {
                  double ndviTest = (((double)*colorBands[3] - (double)*colorBands[2])/
                                     ((double)*colorBands[3] + (double)*colorBands[2]));
                  if(ndviTest > 0.0)
                  {
                     double radiance5 = theBias[5]+(*colorBands[5]*theGain[5]);
//                     double p = (M_PI*radiance5*d2)/(esun[5]*cosd(90-theLightSourceAzimuthAngle));

//                    if(p < .05)
                     {
                        double cosineI = ((*normalBands[0])*theLightDirection[0] +
                                          (*normalBands[1])*theLightDirection[1] +
                                          (*normalBands[2])*theLightDirection[2]);

                        double radiance0 = theBias[0]+(*colorBands[0]*theGain[0]);
                        double radiance1 = theBias[1]+(*colorBands[1]*theGain[1]);
                        double radiance2 = theBias[2]+(*colorBands[2]*theGain[2]);
                        double radiance3 = theBias[3]+(*colorBands[3]*theGain[3]);
                        double radiance4 = theBias[4]+(*colorBands[4]*theGain[4]);
                        cosineI = fabs(cosineI);
                        if(cosineI > 1.0) cosineI = 1.0;
                        if(cosineI < 0.0) cosineI = 0.0;
                       cosineI = acos(cosineI);
// //                         linearRegression[0].addPoint(ossimDpt( cosineI,(double)(*colorBands[0])));
// //                         linearRegression[1].addPoint(ossimDpt( cosineI,(double)(*colorBands[1])));
// //                         linearRegression[2].addPoint(ossimDpt( cosineI,(double)(*colorBands[2])));
// //                         linearRegression[3].addPoint(ossimDpt( cosineI,(double)(*colorBands[3])));
// //                         linearRegression[4].addPoint(ossimDpt( cosineI,(double)(*colorBands[4])));
// //                         linearRegression[5].addPoint(ossimDpt( cosineI,(double)(*colorBands[5])));

                        linearRegression[0].addPoint(ossimDpt( cosineI,radiance0));
                        linearRegression[1].addPoint(ossimDpt( cosineI,radiance1));
                        linearRegression[2].addPoint(ossimDpt( cosineI,radiance2));
                        linearRegression[3].addPoint(ossimDpt( cosineI,radiance3));
                        linearRegression[4].addPoint(ossimDpt( cosineI,radiance4));
                        linearRegression[5].addPoint(ossimDpt( cosineI,radiance5));
                        
                        ++numberOfPixelsUsedForTile;
                        ++totalNumberOfPixelsUsed;
                     }
                  }
               }
               ++colorBands[0];
               ++colorBands[1];
               ++colorBands[2];
               ++colorBands[3];
               ++colorBands[4];
               ++colorBands[5];
               ++normalBands[0];
               ++normalBands[1];
               ++normalBands[2];
            }
         }
      }
      if(totalNumberOfPixelsUsed > 2)
      {
         linearRegression[0].solve();
         linearRegression[1].solve();
         linearRegression[2].solve();
         linearRegression[3].solve();
         linearRegression[4].solve();
         linearRegression[5].solve();
         
         ossimNotify(ossimNotifyLevel_INFO) << "linear regression results = " << endl
                                             << linearRegression[0] << endl
                                             << linearRegression[1] << endl
                                             << linearRegression[2] << endl
                                             << linearRegression[3] << endl
                                             << linearRegression[4] << endl
                                             << linearRegression[5] << endl;
         double b,m;
         linearRegression[0].getEquation(m, b);
         theC[0] = b/m;
         linearRegression[1].getEquation(m, b);
         theC[1] = b/m;
         linearRegression[2].getEquation(m, b);
         theC[2] = b/m;
         linearRegression[3].getEquation(m, b);
         theC[3] = b/m;
         linearRegression[4].getEquation(m, b);
         theC[4] = b/m;
         linearRegression[5].getEquation(m, b);
         theC[5] = b/m;
      }
      else
      {
         ossimNotify(ossimNotifyLevel_INFO) << "No points found in regression test" << endl;
      }
               
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN) << "No input connected to the ossimLandsatTopoCorrectionFilter" << std::endl;
      return;
   }
   
   theCComputedFlag = true;
}
#endif

bool ossimLandsatTopoCorrectionFilter::loadState(const ossimKeywordlist& kwl,
                                                 const char* prefix)
{
   ossimTopographicCorrectionFilter::loadState(kwl, prefix);
   const char* filename = kwl.find(prefix, ossimKeywordNames::FILENAME_KW);

   if(filename)
   {
      setLandsatHeader(filename);
   }

   return true;
}

bool ossimLandsatTopoCorrectionFilter::saveState(ossimKeywordlist& kwl,
                                                 const char* prefix)const
{
   ossimTopographicCorrectionFilter::saveState(kwl, prefix);

   kwl.add(prefix,
           ossimKeywordNames::FILENAME_KW,
           theLandsatHeader.c_str(),
           true);
   
   return true;
}
