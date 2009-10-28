//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// AUTHOR: Garrett Potts (gpotts@imagelinks.com)  
//         Oscar Kramer (oscar@krameranalytic.com)
//
// DESCRIPTION: Contains declaration of ossimImageViewProjectionTransform.
//    This class provides an image to view transform that utilizes two
//    independent 2D-to-3D projections. Intended for transforming view to
//    geographic "world" space to input image space.
//
// LIMITATIONS: None.
//
//*****************************************************************************
//  $Id: ossimImageViewProjectionTransform.h 15766 2009-10-20 12:37:09Z gpotts $

#ifndef ossimImageViewProjectionTransform_HEADER
#define ossimImageViewProjectionTransform_HEADER

#include <ossim/projection/ossimImageViewTransform.h>
#include <ossim/imaging/ossimImageGeometry.h>

class OSSIMDLLEXPORT ossimImageViewProjectionTransform : public ossimImageViewTransform
{
public:
   ossimImageViewProjectionTransform(ossimImageGeometry* imageGeometry=0,
                                     ossimImageGeometry* viewGeometry=0);

   //! copy constructor 
   ossimImageViewProjectionTransform(const ossimImageViewProjectionTransform& src);

   virtual ossimObject* dup() const { return new ossimImageViewProjectionTransform(*this); }
   virtual ~ossimImageViewProjectionTransform();

   //! Satisfies base class pure virtual. Returns TRUE if both input and output geoms exist.
   virtual bool isValid() const { return (m_ImageGeometry.valid() && m_ViewGeometry.valid()); }

   //! Returns TRUE if both input and output geometries are identical. Presently implemented as
   //! limited compare of geometry pointers
   virtual bool isIdentity() const { return (m_ImageGeometry == m_ViewGeometry); }

   //! Assigns the geometry to use for output view. This object does NOT own the geometry.
   void setViewGeometry(ossimImageGeometry* g) { m_ViewGeometry = g; }   

   //! Assigns the geometry to use for input image. This object does NOT own the geometry.
   void setImageGeometry(ossimImageGeometry* g) { m_ImageGeometry = g; }  

   //! Workhorse of the object. Converts image-space to view-space.
   virtual void imageToView(const ossimDpt& imagePoint, ossimDpt& viewPoint) const;

   //! Other workhorse of the object. Converts view-space to image-space.
   virtual void viewToImage(const ossimDpt& viewPoint, ossimDpt& imagePoint) const;

   //! Dumps contents to stream
   virtual std::ostream& print(std::ostream& out) const;
   
   ossimImageGeometry* getImageGeometry()  { return m_ImageGeometry.get(); }
   ossimImageGeometry* getViewGeometry()   { return m_ViewGeometry.get(); }
   const ossimImageGeometry* getImageGeometry()const  { return m_ImageGeometry.get(); }
   const ossimImageGeometry* getViewGeometry()const   { return m_ViewGeometry.get(); }
   
   //! OLK: Not sure where this is used, but needed to satisfy ossimViewInterface base class.
   //! The ownership flag is ignored.
   virtual bool setView(ossimObject* baseObject);
   virtual       ossimObject* getView()       { return m_ViewGeometry.get(); }
   virtual const ossimObject* getView() const { return m_ViewGeometry.get(); }

   //! Returns the GSD of input image.
   virtual ossimDpt getInputMetersPerPixel()const;

   //! Returns the GSD of the output view.
   virtual ossimDpt getOutputMetersPerPixel() const;

   //! Gets the image bounding rect in view-space coordinates
   virtual ossimDrect getImageToViewBounds(const ossimDrect& imageRect)const;
   
   //! After rewrite for incorporating ossimImageGeometry: No longer needed.  
   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix =0);
   
   //! After rewrite for incorporating ossimImageGeometry: No longer needed.  
   virtual bool saveState(ossimKeywordlist& kwl, const char* prefix = 0)const;

protected:
   ossimRefPtr<ossimImageGeometry> m_ImageGeometry;
   ossimRefPtr<ossimImageGeometry> m_ViewGeometry;
   
TYPE_DATA
};

#endif
