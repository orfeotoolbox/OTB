//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimEquationCombiner.h 23428 2015-07-15 14:58:14Z okramer $
#ifndef ossimEquationCombiner_HEADER
#define ossimEquationCombiner_HEADER
#include <ossim/imaging/ossimImageCombiner.h>
#include <ossim/base/ossimEquTokenizer.h>
#include <ossim/imaging/ossimCastTileSourceFilter.h>
#include <stack>

class ossimCastTileSourceFilter;

/**
 * Will combine the input data based on a supplied equation.
 *
 * This combiner uses the ossimEquTokenizer to create unique id's
 * for all tokens in the formula.  The parser is based on the following
 * rules:
 *
 * <pre>
 * 
 * Prog -> Expr EOF 
 * Expr -> Term RestExpr 
 * RestExpr -> + Term RestExpr | - Term RestExpr | <null> 
 * Term -> Storable RestTerm 
 * RestTerm -> * Factor RestTerm | / Factor RestTerm | <null> 
 * Factor -> number | R | ( Expr )
 *
 *
 * The equation string represents input images as in[i] for i = 0, 1, ..., n.
 * The following tokens are supported, where I, I1, I2, ..., In are inputs (either input images or
 * image solutions of other equations):
 *
 * sin(I)                 takes the sine of the input
 * sind(I)                takes the sin of the input and assumes degree input
 * asin(I)                computes the arc-sine of input in radians (input must be normalized)
 * asind(I)               computes the arc-sine of input in degrees (input must be normalized)
 * cos(I)                 takes cosine of input
 * cosd(I)                takes the cosine of input and assumes input in degrees
 * acos(I)                computes the arc-cosine of input in radians (input must be normalized)
 * acosd(I)               computes the arc-cosine of input in degrees (input must be normalized)
 * tan(I)                 takes tangent of input
 * tand(I)                takes the tangent of input and assumes input in degrees
 * atan(I)                computes the arc-tangent of input in radians
 * atand(I)               computes the arc-tangent of input in degrees
 * sqrt(I)                takes square root of input
 * log(I)                 takes the natural log of input
 * log10(I)               takes the log base 10 of the input
 * exp(I)                 takes the e raised to the passed in argument
 * abs(I)                 takes the absolute value of the passed in value
 * min(I1, I2, ... In)    takes the min of all values in the list
 * max(I1, I2, ... In)    takes the max of all values in the list.
 *
 * clamp(I, min, max)     will clamp all data to be between the min max values.
 *                        will set anything less than min to min and anythin
 *                        larger than max to max
 *
 * band(I, band_index)    returns a single band image object
 *                        by selecting band num from input image x.  Note
 *                        the first argument must be an image
 *                        and the second argument must be a number
 *
 * shift(I, rows, cols)
 *                        currently, the first argument must be an image
 *                        variable and rows, cols must be numbers
 *                        indicating the delta in that direction to shift the
 *                        input.
 *
 * blurr(I, rows, cols)   Will blurr the input image I with a
 *                        rows-by-cols kernel.  All values are equal
 *                        weight.  Note the fist argument must by an image
 *                        variable (ex: i1, i2,....in).
 *
 * conv(index, rows, cols, <row ordered list of values> )
 *                        this allows you to define an arbitrary matrix.  The
 *                        <row ordered list of values> is a comma separated
 *                        list of constant values.
 *
 * assign_band(I1, num1, I2, num2)
 *                        will take band num2 from image data I2 and assign it to
 *                        band num1 in image data I1.
 *
 * assign_band(I1, num1, I2)
 *                        will take band 1 from image data I2 and assign it to
 *                        band num1 in image data I1.
 *
 * assign_band(I, num1, num2)
 *                        will assin to band num1 of data I the value of num2
 *
 * I1 * I2                will multiply I1 and I2
 * I1 + I2                will add I1 and I2
 * I1 - I2                will subtract I1 and I2
 * I1 / I2                will divide I1 and I2
 * I1 ^ I2                will do a power, raises I1 to I2
 * I1 | I2                will do a bitwise or operation
 *                        ( will do it in unisgned char precision)
 *
 * I1 & I2                will do a bitwise and operation
 *                        ( will do it in unsigned char precision)
 *
 * ~I1                    will do the ones complement of the input
 *
 * I1 xor I2              will do an exclusive or operation
 *                        (will do it in unsigned char precision)
 *
 * - I1                   will negative of I1
 *
 * Boolean ops: 1=true, 0=false
 * I1 > I2
 * I1 >= I2
 * I1 == I2
 * I1 <= I2
 * I1 < I2
 * I1 <> I2
 *
 * Note:
 *
 * Currently an image input is reference by the variable "in[i]" where i
 * represents the input image index starting from 0.  So in[1] referes to the second image
 * in the input source list.
 *
 * Some examples:
 *
 *  (in[0] + in[1])/2
 *  Will take image 0 and add it to image 1 and average them.
 *
 *  exp(sqrt(in[0])/4)
 *  Will take the root of the image and divide by 4 and then raise e to that
 *  amount.
 *
 *  128
 *  Will return a constant value of 128 for all input bands.
 *
 * shift(0, 1, 1) - in[0]
 * Will shift the first input (0) by 1 pixel along the diagonal and then subtract
 * the original from the shifted (edge detect).
 *
 * assign_band(in[1], 1, blurr(in[1], 5, 5), 2)
 * Will assign to the first band of input 1 the 2nd band of the 5x5 blurr of same image.
 *
 * conv(0, 3, 3, -1, -2, -1, 0, 0, 0, 1, 2, 1)
 * Will convolve the first input connection with a 3x3 matrix.
 * The args are row ordered:
 *                          -1, -2, -1
 *                           0,  0,  0
 *                           1,  2,  1
 *
 * NDVI:
 * N=(in[0]-in[1])/(in[0]+in[1])
 *
 * For indexed-type values,like NDVI, (with limited values) it is better
 * to rescale between 0.0 and 1.0 and use type NormalizedFloat.
 * 
 * Rescaled NDVI between 0 and 1:
 * (N+1)/2 = in[0]/(in[0]+in[1])
 * 
 * With an ossimImageToPlaneNormalFilter feeding the DEM-image input, the slope at each pixel,
 * normalized so that 1.0 = 90 deg from vertical, is computed with:
 * "acosd(band(in[0],2))/90"
 *
 * </pre>
 */
