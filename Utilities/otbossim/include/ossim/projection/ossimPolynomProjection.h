//*****************************************************************************
// FILE:   ossimPolynomProjection.h
// AUTHOR: Frederic Claudel Meraka/CSIR
//*****************************************************************************

#ifndef ossimPolynomProjection_HEADER
#define ossimPolynomProjection_HEADER

#include <ossim/projection/ossimProjection.h>
#include <ossim/projection/ossimOptimizableProjection.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimPolynom.h>

/**
 * class for ground to image polynomial projection: (x,y,z)|->(x,y)
 * polynom order is configurable
 */
class OSSIM_DLL ossimPolynomProjection : public ossimProjection,
                                         public ossimOptimizableProjection
{
public:
   
   /**
    * Constructors
    */
   ossimPolynomProjection();
   ossimPolynomProjection(const ossimPolynomProjection& copyme);
   ~ossimPolynomProjection();

   /**
    * Basic operations
    */
   virtual bool operator==(const ossimProjection& projection) const;
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0) const;
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   virtual std::ostream& print(std::ostream& out) const;
   virtual ossimObject* dup() const { return new ossimPolynomProjection(*this); }

   /**
    * Projection / localization methods
    */
   virtual void  worldToLineSample(const ossimGpt& world_point,
                                   ossimDpt&       image_point) const;
   virtual void  lineSampleToWorld(const ossimDpt& image_point,
                                   ossimGpt&       world_point) const;
   virtual void lineSampleHeightToWorld(const ossimDpt& image_point,
                                        const double&   heightEllipsoid,
                                        ossimGpt&       worldPoint) const;
   virtual ossimGpt origin()const;
   virtual ossimDpt getMetersPerPixel() const;
   
   /**
    * optimizableProjection implementation
    */
   virtual bool setupOptimizer(const ossimString& setup);
   inline virtual bool useForward()const {return true;}
   virtual ossim_uint32 degreesOfFreedom()const;
   virtual double optimizeFit(const ossimTieGptSet& tieSet, double* targetVariance=NULL);
   
   /**
    * accessors (not all of them inline)
    */
   const ossimPolynom< ossim_float64 , 3 >::EXPT_SET& getExpSet()const { return theExpSet; }
   void setExpSet(const ossimPolynom< ossim_float64 , 3 >::EXPT_SET& nset) { theExpSet = nset; }
   void setupDesiredExponents(int horizonal_ord, int vertical_ord); //!added for easy setup
   bool setupDesiredExponents(const ossimString& monoms);//!added for easy setup

   const ossimPolynom< ossim_float64 , 3 >& getPolyLine()const {return thePolyLine;}
   void setPolyLine(const ossimPolynom< ossim_float64 , 3 >& poly);
   const ossimPolynom< ossim_float64 , 3 >& getPolySamp()const {return thePolySamp;}
   void setPolySamp(const ossimPolynom< ossim_float64 , 3 >& poly);

   const ossim_float64& getLonOffset()const { return theLonOffset;}
   void setLonOffset(const ossim_float64& v) { theLonOffset = v; }
   const ossim_float64& getLonScale()const { return theLonScale;}
   void setLonScale(const ossim_float64& v) { theLonScale = v; }

   const ossim_float64& getLatOffset()const { return theLatOffset;}
   void setLatOffset(const ossim_float64& v) { theLatOffset = v; }
   const ossim_float64& getLatScale()const { return theLatScale;}
   void setLatScale(const ossim_float64& v) { theLatScale = v; }

   const ossim_float64& getHgtOffset()const { return theHgtOffset;}
   void setHgtOffset(const ossim_float64& v) { theHgtOffset = v; }
   const ossim_float64& getHgtScale()const { return theHgtScale;}
   void setHgtScale(const ossim_float64& v) { theHgtScale = v; }

   /**
    * @brief Implementation of pure virtual
    * ossimProjection::isAffectedByElevation method.
    * @return true.
    */
   virtual bool isAffectedByElevation() const { return true; }

protected:
   /**
    * desired exponents tuples set (for both line and sample polynoms)
    * used for optimization
    */
   ossimPolynom< ossim_float64 , 3 >::EXPT_SET theExpSet;

   /** 
    * ground (3d) to image(2D) polynoms
    */
   ossimPolynom< ossim_float64 , 3 > thePolyLine;//!xyz polynom
   ossimPolynom< ossim_float64 , 3 > thePolySamp;

   /**
    * polynom partial derivatives
    */
   ossimPolynom< ossim_float64 , 3 > thePolyLine_DLon;
   ossimPolynom< ossim_float64 , 3 > thePolyLine_DLat;
   ossimPolynom< ossim_float64 , 3 > thePolySamp_DLon;
   ossimPolynom< ossim_float64 , 3 > thePolySamp_DLat;

   /**
    * normalization parameters
    */
   ossim_float64 theLonOffset;
   ossim_float64 theLonScale;
   ossim_float64 theLatOffset;
   ossim_float64 theLatScale;
   ossim_float64 theHgtOffset;
   ossim_float64 theHgtScale;

   /**
    * protected methods
    */
   void buildDerivatives();
   void buildLineDerivatives();
   void buildSampDerivatives();
   bool stringToExp(const ossimString& s, ossimPolynom< ossim_float64 , 3 >::EXP_TUPLE& et)const; //!takes string of xyz monoms, x2z3
   int getSymbol(ossimString& ts,const ossimString& symbols)const;
   int getExponent(ossimString& ts)const;

TYPE_DATA
};

#endif

