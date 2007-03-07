/* -*-c++-*- libossim - Copyright (C) since 2004 Garrett Potts 
 * LICENSE: LGPL
 * Author: Garrett Potts
*/
#ifndef ossimReferenced_HEADER
#define ossimReferenced_HEADER
#include <base/common/ossimConstants.h>

class OSSIMDLLEXPORT ossimReferenced
{
 public:
   ossimReferenced()
      : theRefCount(0)
      {}
   
   ossimReferenced(const ossimReferenced&)
      : theRefCount(0)
      {}
   
   inline ossimReferenced& operator = (const ossimReferenced&) { return *this; }
   

   /*! increment the reference count by one, indicating that 
       this object has another pointer which is referencing it.*/
   inline void ref() const { ++theRefCount; }
   
   /*! decrement the reference count by one, indicating that 
       a pointer to this object is referencing it.  If the
       reference count goes to zero, it is assumed that this object
       is no longer referenced and is automatically deleted.*/
   inline void unref() const;
   
   /*! decrement the reference count by one, indicating that 
       a pointer to this object is referencing it.  However, do
       not delete it, even if ref count goes to 0.  Warning, unref_nodelete() 
       should only be called if the user knows exactly who will
       be resonsible for, one should prefer unref() over unref_nodelete() 
       as the later can lead to memory leaks.*/
   inline void unref_nodelete() const { --theRefCount; }
   
   /*! return the number pointers currently referencing this object. */
   inline int referenceCount() const { return theRefCount; }
   
   
 protected:
   virtual ~ossimReferenced();
   mutable int theRefCount;
};

inline void ossimReferenced::unref() const
{
    --theRefCount;
    if (theRefCount==0)
    {
        delete this;
    }
}

#endif
