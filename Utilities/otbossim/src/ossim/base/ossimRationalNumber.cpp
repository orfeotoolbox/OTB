//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Garrett Potts (gpotts@imagelinks.com)
//
//*******************************************************************
// $Id: ossimRationalNumber.cpp 11347 2007-07-23 13:01:59Z gpotts $
#include <ossim/base/ossimRationalNumber.h>

// Normalisation
void ossimRationalNumber::normalize()
{
    if (theDen == 0)
    {
       return;
    }

    // Handle the case of zero separately, to avoid division by zero
    if (theNum == 0)
    {
       theDen = 1;
       return;
    }

    ossim_int32 g = ossim::gcd(theNum, theDen);
    
    theNum /= g;
    theDen /= g;

    // Ensure that the denominator is positive
    if (theDen < 0)
    {
       theNum = -theNum;
       theDen = -theDen;
    }
}

const ossimRationalNumber& ossimRationalNumber::assign(double value, long precision)
{
  ossim_sint32 s = 1;
  if(value <= 0.0)
    {
      s = -1;
      value *= -1.0;
    }
  ossim_int32 integerPart = (ossim_int32)std::floor(value);
  ossim_int32 decimalPart = (ossim_int32)((value - integerPart)*precision);
  ossimRationalNumber temp(integerPart);
  ossimRationalNumber temp2(decimalPart, precision);
  temp2.normalize();
  *this = (temp + temp2);
  theNum *= s; 
  normalize();
  return *this;
}

const ossimRationalNumber& ossimRationalNumber::operator+= (const ossimRationalNumber& r)
{
    // This calculation avoids overflow, and minimises the number of expensive
    // calculations. Thanks to Nickolay Mladenov for this algorithm.
    //
    // Proof:
    // We have to compute a/b + c/d, where gcd(a,b)=1 and gcd(b,c)=1.
    // Let g = gcd(b,d), and b = b1*g, d=d1*g. Then gcd(b1,d1)=1
    //
    // The result is (a*d1 + c*b1) / (b1*d1*g).
    // Now we have to normalize this ratio.
    // Let's assume h | gcd((a*d1 + c*b1), (b1*d1*g)), and h > 1
    // If h | b1 then gcd(h,d1)=1 and hence h|(a*d1+c*b1) => h|a.
    // But since gcd(a,b1)=1 we have h=1.
    // Similarly h|d1 leads to h=1.
    // So we have that h | gcd((a*d1 + c*b1) , (b1*d1*g)) => h|g
    // Finally we have gcd((a*d1 + c*b1), (b1*d1*g)) = gcd((a*d1 + c*b1), g)
    // Which proves that instead of normalizing the result, it is better to
    // divide num and den by gcd((a*d1 + c*b1), g)

    ossim_int32 g = ossim::gcd(theDen, r.theDen);
    theDen /= g;  // = b1 from the calculations above
    theNum = theNum * (r.theDen / g) + r.theNum * theDen;
    g = ossim::gcd(theNum, g);
    theNum /= g;
    theDen *= r.theDen/g;

    return *this;
}

const ossimRationalNumber& ossimRationalNumber::operator-= (const ossimRationalNumber& r)
{
    // This calculation avoids overflow, and minimises the number of expensive
    // calculations. It corresponds exactly to the += case above
    ossim_int32 g = ossim::gcd(theDen, r.theDen);
    theDen /= g;
    theNum = theNum * (r.theDen / g) - r.theNum * theDen;
    g = ossim::gcd(theNum, g);
    theNum /= g;
    theDen *= r.theDen/g;

    return *this;
}

const ossimRationalNumber& ossimRationalNumber::operator*= (const ossimRationalNumber& r)
{
    // Avoid overflow and preserve normalization
    ossim_int32 gcd1 = ossim::gcd(theNum, r.theDen);
    ossim_int32 gcd2 = ossim::gcd(r.theNum, theDen);
    theNum = (theNum/gcd1) * (r.theNum/gcd2);
    theDen = (theDen/gcd2) * (r.theDen/gcd1);
    
    return *this;
}

