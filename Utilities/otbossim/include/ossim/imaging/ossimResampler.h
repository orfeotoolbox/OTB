//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Garrett Potts
//*******************************************************************
//  $Id: ossimResampler.h 17195 2010-04-23 17:32:18Z dburken $

#ifndef ossimResampler_HEADER
#define ossimResampler_HEADER

#include <ossim/base/ossimConnectableObject.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimRationalNumber.h>

class ossimImageData;

/*!
 * This is currently implemented as a  symmetric kernel resampler.
 * It will use a lookup table idea to precompute
 *
 * The resampler will support Nearest neighbor, Bilinear and Bicubic
 *
 * The lookup table approach will be borrowed from Michael J. Aramini
 * implementation of the Bicubic convolution:
 *
 *       http://www.ultranet.com/~aramini/
 *       under:
 *        Efficient Image Magnification by Bicubic Spline Interpolation.
 */
class ossimResampler : public ossimConnectableObject
{   
public:
   enum ossimResLevelResamplerType
   {
      ossimResampler_NONE             = 0,
      ossimResampler_NEAREST_NEIGHBOR = 1,
      ossimResampler_BILINEAR         = 2,
      ossimResampler_BICUBIC          = 3
   };
   ossimResampler();

   
   /*!
    * Will apply the kernel to the input and write it to the output.
    *
    * Note: theTable is re-generated if the Max(out_width, out_height).
    *       changes from the previous call.
    */
   virtual void resample(ossimImageData* input, // input buffer
                         ossimImageData* output);


   virtual void resample(ossimImageData* input,
                         ossimImageData* output,
                         const ossimDpt& ul,
                         const ossimDpt& ur,
                         const ossimDpt& deltaUl,
                         const ossimDpt& deltaUr,
                         const ossimDpt& length);

   virtual void resample(ossimImageData* input,
                         ossimImageData* output,
                         const ossimIrect& outputSubRect,
                         const ossimDpt& ul,
                         const ossimDpt& ur,
                         const ossimDpt& deltaUl,
                         const ossimDpt& deltaUr,
                         const ossimDpt& length);

   virtual void resampleNearestNeighbor(ossimImageData* input,
                                        ossimImageData* output,
                                        const ossimIrect& outputSubRect,
                                        const ossimDpt& ul,
                                        const ossimDpt& ur,
                                        const ossimDpt& deltaUl,
                                        const ossimDpt& deltaUr,
                                        const ossimDpt& length);

   virtual void resampleNearestNeighbor(ossimImageData* input,
                                        ossimImageData* output,
                                        const ossimDpt& ul,
                                        const ossimDpt& ur,
                                        const ossimDpt& deltaUl,
                                        const ossimDpt& deltaUr,
                                        const ossimDpt& length);
   /*!
    * Will re-allocate the table
    */
   virtual void setResamplerType(ossimResLevelResamplerType type);

   virtual ossimResLevelResamplerType getResamplerType()const
      {
         return theResamplerType;
      }

   virtual double getCubicParameter()const
      {
         return theCubicAdjustableParameter;
      }
   virtual void setCubicParameter(double parameter)
      {
         theCubicAdjustableParameter = parameter;
         theCubicAdjustableParameter = theCubicAdjustableParameter<-1?-1:theCubicAdjustableParameter;
         theCubicAdjustableParameter = theCubicAdjustableParameter>0?0:theCubicAdjustableParameter;
         if(theResamplerType ==  ossimResampler_BICUBIC)
         {
            generateWeightTable();
         }
      }
   /*!
    * Will reallocate the table
    */
   void setRatio(double outputToInputRatio);
   void setRatio(const ossimDpt& outputToInputRatio);
   ossimDpt getRatio()const
      {
         return theOutputToInputRatio;
      }
   virtual ossim_int32 getKernelWidth()const;
   virtual ossim_int32 getKernelHeight()const;
   
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

   bool canConnectMyInputTo(ossim_int32 /* inputIndex */,
                            const ossimConnectableObject* /* object */)const
   {
      return false;
   }
protected:
   virtual ~ossimResampler();

   ossimDpt                   theOutputToInputRatio;
   ossimResLevelResamplerType theResamplerType;  

   ossim_int32                     theTableWidthX;
   ossim_int32                     theTableWidthY;
   ossim_int32                     theTableHeight;
   ossim_int32                     theKernelWidth;
   ossim_int32                     theKernelHeight;
   /*!
    */
   double **theWeightTableX;
   double **theWeightTableY;

   /*!
    * This adjustable parameter can vary between
    * -1 to 0.  as the paramter goes from 0
    * to -1 the output goes from blocky to smooth.
    * The default value is -.5
    */
   double   theCubicAdjustableParameter;

   template <class T>
   void resampleTile(T, // dummy tmeplate variable
                     ossimImageData* input,
                     ossimImageData* output);
   
   template <class T>
   void resampleFullTile(T, // dummy tmeplate variable
                         ossimImageData* input,
                         ossimImageData* output);
   template <class T>
   void resamplePartialTile(T, // dummy tmeplate variable
                            ossimImageData* input,
                            ossimImageData* output);
   
   template <class T>
   void resampleTile(T, // dummy template variable
                     ossimImageData* input,
                     ossimImageData* output,
                     const ossimIrect& outputSubRect,
                     const ossimDpt& ul,
                     const ossimDpt& ur,
                     const ossimDpt& deltaUl,
                     const ossimDpt& deltaUr,
                     const ossimDpt& length);

   template <class T>
   void resampleTileNearestNeighbor(T, // dummy template variable
                                    ossimImageData* input,
                                    ossimImageData* output,
                                    const ossimDpt& ul,
                                    const ossimDpt& ur,
                                    const ossimDpt& deltaUl,
                                    const ossimDpt& deltaUr,
                                    const ossimDpt& length);

   template <class T>
   void resampleTileNearestNeighbor(T, // dummy template variable
                                    ossimImageData* input,
                                    ossimImageData* output,
                                    const ossimIrect& subRect,
                                    const ossimDpt& ul,
                                    const ossimDpt& ur,
                                    const ossimDpt& deltaUl,
                                    const ossimDpt& deltaUr,
                                    const ossimDpt& length);
   
   virtual void deleteWeightTable();
   virtual void allocateWeightTable();
   virtual void generateWeightTable();

   double getCubicC0(double t)const;
   double getCubicC1(double t)const;
   double getCubicC2(double t)const;
   double getCubicC3(double t)const;
   
TYPE_DATA
};

#endif
