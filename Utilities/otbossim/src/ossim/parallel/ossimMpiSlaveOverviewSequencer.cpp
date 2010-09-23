//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Class definition for mpi slave sequencer for building
// overview files.
// 
//----------------------------------------------------------------------------
// $Id: ossimMpiSlaveOverviewSequencer.cpp 17870 2010-08-12 13:12:32Z sbortman $

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
   m_rank(0),
   m_numberOfProcessors(1)
{
#if OSSIM_HAS_MPI   
   MPI_Comm_rank(MPI_COMM_WORLD, &m_rank);
   MPI_Comm_size(MPI_COMM_WORLD, &m_numberOfProcessors);
#endif
   
   if( m_rank != 0 )
   {
      //---
      // Master process (rank 0) does not resample tiles so our process rank
      // is 1 then we'll start at tile 0, rank 2 starts at 1, rank 3 starts
      // at 2 and so on...
      //---
      m_currentTileNumber = m_rank -1;
   }
   else
   {
      m_currentTileNumber = 0;
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

   m_currentTileNumber = m_rank-1;
}

void ossimMpiSlaveOverviewSequencer::setToStartOfSequence()
{
   if(m_rank != 0)
   {
      //---
      // Subtract one since the masters job is just writing and not issue
      // getTiles.
      //---
      m_currentTileNumber = m_rank-1;
   }
   else
   {
      // the master will start at 0
      m_currentTileNumber = 0;
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
   if (m_imageHandler.valid())
   {
      if (m_imageHandler->getOutputScalarType() != OSSIM_UINT8)
      {
         if (ossim::byteOrder() != OSSIM_BIG_ENDIAN)
         {
            endian = new ossimEndian();
         }
      }
   }

   // Total number of tiles for all processes.
   ossim_uint32 numberOfTiles = getNumberOfTiles();

   int         errorValue; // Needed for MPI_Isend and MPI_Wait.
   MPI_Request request;    // Needed for MPI_Isend and MPI_Wait.

   while(m_currentTileNumber < numberOfTiles)
   {
      ossimRefPtr<ossimImageData> data = ossimOverviewSequencer::getNextTile();

      void* buf = data->getBuf();

      // Data always sent in big endian byte order.
      if (endian)
      {
         endian->swap(m_tile->getScalarType(), buf, m_tile->getSize());
      }

      // Send the buffer to the master process.
      request = MPI_REQUEST_NULL;
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
      errorValue = MPI_Wait(&request, MPI_STATUS_IGNORE);

      //---
      // If we have eight processes only seven are used for resampling tiles,
      // so we would process every seventh tile.
      // 
      // The call to getNextTile has already incremented m_currentTileNumber
      // by one so adjust accordingly.
      //---
      if (m_numberOfProcessors>2)
      {
         m_currentTileNumber += (m_numberOfProcessors-2);
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
