//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// AUTHOR: Oscar Kramer
//
// DESCRIPTION:
//   Contains declaration of class ossimWarpProjection. This is an
//   implementation of a warping interpolation model.
//
//*****************************************************************************
//  $Id: ossimWarpProjection.h 17207 2010-04-25 23:21:14Z dburken $

#ifndef ossimWarpProjection_HEADER
#define ossimWarpProjection_HEADER

#include <ossim/projection/ossimProjection.h>
#include <ossim/base/ossimIpt.h>

class ossim2dTo2dTransform;

/*!****************************************************************************
 *
 * CLASS:  ossimWarpProjection
 *
 *****************************************************************************/
class OSSIMDLLEXPORT ossimWarpProjection : public ossimProjection
{
public:
   /*!
    * Default Contructor:
    */
   ossimWarpProjection();

   /*!
    * Primary constructor accepting pointer to the underlying client projection.
    */
   ossimWarpProjection(ossimProjection* client);

   /*!
    * Constructor accepts OSSIM keywordlist geometry file.
    */
   ossimWarpProjection(const ossimKeywordlist& geom_kwl,
                       const char* prefix=0);
   
   ~ossimWarpProjection();
   
   /*!
    * METHOD: worldToLineSample()
    * Performs the forward projection from ground point to line, sample.
    */
   virtual void worldToLineSample(const ossimGpt& worldPoint,
                                  ossimDpt&       lineSampPt) const;

   /*!
    * METHOD: lineSampleToWorld()
    * Performs the inverse projection from line, sample to ground (world):
    */
   virtual void lineSampleToWorld(const ossimDpt& lineSampPt,
                                      ossimGpt&       worldPt) const;
   
   /*!
    * Performs the inverse projection from line, sample to ground, bypassing
    * reference to elevation surface:
    */
   virtual void lineSampleHeightToWorld(const ossimDpt& lineSampPt,
                                        const double&   hgtEllipsoid,
                                        ossimGpt&       worldPt) const;
   
   /*!
    * Extends base-class implementation. Dumps contents of object to ostream.
    */
   virtual std::ostream& print(std::ostream& out) const;
   friend std::ostream& operator<<(std::ostream& os,
                                   const ossimWarpProjection& m);
   
   /*!
    * Fulfills ossimObject base-class pure virtuals. Loads and saves geometry
    * KWL files. Returns true if successful.
    */
   virtual bool saveState(ossimKeywordlist& kwl, const char* prefix=0) const;
   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix=0);

   /*!
    * Returns pointer to a new instance, copy of this.
    */
   virtual ossimObject* dup() const;
   
   /*!
    * Returns projection's ground point origin. That is the GP corresponding
    * to line=0, sample=0.
    */
   virtual ossimGpt origin() const;

   /*!
    * Compares this instance with arg projection. NOT IMPLEMENTED.
    */
   virtual bool operator==(const ossimProjection& /* projection */) const
      { return false; }

   virtual ossimDpt getMetersPerPixel() const;

   virtual ossim2dTo2dTransform* getWarpTransform()
      {
         return theWarpTransform.get();
      }
   virtual ossim2dTo2dTransform* getAffineTransform()
      {
         return theAffineTransform.get();
      }
   virtual ossimProjection* getClientProjection()
      {
         return theClientProjection.get();
      }
   virtual void setNewWarpTransform(ossim2dTo2dTransform* warp);
   virtual void setNewAffineTransform(ossim2dTo2dTransform* affine);

   /**
    * @brief Implementation of pure virtual
    * ossimProjection::isAffectedByElevation method.
    * @return based on theClientProjection->isAffectedByElevation().
    * If theClientProjection is null this returns true just to be safe.
    */
   virtual bool isAffectedByElevation() const;
   
protected:
   
   /*!
    * Data Members:
    */
   ossimRefPtr<ossimProjection>      theClientProjection;
   ossimRefPtr<ossim2dTo2dTransform> theWarpTransform;
   ossimRefPtr<ossim2dTo2dTransform> theAffineTransform;
   
   TYPE_DATA
};

inline bool ossimWarpProjection::isAffectedByElevation() const
{
   return ( theClientProjection.valid() ?
            theClientProjection->isAffectedByElevation() :
            false );
}

#endif
