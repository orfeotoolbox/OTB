#ifndef RTTI_H
#define RTTI_H

/////////////////////////////////////////////////////////////////////////////////////////
/* 
   RTTI_H:	This file provides support for RTTI and generalized (virtual-base to derived and 
		separate hierarchy branches) casting. There is also support for RT obj creation
	  	from type names.

		In order to enable these features for a class, two things should be done:

		1)	insert the text TYPE_DATA (without ';') as the last item in the class-decl.
		2)	in the .C file where the class's implementation resides, insert the following (without';'):
		
			RTTI_DEF(classname,"RT classname")	     if the class has no bases with RTTI
 			RTTI_DEFn(classname,"RT classname",b1,...bn) if the class has bases b1,...bn with RTTI

			Use RTTI_DEF_INST instead of RTTI_DEF if you want to enable RT obj creation for classname.
			You should provide then a public default ctor.
			
   RTTI is used via a class called RTTItypeid. A typeid describes a type of a class. typeids can be compared
   with operator== and operator!= and can be retrieved from classes/pointers. They should provide all necessary 
   support for any kind of RTTI/casting, as described by the macros below (by RTTI-class we mean a class having RTTI,
   as described above). The 'return type' of the macros is listed between quotes:

   'RTTItypeid'
   STATIC_TYPE_INFO(T)		T=RTTI-class name. Returns a RTTItypeid with T's type. if T hasn't RTTI, a 
				compile-time error occurs.
   'RTTItypeid'
   TYPE_INFO(p)		        p=ptr to a RTTI-class.  
				Returns the RTTItypeid of the class-obj p is really pointing at. 
			        If *p hasn't RTTI, a compile-time error occurs.	
				If p==NULL, a special typeid for the NULL pointer is returned. 
				This typeid has the name (const char*) "NULL".  
   'T*'
   PTR_CAST(T,p)		T=RTTI-class, p=RTTI-class ptr.
				Returns p cast to the type T as a T*, if cast is possible, else
				returns NULL. If *p or T have no RTTI, a compile-time error occurs.
				Note that p can point to virtual base classes. Casting between separate
				branches of a class hierarchy is also supported, as long as all classes
				have RTTI. Therefore PTR_CAST is a fully general and safe operator.
				If p==NULL, the operator returns NULL.

   Some other macros. Not essential, provided mostly for convenience:
   =================
   const char*
   STATIC_TYPE_NAME(T)		T=RTTI-class name. Returns the name (const char*) of T's type.
				Provided for convenience. Compile-time error if T hasn't RTTI.
   const char*
   TYPE_NAME(p)			p=RTTI-class ptr. Returns the class name (char*) of real *p.
				Provided for convenience.  Compile-time error if *p hasn't RTTI.
   int				If p==NULL, "NULL" is returned.
   DYN_CAST(t,p)		t=RTTItypeid. p=RTTI-class ptr.
				Returns 1 if p can indeed be cast to t, else returns 0.
				Compile-time error if *p hasn't RTTI.
				If p==NULL, 1 is returned (NULL can be cast to anything).
				This operator is useful when we just want to check that a pointer can be cast to
				a RTTItypeid variable (which can be created/selected at run-time). For creating a RTTItypeid, 
				construct a subclass of RTTItypeid called RTTIdyntypeid which allows construction from a const char*, i.e. from
				a user specification and pass it to DYN_CAST.
				DYN_TYPE can't return a typed pointer (since t is a RTTItypeid var and C++ has no 'real' type variables), 
				but returns a 1/0 indicating whether the cast is possible or not. 
   void*
   UPTR_CAST(t,p)		t=RTTItypeid. p=RTTI-class ptr.
				Returns p cast to a void* if cast succeeds, else returns NULL.
				Again, p can point to virtual-bases and casts between separate branches of a class dag
				are supported. this operator is practically the 'untyped' version of PTR_CAST, offering
				the extra feature that the type is expressed by a run-time type variable (RTTItypeid).
				Compile-time error if *p hasn't RTTI.

  
  Obsolete macros:		This set of macros is based on Stroustrup. They are not general and safe 
  ===============		(e.g. virtual-base to derived casts are trapped as compile-time errors and casts
				between separate class-dag branches are incorrectly done without any warnings). 
				The above macros are including ALL functionality of these macros, which are
				provided only for completeness:

  OLD_PTR_CAST(T,p)		Like PTR_CAST(T,p), but without accepting p=ptr to virtual bases and performing
				incorrect casts between separate branches of a class-dag.
  OLD_UPTR_CAST(t,p)		Like UPTR_CAST(t,p), but with same problems as above.
  OLD_DYN_CAST(t,p)		Like DYN_CAST(t,p), but with same problems as above.
 

  RT Object Creation:		This feature enables the user to create objects of a RT-selected type in a generic manner.
  ==================		Typically, we create/obtain a RT type (via a RTTItypeid t1). Then, having another RTTItypeid t2
				(typically from the (STATIC_)TYPE_INFO of some ROOT class / ROOT* ptr, we call:

				ROOT* new_obj = (ROOT*)t2.create(t1);

				create() searches for t1 in the class DAG rooted at t2. If found, it creates a new obj of type 
				t1 and returns it as a '(void*)(t2*)'. For example, we know above that we get a ROOT* since
				t2 is ROOT's RTTItypeid, so we can _safely_ cast the void* to ROOT. If create() can't make the t1
				object (there's no such type or the type is not declared via RTTI_DEF_INST), it returns NULL.
				This gives a fully generic typeid-based factory method for RT obj creation.

  REMARK:	RTTI support adds some additional info, both static+virtual, to a class. The current implementation
  =======	adds a static object and some static functions and 2 virtual functions to each class desiring RTTI.
		This may of course cause potential name-clash problems. In order to avoid this, all added identifiers
		in the RTTI system are prefixed with RTTI_ (see the TYPE_DATA macro).
  
  REMARK_2:	There are two classes related to RTTI: RTTItypeid and RTTITypeinfo. A RTTItypeid is, as it says, an 'id for a type'
  ========	It actually wraps a RTTITypeinfo*, where a RTTITypeinfo contains the actual encoding of a class type.
		You can freely create/copy/destroy/manipulate RTTItypeid's, but you should NEVER deal directly
		with RTTITypeinfo. A RTTITypeinfo should actually be created ONLY by the TYPE_DATA macros, as part of a class definition,
		since the RTTITypeinfo encodes a type info for an EXISTING class. All type-related stuff should be therefore
		handled via RTTItypeid's. If you really want to dynamically create a dummy typeid, use RTTIdyntypeid class.

  REMARK 3:	All names introduced by this RTTI implementation (RTTItypeid,RTTIdyntypeid,RTTITypeinfo,etc)
  =========	are prefixed by RTTI, to make this system easily acceptable by e.g. C++ environments which happen to 
	 	already support typeids.

*/				

	
//////////////////////////////////////////////////////////////

