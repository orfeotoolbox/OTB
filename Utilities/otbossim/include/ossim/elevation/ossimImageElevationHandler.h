//----------------------------------------------------------------------------
//
// File: ossimImageElevationHandler.h
// 
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: See description for class below.
// 
//----------------------------------------------------------------------------
// $Id$

#ifndef ossimImageElevationHandler_HEADER
#define ossimImageElevationHandler_HEADER 1

#include <ossim/elevation/ossimElevCellHandler.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/imaging/ossimImageGeometry.h>
#include <ossim/imaging/ossimImageHandler.h>

/**
 * @class ossimImageElevationHandler
 *
 * Elevation source for a generic image opened via ossimImageHandler.
 */
class OSSIM_DLL ossimImageElevationHandler : public ossimElevCellHandler
{
public:

   /** default constructor */
   ossimImageElevationHandler();

   /**
    * METHOD: getHeightAboveMSL
    * Height access methods.
    */
   virtual double getHeightAboveMSL(const ossimGpt&);

   /**
    *  METHOD:  getSizeOfElevCell
    *  Returns the number of post in the cell.  Satisfies pure virtual.
    *  Note:  x = longitude, y = latitude
    */
   virtual ossimIpt getSizeOfElevCell() const;
      
   /**
    *  METHOD:  getPostValue
    *  Returns the value at a given grid point as a double.
    *  Satisfies pure virtual.
    */
   virtual double getPostValue(const ossimIpt& gridPt) const;

   /** @return True if open, false if not. */
   virtual bool isOpen()const;
   
   /**
    * Opens a stream to the srtm cell.
    *
    * @return Returns true on success, false on error.
    */
   virtual bool open(const ossimFilename& file);

   /** @brief Closes the stream to the file. */
   virtual void close();

   /**
    * @brief pointHasCoverage(gpt)
    *
    * Overrides ossimElevCellHandler::pointHasCoverage
    * @return TRUE if coverage exists over gpt.
    */
   virtual bool pointHasCoverage(const ossimGpt&) const;

protected:
   /**
    * @Brief Protected destructor.
    *
    * This class is derived from ossimReferenced so users should always use
    * ossimRefPtr<ossimImageElevationHandler> to hold instance.
    */
   virtual ~ossimImageElevationHandler();
   
private:

   /** Hidden from use copy constructor */
   ossimImageElevationHandler(const ossimImageElevationHandler&);
   
   /** Hidden from use assignment operator */
   const ossimImageElevationHandler& operator=
      (const ossimImageElevationHandler& rhs);


   /** Pointers to links in chain. */
   ossimRefPtr<ossimImageHandler>      m_ih;
   ossimRefPtr<ossimImageGeometry>     m_geom;

   /** Image space rect stored as drect for inlined pointHasCoverage method. */
   ossimDrect                          m_rect;

   TYPE_DATA
};

inline bool ossimImageElevationHandler::isOpen() const
{
   return m_ih.valid();
}

inline void ossimImageElevationHandler::close()
{
   m_geom  = 0;
   m_ih    = 0;
}

inline bool ossimImageElevationHandler::pointHasCoverage(const ossimGpt& gpt) const
{
   if ( m_geom.valid() )
   {
      ossimDpt dpt;
      m_geom->worldToLocal(gpt, dpt);
      return m_rect.pointWithin(dpt);
   }
   return false;
}

#endif /* ossimImageElevationHandler_HEADER */
