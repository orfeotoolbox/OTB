//*****************************************************************************
// FILE: ossimElevCellHandler.h
//
// Copyright (C) 2001 ImageLinks, Inc.
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// DESCRIPTION:
//   Contains declaration of class osimElevHandler. This class provides a
//   base-class interface for the various DEM file formats.
//
// SOFTWARE HISTORY:
//>
//   13Apr2001  Oscar Kramer
//              Initial coding.
//<
//*****************************************************************************
// $Id: ossimElevCellHandler.h,v 1.14 2005/01/22 16:46:02 dburken Exp $


#ifndef ossimElevCellHandler_HEADER
#define ossimElevCellHandler_HEADER

#include <elevation/ossimElevSource.h>
#include <base/data_types/ossimFilename.h>
#include <base/data_types/ossimIpt.h>
#include <base/data_types/ossimDpt.h>
#include <base/data_types/ossimGpt.h>
#include <base/data_types/ossimDrect.h>

// class ossimDblGrid;

/******************************************************************************
 *
 * CLASS:  ossimElevCellHandler
 *
 *****************************************************************************/
class ossimElevCellHandler : public ossimElevSource
{
public:
   /**
    * Constructors:
    */

   ossimElevCellHandler (const char* elev_filename);

   virtual ~ossimElevCellHandler();

   virtual ossimFilename getFilename() const;
   
   
   /**
    *  METHOD:  getSizeOfElevCell 
    *  Returns the number of post in the cell.
    *  Note:  x = longitude, y = latitude
    */
   virtual ossimIpt getSizeOfElevCell() const = 0;

   /**
    *  METHOD:  getPostValue
    *  Returns the value at a given grid point as a double.
    */
   virtual double getPostValue(const ossimIpt& gridPt) const = 0;
      
   /**
    * METHOD: meanSpacingMeters()
    * Implements pure virtual for. This method is used for deterrming
    * preferred DEM among multiple overlapping elev sources:
    */
   virtual double getMeanSpacingMeters() const;

   /**
    * METHOD: pointIsInsideRect()
    * Method to check if the ground point elevation is defined:
    */
   virtual bool pointHasCoverage(const ossimGpt&) const;

   /**
    * METHODS: accuracyLE90(), accuracyCE90()
    * Returns the vertical and horizontal accuracy (90% confidence):
    */
   virtual double getAccuracyLE90(const ossimGpt&) const;
   virtual double getAccuracyCE90(const ossimGpt&) const;

   bool canConnectMyInputTo(ossim_int32 inputIndex,
                            const ossimConnectableObject* object)const;
      
protected:
   ossimElevCellHandler ();
   
   /**
    * Virtual method for reading 
    */
   ossimFilename        theFilename;
   double               theMeanSpacing;  // meters
   double               theAbsLE90;
   double               theAbsCE90;

private:
   // disallow...
   ossimElevCellHandler (const ossimElevCellHandler&);
   const ossimElevCellHandler& operator=(const ossimElevCellHandler& rhs);

   
TYPE_DATA
};

inline bool ossimElevCellHandler::pointHasCoverage(const ossimGpt& gpt) const
{
   return theGroundRect.pointWithin(gpt);
}

#endif