#include <cstring> // for strdup,strcmp
#include <string> // for strdup,strcmp
#include <ossim/base/ossimConstants.h>
#include <vector>
/////////////////////////////////////////////////////////////

class RTTITypeinfo;

class OSSIMDLLEXPORT RTTItypeid	
		{							// Main class for RTTI interface.
		public:

		                    RTTItypeid(const RTTITypeinfo* p):  theId(p)  {}
				    RTTItypeid(); 			 
		   int 		    operator==(RTTItypeid) const;
		   int 		    operator!=(RTTItypeid) const;
		   const RTTITypeinfo* get_info() const    	 { return theId; }
		   int 		    can_cast(RTTItypeid) const;	 	// 1 if the arg can be cast to this, else 0
		   const char* 	    getname() const;
		   int 		    num_subclasses() const;		// Return # subclasses of this
		   RTTItypeid	    subclass(int) const;		// Return ith subclass of this
		   int		    num_baseclasses() const;		// Return # baseclasses of this
		   RTTItypeid 	    baseclass(int) const;		// Return ith baseclass of this
		   void*	    create(RTTItypeid) const;	 	// Tries to create an instance of a subclass of this
								 	// having of type given by the RTTItypeid arg. If ok, it returns it 
								 	// casted to the class-type of this and then to void* 
		   int		    can_create() const;			// Return 1 if this type is instantiable, else 0
                   RTTItypeid       find_baseclass(const char* name)const;// 
		   static RTTItypeid 
                   null_type();			// the RTTItypeid for NULL ptrs
                   
	        protected:

		    const RTTITypeinfo* theId;				// RTTItypeid implementation (the only data-member)
		};


class OSSIMDLLEXPORT RTTIdyntypeid : public RTTItypeid					//Class for dynamic type creation from user strings.
		{							//Useful for creating typeids at RT for comparison
		public:							//purposes.
				    RTTIdyntypeid(const char*);
				   ~RTTIdyntypeid();
		private:
				    static const RTTITypeinfo* a[];
		};

///////////////////////////////////////////////////////////////////

