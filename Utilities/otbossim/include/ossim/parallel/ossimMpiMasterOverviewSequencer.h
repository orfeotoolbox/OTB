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
// $Id: ossimMpiMasterOverviewSequencer.h 17194 2010-04-23 15:05:19Z dburken $
#ifndef ossimMpiMasterOverviewSequencer_HEADER
#define ossimMpiMasterOverviewSequencer_HEADER

#include <ossim/imaging/ossimOverviewSequencer.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/imaging/ossimImageData.h>


/**
 * @class MPI master sequencer for building overview files.
 */
class OSSIM_DLL ossimMpiMasterOverviewSequencer : public ossimOverviewSequencer
{
public:
   
   /** default constructor */
   ossimMpiMasterOverviewSequencer();
   
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
    * Will allow you to get the next tile in the sequence.
    * Note the last tile returned will be an invalid
    * ossimRefPtr<ossimImageData>.  Callers should be able to do:
    * 
    * ossimRefPtr<ossimImageData> id = sequencer->getNextTile();
    * while (id.valid())
    * {
    *    doSomething;
    *    id = sequencer->getNextTile();
    * }
    * 
    */
   virtual ossimRefPtr<ossimImageData> getNextTile();
   
protected:
   /** virtual destructor */
   virtual ~ossimMpiMasterOverviewSequencer();

   int  m_rank;
   int  m_numberOfProcessors;
};

#endif /* #ifndef ossimMpiMasterOverviewSequencer_HEADER */
