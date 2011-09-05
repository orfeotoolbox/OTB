//**************************************************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Oscar Kramer (oscarkramer@yahoo.com)
//
// Description: Class declaration of ossimImageGeometry.
//
//**************************************************************************************************
// $Id$

#ifndef ossimImageGeometry_HEADER
#define ossimImageGeometry_HEADER 1

#include <vector>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimRtti.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/base/ossim2dTo2dTransform.h>

//**************************************************************************************************
//! Container class that holds both 2D transform and 3D projection information for an image
//! Only one instance of this is alive per image. This is  the object that is returned from
//! a call to ossimImageSource::getImageGeometry()
//! 
//! All images in OSSIM have at least a 2D transformation from local (relative to start of file)
//! to full-image coordinates. Often this transform is an identity transform (does nothing). In 
//! some cases however, the first pixel in the file does not correspond to the upper-left pixel of
//! the original, full image. Since the image's sensor model may only apply to full-image pixel 
//! coordinates, it is necessary to reference the latter when making computing ground point 
//! location.  Also the local image could be a reduced resolution level of the full image.  So
//! the image could be a decimated sub image of the full image.
//!
//! Additionally, images typically are accompanied by some form of metadata that defines the mapping
//! from 2D image coordinates to 3D world coordinates. This mapping may be in the form of a map
//! projection for orthorectified images, or a perspective projection such as a sensor model.
//! 
//! This object class maintains both 2D transform and 3D projection information for the associated
//! image. This object will typically be created and "owned" by the image handler object. Therefore,
//! only one copy per image will exist at a time.
//!
//! Notes:
//! 1) Shifts are relative to "full image space".  So if you have a sub image from r2 the shift
//!    given to the transform should be relative to "full image space".
//
//! 2) A decimation of 1.0 is the full image.  This may or may not be r0 as r0 can be decimated.
//!
//**************************************************************************************************
class OSSIM_DLL ossimImageGeometry : public ossimObject
{
public:
   //! Default constructor defaults to unity transform with no projection.
   ossimImageGeometry();
   virtual ~ossimImageGeometry();
   
   //! Copy constructor -- performs a deep copy. This is needed when an imageSource in the chain
   //! needs to modify the image geometry at that point in the chain. The image geometry to the 
   //! left of that imageSource must remain unchanged. This constructor permits that imageSource to 
   //! maintain its own geometry based on the input geometry. All chained objects to the right
   //! would see this alternate geometry. See ossimScaleFilter for an example of this case.
   ossimImageGeometry(const ossimImageGeometry& copy_this);

   //! Constructs with projection and transform objects available for referencing. Either pointer
   //! can be NULL -- the associated mapping would be identity.
   ossimImageGeometry(ossim2dTo2dTransform* transform, ossimProjection* projection);

   //! rnToRn is a utility method that takes a rn resolution image point and maps it to the another
   //! rn resolution image point.
   //!
   //! @param inRnPt Is a point in resolution n.
   //! @param inResolutionLevel Is the resolution of the point inRnPt.
   //! @param outResolutionLevel Is the resolution of the point outRnPt.
   //! @param outRnPt Is the result of the transform.
   //!
   void rnToRn(const ossimDpt& inRnPt, ossim_uint32 inResolutionLevel,
               ossim_uint32 outResolutionLevel,ossimDpt& outRnPt) const;
   
   //! rnToFull is a utility method that takes a rn resolution image point and maps it to the full
   //! image point.
   //!
   //! @param rnPt Is a point in resolution n.
   //! @param resolutionLevel Is the resolution of the point rnPt.  a value of 0 is the local image
   //! @param fullPt Is the result of the transform
   //!
   void rnToFull(const ossimDpt& rnPt, ossim_uint32 resolutionLevel, ossimDpt& fullPt) const;

   //! @brief fullToRn is a utility method that takes a full image point and maps it to a rn
   //! resolution image point.
   //! 
   //! @param fullPt Is a point in full image space.
   //! @param resolutionLevel Is the resolution of the point rnPt. A value of 0 is the local image.
   //! @param fullPt Is the result of the transform
   void fullToRn(const ossimDpt& fullPt, ossim_uint32 resolutionLevel, ossimDpt& rnPt) const;