const ossimRationalNumber& ossimRationalNumber::operator/= (const ossimRationalNumber& r)
{
   ossim_int32 zero(0);
   
   if (r.theNum == zero)
   {
      theNum = OSSIM_INT_NAN;
      theDen = OSSIM_INT_NAN;
      
      return *this;
   }
   *this = (*this)*(ossimRationalNumber(r.theDen, r.theNum));

   return *this;
}

// Mixed-mode operators
const ossimRationalNumber& ossimRationalNumber::operator+= (ossim_int32 i)
{
    return operator += (ossimRationalNumber(i));
}

const ossimRationalNumber& ossimRationalNumber::operator-= (ossim_int32 i)
{
    return operator -= (ossimRationalNumber(i));
}

const ossimRationalNumber& ossimRationalNumber::operator*= (ossim_int32 i)
{
    return operator *= (ossimRationalNumber(i));
}

const ossimRationalNumber& ossimRationalNumber::operator/= (ossim_int32 i)
{
    return operator /= (ossimRationalNumber(i));
}

ossimRationalNumber ossimRationalNumber::operator+ (const ossimRationalNumber& r)const
{
    // This calculation avoids overflow, and minimises the number of expensive
    // calculations. Thanks to Nickolay Mladenov for this algorithm.
    //
    // Proof:
    // We have to compute a/b + c/d, where gcd(a,b)=1 and gcd(b,c)=1.
    // Let g = gcd(b,d), and b = b1*g, d=d1*g. Then gcd(b1,d1)=1
    //
    // The result is (a*d1 + c*b1) / (b1*d1*g).
    // Now we have to normalize this ratio.
    // Let's assume h | gcd((a*d1 + c*b1), (b1*d1*g)), and h > 1
    // If h | b1 then gcd(h,d1)=1 and hence h|(a*d1+c*b1) => h|a.
    // But since gcd(a,b1)=1 we have h=1.
    // Similarly h|d1 leads to h=1.
    // So we have that h | gcd((a*d1 + c*b1) , (b1*d1*g)) => h|g
    // Finally we have gcd((a*d1 + c*b1), (b1*d1*g)) = gcd((a*d1 + c*b1), g)
    // Which proves that instead of normalizing the result, it is better to
    // divide num and den by gcd((a*d1 + c*b1), g)

   ossim_int32 g = ossim::gcd(theDen, r.theDen);
   ossim_int32 den = theDen;
   ossim_int32 num = theNum;
   den /= g;  // = b1 from the calculations above
   num = num * (r.theDen / g) + r.theNum * den;
   g = ossim::gcd(num, g);
   num /= g;
   den *= r.theDen/g;

    return ossimRationalNumber(num, den);
}

ossimRationalNumber ossimRationalNumber::operator-(const ossimRationalNumber& r)const
{
   ossimRationalNumber result = *this;
    // This calculation avoids overflow, and minimises the number of expensive
    // calculations. It corresponds exactly to the += case above
    ossim_int32 g = ossim::gcd(result.theDen, r.theDen);
    result.theDen /= g;
    result.theNum = result.theNum * (r.theDen / g) - r.theNum * result.theDen;
    g = ossim::gcd(result.theNum, g);
    result.theNum /= g;
    result.theDen *= r.theDen/g;

    return result;
}

ossimRationalNumber ossimRationalNumber::operator*(const ossimRationalNumber& r)const
{
   ossimRationalNumber result = *this;
   // Avoid overflow and preserve normalization
   ossim_int32 gcd1 = ossim::gcd(result.theNum, r.theDen);
   ossim_int32 gcd2 = ossim::gcd(r.theNum, result.theDen);
   result.theNum = (result.theNum/gcd1) * (r.theNum/gcd2);
   result.theDen = (result.theDen/gcd2) * (r.theDen/gcd1);
   
   return result;
}

ossimRationalNumber ossimRationalNumber::operator/(const ossimRationalNumber& r)const
{
   ossim_int32 zero(0);
   
   if (r.theNum == zero)
   {
      return ossimRationalNumber(OSSIM_INT_NAN, OSSIM_INT_NAN);;
   }
   
   return (*this)*(ossimRationalNumber(r.theDen, r.theNum));
}
