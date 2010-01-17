//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description:
//
// This class is specific to the Master connection and is optimized
// for the writer sequence and batch processing chain.  For the Slave
// implementation look for the ossimImageMpiSWriterSequenceConnection.cc and .h
// files.  The Master connection is currently implemented to allways do
// a recieve and does no processing itself.  The slave connection does
// all the actual work and processing.
//
//*******************************************************************
//  $Id: ossimImageMpiMWriterSequenceConnection.cpp 12099 2007-12-01 16:09:36Z dburken $


#include <ossim/ossimConfig.h> /* To pick up OSSIM_HAS_MPI. */

#ifdef OSSIM_HAS_MPI
#  if OSSIM_HAS_MPI
#    include <mpi.h>
#  endif
#endif

#include <ossim/parallel/ossimImageMpiMWriterSequenceConnection.h>
#include <ossim/parallel/ossimMpi.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimEndian.h>

static ossimTrace traceDebug = ossimTrace("ossimImageMpiMWriterSequenceConnection:debug");

RTTI_DEF1(ossimImageMpiMWriterSequenceConnection, "ossimImageMpiMWriterSequenceConnection", ossimImageSourceSequencer)


ossimImageMpiMWriterSequenceConnection::ossimImageMpiMWriterSequenceConnection(
   ossimImageSource* inputSource,
   ossimObject* owner)
   :ossimImageSourceSequencer(inputSource, owner),
    theOutputTile(NULL)
{
   theRank = 0;
   theNumberOfProcessors = 1;

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
   theNeedToSendRequest = true;
}

ossimImageMpiMWriterSequenceConnection::ossimImageMpiMWriterSequenceConnection(ossimObject* owner)
   :ossimImageSourceSequencer(NULL, owner),
    theOutputTile(NULL)
{
   theRank = 0;
   theNumberOfProcessors = 1;
   
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
   theNeedToSendRequest = true;
}

ossimImageMpiMWriterSequenceConnection::~ossimImageMpiMWriterSequenceConnection()
{
}

void ossimImageMpiMWriterSequenceConnection::initialize()
{
  ossimImageSourceSequencer::initialize();

  theCurrentTileNumber = theRank;//-1;
  theOutputTile = NULL;
  
  if(theInputConnection)
  {
     theOutputTile = ossimImageDataFactory::instance()->create(this, this);
     theOutputTile->initialize();
  }
}

void ossimImageMpiMWriterSequenceConnection::setToStartOfSequence()
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

/*!
 * Will allow you to get the next tile in the sequence.
 */
ossimRefPtr<ossimImageData> ossimImageMpiMWriterSequenceConnection::getNextTile(ossim_uint32 resLevel)
{
#if OSSIM_HAS_MPI
   ossimEndian endian;
   if(!theOutputTile)
   {
      initialize();
      if (!theOutputTile)
      {
         return theOutputTile;
      }
   }
   
   void* buf = theOutputTile->getBuf();

   ossim_uint32 numberOfTiles = getNumberOfTiles();
   int errorValue = 0;
   
   if(theCurrentTileNumber >= numberOfTiles)
   {
      return NULL;
   }
   errorValue = MPI_Recv(buf,
                         theOutputTile->getSizeInBytes(),
                         MPI_UNSIGNED_CHAR,
                         theCurrentTileNumber%(theNumberOfProcessors-1)+1,
                         0,
                         MPI_COMM_WORLD,
                         MPI_STATUS_IGNORE);
   if((endian.getSystemEndianType()!=OSSIM_BIG_ENDIAN)&&
      (theOutputTile->getScalarType()!=OSSIM_UINT8))
   {
      endian.swap(theOutputTile->getScalarType(),
                  buf,
                  theOutputTile->getSize());
   }
   
#if 0
   // Since MPI can run on different platforms
   // I am not sure if MPI has built in byte
   // swap.  If so I will name each datatype
   // so it can handle byte swap.  Does anyone
   // know if MPI does byte swapping.
   switch(theOutputTile->getScalarType())
   {
      case OSSIM_UINT8:
      {
         // we add 1 to the rank so we don't wait on a message
         // to ourself (rank = 0).
         errorValue = MPI_Recv(buf,
                               theOutputTile->getSize(),
                               MPI_UNSIGNED_CHAR,
                               theCurrentTileNumber%(theNumberOfProcessors-1)+1,
                               0,
                               MPI_COMM_WORLD,
                               MPI_STATUS_IGNORE);
         break;
      }
      case OSSIM_SINT8:
      {
         // we add 1 to the rank so we don't wait on a message
         // to ourself (rank = 0).
         errorValue = MPI_Recv(buf,
                               theOutputTile->getSize(),
                               MPI_CHAR,
                               theCurrentTileNumber%(theNumberOfProcessors-1)+1,
                               0,
                               MPI_COMM_WORLD,
                               MPI_STATUS_IGNORE);
         break;
      }
      case OSSIM_UINT16:
      case OSSIM_USHORT11:
      {
         errorValue = MPI_Recv(buf,
                               theOutputTile->getSize(),
                               MPI_UNSIGNED_SHORT,
                               theCurrentTileNumber%(theNumberOfProcessors-1)+1,
                               0,
                               MPI_COMM_WORLD,
                               MPI_STATUS_IGNORE);
         break;
      }
      case OSSIM_SINT16:
      {
         errorValue = MPI_Recv(buf,
                               theOutputTile->getSize(),
                               MPI_SHORT,
                               theCurrentTileNumber%(theNumberOfProcessors-1)+1,
                               0,
                               MPI_COMM_WORLD,
                               MPI_STATUS_IGNORE);
         break;
      }
      case OSSIM_SINT32:
      {
         errorValue = MPI_Recv(buf,
                               theOutputTile->getSize(),
                               MPI_LONG,
                               theCurrentTileNumber%(theNumberOfProcessors-1)+1,
                               0,
                               MPI_COMM_WORLD,
                               MPI_STATUS_IGNORE);
         break;
      }
      case OSSIM_UINT32:
      {
         errorValue = MPI_Recv(buf,
                               theOutputTile->getSize(),
                               MPI_UNSIGNED_LONG,
                               theCurrentTileNumber%(theNumberOfProcessors-1)+1,
                               0,
                               MPI_COMM_WORLD,
                               MPI_STATUS_IGNORE);
         break;
      }
      case OSSIM_FLOAT32:
      case OSSIM_NORMALIZED_FLOAT:
      {
         errorValue = MPI_Recv(buf,
                               theOutputTile->getSize(),
                               MPI_FLOAT,
                               theCurrentTileNumber%(theNumberOfProcessors-1)+1,
                               0,
                               MPI_COMM_WORLD,
                               MPI_STATUS_IGNORE);
         break;
      }
      case OSSIM_FLOAT64:
      case OSSIM_NORMALIZED_DOUBLE:
      {
         errorValue = MPI_Recv(buf,
                               theOutputTile->getSize(),
                               MPI_DOUBLE,
                               theCurrentTileNumber%(theNumberOfProcessors-1)+1,
                               0,
                               MPI_COMM_WORLD,
                               MPI_STATUS_IGNORE);
         break;
      }
      default:
      {
         break;
      }
   }
#endif
   ossimIpt origin;
   getTileOrigin(theCurrentTileNumber,
                 origin);
   theOutputTile->setOrigin(origin);
   theOutputTile->validate();
   ++theCurrentTileNumber;
   return theOutputTile;
#else
   return ossimImageSourceSequencer::getNextTile(resLevel);
#endif
   
}
