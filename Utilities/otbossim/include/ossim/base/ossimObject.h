//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// 
// Description: Base class for ossim objects.
//
//*************************************************************************
// $Id: ossimObject.h 19852 2011-07-21 15:26:12Z gpotts $

#ifndef ossimObject_HEADER
#define ossimObject_HEADER

#include <iosfwd>
#include <ossim/base/ossimReferenced.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimRtti.h>

class ossimKeywordlist;
class ossimString;
class ossimVisitor;

class OSSIMDLLEXPORT ossimObject : public ossimReferenced
{
public:
   ossimObject();
   virtual ~ossimObject();
   
   virtual ossimObject* dup() const;
   
   virtual ossimString getShortName()   const;
   virtual ossimString getLongName()    const;
   virtual ossimString getDescription() const;
   virtual ossimString getClassName()   const;
   
   /*!
    * Will return the type of this class in an RTTI format.
    */
   virtual RTTItypeid getType() const;

   virtual bool canCastTo(ossimObject* obj) const;
   
   virtual bool canCastTo(const RTTItypeid& id) const;
   
   /*!
    * Will use RTTI to search the derived classes to see if it derives from
    * the passed in type.  If so then this object can be casted to the
    * passed in class name.
    */
   virtual bool canCastTo(const ossimString& parentClassName) const;

   /*!
    * Method to save the state of the object to a keyword list.
    * Return true if ok or false on error.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0) const;

   /*!
    * Method to the load (recreate) the state of the object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   
   /**
    * Generic print method.  Derived classes should re-implement as they see
    * fit.
    * 
    * @return std::ostream&
    */
   virtual std::ostream& print(std::ostream& out) const;

   /**
    * @note  Since the print method is virtual, derived classes only need
    *        to implement that, not an addition operator<<.
    */
   friend OSSIMDLLEXPORT std::ostream& operator<<(std::ostream& out,
                                                  const ossimObject& obj);
   
   virtual bool isEqualTo(const ossimObject& obj, ossimCompareType compareType = OSSIM_COMPARE_FULL)const;
   virtual void accept(ossimVisitor& visitor);

 protected:
TYPE_DATA
};

#endif /* #ifndef ossimObject_HEADER */
