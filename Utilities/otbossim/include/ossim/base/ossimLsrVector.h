//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// DESCRIPTION:
//  Class for representing vectors in some local space rectangular (LSR)
//  coordinate system. This coordinate system is related to the ECEF system
//  by the ossimLsrSpace member object. This class simplifies coordinate
//  conversions between LSR and ECEF, and other LSR vectors.
//
// SOFTWARE HISTORY:
//>
//   08Aug2001  Oscar Kramer
//              Initial coding.
//<
//*****************************************************************************
//  $Id: ossimLsrVector.h 12790 2008-05-05 13:41:33Z dburken $

#ifndef ossimLsrVector_HEADER
#define ossimLsrVector_HEADER

#include <iosfwd>

#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimLsrPoint.h>
#include <ossim/base/ossimLsrSpace.h>
#include <ossim/base/ossimEcefVector.h>
#include <ossim/base/ossimColumnVector3d.h>

class ossimGpt;

//*****************************************************************************
//  CLASS: ossimLsrVector
//
//*****************************************************************************
class OSSIMDLLEXPORT ossimLsrVector
{
public:
   /*!
    * CONSTRUCTORS: 
    */
   ossimLsrVector()
      : theData (0,0,0) {}
   
   ossimLsrVector(const ossimLsrVector& copy_this)
      : theData(copy_this.theData), theLsrSpace(copy_this.theLsrSpace) {}

   ossimLsrVector(const ossimColumnVector3d& assign_this,
                  const ossimLsrSpace& space)
      : theData(assign_this), theLsrSpace(space) {}
   
   ossimLsrVector(const double& x,
                  const double& y,
                  const double& z,
                  const ossimLsrSpace& space)
      : theData(x,y,z), theLsrSpace(space) {}
   
   ossimLsrVector(const ossimEcefVector& convert_this,
                  const ossimLsrSpace&);
   ossimLsrVector(const ossimLsrVector& convert_this,
                  const ossimLsrSpace&);

   /*!
    * OPERATORS: (all methods inlined below)
    */
   const ossimLsrVector&  operator= (const ossimLsrVector&);
   ossimLsrVector         operator- ()                      const;
   ossimLsrVector         operator+ (const ossimLsrVector&) const;
   ossimLsrVector         operator- (const ossimLsrVector&) const;
   ossimLsrPoint          operator+ (const ossimLsrPoint&)  const;
   ossimLsrVector         operator* (const double& scalar)  const;
   ossimLsrVector         operator/ (const double& scalar)  const;
   bool                   operator==(const ossimLsrVector&) const;
   bool                   operator!=(const ossimLsrVector&) const;

   /*!
    * CASTING OPERATOR:
    * Used as: myEcefVector = ossimEcefVector(this) -- looks like a constructor
    * but is an operation on this object. ECEF knows nothing about LSR, so
    * cannot provide an ossimEcefVector(ossimLsrVector) constructor.
    */
   operator ossimEcefVector() const; // inline below
   
   /*!
    * Vector-related functions: 
    */
   double                 dot(const ossimLsrVector&)       const;
   double                 angleTo(const ossimLsrVector&)   const;
   ossimLsrVector         cross(const ossimLsrVector&)     const;
   ossimLsrVector         unitVector()                     const;//inline below
   double                 magnitude()                      const;//inline below
   void                   normalize(); // inline below
   
   /*!
    * DATA ACCESS METHODS: 
    */
   double    x() const { return theData[0]; }
   double&   x()       { return theData[0]; }
   double    y() const { return theData[1]; }
   double&   y()       { return theData[1]; }
   double    z() const { return theData[2]; }
   double&   z()       { return theData[2]; }

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
   ossimColumnVector3d&       data()           { return theData; }
   const ossimColumnVector3d& data()     const { return theData; }

   ossimLsrSpace&             lsrSpace()       { return theLsrSpace; }
   const ossimLsrSpace&       lsrSpace() const { return theLsrSpace; }

   /*!
    * Debug Dump: 
    */
   std::ostream& print(ostream& stream) const;

   friend std::ostream& operator<< (std::ostream& os ,
                                    const ossimLsrVector& instance);

protected:
   /*!
    * METHOD: initialize(ossimEcefVector)
    * Convenience method used by several constructors for initializing theData
    * given an ECEF vector. Assumes theLsrSpace has been previously initialized
    */
   void initialize(const ossimEcefVector& ecef_point);
   
   ossimColumnVector3d theData;
   ossimLsrSpace       theLsrSpace;

};

//================== BEGIN DEFINITIONS FOR INLINE METHODS =====================

//*****************************************************************************
// INLINE OPERATOR: ossimLsrVector::operator=(ossimLsrVector)
//*****************************************************************************
inline const ossimLsrVector& ossimLsrVector::operator=(const ossimLsrVector& v)
{
   theData = v.theData;
   theLsrSpace = v.theLsrSpace;

   return *this;
}