   //! rnToWorld is a utility method that takes a rn resolution image point and maps it to the 
   //! world point.
   //!
   //! @param rnPt Is a point in resolution n.
   //! @param resolutionLevel Is the resolution of the point rnPt.  a value of 0 is the local image
   //! @param wpt Is the result of the transform
   //!
   void rnToWorld(const ossimDpt& rnPt, ossim_uint32 resolutionLevel, ossimGpt& wpt) const;
   
   //! worldToRn is a utility method that takes a world point allows one to transform all the way back to
   //! an rn point.
   //!
   //! @param wpt Ground point.
   //! @param resolutionLevel Is the resolution of the point rnPt.  a value of 0 is the local image
   //! @param rnPt Is the resoltion point.
   //!
   void worldToRn(const ossimGpt& wpt, ossim_uint32 resolutionLevel, ossimDpt& rnPt) const;

   //! Exposes the 3D projection from image to world coordinates. The caller should verify that
   //! a valid projection exists before calling this method. Returns TRUE if a valid ground point
   //! is available in the ground_pt argument. This method depends on the existence of elevation
   //! information. If no DEM is available, the results will be incorrect or inaccurate.
   bool localToWorld(const ossimDpt& local_pt, ossimGpt& world_pt) const;

   //! Exposes the 3D world-to-local image coordinate reverse projection. The caller should verify 
   //! that a valid projection exists before calling this method. Returns TRUE if a valid image 
   //! point is available in the local_pt argument.
   bool worldToLocal(const ossimGpt& world_pt, ossimDpt& local_pt) const;

   //! Sets the transform to be used for local-to-full-image coordinate transformation
   void setTransform(ossim2dTo2dTransform* transform);

   //! Sets the projection to be used for local-to-world coordinate transformation
   void setProjection(ossimProjection* projection);

   //! Access methods for transform (may be NULL pointer).
   const ossim2dTo2dTransform* getTransform() const { return m_transform.get(); }
   ossim2dTo2dTransform*       getTransform()       { return m_transform.get(); }

   //! Access methods for projection (may be NULL pointer).
   const ossimProjection* getProjection() const { return m_projection.get(); }
   ossimProjection*       getProjection()       { return m_projection.get(); }

   //! Returns TRUE if valid projection defined
   bool hasProjection() const { return m_projection.valid(); }
   
   //! Returns TRUE if valid transform defined
   bool hasTransform() const { return m_transform.valid(); }

   //! Returns TRUE if this geometry is sensitive to elevation
   bool isAffectedByElevation() const;

   //! Returns the GSD associated with this image in the active projection. Note that this only
   //! makes sense if there is a projection associated with the image. Returns NaNs if no 
   //! projection defined.
   const ossimDpt& getMetersPerPixel() const;

   //! Assigns the ossimGpts with the ground coordinates of the four corresponding image 
   //! corner points. Returns true if points are valid.
   bool getCornerGpts(ossimGpt& ul, ossimGpt& ur, ossimGpt& lr, ossimGpt& ll) const;

   //! Prints contents to output stream.
   std::ostream& print(std::ostream& out) const;

   //! Returns TRUE if argument geometry has exactly the same instances for transform and 
   //! m_projection. This should be expanded to permit different instances of same transforms
   bool operator==(const ossimImageGeometry& compare_this) const;

   //! Replaces any existing transform and projection with those in the copy_this object
   const ossimImageGeometry& operator=(const ossimImageGeometry& copy_this);

   //! Returns the decimation factor from R0 for the resolution level specified. For r_index=0, the 
   //! decimation factor is by definition 1.0. For the non-discrete case, r_index=1 returns a 
   //! decimation of 0.5. If the vector of discrete decimation factors (m_decimationFactors) is 
   //! empty, the factor will be computed as f=1/2^n
   ossimDpt decimationFactor(ossim_uint32 r_index) const;

