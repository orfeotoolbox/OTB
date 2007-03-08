/* -*-c++-*- ossim - Copyright (C) since 2004 Garrett Potts 
 *
 * This was taken directly from OpenSceneGraph and will retain OSGGPL license.
 * This is basically an LGPL.
 * 
*/
#ifndef ossimRefPtr_HEADER
#define ossimRefPtr_HEADER
#include <ossim/base/ossimConstants.h>

template<class T> class ossimRefPtr
{
 public:
   typedef T element_type;
   
   ossimRefPtr() :thePtr(0) {}
   ossimRefPtr(T* t):thePtr(t)              { if (thePtr) thePtr->ref(); }
   ossimRefPtr(const ossimRefPtr& rp):thePtr(rp.thePtr)  { if (thePtr) thePtr->ref(); }
   ~ossimRefPtr()                           { if (thePtr) thePtr->unref(); thePtr=0; }
   
   inline ossimRefPtr& operator = (const ossimRefPtr& rp)
      {
         if (thePtr==rp.thePtr) return *this;
         T* tmpPtr = thePtr;
         thePtr = rp.thePtr;
         if (thePtr) thePtr->ref();
         // unref second to prevent any deletion of any object which might
         // be referenced by the other object. i.e rp is child of the
         // original _ptr.
         if (tmpPtr) tmpPtr->unref();
         return *this;
      }
   
   inline ossimRefPtr& operator = (T* ptr)
      {
         if (thePtr==ptr) return *this;
         T* tmpPtr = thePtr;
         thePtr = ptr;
         if (thePtr) thePtr->ref();
         // unref second to prevent any deletion of any object which might
         // be referenced by the other object. i.e rp is child of the
         // original thePtr.
         if (tmpPtr) tmpPtr->unref();
         return *this;
      }
   
   // comparison operators for ossimRefPtr.
   inline bool operator == (const ossimRefPtr& rp) const { return (thePtr==rp.thePtr); }
   inline bool operator != (const ossimRefPtr& rp) const { return (thePtr!=rp.thePtr); }
   inline bool operator < (const ossimRefPtr& rp) const { return (thePtr<rp.thePtr); }
   inline bool operator > (const ossimRefPtr& rp) const { return (thePtr>rp.thePtr); }
   
   // comparion operator for const T*.
   inline bool operator == (const T* ptr) const { return (thePtr==ptr); }
   inline bool operator != (const T* ptr) const { return (thePtr!=ptr); }
   inline bool operator < (const T* ptr) const { return (thePtr<ptr); }
   inline bool operator > (const T* ptr) const { return (thePtr>ptr); }
   
   
   inline T& operator*()  { return *thePtr; }
   
   inline const T& operator*() const { return *thePtr; }
   
   inline T* operator->() { return thePtr; }
   
   inline const T* operator->() const   { return thePtr; }
   
   inline bool operator!() const	{ return thePtr==0L; }
   
   inline bool valid() const	{ return thePtr!=0L; }
   
   inline T* get() { return thePtr; }
   
   inline const T* get() const { return thePtr; }
   
   /** take control over the object pointed to by ref_ptr, unreference but do not delete even if ref count goes to 0,
    * return the pointer to the object.
    * Note, do not use this unless you are 100% sure your code handles the deletion of the object correctly, and
    * only use when absolutely required.*/
   inline T* take() { return release();}
   
   inline T* release() { T* tmp=thePtr; if (thePtr) thePtr->unref_nodelete(); thePtr=0; return tmp;}
   
 private:
   T* thePtr;
};

#endif