//*****************************************************************************
//  INLINE OPERATOR: ossimLsrVector::operator-() (negate)
//*****************************************************************************
inline ossimLsrVector ossimLsrVector::operator-() const
{
   return ossimLsrVector(-theData, theLsrSpace);
}

//*****************************************************************************
//  INLINE OPERATOR: ossimLsrVector::operator+(ossimLsrVector)
//*****************************************************************************
inline ossimLsrVector ossimLsrVector::operator+(const ossimLsrVector& v) const
{
   if ((theLsrSpace != v.theLsrSpace)||hasNans()||v.hasNans())
   {
      theLsrSpace.lsrSpaceErrorMessage();
      return ossimLsrVector(ossim::nan(), ossim::nan(), ossim::nan(), theLsrSpace);
   }
   return ossimLsrVector(theData + v.theData, theLsrSpace);

}

//*****************************************************************************
//  INLINE OPERATOR: ossimLsrVector::operator-(ossimLsrVector)
//*****************************************************************************
inline ossimLsrVector ossimLsrVector::operator-(const ossimLsrVector& v) const
{
   if ((theLsrSpace != v.theLsrSpace)||hasNans()||v.hasNans())
   {
      theLsrSpace.lsrSpaceErrorMessage();
      return ossimLsrVector(ossim::nan(), ossim::nan(), ossim::nan(), theLsrSpace);
   }
   return ossimLsrVector(theData - v.data(), theLsrSpace);
}

//*****************************************************************************
//  INLINE OPERATOR: ossimLsrVector::operator+(ossimLsrPoint)
//*****************************************************************************
inline ossimLsrPoint ossimLsrVector::operator+(const ossimLsrPoint& p) const
{
   if ((theLsrSpace != p.lsrSpace())||hasNans()||p.hasNans())
   {
      theLsrSpace.lsrSpaceErrorMessage();
      return ossimLsrPoint(ossim::nan(), ossim::nan(), ossim::nan(), theLsrSpace);
   }
   return ossimLsrPoint(theData + p.data(), theLsrSpace);
}

//*****************************************************************************
//  INLINE OPERATOR: ossimLsrVector::operator*(double scalar)
//*****************************************************************************
inline ossimLsrVector ossimLsrVector::operator*(const double& scalar) const
{
   return ossimLsrVector(theData*scalar, theLsrSpace);
}

//*****************************************************************************
//  INLINE OPERATOR: ossimLsrVector::operator/(double scalar)
//*****************************************************************************
inline ossimLsrVector ossimLsrVector::operator/(const double& scalar) const
{
   return ossimLsrVector(theData/scalar, theLsrSpace);
}

//*****************************************************************************
//  INLINE OPERATOR: ossimLsrVector::operator==(ossimLsrVector)
//*****************************************************************************
inline bool ossimLsrVector::operator==(const ossimLsrVector& v) const
{
   return ((theData == v.theData) && (theLsrSpace == v.theLsrSpace));
}

//*****************************************************************************
//  INLINE OPERATOR: ossimLsrVector::operator!=(ossimLsrVector)
//*****************************************************************************
inline bool ossimLsrVector::operator!=(const ossimLsrVector& v) const
{
   return (!(*this == v));
}

//*****************************************************************************
//  INLINE OPERATOR:  ossimEcefVector()
//
//  Looks like a constructor for an ossimEcefVector but is an operation on this
//  object. Returns the ossimEcefVector equivalent.
//
//*****************************************************************************
inline ossimLsrVector::operator ossimEcefVector() const
{   
   return ossimEcefVector(theLsrSpace.lsrToEcefRotMatrix()*theData);
}   

//*****************************************************************************
//  INLINE METHOD:  ossimLsrVector::unitVector()
//  Returns a unit vector parallel to this.
//*****************************************************************************
inline ossimLsrVector ossimLsrVector::unitVector() const
{
   if(hasNans()) return ossimLsrVector(ossim::nan(), ossim::nan(), ossim::nan(), theLsrSpace);

   return ossimLsrVector(theData/theData.magnitude(), theLsrSpace);
}

//*****************************************************************************
//  INLINE METHOD: ossimLsrVector::magnitude()
//*****************************************************************************
inline double ossimLsrVector::magnitude() const
{
   if(hasNans()) return ossim::nan();
   return theData.magnitude();
}

//*****************************************************************************
//  INLINE METHOD: ossimLsrVector::normalize()
//  Normalizes this vector.
//*****************************************************************************
inline void ossimLsrVector::normalize()
{
   theData /= theData.magnitude();
}

//*****************************************************************************
//  PROTECTED INLINE METHOD: ossimLsrPoint::initialize(ossimEcefPoint)
//  
//  Convenience method used by several constructors for initializing theData
//  given an ECEF point. Assumes theLsrSpace has been previously initialized.
//  
//*****************************************************************************
inline void ossimLsrVector::initialize(const ossimEcefVector& ecef_vector)
{
   theData = theLsrSpace.ecefToLsrRotMatrix() * ecef_vector.data();
}

#endif


