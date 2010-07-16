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
// $Id: ossimMpiSlaveOverviewSequencer.h 17194 2010-04-23 15:05:19Z dburken $
#ifndef ossimMpiSlaveOverviewSequencer_HEADER
#define ossimMpiSlaveOverviewSequencer_HEADER

#include <ossim/imaging/ossimOverviewSequencer.h>
#include <ossim/base/ossimConstants.h>

/**
 * @class MPI slave sequencer for building overview files.
 */
class OSSIM_DLL ossimMpiSlaveOverviewSequencer : public ossimOverviewSequencer
{
public:
   
   /** default constructor */
   ossimMpiSlaveOverviewSequencer();
   

   /**
    * @return Always false for this implementation.
    */
   virtual bool isMaster()const;

   /**
    * This must be called.  We can only initialize this
    * object completely if we know all connections
    * are valid.  Some other object drives this and so the
    * connection's initialize will be called after.  The job
    * of this connection is to set up the sequence.  It will
    * default to the bounding rect.  The area of interest can be
    * set to some other rectagle (use setAreaOfInterest).
    */
   virtual void initialize();

   /**
    * @brief Will set the internal pointers to the upperleft
    * tile number.  To go to the next tile in the sequence
    * just call getNextTile.
    */
   virtual void setToStartOfSequence();

   /**
    * @brief This is a trigger to start resampling tiles.
    */
   virtual void slaveProcessTiles();
   
protected:
   /** virtual destructor */
   virtual ~ossimMpiSlaveOverviewSequencer();

   int  m_rank;
   int  m_numberOfProcessors;
};

#endif /* #ifndef ossimMpiSlaveOverviewSequencer_HEADER */
