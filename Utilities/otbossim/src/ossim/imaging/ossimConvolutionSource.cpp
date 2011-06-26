// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimConvolutionSource.cpp 18602 2010-12-28 05:49:36Z gpotts $

#include <ossim/imaging/ossimConvolutionSource.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimDiscreteConvolutionKernel.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeyword.h>

static const ossimKeyword NUMBER_OF_MATRICES = ossimKeyword("number_of_matrices", "");
static const ossimKeyword NUMBER_OF_ROWS = ossimKeyword("rows", "");
static const ossimKeyword NUMBER_OF_COLS = ossimKeyword("cols", "");

RTTI_DEF1(ossimConvolutionSource, "ossimConvolutionSource", ossimImageSourceFilter);

ossimConvolutionSource::ossimConvolutionSource()
   : ossimImageSourceFilter(),
     theTile(NULL)
{
}

ossimConvolutionSource::ossimConvolutionSource(ossimImageSource* inputSource,
                                               const NEWMAT::Matrix& convolutionMatrix)
   : ossimImageSourceFilter(inputSource),
     theTile(NULL)
{
   theConvolutionKernelList.push_back(new ossimDiscreteConvolutionKernel(convolutionMatrix));
   setKernelInformation();
   initialize();
}

ossimConvolutionSource::ossimConvolutionSource(ossimImageSource* inputSource,
                                               const vector<NEWMAT::Matrix>& convolutionList)
   : ossimImageSourceFilter(inputSource),
     theTile(NULL)
{
   setConvolutionList(convolutionList);
}

ossimConvolutionSource::~ossimConvolutionSource()
{
   deleteConvolutionList();
}

void ossimConvolutionSource::setConvolution(const double* kernel,
                                            int nrows,
                                            int ncols,
                                            bool doWeightedAverage)
{
   NEWMAT::Matrix m(nrows, ncols);
   const double* tempPtr = kernel;
   for(int row = 0; row < nrows; ++row)
   {
      for(int col = 0; col < ncols; ++col)
      {
         m[row][col] =*tempPtr;
         ++tempPtr;
      }
   }
   
   setConvolution(m, doWeightedAverage);
}

void ossimConvolutionSource::setConvolutionList(const vector<NEWMAT::Matrix>& convolutionList,
                                                bool doWeightedAverage)
{
   deleteConvolutionList();
   ossim_uint32 idx;
   for(idx = 0; idx < convolutionList.size(); ++idx)
   {
      theConvolutionKernelList.push_back(new ossimDiscreteConvolutionKernel(convolutionList[idx],
                                                                            doWeightedAverage));
   }

   setKernelInformation();
}

