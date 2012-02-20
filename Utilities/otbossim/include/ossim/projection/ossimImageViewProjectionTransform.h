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
//  $Id: ossimImageViewProjectionTransform.h 20352 2011-12-12 17:24:52Z dburken $

#ifndef ossimImageViewProjectionTransform_HEADER
#define ossimImageViewProjectionTransform_HEADER 1

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
   virtual bool isValid() const { return (m_imageGeometry.valid() && m_viewGeometry.valid()); }

   //! Returns TRUE if both input and output geometries are identical. Presently implemented as
   //! limited compare of geometry pointers
   virtual bool isIdentity() const { return (m_imageGeometry == m_viewGeometry); }

   //! Assigns the geometry to use for output view. This object does NOT own the geometry.
   void setViewGeometry(ossimImageGeometry* g) { m_viewGeometry = g; }   

   //! Assigns the geometry to use for input image. This object does NOT own the geometry.
   void setImageGeometry(ossimImageGeometry* g) { m_imageGeometry = g; }  

   //! Workhorse of the object. Converts image-space to view-space.
   virtual void imageToView(const ossimDpt& imagePoint, ossimDpt& viewPoint) const;

   //! Other workhorse of the object. Converts view-space to image-space.
   virtual void viewToImage(const ossimDpt& viewPoint, ossimDpt& imagePoint) const;

   //! Dumps contents to stream
   virtual std::ostream& print(std::ostream& out) const;
   
   ossimImageGeometry* getImageGeometry()  { return m_imageGeometry.get(); }
   ossimImageGeometry* getViewGeometry()   { return m_viewGeometry.get(); }
   const ossimImageGeometry* getImageGeometry()const  { return m_imageGeometry.get(); }
   const ossimImageGeometry* getViewGeometry()const   { return m_viewGeometry.get(); }
   
   //! OLK: Not sure where this is used, but needed to satisfy ossimViewInterface base class.
   //! The ownership flag is ignored.
   virtual bool setView(ossimObject* baseObject);
   virtual       ossimObject* getView()       { return m_viewGeometry.get(); }
   virtual const ossimObject* getView() const { return m_viewGeometry.get(); }

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

   /**
    * @brief Initializes the view geometry image size from image geometry
    * bounding rect.
    *
    * This is needed for the ossimImageGeometry::worldToLocal if the underlying
    * projection is geographic to handle images that cross the date line.
    * 
    * @param Input image rectangle.
    * @return true on success, false on error.
    */
   bool initializeViewSize();  
   
   ossimRefPtr<ossimImageGeometry> m_imageGeometry;
   ossimRefPtr<ossimImageGeometry> m_viewGeometry;
   
TYPE_DATA
};

#endif
