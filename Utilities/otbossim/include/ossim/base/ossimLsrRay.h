//*****************************************************************************
// FILE: ossimLsrRay.h
//
// Copyright (C) 2001 ImageLinks, Inc.
//
// License:  See top level LICENSE.txt file.
//
// DESCRIPTION:
//  Class for representing rays in some local space rectangular (LSR)
//  coordinate system. This coordinate system is related to the ECEF system
//  by the ossimLsrSpace member object. This class simplifies coordinate
//  conversions between LSR and ECEF, and other LSR spaces.
//
//  An LSR ray is defined as having an LSR origin point and an LSR unit
//  direction vector radiating from the origin.
//
// SOFTWARE HISTORY:
//>
//   08Aug2001  Oscar Kramer
//              Initial coding.
//<
//*****************************************************************************
//  $Id: ossimLsrRay.h 11428 2007-07-27 18:44:18Z gpotts $

#ifndef ossimLsrRay_HEADER
#define ossimLsrRay_HEADER

#include <ossim/base/ossimLsrPoint.h>
#include <ossim/base/ossimLsrVector.h>
#include <ossim/base/ossimEcefRay.h>
#include <ossim/base/ossimNotifyContext.h>

//*****************************************************************************
//  CLASS: ossimLsrRay
//
//*****************************************************************************
class OSSIMDLLEXPORT ossimLsrRay
{
public:
   /*!
    * CONSTRUCTORS: 
    */
   ossimLsrRay() {}
   
   ossimLsrRay(const ossimLsrRay& copy_this)
      : theOrigin(copy_this.theOrigin), theDirection(copy_this.theDirection) {}

   ossimLsrRay(const ossimLsrPoint&  origin,
               const ossimLsrVector& direction);

   ossimLsrRay(const ossimLsrPoint&  origin,
               const ossimLsrPoint&  towards);

   ossimLsrRay(const ossimLsrRay& convert_this,
               const ossimLsrSpace& new_space)
      : theOrigin(convert_this.theOrigin, new_space),
	theDirection(convert_this.theDirection, new_space) {}

   /*!
    * OPERATORS:
    */
   const ossimLsrRay&  operator= (const ossimLsrRay& r);       // inline below
   bool                operator==(const ossimLsrRay& r) const; // inline below
   bool                operator!=(const ossimLsrRay& r) const; // inline below

   /*!
    * DATA ACCESS METHODS:
    */
   const ossimLsrPoint&  origin()    const { return theOrigin; }
   const ossimLsrVector& direction() const { return theDirection; }
   const ossimLsrSpace&  lsrSpace()  const { return theOrigin.lsrSpace(); }

   /*!
    * CASTING OPERATOR: ossimEcefRay()
    * Looks like a constructor for an ossimEcefRay but is an operation on this
    * object. Returns the ossimEcefRay equivalent.
    */
   operator ossimEcefRay () const;  // inline below
   
   bool hasNans()const
   {
      return (theOrigin.hasNans()||theDirection.hasNans());
   }

   void makeNan()
   {
      theOrigin.makeNan();
      theDirection.makeNan();
   }
   /*!
    * METHOD: extend(t)
    * Extends the ray by distance t (meters) from the origin to the LSR
    * point returned (in same space).
    */
   ossimLsrPoint extend(const double& t) const
      {
         if(!hasNans())
         {
            return (theOrigin + theDirection*t);
         }

         ossimLsrPoint p;
         p.makeNan();
         return p;
      }

   /*!
    * Debug Dump: 
    */
   void print(ostream& stream = ossimNotify(ossimNotifyLevel_INFO)) const;  // inline below
   
   friend ostream& operator<< (ostream& os , const ossimLsrRay& instance)
      { instance.print(os); return os; }

private:
   ossimLsrPoint  theOrigin;
   ossimLsrVector theDirection;

};

//================== BEGIN DEFINITIONS FOR INLINE METHODS =====================

//*****************************************************************************
//  INLINE METHOD: ossimEcefRay::operator=(ossimEcefRay)
//*****************************************************************************
inline const ossimLsrRay& ossimLsrRay::operator=(const ossimLsrRay& r)
{
   theOrigin = r.theOrigin;
   theDirection = r.theDirection;
   return *this;
}

//*****************************************************************************
//  INLINE METHOD: ossimEcefRay::operator==(ossimEcefRay)
//*****************************************************************************
inline bool ossimLsrRay::operator==(const ossimLsrRay& r) const
{
   return ((theOrigin == r.theOrigin) && (theDirection == r.theDirection));
}

//*****************************************************************************
//  INLINE METHOD: ossimEcefRay::operator!=(ossimEcefRay)
//*****************************************************************************
inline bool ossimLsrRay::operator!=(const ossimLsrRay& r) const 
{
   return !(*this == r);
}

//*****************************************************************************
//  INLINE CASTING OPERATOR: ossimEcefRay()
//  
//  Looks like a constructor for an ossimEcefRay but is an operation on this
//  object. Returns the ossimEcefRay equivalent.
//*****************************************************************************
inline ossimLsrRay::operator ossimEcefRay() const
{
   return ossimEcefRay(ossimEcefPoint(theOrigin),
                       ossimEcefVector(theDirection));
}
   
//*****************************************************************************
//  INLINE METHOD: ossimLsrRay::print(ostream)
//  Dumps contents for debug purposes.
//*****************************************************************************
inline void ossimLsrRay::print(ostream& os) const
{
   os << "(ossimLsrRay)"
      << "\n   theOrigin    = " << theOrigin
      << "\n   theDirection = " << theDirection << ends;
}
   
#endif