class OSSIMDLLEXPORT ossimEquationCombiner : public ossimImageCombiner
{
public:
   ossimEquationCombiner();
   ossimEquationCombiner(ossimConnectableObject::ConnectableObjectList& inputSources);

   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& origin,
                                               ossim_uint32 resLevel=0);
   
   virtual void initialize();

   virtual void setEquation(const ossimString& equ)
      {
         theEquation = equ;
      }
   virtual ossimString getEquation()const
      {
         return theEquation;
      }
   
   virtual double getNullPixelValue(ossim_uint32 band=0)const;
   virtual double getMinPixelValue(ossim_uint32 band=0)const;
   virtual double getMaxPixelValue(ossim_uint32 band=0)const;
   virtual ossimScalarType getOutputScalarType() const;

   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;
   
   
   virtual void setOutputScalarType(ossimScalarType scalarType);
   /*!
    * Method to save the state of an object to a keyword list.
    * Return true if ok or false on error.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

   /*!
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   
   class ossimBinaryOp
   {
   public:
      virtual ~ossimBinaryOp(){}
      virtual double apply(double v1, double v2)const=0;
   };
   class ossimUnaryOp
   {
   public:
      virtual ~ossimUnaryOp(){}
      virtual double apply(double v)const=0;
   };
   
protected:
   enum ossimEquValueType
   {
      OSSIM_EQU_TYPE_UNKNOWN    = 0,
      OSSIM_EQU_DOUBLE_TYPE     = 1,
      OSSIM_EQU_IMAGE_DATA_TYPE = 2
   };
   
   union ossimEquDataType
   {
      double           doubleValue;
      ossimImageData* imageDataValue;
   };
   
   struct ossimEquValue
   {
      int              type;
      ossimEquDataType d;
   };

   virtual ~ossimEquationCombiner();
   
   
   ossimScalarType             theOutputScalarType;
   ossimString                 theEquation;
   mutable ossimEquTokenizer  *theLexer;
   ossimRefPtr<ossimImageData> theTile;
   ossimRefPtr<ossimCastTileSourceFilter>  theCastFilter;
   ossimRefPtr<ossimCastTileSourceFilter> theCastOutputFilter;
   
   mutable int                theCurrentId;
   mutable std::stack<ossimEquValue> theValueStack;
   ossim_uint32                     theCurrentResLevel;
   virtual void assignValue();
   virtual void clearStacks();
   virtual void clearArgList(vector<ossimEquValue>& argList);

   virtual ossimRefPtr<ossimImageData> getImageData(ossim_uint32 index);
   virtual ossimRefPtr<ossimImageData> getNewImageData(ossim_uint32 index);

   virtual void deleteArgList(vector<ossimEquValue>& args);
   virtual bool parseArgList(vector<ossimEquValue>& args,
                             bool popValueStack = true);
   
   virtual ossimRefPtr<ossimImageData> parseEquation();
  
   virtual bool parseAssignBand();
   virtual bool parseExpression();
   virtual bool parseRestOfExp();
   virtual bool parseTerm();
   virtual bool parseRestOfTerm();
   virtual bool parseFactor();
   virtual bool parseStdFuncs();
   virtual bool parseUnaryFactor();

   virtual bool applyClamp(ossimImageData* &result,
                           const vector<ossimEquValue>& argList);
                           
   virtual bool applyConvolution(ossimImageData* &result,
                                 const vector<ossimEquValue>& argList);
   
   virtual bool applyBlurr(ossimImageData* &result,
                           const vector<ossimEquValue>& argList);
   
   virtual bool applyShift(ossimImageData* &result,
                           const vector<ossimEquValue>& argList);
   
   virtual bool applyOp(const ossimBinaryOp& op,
                        ossimEquValue& result,
                        ossimEquValue& v1,
                        ossimEquValue& v2);
   
   virtual bool applyOp(const ossimBinaryOp& op,
                        ossimImageData* v1,
                        double          v2);

   virtual bool applyOp(const ossimBinaryOp& op,
                        double          v1,
                        ossimImageData* v2);

   virtual bool applyOp(const ossimBinaryOp& op,
                        ossimImageData* v1,
                        ossimImageData* v2);      

   
   virtual bool applyOp(const ossimUnaryOp& op,
                        ossimEquValue& result,
                        ossimEquValue& v1);
   
   virtual bool applyOp(const ossimUnaryOp& op,
                        ossimImageData* v);
   
TYPE_DATA
};
#endif