   /**
    * @brief Method to get the decimation factor for a given resolution
    * level.
    *
    * If the array of decimations is not initialized by owner, the default is:
    * r_index=0 is by definition 1.0.
    * r_index=n Where n is some level the factor will be computed as f=1/2^n.
    *
    * @param resLevel Reduced resolution set for requested decimation.
    *
    * @param result ossimDpt to initialize with requested decimation.
    */
   void decimationFactor(ossim_uint32 r_index, ossimDpt& result) const;

   /**
    * @brief Gets array of all decimation levels.
    * @param decimations Array to initialiaze.
    */
   void decimationFactors(std::vector<ossimDpt>& decimations) const;

   //! Sets the decimation scheme to a discrete list of decimation factors.
   void setDiscreteDecimation(const std::vector<ossimDpt>& decimation_list) 
      { m_decimationFactors = decimation_list; }

   //! @return The number of decimation factors
   ossim_uint32 getNumberOfDecimations()const
   {
      return (ossim_uint32)m_decimationFactors.size();
   }
   void setImageSize(const ossimIpt& size)
   {
      m_imageSize = size;
   }
   const ossimIpt& getImageSize()const
   {
      return m_imageSize;
   }
   //! Creates a new instance of ossimImageGeometry with the same transform and projection.
   //! Overrides base-class version requiring loadState() and saveState() (not implemented yet)
   virtual ossimObject* dup() const { return new ossimImageGeometry(*this); }

   //! Attempts to initialize a transform and a projection given the KWL. Returns TRUE if no
   //! error encountered.
   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix=0);

   //! Saves the transform (if any) and projection (if any) states to the KWL.
   virtual bool saveState(ossimKeywordlist& kwl, const char* prefix=0) const;

   /**
    * @brief Set m_targetRrds data member.
    *
    * This is used by methods worldToRn and localToWorld that do not take a rrds
    * argument and methods rnToWorld and worldToRn.  If the target rrds is set to 2,
    * then the resulting point from worldToLocal is relative to reduced
    * resolution level 2.  Vice versa for localToWorld it is assumed the local
    * point is relative to the target
    * reduced resolution data set.
    *
    * @param rrds Target (zero based) reduced resolution data set.
    */
   void setTargetRrds(ossim_uint32 rrds);

   /**
    * @return The target zero based reduced resolution data set used for localToWorld and
    * worldToLocal.
    * @see setTargetRrds 
    */
   ossim_uint32 getTargetRrds() const;

   //! When either the projection or the transform changes, this method recomputes the GSD.
   void computeGsd()const;

   virtual bool isEqualTo(const ossimObject& obj, ossimCompareType compareType = OSSIM_COMPARE_FULL)const;
protected:
   //! @brief Method to back out decimation of a point.
   //! @param rnPt Is a point in resolution n.
   //! @param resolutionLevel Is the resolution of the point rnPt.
   //! @param outPt Is the result of the transform a non-decimated point.
   void undecimatePoint(const ossimDpt& rnPt,
                        ossim_uint32 resolutionLevel,
                        ossimDpt& outPt) const;

   //! @brief Method to apply decimation of a point.
   //! @param inPt Is a point with no decimation.
   //! @param resolutionLevel Is the resolution of the point rnPt.
   //! @param rnPt Is the result of the transform
   void decimatePoint(const ossimDpt& inPt,
                      ossim_uint32 resolutionLevel,
                      ossimDpt& rnPt) const;

   ossimRefPtr<ossim2dTo2dTransform> m_transform;   //!< Maintains local_image-to-full_image transformation 
   ossimRefPtr<ossimProjection>      m_projection;  //!< Maintains full_image-to-world_space transformation
   mutable ossimDpt                  m_gsd;         //!< meters per pixel
   std::vector<ossimDpt>             m_decimationFactors; //!< List of decimation factors for R-levels
   ossimIpt                          m_imageSize; // Image width and height

   /** @brief Target rrds for localToWorld and worldToLocal methods. */
   ossim_uint32                      m_targetRrds; 

   TYPE_DATA
};

#endif /* #ifndef ossimImageGeometry_HEADER */
