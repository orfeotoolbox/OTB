//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// AUTHOR: Oscar Kramer
//
// DESCRIPTION:
//   Contains declaration of class ossimAffineProjection. This is an
//   implementation of a warping interpolation model.
//
//*****************************************************************************
//  $Id: ossimAffineProjection.h 17207 2010-04-25 23:21:14Z dburken $

#ifndef ossimAffineProjection_HEADER
#define ossimAffineProjection_HEADER

#include <ossim/projection/ossimProjection.h>
#include <ossim/base/ossimIpt.h>
#include <iosfwd>
#include <ossim/base/ossim2dTo2dTransform.h>
#include <ossim/base/ossimRefPtr.h>
/*!****************************************************************************
 *
 * CLASS:  ossimAffineProjection
 *
 *****************************************************************************/
class ossimAffineProjection : public ossimProjection
{
public:
   /*!
    * Default Contructor:
    */
   ossimAffineProjection();

   /*!
    * Primary constructor accepting pointer to the underlying client projection.
    */
   ossimAffineProjection(ossimProjection* client);

   /*!
    * Constructor accepts OSSIM keywordlist geometry file.
    */
   ossimAffineProjection(const ossimKeywordlist& geom_kwl,
                       const char* prefix=NULL);
   
   ~ossimAffineProjection();
   
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
                                   const ossimAffineProjection& m);
   
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

   /**
    * @brief Implementation of pure virtual
    * ossimProjection::isAffectedByElevation method.
    * @return false.
    */
   virtual bool isAffectedByElevation() const { return false; }
      
protected:
   
   /*!
    * Data Members:
    */
   ossimRefPtr<ossimProjection>      theClientProjection;
   ossimRefPtr<ossim2dTo2dTransform> theAffineTransform;
   
   TYPE_DATA
};

#endif