class OSSIMDLLEXPORT RTTITypeinfo {							//Implementation of type-related info
		public:
			typedef std::vector<const RTTITypeinfo*> SubtypesConstVector;

		               RTTITypeinfo(const char* name, const RTTITypeinfo* bb[],
					 void* (*)(int,void*),void* (*)());
		  	      ~RTTITypeinfo();
		   const char* getname() const;				//Returns name of this RTTITypeinfo	
		   int 	       same(const RTTITypeinfo*) const;		//Compares 2 RTTITypeinfo objs
		   int         can_cast(const RTTITypeinfo*) const;	//1 if the arg can be cast to this, else 0
		   int         has_base(const RTTITypeinfo*) const;	//1 if this has the arg as some base, else 0		
		   
		   
		   const RTTITypeinfo*  
		   	       subclass(int=0) const;			//get i-th subclass of this, if any, else NULL
		   int	       num_subclasses() const;			//get # subclasses of this 
		   void*       create(const RTTITypeinfo*,const char*) const;	//search for a subclass named char*,
									//create obj of it and return it cast to 
									//the RTTITypeinfo* type, which is either 
									//this or a direct base of this. 
		   int	       can_create() const;			//Returns 1 if this type has a default ctor, else 0			
			
		private:

		   //char* 	          n;				//type name 
			std::string n;
	           const RTTITypeinfo**   b;				//base types (NULL-ended array of RTTITypeinfo's for this's direct bases)
		   int			  ns;				//#subtypes of this type
		   SubtypesConstVector subtypes;
		   //const RTTITypeinfo**	  subtypes;			//types derived from this type
		   static const RTTITypeinfo null_type;			//convenience type info for a 'null' type
		   void*		  (*new_obj)();			//func to create a new obj of this type
		   void*		  (*cast)(int,void*);		//func to cast an obj of this type to
									//ith baseclass of it or to itself
		   
		   void 		  add_subtype(const RTTITypeinfo*);//adds a subtype to this's subtypes[]	
		   void			  del_subtype(const RTTITypeinfo*);//dels a subtype from this's subtypes[]

		   friend class		  RTTItypeid;			//for null_type
		};      	




inline int RTTITypeinfo::num_subclasses() const				//Return # subclasses of this
{
  return ns;
} 

inline const RTTITypeinfo* RTTITypeinfo::subclass(int i) const		//Return ith subclass of this, else NULL;
{
  return (i>=0 && i<ns)? subtypes[i]: 0;
} 

inline int RTTITypeinfo::same(const RTTITypeinfo* p) const			//Compare 2 RTTITypeinfo's:
{  										//First, try to see if it's the same
   return this==p || !strcmp(n.c_str(),p->n.c_str()); 						//'physical' RTTITypeinfo (which should be the case,
}										//since we create them per-class and not per-obj).
  										//If this fails, still do a textual name comaprison.

inline int RTTITypeinfo::can_cast(const RTTITypeinfo* p) const
{  
   return same(p) || p->has_base(this); 
}

inline int RTTITypeinfo::can_create() const
{
   return new_obj!=0;
}

/////////////////////////////////////////////////////////////////
//
// RTTItypeid methods:
//

inline RTTItypeid RTTItypeid::null_type()
{
   return &(RTTITypeinfo::null_type);
}

inline RTTItypeid::RTTItypeid(): theId(null_type().theId)   
{
}

inline int RTTItypeid::operator==(RTTItypeid i) const
{ 
  return theId->same(i.theId); 
}

inline int RTTItypeid::operator!=(RTTItypeid i) const
{
  return !(theId->same(i.theId));
}

inline int RTTItypeid::can_cast(RTTItypeid i) const
{
  return theId->can_cast(i.theId);
}


inline const char* RTTItypeid::getname() const
{
  return theId->getname();
}

inline int RTTItypeid::num_subclasses() const
{
  return theId->num_subclasses();
}

inline RTTItypeid RTTItypeid::subclass(int i) const
{
  return theId->subclass(i);
}

inline int RTTItypeid::num_baseclasses() const
{
  int i;
  for(i=0;theId->b[i];i++){}
  return i;
}

inline RTTItypeid RTTItypeid::baseclass(int i) const
{
  return theId->b[i];
}

inline void* RTTItypeid::create(RTTItypeid t) const
{
   return theId->create(theId,t.getname());
}

inline int RTTItypeid::can_create() const
{
  return theId->can_create();
}

////////////////////////////////////////////////////////////////////////////////


