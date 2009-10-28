//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Hicks
//
// Description: Base class for Synthetic Aperture Radar model.
//
//              This model represents a standard model using relatively
//              generic support data based on the following references:
//                [1] Modern Photogrammetry; Mikhail, Bethel, & McGlone;
//                    Sections 11.7-11.9
//                [2] The Compendium of Controlled Extensions for NITFS
//                    21 Mar 2006, paragraph E.3.8, SAR MPDSR
//
//----------------------------------------------------------------------------
// $Id: ossimSarModel.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimSarModel_HEADER
#define ossimSarModel_HEADER

#include <ossim/projection/ossimSensorModel.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimEcefPoint.h>
#include <ossim/base/ossimEcefVector.h>

class ossimHgtRef;


class OSSIM_DLL ossimSarModel : public ossimSensorModel
{
public:

   enum AcquisitionMode
   {
      UNKNOWN = 0,
      SCAN    = 1,
      SPOT    = 2
   };

   enum AdjustParamIndex
   {
      X_POS = 0,
      Y_POS,
      Z_POS,
      NUM_ADJUSTABLE_PARAMS // not an index
   };

   /** @brief default constructor */
   ossimSarModel();

   /**
    * @brief Method to load or recreate the state of an ossimSarModel from
    * a keyword list.
    *
    * @param kwl    Keyword list to load from.
    * @param prefix Prefix for keywords, like "image01.".
    *
    * @return true on success, false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   /**
    * @brief Method to save the state of this object to a keyword list.
    *
    * @param kwl    Keyword list to save to.
    * @param prefix Prefix for keywords, like "image01.".
    *
    * @return true on success, false on error.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0) const;
   
   /*!
    * Writes a template of geom keywords processed by loadState and saveState
    * to output stream.
    */
   static void writeGeomTemplate(ostream& os);
                          
                          
   /*!
    * Returns pointer to a new instance, copy of this.
    */
   virtual ossimObject* dup() const { return 0; } // TBR

   /**
    * @brief print method.
    */
   virtual std::ostream& print(std::ostream& out) const;


   /**
    * @brief 
    */
   virtual void lineSampleToWorld(const ossimDpt& image_point,
                                  ossimGpt&       world_point) const;
   virtual void worldToLineSample(const ossimGpt& world_point,
                                  ossimDpt&       image_point) const;
   virtual void lineSampleHeightToWorld(const ossimDpt& lineSampPt,
                                        const double&   heightAboveEllipsoid,
                                        ossimGpt&       worldPt) const;
   /**
    * METHOD: imagingRay(image_point, &ossimEcefRay)
    *  Given an image point, returns a ray originating at the ARP position
    *  and pointing towards the target's position in the Output
    *  Plane.
    *  This DOES NOT provide the conventional definition for an imaging ray
    *  because the imaging locus for SAR is not a ray.
    *
    *  It DOES provide a radius vector for the range/Doppler circle.
    */
   virtual void imagingRay(const ossimDpt& image_point,
                           ossimEcefRay&   image_ray) const;

   
   /**
    * @brief Compute partials of range/Doppler WRT ground point
    *
    * @param parmIdx computational mode:
    *        OBS_INIT, EVALUATE, P_WRT_X, P_WRT_X, P_WRT_X.
    * @param gpos Current ground point estimate.
    * @param h Not used.
    *
    * @return OBS_INT: n/a, EVALUATE: residuals, P_WRT_X/Y/Z: partials.
    */
   virtual ossimDpt getForwardDeriv(int parmIdx, const ossimGpt& gpos, double h);

   /**
    * ossimOptimizableProjection
    */
   inline virtual bool useForward() const
   {
      return false; //!image to ground faster
   } 

   /**
    * @brief Compute other parameters & update the model.
    */
   virtual void updateModel();

   /*!
    * METHOD: getObsCovMat()
    * gives 2X2 covariance matrix of observations
    */
   virtual ossimSensorModel::CovMatStatus getObsCovMat(
      const ossimDpt& ipos, NEWMAT::SymmetricMatrix& Cov);
   
protected:
   /** @brief virtual destructor */
   virtual ~ossimSarModel();
   
   
   /*!
    * Assigns initial default values to adjustable parameters and related
    * members.
    */
   void initAdjustableParameters();

   /**
    * @brief Get ARP time for SPOT mode (constant time).
    */
   virtual ossim_float64 getArpTime() const;

   /**
    * @brief Get ARP time for SCAN mode (varying time).
    *
    * @param imgPt The image coordinates.
    */
   virtual ossim_float64 getArpTime(const ossimDpt& imgPt) const;

   /**
    * @brief Get ARP position for SPOT mode (constant time).
    */
   virtual ossimEcefPoint  getArpPos() const;

   /**
    * @brief Get ARP position for SCAN mode (varying time).
    */
   virtual ossimEcefPoint  getArpPos(const ossim_float64& time) const;

   /**
    * @brief Get ARP velocity for SPOT mode (constant time).
    */
   virtual ossimEcefVector getArpVel() const;

