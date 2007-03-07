//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author:  Garrett Potts (gpotts@imagelinks.com)
//
//*******************************************************************
//  $Id: ossimRationalNumber.h,v 1.14 2003/09/14 18:49:42 dburken Exp $
#ifndef ossimRationalNumber_HEADER
#define ossimRationalNumber_HEADER
#include "base/common/ossimCommon.h"
#include "base/common/ossimConstants.h"
#include <iostream>

class OSSIMDLLEXPORT ossimRationalNumber
{
public:

   friend std::ostream& operator<<(std::ostream& out, const ossimRationalNumber& rhs)
      {
         out<<rhs.theNum << "/" << rhs.theDen;
         
         return out;
      }
   ossimRationalNumber()
      :theNum(1),
       theDen(1)
      {
      }
   ossimRationalNumber(ossim_int32 num,
                       ossim_int32 den)
      :theNum(num),
       theDen(den)
      {
      }
   ossimRationalNumber(ossim_int32 num)
      :theNum(num),
       theDen(1)
      {}
   double toDouble()const
      {
         return (static_cast<double>(theNum)/
                 static_cast<double>(theDen));
      }
   float toFloat()const
      {
         return (static_cast<float>(theNum)/
                 static_cast<float>(theDen));
      }

   void normalize();
   
   const ossimRationalNumber& operator=(ossim_int32 n)
      {
         return assign(n, 1);
      }

   const ossimRationalNumber& operator=(double n)
      {
         return assign(n);
      }

   ossimRationalNumber operator-()const
      {
         return ossimRationalNumber(-theNum, theDen);
      }
   const ossimRationalNumber& operator+= (const ossimRationalNumber& r);
   const ossimRationalNumber& operator-= (const ossimRationalNumber& r);
   const ossimRationalNumber& operator*= (const ossimRationalNumber& r);
   const ossimRationalNumber& operator/= (const ossimRationalNumber& r);
   
   inline const ossimRationalNumber& operator+= (ossim_int32 i);
   inline const ossimRationalNumber& operator-= (ossim_int32 i);
   inline const ossimRationalNumber& operator*= (ossim_int32 i);
   inline const ossimRationalNumber& operator/= (ossim_int32 i);

   ossimRationalNumber operator+ (const ossimRationalNumber& r)const;
   ossimRationalNumber operator- (const ossimRationalNumber& r)const;
   ossimRationalNumber operator* (const ossimRationalNumber& r)const;
   ossimRationalNumber operator/ (const ossimRationalNumber& r)const;
   
   inline ossimRationalNumber operator+ (ossim_int32 i)const;
   inline ossimRationalNumber operator- (ossim_int32 i)const;
   inline ossimRationalNumber operator* (ossim_int32 i)const;
   inline ossimRationalNumber operator/ (ossim_int32 i)const;

   friend inline ossimRationalNumber operator+ (ossim_int32 i, ossimRationalNumber& r);
   friend inline ossimRationalNumber operator- (ossim_int32 i, ossimRationalNumber& r);
   friend inline ossimRationalNumber operator* (ossim_int32 i, ossimRationalNumber& r);
   friend inline ossimRationalNumber operator/ (ossim_int32 i, ossimRationalNumber& r);
   
     // Comparison operators
//   inline bool operator< (const ossimRationalNumber& r) const;
   inline bool operator== (const ossimRationalNumber& r) const;
   
   inline bool operator== (ossim_int32 i) const;
   
   // Increment and decrement
   inline const ossimRationalNumber& operator++();
   inline const ossimRationalNumber& operator--();
   
   // Assign in place
   inline const ossimRationalNumber& assign(ossim_int32 n, ossim_int32 d);
  /*!
   * default estimation is out to the 10000 place.  Will set this 
   * rational to a value that is close to the passed in number.
   */
  const ossimRationalNumber& assign(double value, long precision=10000);

   /*!
    * Holds the value of the numberator.
    */
   ossim_int32 theNum;

   /*!
    * Holds the value of the denominator.
    */
   ossim_int32 theDen;

};

inline const ossimRationalNumber& ossimRationalNumber::assign(ossim_int32 n, ossim_int32 d)
{
   theNum = n;
   theDen = d;
   normalize();
   
   return *this;  
}

// Mixed-mode operators
inline ossimRationalNumber ossimRationalNumber::operator+(ossim_int32 i)const
{
    return operator + (ossimRationalNumber(i));
}

inline ossimRationalNumber ossimRationalNumber::operator-(ossim_int32 i)const
{
    return operator - (ossimRationalNumber(i));
}

inline ossimRationalNumber ossimRationalNumber::operator*(ossim_int32 i)const
{
    return operator * (ossimRationalNumber(i));
}

inline ossimRationalNumber ossimRationalNumber::operator/(ossim_int32 i)const
{
    return operator / (ossimRationalNumber(i));
}

inline ossimRationalNumber operator+ (ossim_int32 i, ossimRationalNumber& r)
{
   return ossimRationalNumber(i)+r;
}

inline ossimRationalNumber operator- (ossim_int32 i, ossimRationalNumber& r)
{
   return ossimRationalNumber(i)-r;
}

inline ossimRationalNumber operator* (ossim_int32 i, ossimRationalNumber& r)
{
   return ossimRationalNumber(i)*r;
}

inline ossimRationalNumber operator/ (ossim_int32 i, ossimRationalNumber& r)
{
   return ossimRationalNumber(i)/r;
}

inline const ossimRationalNumber& ossimRationalNumber::operator++()
{
   theNum += theDen;
   
   return *this;
}

inline const ossimRationalNumber& ossimRationalNumber::operator--()
{
   theNum -= theDen;
   
   return *this;   
}

inline bool ossimRationalNumber::operator== (const ossimRationalNumber& r)const
{
    return ((theNum == r.theNum) && (theDen == r.theDen));
}

inline bool ossimRationalNumber::operator== (ossim_int32 i) const
{
    return ((theDen == ossim_int32(1)) && (theNum == i));
}

#endif
