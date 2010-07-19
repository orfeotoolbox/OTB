//----------------------------------------------------------------------------
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
// $Id: ossimImageMpiSWriterSequenceConnection.cpp 17206 2010-04-25 23:20:40Z dburken $
//----------------------------------------------------------------------------

#include <ossim/ossimConfig.h> /* To pick up OSSIM_HAS_MPI. */

#ifdef OSSIM_HAS_MPI
#  if OSSIM_HAS_MPI
#    include <mpi.h>
#  endif
#endif

#include <ossim/parallel/ossimImageMpiSWriterSequenceConnection.h>
#include <ossim/parallel/ossimMpi.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimNotifyContext.h>

static ossimTrace traceDebug = ossimTrace("ossimImageMpiSWriterSequenceConnection:debug");

RTTI_DEF1(ossimImageMpiSWriterSequenceConnection, "ossimImageMpiSWriterSequenceConnection", ossimImageSourceSequencer)

ossimImageMpiSWriterSequenceConnection::ossimImageMpiSWriterSequenceConnection(ossimObject* owner,
                                                                               long numberOfTilesToBuffer)
   :ossimImageSourceSequencer(NULL,
                              owner),
    theNumberOfTilesToBuffer(numberOfTilesToBuffer),
    theOutputTile(NULL)
{
   theRank = 0;
   theNumberOfProcessors = 1;
   theNumberOfTilesToBuffer = ((theNumberOfTilesToBuffer>0)?theNumberOfTilesToBuffer:2);

#ifdef OSSIM_HAS_MPI   
#  if OSSIM_HAS_MPI
   MPI_Comm_rank(MPI_COMM_WORLD, &theRank);
   MPI_Comm_size(MPI_COMM_WORLD, &theNumberOfProcessors);
#  endif
#endif
   
   if(theRank!=0)
   {
      theCurrentTileNumber = theRank -1;
   }
   else
   {
      theCurrentTileNumber = 0;
   }
}

ossimImageMpiSWriterSequenceConnection::ossimImageMpiSWriterSequenceConnection(ossimImageSource* inputSource,
                                                                               ossimObject* owner,
                                                                               long numberOfTilesToBuffer)
   :ossimImageSourceSequencer(inputSource,
                                 owner),
    theNumberOfTilesToBuffer(numberOfTilesToBuffer),
    theOutputTile(NULL)
{
   theRank = 0;
   theNumberOfProcessors = 1;
   theNumberOfTilesToBuffer = ((theNumberOfTilesToBuffer>0)?theNumberOfTilesToBuffer:2);
#if OSSIM_HAS_MPI
   MPI_Comm_rank(MPI_COMM_WORLD, &theRank);
   MPI_Comm_size(MPI_COMM_WORLD, &theNumberOfProcessors);
#endif
   if(theRank!=0)
   {
      theCurrentTileNumber = theRank -1;
   }
   else
   {
      theCurrentTileNumber = 0;
   }   
}

ossimImageMpiSWriterSequenceConnection::~ossimImageMpiSWriterSequenceConnection()
{   
   deleteOutputTiles();
}

void ossimImageMpiSWriterSequenceConnection::deleteOutputTiles()
{
   if(theOutputTile)
   {
      delete [] theOutputTile;
      theOutputTile = NULL;
   }
}

void ossimImageMpiSWriterSequenceConnection::initialize()
{
  ossimImageSourceSequencer::initialize();

  theCurrentTileNumber = theRank-1;
  if(theOutputTile)
  {
     deleteOutputTiles();
  }
  
  if(theInputConnection)
  {
     theOutputTile = new ossimRefPtr<ossimImageData>[theNumberOfTilesToBuffer];
     
     for(long index = 0; index < theNumberOfTilesToBuffer; ++index)
     {
        theOutputTile[index] = ossimImageDataFactory::instance()->create(this,
                                                                         this);
        theOutputTile[index]->initialize();
     }
  }
}

