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
// $Id: ossimMpiMasterOverviewSequencer.cpp 17870 2010-08-12 13:12:32Z sbortman $

#include <ossim/parallel/ossimMpiMasterOverviewSequencer.h>
#include <ossim/ossimConfig.h> /* To pick up OSSIM_HAS_MPI. */
#include <ossim/base/ossimEndian.h>

#if OSSIM_HAS_MPI
#  include <mpi.h>
#endif

ossimMpiMasterOverviewSequencer::ossimMpiMasterOverviewSequencer()
   :
   ossimOverviewSequencer(),
   m_rank(0),
   m_numberOfProcessors(1)
{
#if OSSIM_HAS_MPI   
   MPI_Comm_rank(MPI_COMM_WORLD, &m_rank);
   MPI_Comm_size(MPI_COMM_WORLD, &m_numberOfProcessors);
#endif

   //---
   // Since this is the master sequencer it should always be rank 0; else,
   // we have a coding error.  Since we have the getNextTile implemented to
   // recieve from the slave processes we always start the tile count at 0.
   //---
   m_currentTileNumber = 0;
}

ossimMpiMasterOverviewSequencer::~ossimMpiMasterOverviewSequencer()
{
}

void ossimMpiMasterOverviewSequencer::initialize()
{
   ossimOverviewSequencer::initialize();
}

void ossimMpiMasterOverviewSequencer::setToStartOfSequence()
{
   m_currentTileNumber = 0;
}

ossimRefPtr<ossimImageData> ossimMpiMasterOverviewSequencer::getNextTile()
{
   if ( m_dirtyFlag )
   {
      //---
      // If this happens we have a coding error.  Someone started sequencing
      // without initializing us properly.
      //---
      return ossimRefPtr<ossimImageData>();
   }
   
#if OSSIM_HAS_MPI

   //---
   // Using mpi.  The slaves will send us tiles to be returned by this method.
   // They will alway be sent in big endian (network byte order) so we must
   // swap back if the scalar type is not 8 bit and the system byte order is
   // little endian. We will use the endian pointer itself as a flag to swap.
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

   int         errorValue;

   // Buffer to receive the data from slaves.
   void* buf = m_tile->getBuf();

   // Total number of tiles to process...
   ossim_uint32 numberOfTiles = getNumberOfTiles();
   
   if(m_currentTileNumber >= numberOfTiles)
   {
      return ossimRefPtr<ossimImageData>();
   }
   
   errorValue = MPI_Recv(buf,
                         m_tile->getSizeInBytes(),
                         MPI_UNSIGNED_CHAR,
                         m_currentTileNumber%(m_numberOfProcessors-1)+1,
                         0,
                         MPI_COMM_WORLD,
                         MPI_STATUS_IGNORE);

   // Data always sent in big endian order.
   if ( endian )
   {
      endian->swap(m_tile->getScalarType(), buf, m_tile->getSize());
   }

   // Get the output rectangle.
   ossimIrect outputRect;
   getOutputTileRectangle(outputRect);

   // Capture the output rectangle.
   m_tile->setImageRectangle(outputRect);

   // Set the tile status.
   m_tile->validate();

   // Increment the tile index.
   ++m_currentTileNumber;

   // cleanup...
   if ( endian )
   {
      delete endian;
      endian = 0;
   }

   return m_tile;

#else

   // Not compiled with mpi.
   return ossimOverviewSequencer::getNextTile();

#endif /* End of #if OSSIM_HAS_MPI */

}