ossimRefPtr<ossimImageData> ossimConvolutionSource::getTile(
   const ossimIrect& tileRect,
   ossim_uint32 resLevel)
{
   if(!theInputConnection) return ossimRefPtr<ossimImageData>();

   if((!isSourceEnabled())||
      (theConvolutionKernelList.size() < 1))
   {
      return theInputConnection->getTile(tileRect, resLevel);
   }
   
   if(!theTile.valid())
   {
      allocate();
      if(!theTile.valid())  // Throw exception???
      {
         return theInputConnection->getTile(tileRect, resLevel);
      }
   }
   
   ossim_uint32 w  = tileRect.width();
   ossim_uint32 h  = tileRect.height();
   ossim_uint32 tw = theTile->getWidth();
   ossim_uint32 th = theTile->getHeight();
   theTile->setWidth(w);
   theTile->setHeight(h);
   if((w*h)!=(tw*th))
   {
      theTile->initialize();
      theTile->makeBlank();
   }
   else
   {
      theTile->makeBlank();
   }
   theTile->setOrigin(tileRect.ul());
   long offsetX = (theMaxKernelWidth)/2;
   long offsetY = (theMaxKernelHeight)/2;
   ossimIrect requestRect(tileRect.ul().x - offsetX,
                          tileRect.ul().y - offsetY,
                          tileRect.lr().x + offsetX,
                          tileRect.lr().y + offsetY);
   
   ossimRefPtr<ossimImageData> input = theInputConnection->getTile(requestRect,
                                                                   resLevel);

   if(!input.valid() ||
      (input->getDataObjectStatus() == OSSIM_NULL)||
      (input->getDataObjectStatus() == OSSIM_EMPTY))
   {
      return input;
   }
   switch(theTile->getScalarType())
   {
   case OSSIM_UCHAR:
   {
      if(theConvolutionKernelList.size() == 1)
      {
         convolve(static_cast<ossim_uint8>(0),
                  input,
                  theConvolutionKernelList[0]);
      }
      else
      {
         ossim_uint32 upperBound = (ossim_uint32)theConvolutionKernelList.size();
         ossim_uint32 idx;
         for(idx = 0; idx < upperBound; ++idx)
         {
            convolve(static_cast<ossim_uint8>(0),
                     input,
                     theConvolutionKernelList[idx]);
            input->loadTile(theTile.get());
         }
      }
      break;
   }
   case OSSIM_USHORT16:
   case OSSIM_USHORT11:
   {
      if(theConvolutionKernelList.size() == 1)
      {
         convolve(static_cast<ossim_uint16>(0),
                  input,
                  theConvolutionKernelList[0]);
      }
      else
      {
         ossim_uint32 upperBound = (ossim_uint32)theConvolutionKernelList.size();
         ossim_uint32 idx;
         for(idx = 0; idx < upperBound; ++idx)
         {
            convolve(static_cast<ossim_uint16>(0),
                     input,
                     theConvolutionKernelList[idx]);
            input->loadTile(theTile.get());
         }
      }
      break;
   }
   case OSSIM_SSHORT16:
   {
      if(theConvolutionKernelList.size() == 1)
      {
         convolve(static_cast<ossim_sint16>(0),
                  input,
                  theConvolutionKernelList[0]);
      }
      else
      {
         ossim_uint32 upperBound = (ossim_uint32)theConvolutionKernelList.size();
         ossim_uint32 idx;
         for(idx = 0; idx < upperBound; ++idx)
         {
            convolve(static_cast<ossim_sint16>(0),
                     input,
                     theConvolutionKernelList[idx]);
            input->loadTile(theTile.get());
         }
      }
      break;
   }
   case OSSIM_FLOAT:
   case OSSIM_NORMALIZED_FLOAT:
   {
      if(theConvolutionKernelList.size() == 1)
      {
         convolve(static_cast<float>(0),
                  input,
                  theConvolutionKernelList[0]);
      }
      else
      {
         ossim_uint32 upperBound = (ossim_uint32)theConvolutionKernelList.size();
         ossim_uint32 idx;
         for(idx = 0; idx < upperBound; ++idx)
         {
            convolve(static_cast<float>(0),
                     input,
                     theConvolutionKernelList[idx]);
            input->loadTile(theTile.get());
         }
      }
      break;
   }
   case OSSIM_DOUBLE:
   case OSSIM_NORMALIZED_DOUBLE:
   {
      if(theConvolutionKernelList.size() == 1)
      {
         convolve(static_cast<double>(0),
                  input,
                  theConvolutionKernelList[0]);
      }
      else
      {
         ossim_uint32 upperBound = (ossim_uint32)theConvolutionKernelList.size();
         ossim_uint32 idx;
         for(idx = 0; idx < upperBound; ++idx)
         {
            convolve(static_cast<double>(0),
                     input,
                     theConvolutionKernelList[idx]);
            input->loadTile(theTile.get());
         }
      }
      break;
   }
   default:
   {
      theTile->loadTile(input.get());
   }
   }
   theTile->validate();
   return theTile;
}
   
