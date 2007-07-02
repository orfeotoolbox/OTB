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
// $Id: ossimMpiMasterOverviewSequencer.cpp 10273 2007-01-15 15:41:26Z dburken $

#include <ossim/parallel/ossimMpiMasterOverviewSequencer.h>
#include <ossim/ossimConfig.h> /* To pick up OSSIM_HAS_MPI. */
#include <ossim/base/ossimEndian.h>

#if OSSIM_HAS_MPI
#  include <mpi.h>
#endif

ossimMpiMasterOverviewSequencer::ossimMpiMasterOverviewSequencer()
   :
   ossimOverviewSequencer(),
   theRank(0),
   theNumberOfProcessors(1)
{
#if OSSIM_HAS_MPI   
   MPI_Comm_rank(MPI_COMM_WORLD, &theRank);
   MPI_Comm_size(MPI_COMM_WORLD, &theNumberOfProcessors);
#endif

   //---
   // Since this is the master sequencer it should always be rank 0; else,
   // we have a coding error.  Since we have the getNextTile implemented to
   // recieve from the slave processes we always start the tile count at 0.
   //---
   theCurrentTileNumber = 0;
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
   theCurrentTileNumber = 0;
}

ossimRefPtr<ossimImageData> ossimMpiMasterOverviewSequencer::getNextTile()
{
   if ( theDirtyFlag )
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

   int         errorValue;
   MPI_Status  status;

   // Buffer to receive the data from slaves.
   void* buf = theTile->getBuf();

   // Total number of tiles to process...
   ossim_uint32 numberOfTiles = getNumberOfTiles();
   
   if(theCurrentTileNumber >= numberOfTiles)
   {
      return ossimRefPtr<ossimImageData>();
   }
   
   errorValue = MPI_Recv(buf,
                         theTile->getSizeInBytes(),
                         MPI_UNSIGNED_CHAR,
                         theCurrentTileNumber%(theNumberOfProcessors-1)+1,
                         0,
                         MPI_COMM_WORLD,
                         &status);

   // Data always sent in big endian order.
   if ( endian )
   {
      endian->swap(theTile->getScalarType(), buf, theTile->getSize());
   }

   // Get the output rectangle.
   ossimIrect outputRect;
   getOutputTileRectangle(outputRect);

   // Capture the output rectangle.
   theTile->setImageRectangle(outputRect);

   // Set the tile status.
   theTile->validate();

   // Increment the tile index.
   ++theCurrentTileNumber;

   // cleanup...
   if ( endian )
   {
      delete endian;
      endian = 0;
   }

   return theTile;

#else

   // Not compiled with mpi.
   return ossimOverviewSequencer::getNextTile();

#endif /* End of #if OSSIM_HAS_MPI */

}
