// $Header$
/* ----------------------------------------------------------------------------
  id.h
  mbwall 7may95
  Copyright (c) 1995 Massachusetts Institute of Technology
                     all rights reserved

 DESCRIPTION:
  This defines the identifiers for polymorphic classes.  The IDs are used 
primarily for checking to be see if the two objects are the same type before
doing a copy, for example.  The name is often used when printing out error
messages so you'll know where things are happening.
  I hate to derive so many classes from the same base class, especially when 
the derived classes are completely unrelated.  But this is a convenient way to
enumerate the built-in classes, and they DO share the polymorphic behaviour
(even if they do NOT share any other attributes).

 TO DO:
  I leave the id/classname implementation for backward compatibility.  Also, 
as of fall98 there are still some systems that do not support RTTI (or environs
that do not want to use RTTI for some reason or another).
  This whole thing will be replaced with a proper RTTI implementation as soon
as RTTI is stable on all the platforms (and as soon as I have time to do the
update).  So for now, I apologize for the 'hack'iness of this implementation.
---------------------------------------------------------------------------- */
#ifndef _ga_id_h_
#define _ga_id_h_

class GAID {
public:
  enum {
    BaseGA=0, SimpleGA, SteadyStateGA, IncrementalGA, DemeGA,

    Population=10, 

    Scaling=15, 
    NoScaling, LinearScaling, SigmaTruncationScaling, PowerLawScaling, Sharing,

    Selection=40,
    RankSelection, RouletteWheelSelection, TournamentSelection,
    UniformSelection, SRSSelection, DSSelection,

    Genome=50, 
    BinaryStringGenome, BinaryStringGenome2D, BinaryStringGenome3D, 
    Bin2DecGenome, 
    ListGenome,
    TreeGenome, 
    ArrayGenome, ArrayGenome2D, ArrayGenome3D, 
    ArrayAlleleGenome, ArrayAlleleGenome2D, ArrayAlleleGenome3D, 
    StringGenome, FloatGenome, IntGenome, DoubleGenome
  };

  int sameClass(const GAID &b) const {return(classID() == b.classID());} 
  virtual const char * className() const {return "no class";} 
  virtual int classID() const {return 0;}
  virtual ~GAID() { }
};


// Here are the ID values for the GA library:
//
//    class                                  range
//    ---------------------------------------------
//    reserved                                0-199
//    anyone can use these                     200-
//
// The ID numbers for built-in classes are enumerated in the GAID class.  Sorry
// but I had to do the dimension at the end of the names rather than at the
// beginning since you cannot use a digit to start a variable name.  
//   If you derive your own polymorphic class or specialize a
// template class, then give it its own ID number and class name in the
// specialization (see the string and real number specializations for examples)
//   Use these macros to define the identity for your class.  If you do not use
// these macros then your object will be identity-less.
//   Here's how to use the macro.
//
//     class GASimpleGA : public GA {
//     public:
//        GADefineClassIdentity("GASimpleGA", 1);
//       ...
//     };
//
//   Notice that your template classes will all have the same name using this
// method unless you specialize.  For example, GA1DArrayGenome<char> and 
// GA1DArrayGenome<int> will both have the name GA1DArrayGenome as well as 
// the same number.  Beware.

#define GADefineIdentity(name,id) \
     const char * className() const {return name;} \
     int classID() const {return id;}

#define GADeclareIdentity() \
     const char * className() const; \
     int classID() const

#define GADefineIdentitySRC(clss,name,id) \
const char * clss :: className() const {return name;} \
int clss :: classID() const {return id;}

#endif