inline RTTIdyntypeid::RTTIdyntypeid(const char* c) : RTTItypeid(new RTTITypeinfo(c,a,0,0)) { }   //create a dummy RTTITypeinfo
inline RTTIdyntypeid::~RTTIdyntypeid()
{
   if(theId)
   {
      delete theId;
      theId = 0;
   }
}				    //delete the dummy RTTITypeinfo
		

/////////////////////////////////////////////////////////////////////////////////////
		
// 1. Main operators
#define STATIC_TYPE_INFO(T)   T::RTTI_sinfo()
#define TYPE_INFO(p)          ((p)? (p)->RTTI_vinfo() : RTTItypeid::null_type() )
#define PTR_CAST(T,p) 	      ((p)? (T*)((p)->RTTI_cast(STATIC_TYPE_INFO(T))) : 0)


// 2. Convenience operators
#define STATIC_TYPE_NAME(T)   (STATIC_TYPE_INFO(T).getname())	
#define TYPE_NAME(p)	      ((p)? ((p)->RTTI_vinfo().getname()) : RTTItypeid::null_type().getname())	
#define DYN_CAST(t,p)	      ((p)? ((p)->RTTI_cast((t))!=0) : 1)
#define UPTR_CAST(t,p)	      ((p)? ((p)->RTTI_cast((t))) : 0)	

// 3. Unsafe operators (see Stroustrup)
#define OLD_PTR_CAST(T,p)     ((p)? ((STATIC_TYPE_INFO(T).can_cast((p)->RTTI_vinfo()))? (T*)p : 0) : 0)
#define OLD_UPTR_CAST(t,p)    ((p)? ((t).can_cast((p)->RTTI_vinfo())? (void*)p : 0) : 0)
#define OLD_DYN_CAST(t,p)     ((p)? ((t).can_cast((p)->RTTI_vinfo())? 1 : 0) : 1)


// Definition of TYPE_DATA for a RTTI-class: introduces one static RTTITypeinfo data-member
// and a couple of virtuals.

#define TYPE_DATA			 		          \
	protected:					          \
	   static  const  RTTITypeinfo RTTI_obj; 		  \
	   static  void*  RTTI_scast(int,void*);	          \
	   static  void*  RTTI_new();			          \
	public:						          \
	   virtual RTTItypeid RTTI_vinfo() const { return &RTTI_obj; }\
	   static  RTTItypeid RTTI_sinfo()	 { return &RTTI_obj; }\
	   virtual void*  RTTI_cast(RTTItypeid);\
	   virtual const void*  RTTI_cast(RTTItypeid)const;
	


// Definition of auxiliary data-structs supporting RTTI for a class: defines the static RTTITypeinfo
// object of that class and its associated virtuals.

