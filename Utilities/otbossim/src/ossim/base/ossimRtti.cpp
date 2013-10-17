
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <iostream>
using namespace std;

#include <ossim/base/ossimRtti.h>

static const RTTITypeinfo* RTTI_base_null_type[] = { 0 };

const RTTITypeinfo* RTTIdyntypeid::a[] = { 0 };

const RTTITypeinfo RTTITypeinfo::null_type = RTTITypeinfo("NULL", RTTI_base_null_type,0,0);

void RTTITypeinfo::add_subtype(const RTTITypeinfo* t)		//Adds t as last RTTITypeinfo in the
{		
  subtypes.push_back(t);
  ns = subtypes.size();
							//'subtypes' list. For this, the
/*   const RTTITypeinfo** ptr = new const RTTITypeinfo*[ns+1];		//list is realloc'd with one extra entry.
   int i; for(i=0;i<ns;i++) ptr[i] = subtypes[i];
   ptr[i] = t;
   ns++;
   delete[] subtypes;
   subtypes = ptr;
   */
}

void RTTITypeinfo::del_subtype(const RTTITypeinfo* t) 		//Searches for t in the subtypes list
{	

  SubtypesConstVector::iterator iter = std::find(subtypes.begin(), subtypes.end(), t);
  if(iter != subtypes.end())
  {
    subtypes.erase(iter);
    ns = subtypes.size();
  }
								//of this and removes it, if found.

//   int i; for(i=0;i<ns && subtypes[i]!=t;i++);
//   if (i<ns)
//     for(;i<ns-1;i++) subtypes[i] = subtypes[i+1];
}

RTTITypeinfo::RTTITypeinfo(const char* name,
                           const RTTITypeinfo* bb[],
                           void* (*f1)(int,void*),
                           void* (*f2)())
{
  n = "";
  if(name) n = name;
  b = bb; //ns = 0; subtypes = 0;		//Create default RTTITypeinfo
  cast    = f1;								//Attach casting func
  new_obj = f2;								//Attach creation func
  int i = 0;
  for(i=0;b[i];i++)							//Add this as subtype to all its basetypes
     ((RTTITypeinfo**)b)[i]->add_subtype(this);				//REMARK: Harmless const castaway
}

const char* RTTITypeinfo::getname() const
{
  return n.c_str();
}


RTTITypeinfo::~RTTITypeinfo()
{
//   if(n)
//   {
//      free(n);
//      n = NULL;
//   }
   int i = 0;
   for(i=0;b[i];i++)
   {
      //Del this subtype from all its basetypes
      ((RTTITypeinfo**)b)[i]->del_subtype(this); //REMARK: Harmless const castaway
   }
}

int RTTITypeinfo::has_base(const RTTITypeinfo* p) const
{
  int i = 0;
   for(i=0;b[i];i++)							//for all bases of this...
      if (p->same(b[i]) || b[i]->has_base(p)) return 1;				//match found, return 1 or no match, search deeper
   return 0;									//no match at all, return 0
}

void* RTTITypeinfo::create(const RTTITypeinfo* bt, const char* c) const	//Tries to create an obj of type-name
{										//given by char*. Searches for this type in the
   void* p = 0; int i;								//type-DAG rooted by this, creates it and returns
										//it as cast to 'bt', where bt is either this or a
										//direct base of this.
   if (!strcmp(c,getname()))								//Want to create an obj of this type ?
      p = (new_obj)? new_obj() : 0;						//Yes, do it if this type is instantiable.
   else										//No, try with subclasses...
      for(i=0;i<ns && !((p=subtypes[i]->create(this,c)));i++);			//Succeeded creating on ith subclass branch ?
   if (!p) return 0;								//Couldn't create it in any way, abort.
   if (bt==this) i = -1;							//Must cast to this's own type (i==-1)
   else for(i=0;b[i] && b[i]!=bt;i++);						//Search to which base of this we should cast
									        //Found: cast to ith base of this
   return cast(i,p);								//Cast to ith base of to this, return as void*
}

RTTItypeid RTTItypeid::find_baseclass(const char* name)const
{
   if(strcmp(name, getname())==0)
   {
      return *this;
   }
   else if(num_baseclasses() == 0)
   {
      if(strcmp(name, getname()) == 0)
      {
         return *this;
      }
      return null_type();
   }
   else
   {
      int index = 0;

      while(index < num_baseclasses())
      {
         RTTItypeid typeId = baseclass(index);
         if(typeId == *this)
         {
	   return *this;//null_type();
         }
         if(strcmp(name, typeId.getname()) == 0)
         {
            return *this;
         }
         ++index;
      }
      index = 0;
      while(index < num_baseclasses())
      {
         RTTItypeid typeId = baseclass(index);
         if(typeId.find_baseclass(name) == typeId.null_type())
         {
            ++index;
         }
         else
         {
            return typeId;
         }
      }
   }
   return null_type();
}
