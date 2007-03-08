//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Class definition for mpi master sequencer for building
// overview files.
// 
//----------------------------------------------------------------------------
// $Id: ossimMpiSlaveOverviewSequencer.cpp 10152 2006-12-30 20:54:39Z dburken $

#include <ossim/parallel/ossimMpiSlaveOverviewSequencer.h>
#include <ossim/ossimConfig.h>        /* To pick up OSSIM_HAS_MPI. */
#include <ossim/base/ossimCommon.h>   /* For get byte order. */
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/imaging/ossimImageData.h>



#if OSSIM_HAS_MPI
#  include <mpi.h>
#endif

ossimMpiSlaveOverviewSequencer::ossimMpiSlaveOverviewSequencer()
   :
   ossimOverviewSequencer(),
   theRank(0),
   theNumberOfProcessors(1)
{
#if OSSIM_HAS_MPI   
   MPI_Comm_rank(MPI_COMM_WORLD, &theRank);
   MPI_Comm_size(MPI_COMM_WORLD, &theNumberOfProcessors);
#endif
   
   if( theRank != 0 )
   {
      //---
      // Master process (rank 0) does not resample tiles so our process rank
      // is 1 then we'll start at tile 0, rank 2 starts at 1, rank 3 starts
      // at 2 and so on...
      //---
      theCurrentTileNumber = theRank -1;
   }
   else
   {
      theCurrentTileNumber = 0;
   }   
}

ossimMpiSlaveOverviewSequencer::~ossimMpiSlaveOverviewSequencer()
{
}

bool ossimMpiSlaveOverviewSequencer::isMaster()const
{
   return false;
}

void ossimMpiSlaveOverviewSequencer::initialize()
{
   ossimOverviewSequencer::initialize();

   theCurrentTileNumber = theRank-1;
}

void ossimMpiSlaveOverviewSequencer::setToStartOfSequence()
{
   if(theRank != 0)
   {
      //---
      // Subtract one since the masters job is just writing and not issue
      // getTiles.
      //---
      theCurrentTileNumber = theRank-1;
   }
   else
   {
      // the master will start at 0
      theCurrentTileNumber = 0;
   }
}

void ossimMpiSlaveOverviewSequencer::slaveProcessTiles()
{
#if OSSIM_HAS_MPI 

   //---
   // Bytes must be swapped to big endian (network byte order) if the scalar
   // type is not 8 bit and the system byte order is little endian.  We will
   // use the endian pointer itself as a flag to swap.
   //---
   ossimEndian* endian = 0;
   if (theImageHandler)
   {
      if (theImageHandler->getOutputScalarType() != OSSIM_UINT8)
      {
         if (ossimGetByteOrder() != OSSIM_BIG_ENDIAN)
         {
            endian = new ossimEndian();
         }
      }
   }

   // Total number of tiles for all processes.
   ossim_uint32 numberOfTiles = getNumberOfTiles();

   int         errorValue; // Needed for MPI_Isend and MPI_Wait.
   MPI_Status  status;     // Needed for MPI_Wait.
   MPI_Request request;    // Needed for MPI_Isend and MPI_Wait.

   while(theCurrentTileNumber < numberOfTiles)
   {
      ossimRefPtr<ossimImageData> data = ossimOverviewSequencer::getNextTile();

      void* buf = data->getBuf();

      // Data always sent in big endian byte order.
      if (endian)
      {
         endian->swap(theTile->getScalarType(), buf, theTile->getSize());
      }

      // Send the buffer to the master process.
      request = 0;
      errorValue = MPI_Isend(buf,
                             data->getSizeInBytes(),
                             MPI_UNSIGNED_CHAR,
                             0,
                             0,
                             MPI_COMM_WORLD,
                             &request);

      //---
      // Wait for send to complete before we overwrite the buffer with the
      // next tile.
      //---
      errorValue = MPI_Wait(&request, &status);

      //---
      // If we have eight processes only seven are used for resampling tiles,
      // so we would process every seventh tile.
      // 
      // The call to getNextTile has already incremented theCurrentTileNumber
      // by one so adjust accordingly.
      //---
      if (theNumberOfProcessors>2)
      {
         theCurrentTileNumber += (theNumberOfProcessors-2);
      }
      
   } // End of while loop through number of tiles.

   // cleanup...
   if (endian)
   {
      delete endian;
      endian = 0;
   }
   
#endif /* End of #if OSSIM_HAS_MPI */
   
}