void ossimImageMpiSWriterSequenceConnection::setToStartOfSequence()
{
   ossimImageSourceSequencer::setToStartOfSequence();
   if(theRank != 0)
   {
      // we will subtract one since the masters job is just
      // writting and not issue getTiles.
      theCurrentTileNumber = theRank-1;

   }
   else
   {
      // the master will start at 0
      theCurrentTileNumber = 0;
   }
}

void ossimImageMpiSWriterSequenceConnection::slaveProcessTiles()
{
#ifdef OSSIM_HAS_MPI 
#  if OSSIM_HAS_MPI
   ossimEndian endian;
   ossim_uint32 numberOfTiles    = getNumberOfTiles();
   long currentSendRequest = 0;
   long numberOfTilesSent  = 0;
   int errorValue= 0;
   MPI_Request *requests   = new MPI_Request[theNumberOfTilesToBuffer];
   for (int i = 0; i < theNumberOfTilesToBuffer; ++i)
   {
      requests[i] = MPI_REQUEST_NULL;
   }

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimImageMpiSWriterSequenceConnection::slaveProcessTiles(): entering slave and will look at " << numberOfTiles << " tiles" << std::endl;
   }
   while(theCurrentTileNumber < numberOfTiles)
   {
      ossimRefPtr<ossimImageData> data = ossimImageSourceSequencer::getTile(theCurrentTileNumber);

      // if the current send requests have looped around
      // make sure we wait to see if it was sent
      //
      errorValue = MPI_Wait(&requests[currentSendRequest], MPI_STATUS_IGNORE);
      requests[currentSendRequest] = MPI_REQUEST_NULL;
      if(data.valid() &&
         (data->getDataObjectStatus()!=OSSIM_NULL)&&
         (data->getDataObjectStatus()!=OSSIM_EMPTY))
      {
         theOutputTile[currentSendRequest]->setImageRectangle(data->getImageRectangle());
         theOutputTile[currentSendRequest]->initialize();
         
         theOutputTile[currentSendRequest]->loadTile(data.get());
         theOutputTile[currentSendRequest]->setDataObjectStatus(data->getDataObjectStatus());

         if(traceDebug())
         {
            if(data->getDataObjectStatus() == OSSIM_EMPTY)
            {
               ossimNotify(ossimNotifyLevel_DEBUG)
                  << "DEBUG ossimImageMpiSWriterSequenceConnection::slaveProcessTiles(): In salve = "
                  << theRank << " tile is empty" << std::endl;
            }
         }
      }
      else
      {
         if(traceDebug())
         {
            if(!data)
            {
               ossimNotify(ossimNotifyLevel_DEBUG)
                  << "DEBUG ossimImageMpiSWriterSequenceConnection::slaveProcessTiles(): In slave = "
                  << theRank << " ptr is null " << std::endl;
            }
            else
            {
               ossimNotify(ossimNotifyLevel_DEBUG)
                  << "DEBUG ossimImageMpiSWriterSequenceConnection::slaveProcessTiles(): In slave = " << theRank << " tile is empty" << std::endl;
            }
         }
         theOutputTile[currentSendRequest]->makeBlank();
      }

      void* buf = theOutputTile[currentSendRequest]->getBuf();
      if((endian.getSystemEndianType()!=OSSIM_BIG_ENDIAN)&&
         (theOutputTile[currentSendRequest]->getScalarType()!=OSSIM_UINT8))
      {
         endian.swap(theOutputTile[currentSendRequest]->getScalarType(),
                     buf,
                     theOutputTile[currentSendRequest]->getSize());
      }
      errorValue = MPI_Isend(buf,
                             theOutputTile[currentSendRequest]->getSizeInBytes(),
                             MPI_UNSIGNED_CHAR,
                             0,
                             0,
                             MPI_COMM_WORLD,
                             &requests[currentSendRequest]);
#if 0      
      switch(theOutputTile[currentSendRequest]->getScalarType())
      {
         case OSSIM_UINT8:
         {
            errorValue = MPI_Isend(buf,
                                    theOutputTile[currentSendRequest]->getSize(),
                                    MPI_UNSIGNED_CHAR,
                                    0,
                                    0,
                                    MPI_COMM_WORLD,
                                    &requests[currentSendRequest]);
            break;
         }
         case OSSIM_SINT8:
         {
            errorValue = MPI_Isend(buf,
                                    theOutputTile[currentSendRequest]->getSize(),
                                    MPI_CHAR,
                                    0,
                                    0,
                                    MPI_COMM_WORLD,
                                    &requests[currentSendRequest]);
            break;
         }
         case OSSIM_UINT16:
         {
            errorValue = MPI_Isend(buf,
                                    theOutputTile[currentSendRequest]->getSize(),
                                    MPI_UNSIGNED_SHORT,
                                    0,
                                    0,
                                    MPI_COMM_WORLD,
                                    &requests[currentSendRequest]);
            break;
         }
         case OSSIM_SINT16:
         {
            errorValue = MPI_Isend(buf,
                                    theOutputTile[currentSendRequest]->getSize(),
                                    MPI_SHORT,
                                    0,
                                    0,
                                    MPI_COMM_WORLD,
                                    &requests[currentSendRequest]);
            break;
         }
         case OSSIM_UINT32:
         {
            errorValue = MPI_Isend(buf,
                                    theOutputTile[currentSendRequest]->getSize(),
                                    MPI_UNSIGNED_LONG,
                                    0,
                                    0,
                                    MPI_COMM_WORLD,
                                    &requests[currentSendRequest]);
            break;
         }
         case OSSIM_SINT32:
         {
            errorValue = MPI_Isend(buf,
                                    theOutputTile[currentSendRequest]->getSize(),
                                    MPI_LONG,
                                    0,
                                    0,
                                    MPI_COMM_WORLD,
                                    &requests[currentSendRequest]);
            break;
         }
         case OSSIM_FLOAT32:
         case OSSIM_NORMALIZED_FLOAT:
         {
            errorValue = MPI_Isend(buf,
                                    theOutputTile[currentSendRequest]->getSize(),
                                    MPI_FLOAT,
                                    0,
                                    0,
                                    MPI_COMM_WORLD,
                                    &requests[currentSendRequest]);
            break;
         }
         case OSSIM_FLOAT64:
         case OSSIM_NORMALIZED_DOUBLE:
         {
            errorValue = MPI_Isend(buf,
                                    theOutputTile[currentSendRequest]->getSize(),
                                    MPI_DOUBLE,
                                    0,
                                    0,
                                    MPI_COMM_WORLD,
                                    &requests[currentSendRequest]);
            break;
         }
         default:
            break;
      }
#endif
      theCurrentTileNumber += (theNumberOfProcessors-1);
      numberOfTilesSent++;
      currentSendRequest++;
      currentSendRequest %= theNumberOfTilesToBuffer;
   }
   ossim_int32 tempCount = 0;
   // must wait in the correct order
   //
   while(tempCount < theNumberOfTilesToBuffer)
   {
      currentSendRequest++;
      currentSendRequest %= theNumberOfTilesToBuffer;
      
      errorValue = MPI_Wait(&requests[currentSendRequest], MPI_STATUS_IGNORE);
      ++tempCount;
   }
   
//   MPI_Waitall(theNumberOfTilesToBuffer,
//               requests,
//               MPI_STATUS_IGNORE);
   
   delete [] requests;
#  endif
#endif
}


ossimRefPtr<ossimImageData> ossimImageMpiSWriterSequenceConnection::getNextTile(
   ossim_uint32 /* resLevel */)
{
   ossimNotify(ossimNotifyLevel_FATAL)
      << "FATAL ossimImageMpiSWriterSequenceConnection::getNextTile(): "
      << "should not be called" << std::endl;
   return ossimRefPtr<ossimImageData>();
}