   /**
    * @brief Get ARP velocity for SPOT mode (constant time).
    */
   virtual ossimEcefVector getArpVel(const ossim_float64& time) const;
   
   /**
    * @brief Method to compute range & Doppler.
    *
    * @param pt ECF ground point coordinates.
    * @param arpPos ECF ARP position.
    * @param arpVel ECF ARP velocity.
    * @param range  range.
    * @param arpVel Doppler.
    *
    * @return true on success, false on error.
    */
   virtual bool computeRangeDoppler(const ossimEcefPoint& pt,
                                    const ossimEcefPoint& arpPos,
                                    const ossimEcefVector& arpVel,
                                    ossim_float64& range,
                                    ossim_float64& doppler) const;

  /**
    * @brief Method to compute image coordinates from output plane coordinates.
    *
    * @param opPt  ECF output plane position of point.
    * @param imgPt image position of point.
    *
    * @return true on success, false on error.
    */
   virtual bool computeImageFromOP(const ossimEcefPoint& opPt, ossimDpt& imgPt) const;
   
   /**
    * @brief Method to compute output plane coordinates from image coordinates.
    *
    * @param imgPt image position of point.
    * @param opPt  ECF output plane position of point.
    *
    * @return true on success, false on error.
    */
   virtual bool computeOPfromImage(const ossimDpt& imgPt, ossimEcefPoint& opPt) const;
   
   /**
    * @brief Method to project output plane coordinates to surface.
    *
    * @param opPt   ECF output plane point coordinates.
    * @param range  range.
    * @param arpVel Doppler.
    * @param arpPos ECF ARP position.
    * @param arpVel ECF ARP velocity.
    * @param hgtRef Height reference defining intersection surface.
    * @param ellPt  ECF point coordinates.
    *
    * @return true on success, false on error.
    */
   virtual bool projOPtoSurface(const ossimEcefPoint& opPt,
                                const ossim_float64& range,
                                const ossim_float64& doppler,
                                const ossimEcefPoint& arpPos,
                                const ossimEcefVector& arpVel,
                                const ossimHgtRef* hgtRef,
                                ossimEcefPoint& ellPt) const;

   /**
    * @brief Method to project ellipsoid coordinates to output plane.
    *
    * @param ellPt ECF ellipsoid position.
    * @param opPt  ECF output plane position of point.
    *
    * @return true on success, false on error.
    */
   virtual bool projEllipsoidToOP(const ossimEcefPoint& ellPt,
                                  ossimEcefPoint& opPt) const;



   /**
    * Returns the acquisition mode as a string.
    */
   ossimString getAcquistionModeString() const;

   /**
    * @brief Sets the acquisition mode from string.
    *
    * @param mode The string representing mode.
    */
   void setAcquisitionMode(const ossimString& mode);

   /** acquisition mode */
   AcquisitionMode theAcquisitionMode;

   /** Output/Ground Reference Point (ORP) position */
   ossimEcefPoint theOrpPosition;

   /** sample (x)/line(y) image coordinates of ORP  */
   ossimDpt theOrpCenter;

   /** output plane normal */
   ossimEcefVector theOutputPlaneNormal;

   /** output plane x-axis */
   ossimEcefVector theOutputPlaneXaxis;

   /** output impulse response */
   ossim_float64 theOipr;

   /** pixel size */
   ossim_float64 thePixelSize;

   /** Aperture Reference/Center Point (ARP) time in seconds. */
   ossim_float64 theArpTime;

   /**
    * Aperture Reference Point (ARP) Polynomials.
    * Note: Size is derived dependent.
    */
   vector<ossim_float64> theArpXPolCoeff;
   vector<ossim_float64> theArpYPolCoeff;
   vector<ossim_float64> theArpZPolCoeff;

   /** Time Coefficients.  Note: Size is derived dependent. */
   vector<ossim_float64> theTimeCoeff;

   /**
    * Other computed parameters
    */
    
   /** Pixel spacing */
   ossim_float64 thePixelSpacing;
   
   /** Output/slant plane unit vectors */
   ossimEcefVector theOPX;
   ossimEcefVector theOPY;
   ossimEcefVector theOPZ;
   
   
   /**
    * Adjustment-related data used and set by getForwardDeriv
    */
   /** Observations for current point */
   ossim_float64   theObsRng;
   ossim_float64   theObsDop;
   ossim_float64   theObsTime;
   ossimEcefPoint  theObsPosition;
   ossimEcefPoint  theObsOP;
   ossimEcefPoint  theObsArpPos;
   ossimEcefVector theObsArpVel;
   ossimEcefVector theObsArpVel_U;
   ossim_float64   theObsArpVel_Mag;
   
   /** Partials for current point */
   ossim_float64 theParDopWRTaz;

   /** Adjustable parameters */
   ossimEcefVector theLsrOffset;      // meters

   TYPE_DATA
};

#endif /* #ifndef ossimSarModel_HEADER */