// Auxiliary definition of the construction method:
#define RTTI_NEW(cls,name)     void* cls::RTTI_new() { return new cls; }	\
			       const RTTITypeinfo cls::RTTI_obj = RTTITypeinfo(name,RTTI_base_##cls,cls::RTTI_scast,cls::RTTI_new);

#define RTTI_NO_NEW(cls,name)  const RTTITypeinfo cls::RTTI_obj = RTTITypeinfo(name,RTTI_base_##cls,cls::RTTI_scast,0);



//////////////////////////////////////////////////////////////////
//
//	Top-level macros:
//

#define RTTI_DEF_BASE(cls,name)					\
	static const RTTITypeinfo* RTTI_base_##cls [] = { 0 };\
	void* cls::RTTI_cast(RTTItypeid t)			\
	{							\
	   if (t == &RTTI_obj) return this;			\
	   return 0;						\
	}							\
	const void* cls::RTTI_cast(RTTItypeid t)const	\
	{							\
	   if (t == &RTTI_obj) return this;			\
	   return 0;						\
	}							\
        void* cls::RTTI_scast(int /* i */,void* p)			\
	{  cls* ptr = (cls*)p; return ptr; }			
	

#define RTTI_DEF1_BASE(cls,name,b1)				\
        static const RTTITypeinfo* RTTI_base_##cls [] = 	\
	       { STATIC_TYPE_INFO(b1).get_info(),0 };		\
  	void* cls::RTTI_cast(RTTItypeid t)			\
	{							\
	   if (t == &RTTI_obj) return this;			\
	   void* ptr;						\
	   if ((ptr=b1::RTTI_cast(t))) return ptr;		\
	   return 0;						\
	}							\
  	const void* cls::RTTI_cast(RTTItypeid t)const	\
	{							\
	   if (t == &RTTI_obj) return this;			\
	   const void* ptr;						\
	   if ((ptr=b1::RTTI_cast(t))) return ptr;		\
	   return 0;						\
	}							\
	void* cls::RTTI_scast(int i,void* p)			\
	{  cls* ptr = (cls*)p;					\
	   switch(i)						\
	   {  case  0: return (b1*)ptr;	 }			\
	   return ptr;						\
	}							
									

#define RTTI_DEF2_BASE(cls,name,b1,b2)				\
        static const RTTITypeinfo* RTTI_base_##cls [] = 	\
	       { STATIC_TYPE_INFO(b1).get_info(),		\
		 STATIC_TYPE_INFO(b2).get_info(),0 };		\
  	void* cls::RTTI_cast(RTTItypeid t)			\
	{							\
	   if (t == &RTTI_obj) return this;			\
	   void* ptr;						\
	   if ((ptr=b1::RTTI_cast(t))) return ptr;		\
	   if ((ptr=b2::RTTI_cast(t))) return ptr;		\
	   return 0;						\
	}							\
  	const void* cls::RTTI_cast(RTTItypeid t)const	\
	{							\
	   if (t == &RTTI_obj) return this;			\
	   const void* ptr;					\
	   if ((ptr=b1::RTTI_cast(t))) return ptr;		\
	   if ((ptr=b2::RTTI_cast(t))) return ptr;		\
	   return 0;						\
	}							\
	void* cls::RTTI_scast(int i,void* p)			\
	{  cls* ptr = (cls*)p;					\
	   switch(i)						\
	   {  case  0: return (b1*)ptr;				\
	      case  1: return (b2*)ptr;				\
	   }							\
	   return ptr;						\
	}							
	
#define RTTI_DEF3_BASE(cls,name,b1,b2,b3)			\
        static const RTTITypeinfo* RTTI_base_##cls [] = 	\
	       { STATIC_TYPE_INFO(b1).get_info(),		\
		 STATIC_TYPE_INFO(b2).get_info(),		\
		 STATIC_TYPE_INFO(b3).get_info(), 0 };		\
  	void* cls::RTTI_cast(RTTItypeid t)			\
	{							\
	   if (t == &RTTI_obj) return this;			\
	   void* ptr;						\
	   if ((ptr=b1::RTTI_cast(t))) return ptr;		\
	   if ((ptr=b2::RTTI_cast(t))) return ptr;		\
	   if ((ptr=b3::RTTI_cast(t))) return ptr;		\
	   return 0;						\
	}							\
  	const void* cls::RTTI_cast(RTTItypeid t)const	\
	{							\
	   if (t == &RTTI_obj) return this;			\
	   const void* ptr;					\
	   if ((ptr=b1::RTTI_cast(t))) return ptr;		\
	   if ((ptr=b2::RTTI_cast(t))) return ptr;		\
	   if ((ptr=b3::RTTI_cast(t))) return ptr;		\
	   return 0;						\
	}							\
	void* cls::RTTI_scast(int i,void* p)			\
	{  cls* ptr = (cls*)p;					\
	   switch(i)						\
	   {  case  0: return (b1*)ptr;				\
	      case  1: return (b2*)ptr;				\
	      case  2: return (b3*)ptr;				\
	   }							\
	   return ptr;						\
	}							
	
					

#define RTTI_DEF_INST(cls,name)					\
	RTTI_DEF_BASE(cls,name)					\
	RTTI_NEW(cls,name)			

#define RTTI_DEF(cls,name)					\
	RTTI_DEF_BASE(cls,name)					\
	RTTI_NO_NEW(cls,name)

#define RTTI_DEF1_INST(cls,name,b1)				\
	RTTI_DEF1_BASE(cls,name,b1)				\
	RTTI_NEW(cls,name)			

#define RTTI_DEF1(cls,name,b1)					\
	RTTI_DEF1_BASE(cls,name,b1)				\
	RTTI_NO_NEW(cls,name)			
	
#define RTTI_DEF2_INST(cls,name,b1,b2)				\
	RTTI_DEF2_BASE(cls,name,b1,b2)				\
	RTTI_NEW(cls,name)			

#define RTTI_DEF2(cls,name,b1,b2)				\
	RTTI_DEF2_BASE(cls,name,b1,b2)				\
	RTTI_NO_NEW(cls,name)			

#define RTTI_DEF3_INST(cls,name,b1,b2,b3)			\
	RTTI_DEF3_BASE(cls,name,b1,b2,b3)			\
	RTTI_NEW(cls,name)	

#define RTTI_DEF3(cls,name,b1,b2,b3)				\
	RTTI_DEF3_BASE(cls,name,b1,b2,b3)			\
	RTTI_NO_NEW(cls,name)			


#endif


			 

		