template <class T>
void ossimConvolutionSource::convolve(T /* dummyVariable */,
                                      ossimRefPtr<ossimImageData> inputTile,
                                      ossimDiscreteConvolutionKernel* kernel)
{
   ossimIpt startOrigin   = theTile->getOrigin();

   // Make sure that the patch is not empty or NULL
   //
   ossimIpt startDelta(startOrigin.x - inputTile->getOrigin().x,
                       startOrigin.y - inputTile->getOrigin().y);

   ossimDataObjectStatus status = inputTile->getDataObjectStatus();
   // let's setup some variables that we will need to do the
   // convolution algorithm.
   //
   ossimIrect patchRect   = inputTile->getImageRectangle();
   long tileHeight        = theTile->getHeight();
   long tileWidth         = theTile->getWidth();
   long outputBands       = theTile->getNumberOfBands();
   long convolutionWidth  = kernel->getWidth();
   long convolutionHeight = kernel->getHeight();
   long convolutionOffsetX= convolutionWidth/2;
   long convolutionOffsetY= convolutionHeight/2;
   long patchWidth        = patchRect.width();
   long convolutionTopLeftOffset = 0;
   long convolutionCenterOffset  = 0;
   
   long outputOffset          = 0;
   T np = 0;
   
   const double minPix  = ossim::defaultMin(getOutputScalarType());
   const double maxPix  = ossim::defaultMax(getOutputScalarType());
//   const double* maxPix  = inputTile->getMaxPix();
   const double* nullPix = inputTile->getNullPix();
   double convolveResult = 0;
   
   if(status == OSSIM_PARTIAL) // must check for NULLS
   {
      for(long y = 0; y <tileHeight; y++)
      {
         convolutionCenterOffset  = patchWidth*(startDelta.y + y) + startDelta.x;
         convolutionTopLeftOffset = patchWidth*(startDelta.y + y - convolutionOffsetY) + startDelta.x-convolutionOffsetX;
         
         for(long x =0; x < tileWidth; x++)
         {
            if(!inputTile->isNull(convolutionCenterOffset))
            {
               for(long b = 0; b < outputBands; ++b)
               {
                  T* buf    = (T*)(inputTile->getBuf(b)) + convolutionTopLeftOffset;
                  T* outBuf = (T*)(theTile->getBuf(b));
                  kernel->convolveSubImage(buf,
                                           patchWidth,
                                           convolveResult,
                                           (T)nullPix[b]);

                  convolveResult = convolveResult < minPix? minPix:convolveResult;
                  convolveResult = convolveResult > maxPix? maxPix:convolveResult;
                  
                  outBuf[outputOffset] = (T)convolveResult;
               }
            }
            else
            {
               theTile->setNull(outputOffset);
               
            }
            ++convolutionCenterOffset;
            ++convolutionTopLeftOffset;
            ++outputOffset;
         }
      }
   }
   else  // do not need to check for nulls here.
   {
      for(long b = 0; b < outputBands; ++b)
      {                  
         double convolveResult = 0;
         const T* buf = (const T*)inputTile->getBuf(b);
         T* outBuf    = (T*)(theTile->getBuf(b));
         np =(T)nullPix[b];
         outputOffset = 0;
         
         for(long y = 0; y <tileHeight; y++)
         {
            convolutionTopLeftOffset = patchWidth*(startDelta.y + y - convolutionOffsetY) + startDelta.x-convolutionOffsetX;
            
            for(long x =0; x < tileWidth; x++)
            {
               kernel->convolveSubImage(&buf[convolutionTopLeftOffset],
                                        patchWidth,
                                        convolveResult,
                                        np);
// NOT SURE IF I WANT TO CLAMP IN A CONVOLUTION SOURCE  
// seems better to clamp to a scalar range instead of an input min max
               convolveResult = convolveResult < minPix? (T)minPix:convolveResult;
               convolveResult = convolveResult > maxPix?(T)maxPix:convolveResult;
               outBuf[outputOffset] = (T)convolveResult;
               ++outputOffset;
               ++convolutionTopLeftOffset;
            }
         }
      }
   }
}

void ossimConvolutionSource::initialize()
{
   ossimImageSourceFilter::initialize();
   theTile = NULL;
}

void ossimConvolutionSource::allocate()
{
   if(theInputConnection)
   {
      theTile = ossimImageDataFactory::instance()->create(this,
                                                          theInputConnection);
      theTile->initialize();
   }
}

