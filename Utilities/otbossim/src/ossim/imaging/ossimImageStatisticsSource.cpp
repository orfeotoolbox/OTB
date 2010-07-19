//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
#include <ossim/imaging/ossimImageStatisticsSource.h>
#include <ossim/imaging/ossimImageSource.h>
#include <ossim/imaging/ossimImageSourceSequencer.h>


ossimImageStatisticsSource::ossimImageStatisticsSource()
      :ossimSource(0,
                   1,
                   0,
                   true,
                   false)
{
}

ossimImageStatisticsSource::~ossimImageStatisticsSource()
{
   
}

void ossimImageStatisticsSource::computeStatistics()
{
   ossimImageSource* anInterface = PTR_CAST(ossimImageSource,
                                                     getInput());

   if(anInterface&&isSourceEnabled())
   {
      clearStatistics();
      ossimScalarType outputScalarType = anInterface->getOutputScalarType();

      switch(outputScalarType)
      {
         case OSSIM_UCHAR:
         {
            computeStatisticsTemplate((ossim_uint8)0);
            break;
         }
         case OSSIM_USHORT16:
         case OSSIM_USHORT11:
         {
            computeStatisticsTemplate((ossim_uint16)0);
            break;
         }
         case OSSIM_SSHORT16:
         {
            computeStatisticsTemplate((ossim_sint16)0);
            break;
         }
         case OSSIM_DOUBLE:
         case OSSIM_NORMALIZED_DOUBLE:
         {
            computeStatisticsTemplate((ossim_float64)0);
            break;
         }
         case OSSIM_FLOAT:
         case OSSIM_NORMALIZED_FLOAT:
         {
            computeStatisticsTemplate((ossim_float32)0);
            break;
         }
		 default :
		 {
			 break;
		 }
      }
   }
}

bool ossimImageStatisticsSource::canConnectMyInputTo(ossim_int32 inputIndex,
                                                     const ossimConnectableObject* object)const
{
   return (PTR_CAST(ossimImageSource, object)&&(inputIndex < 1));
}

template <class T>
void ossimImageStatisticsSource::computeStatisticsTemplate(T /* dummyVariable */)
{
   ossimRefPtr<ossimImageSourceSequencer> sequencer = new ossimImageSourceSequencer;

   sequencer->connectMyInputTo(getInput());
   sequencer->setToStartOfSequence();
   ossim_uint32 bands = sequencer->getNumberOfOutputBands();

   if(bands)
   {
      setStatsSize(bands);
      ossimRefPtr<ossimImageData> dataObject;
      
      while( (dataObject=sequencer->getNextTile()).valid() )
      {
		  ossim_uint32 bandIdx = 0;
		  bands                = dataObject->getNumberOfBands();
		  ossimDataObjectStatus status = dataObject->getDataObjectStatus();
		  if((status != OSSIM_EMPTY)&&
			 (dataObject->getBuf()))
		  {
			  ossim_uint32 offsetMax = dataObject->getWidth()*dataObject->getHeight();
			  for(bandIdx = 0; bandIdx < bands; ++bandIdx)
			  {
				  ossim_float64 pixelCount = 0.0;
				  ossim_uint32 offset = 0;
				  T* dataPtr   = static_cast<T*>(dataObject->getBuf(bandIdx));
				  T nullPixel = static_cast<T>(dataObject->getNullPix(bandIdx)); 
				  for(offset = 0; offset < offsetMax; ++offset)
				  {
					  if((*dataPtr) != nullPixel)
					  {
						  theMean[bandIdx] += *dataPtr;
						  if((*dataPtr) < theMin[bandIdx])
						  {
							  theMin[bandIdx] = (*dataPtr);
						  }
						  if((*dataPtr) > theMax[bandIdx])
						  {
							  theMax[bandIdx] = (*dataPtr);
						  }
						  ++pixelCount;
					  }
					  ++dataPtr;
				  }
				  if(pixelCount > 0)
				  {
					  theMean[bandIdx] /= pixelCount;
				  }
			  }
		  }
      }
   }
   
   sequencer->disconnect();
   sequencer = 0;
}

const std::vector<ossim_float64>& ossimImageStatisticsSource::getMean()const
{
   return theMean;
}

const std::vector<ossim_float64>& ossimImageStatisticsSource::getMin()const
{
   return theMin;
}

const std::vector<ossim_float64>& ossimImageStatisticsSource::getMax()const
{
   return theMax;
}

void ossimImageStatisticsSource::clearStatistics()
{
   theMean.clear();
   theMin.clear();
   theMax.clear();
}

void ossimImageStatisticsSource::setStatsSize(ossim_uint32 size)
{
   theMean.resize(size);
   theMin.resize(size);
   theMax.resize(size);

   std::fill(theMean.begin(),
             theMean.end(),
             (ossim_float64)0.0);
   std::fill(theMin.begin(),
             theMin.end(),
             OSSIM_DEFAULT_MAX_PIX_DOUBLE);
   std::fill(theMax.begin(),
             theMax.end(),
             (ossim_float64)OSSIM_DEFAULT_MIN_PIX_DOUBLE);
}
