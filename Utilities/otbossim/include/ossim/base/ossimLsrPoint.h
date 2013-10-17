//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// DESCRIPTION:
//  Class for representing points in some local space rectangular (LSR)
//  coordinate system. This coordinate system is related to the ECEF system
//  by the ossimLsrSpace member object. This class simplifies coordinate
//  conversions between LSR and ECEF, and other LSR points.
//
// SOFTWARE HISTORY:
//>
//   08Aug2001  Oscar Kramer (okramer@imagelinks.com)
//              Initial coding.
//<
//*****************************************************************************
//  $Id: ossimLsrPoint.h 22197 2013-03-12 02:00:55Z dburken $

#ifndef ossimLsrPoint_HEADER
#define ossimLsrPoint_HEADER

#include <ossim/base/ossimLsrSpace.h>
#include <ossim/base/ossimColumnVector3d.h>
#include <ossim/base/ossimNotify.h>

class ossimGpt;
class ossimLsrVector;

//*****************************************************************************
//  CLASS: ossimLsrPoint
//
//*****************************************************************************
class OSSIMDLLEXPORT ossimLsrPoint
{
public:
   /*!
    * CONSTRUCTORS: 
    */
   ossimLsrPoint()
      : theData(0,0,0) {}
   
   ossimLsrPoint(const ossimLsrPoint& copy_this)
      : theData(copy_this.theData), theLsrSpace(copy_this.theLsrSpace) {}
   
   ossimLsrPoint(const ossimColumnVector3d& assign_this,
                 const ossimLsrSpace& space)
      : theData(assign_this), theLsrSpace(space) {}
   
   ossimLsrPoint(const double& x,
                 const double& y,
                 const double& z,
                 const ossimLsrSpace& space)
      : theData(x, y, z), theLsrSpace(space) {}
   
   ossimLsrPoint(const ossimLsrPoint& convert_this,
                 const ossimLsrSpace&);

   ossimLsrPoint(const ossimGpt& convert_this,
                 const ossimLsrSpace&);

   ossimLsrPoint(const ossimEcefPoint& convert_this,
                 const ossimLsrSpace&);

   /*!
    * OPERATORS:
    */
   inline const ossimLsrPoint& operator= (const ossimLsrPoint&); //inline below
   ossimLsrVector operator- (const ossimLsrPoint&)  const;
   ossimLsrPoint  operator+ (const ossimLsrVector&) const;
   inline bool    operator==(const ossimLsrPoint&)  const;//inline below
   inline bool    operator!=(const ossimLsrPoint&)  const;//inline below

   /*!
    * CASTING OPERATOR:
    * Used as: myEcefVector = ossimEcefPoint(this) -- looks like a constructor
    * but is an operation on this object. ECEF knows nothing about LSR, so
    * cannot provide an ossimEcefVector(ossimLsrPoint) constructor.
    */
   operator ossimEcefPoint() const; // inline below

   /*!
    * DATA ACCESS METHODS: 
    */
   double    x() const { return theData[0]; }
   double&   x()       { return theData[0]; }
   double    y() const { return theData[1]; }
   double&   y()       { return theData[1]; }
   double    z() const { return theData[2]; }
   double&   z()       { return theData[2]; }

   
   ossimColumnVector3d&       data()           { return theData; }
   const ossimColumnVector3d& data()     const { return theData; }

   ossimLsrSpace&             lsrSpace()       { return theLsrSpace; }
   const ossimLsrSpace&       lsrSpace() const { return theLsrSpace; }

   bool hasNans()const
   {
      return (ossim::isnan(theData[0])||
              ossim::isnan(theData[1])||
              ossim::isnan(theData[2]));
   }

   void makeNan()
   {
      theData[0] = ossim::nan();
      theData[1] = ossim::nan();
      theData[2] = ossim::nan();
   }
   /*!
    * Debug Dump: 
    */
   inline void print(ostream& stream = ossimNotify(ossimNotifyLevel_INFO)) const;

   friend ostream& operator<< (ostream& os , const ossimLsrPoint& instance)
      { instance.print(os); return os; }

protected:
   /*!
    * METHOD: initialize(ossimEcefPoint)
    * Convenience method used by several constructors for initializing theData
    * given an ECEF point. Assumes theLsrSpace has been previously initialized.
    */
   void initialize(const ossimEcefPoint& ecef_point);
   
   ossimColumnVector3d theData;
   ossimLsrSpace       theLsrSpace;

};

//================== BEGIN DEFINITIONS FOR INLINE METHODS =====================

//*****************************************************************************
//  INLINE OPERATOR: ossimLsrPoint::operator=(ossimLsrPoint)
//*****************************************************************************
inline const ossimLsrPoint&
ossimLsrPoint::operator=(const ossimLsrPoint& p)
{
   theData = p.theData;
   theLsrSpace = p.theLsrSpace;
   
   return *this;
}

//*****************************************************************************
//  INLINE OPERATOR: ossimLsrPoint::operator==(ossimLsrPoint)
//*****************************************************************************
inline bool ossimLsrPoint::operator==(const ossimLsrPoint& p) const
{
   return ((theData == p.theData) && (theLsrSpace == p.theLsrSpace));
}

//*****************************************************************************
//  INLINE OPERATOR: ossimLsrPoint::operator!=(ossimLsrPoint)
//*****************************************************************************
inline bool ossimLsrPoint::operator!=(const ossimLsrPoint& p) const
{
   return (!(*this == p));
}

//*****************************************************************************
//  INLINE OPERATOR: ossimLsrPoint::operator ossimEcefPoint()
//
//  Looks like a constructor for an ossimEcefPoint but is an operation on this
//  object. Returns the ossimEcefPoint equivalent.
//*****************************************************************************
inline ossimLsrPoint::operator ossimEcefPoint() const
{
   return ossimEcefPoint(theLsrSpace.origin().data() +
                         theLsrSpace.lsrToEcefRotMatrix()*theData);
}

//*****************************************************************************
//  INLINE METHOD: ossimLsrPoint::print(ostream)
//  
//  Dumps contents for debug purposes.
//*****************************************************************************
inline void ossimLsrPoint::print(ostream& os) const
{
   os << "(ossimLsrPoint)\n"
      << "  theData = " << theData
      << "\n  theLsrSpace = " << theLsrSpace;
}

#endif


