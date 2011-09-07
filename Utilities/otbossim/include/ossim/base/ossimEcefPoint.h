//*****************************************************************************
// FILE: ossimEcefPoint.h
//
// License:  See top level LICENSE.txt file.
//
// DESCRIPTION:
//   Contains declaration of a 3D point object in the Earth-centered, earth
//   fixed (ECEF) coordinate system.
//
// SOFTWARE HISTORY:
//>
//   08Aug2001  Oscar Kramer (http://www.oscarkramer.com)
//              Initial coding.
//<
//*****************************************************************************
//  $Id: ossimEcefPoint.h 20043 2011-09-06 15:00:55Z oscarkramer $

#ifndef ossimEcefPoint_HEADER
#define ossimEcefPoint_HEADER
#include <iosfwd>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimColumnVector3d.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimString.h>
#include <ossim/matrix/newmat.h>

class ossimGpt;
class ossimEcefVector;
class ossimDpt3d;

//*****************************************************************************
//  CLASS: ossimEcefPoint
//
//*****************************************************************************
class OSSIMDLLEXPORT ossimEcefPoint
{
public:
   /*!
    * CONSTRUCTORS: 
    */
   ossimEcefPoint()
      : theData(0,0,0) {}

   ossimEcefPoint(const ossimEcefPoint& copy_this)
      : theData (copy_this.theData) {}

   ossimEcefPoint(const ossimGpt& convert_this);

   ossimEcefPoint(const double& x,
                  const double& y,
                  const double& z)
      : theData(x, y, z) {}

   ossimEcefPoint(const ossimColumnVector3d& assign_this)
      : theData(assign_this) {}

   ossimEcefPoint(const NEWMAT::ColumnVector& assign_this)
      : theData(assign_this) {}

   ossimEcefPoint(const ossimDpt3d& pt);
   
   void makeNan()
      {
         theData[0] = ossim::nan();
         theData[1] = ossim::nan();
         theData[2] = ossim::nan();
      }
   
   bool hasNans()const
      {
         return ( ossim::isnan(theData[0]) ||
                  ossim::isnan(theData[1]) ||
                  ossim::isnan(theData[2]) );
                           
      }

   bool isNan()const
      {
         return ( ossim::isnan(theData[0]) &&
                  ossim::isnan(theData[1]) &&
                  ossim::isnan(theData[2]) );
         
      }
   /*!
    * OPERATORS:
    */
   ossimEcefVector       operator- (const ossimEcefPoint&)  const;
   ossimEcefPoint        operator+ (const ossimEcefVector&) const;
   ossimEcefPoint        operator- (const ossimEcefVector&) const;
   const ossimEcefPoint& operator= (const ossimEcefPoint&);        // inline
   bool                  operator==(const ossimEcefPoint&)  const; // inline
   bool                  operator!=(const ossimEcefPoint&)  const; // inline
   
   /*!
    * COMPONENT ACCESS METHODS: 
    */
   double    x() const { return theData[0]; }
   double&   x()       { return theData[0]; }
   double    y() const { return theData[1]; }
   double&   y()       { return theData[1]; }
   double    z() const { return theData[2]; }
   double&   z()       { return theData[2]; }
   double&   operator[](int idx){return theData[idx];}
   const double&   operator[](int idx)const{return theData[idx];}
   const ossimColumnVector3d& data() const { return theData; }
   ossimColumnVector3d&       data()       { return theData; }

   double getMagnitude() const
      {
         return theData.magnitude();
      }
   double magnitude()const
   {
      return theData.magnitude();
   }
   double length()const
   {
      return theData.magnitude();
   }
   double normalize()
   {
      double result = magnitude();

      if(result > 1e-15)
      {
         theData[0]/=result;
         theData[1]/=result;
         theData[2]/=result;
      }
      
      return result;
   }

   /**
    * @brief To string method.
    * 
    * @param precision Output floating point precision.
    * 
    * @return ossimString representing point.
    *
    * Output format:
    * ( 0.0000000,  0.0000000,  0.00000000 )
    *   -----x----  -----y----  ------z----
    */
   ossimString toString(ossim_uint32 precision=15) const;

   /**
    * @brief Initializes this point from string.
    *
    * Expected format:
    * 
    * ( 0.0000000,  0.0000000,  0.00000000 )
    *   -----x----  -----y----  ------z----
    *
    * @param s String to initialize from.
    */
   void toPoint(const std::string& s);
   
   //! Converts this point to a 3D column vector.
   NEWMAT::ColumnVector toVector() const
   {
      NEWMAT::ColumnVector v (3);
      v(0) = theData[0];
      v(1) = theData[1];
      v(2) = theData[2];
      return v;
   }
   
   //! Converts 3D column vector to this point.
   void toPoint(const NEWMAT::ColumnVector& v)
   {
      if (v.Nrows() == 3)
      {
         theData[0] = v[0];
         theData[1] = v[1];
         theData[2] = v[2];
      }
   }

   /*!
    * Debug Dump: 
    */
   void print(std::ostream& os = ossimNotify(ossimNotifyLevel_INFO)) const;
   
   friend OSSIM_DLL std::ostream& operator<<(std::ostream& os ,
                                             const ossimEcefPoint& instance);

protected:
   ossimColumnVector3d theData;
};

//================== BEGIN DEFINITIONS FOR INLINE METHODS =====================

//*****************************************************************************
//  INLINE METHOD: ossimEcefPoint::operator=(ossimEcefPoint)
//*****************************************************************************
inline const ossimEcefPoint&
ossimEcefPoint::operator=(const ossimEcefPoint& p)
{
   theData = p.theData;
   return *this;
}

//*****************************************************************************
//  INLINE METHOD: ossimEcefPoint::operator==(ossimEcefPoint)
//*****************************************************************************
inline bool ossimEcefPoint::operator==(const ossimEcefPoint& p) const
{
   return (theData == p.theData);
}

//*****************************************************************************
//  INLINE METHOD: ossimEcefPoint::operator!=(ossimEcefPoint)
//*****************************************************************************
inline bool ossimEcefPoint::operator!=(const ossimEcefPoint& p) const
{
   return (theData != p.theData);
}

#endif