bool ossimConvolutionSource::saveState(ossimKeywordlist& kwl,
                                       const char* prefix)const
{
   ossim_uint32 numberOfMatrices = 0;
   for(ossim_uint32 m = 0; m < theConvolutionKernelList.size();++m)
   {
      if(theConvolutionKernelList[m])
      {
         ++numberOfMatrices;
         const NEWMAT::Matrix& kernel = theConvolutionKernelList[m]->getKernel();
         ossimString mPrefix = "m" +
                               ossimString::toString(numberOfMatrices) +
                               ".";
         kwl.add(prefix,
                 (mPrefix + "rows").c_str(),
                 kernel.Nrows(),
                 true);
         
         kwl.add(prefix,
                 (mPrefix + "cols").c_str(),
                 kernel.Ncols(),
                 true);
         
         for(ossim_int32 row = 0; row < kernel.Nrows(); ++row)
         {
            for(ossim_int32 col =0; col < kernel.Ncols(); ++col)
            {
               ossimString newPrefix = mPrefix +
                                       ossimString::toString(row+1) + "_" +
                                       ossimString::toString(col+1);
               kwl.add(prefix,
                       newPrefix,
                       kernel[row][col],
                       true);          
            }
         }
      }
   }

   
   kwl.add(prefix,
           NUMBER_OF_MATRICES,
           numberOfMatrices,
           true);
      
   return ossimImageSourceFilter::saveState(kwl, prefix);
}


bool ossimConvolutionSource::loadState(const ossimKeywordlist& kwl,
                                       const char* prefix)
{
   deleteConvolutionList();

   const char* numberOfMatrices = kwl.find(prefix, NUMBER_OF_MATRICES);

   ossim_int32 matrixCount = ossimString(numberOfMatrices).toLong();
   ossim_int32 numberOfMatches = 0;
   ossim_int32 index = 0;
   while(numberOfMatches < matrixCount)
   {
      ossimString newPrefix = prefix;
      newPrefix += ossimString("m");
      newPrefix += ossimString::toString(index);
      newPrefix += ossimString(".");

      const char* rows = kwl.find((newPrefix+NUMBER_OF_ROWS.key()).c_str());
      const char* cols = kwl.find((newPrefix+NUMBER_OF_COLS.key()).c_str());

      if(rows&&cols)
      {
         ++numberOfMatches;
         ossim_int32 numberOfRows = ossimString(rows).toLong();
         ossim_int32 numberOfCols = ossimString(cols).toLong();
         NEWMAT::Matrix convolutionMatrix(numberOfRows, numberOfCols);
         
         
         for(ossim_int32 r = 1; r <= numberOfRows; r++)
         {
            for(ossim_int32 c = 1; c <= numberOfCols; c++)
            {
               convolutionMatrix[r-1][c-1] = 0.0;
               
               ossimString value = ossimString::toString(r);
               value += "_";
               value += ossimString::toString(c);

               const char* v = kwl.find(newPrefix.c_str(),
                                        value.c_str());
               if(v)
               {
                  convolutionMatrix[r-1][c-1] = ossimString(v).toDouble();
               }
            }
         }
         theConvolutionKernelList.push_back(new ossimDiscreteConvolutionKernel(convolutionMatrix));
      }
      ++index;
   }
   setKernelInformation();
   return ossimImageSourceFilter::loadState(kwl, prefix);
}

void ossimConvolutionSource::setKernelInformation()
{
   ossim_uint32 index;
   
   if(theConvolutionKernelList.size() > 0)
   {
      theMaxKernelWidth  = theConvolutionKernelList[0]->getWidth();
      theMaxKernelHeight = theConvolutionKernelList[0]->getHeight();
      
      for(index = 1; index < theConvolutionKernelList.size(); ++index)
      {
         ossim_int32 w = theConvolutionKernelList[index]->getWidth();
         ossim_int32 h = theConvolutionKernelList[index]->getHeight();
         theMaxKernelWidth  = theMaxKernelWidth < w?w:theMaxKernelWidth;
         theMaxKernelHeight = theMaxKernelHeight < h?h:theMaxKernelHeight;
      }
   }
}

void ossimConvolutionSource::deleteConvolutionList()
{
   for(ossim_int32 index = 0; index < (ossim_int32)theConvolutionKernelList.size(); ++index)
   {
      delete theConvolutionKernelList[index];
   }

   theConvolutionKernelList.clear();
}

void ossimConvolutionSource::setConvolution(const NEWMAT::Matrix& convolutionMatrix, bool doWeightedAverage)
{
   std::vector<NEWMAT::Matrix> m;
   
   m.push_back(convolutionMatrix);
   
   setConvolutionList(m, doWeightedAverage);
}
