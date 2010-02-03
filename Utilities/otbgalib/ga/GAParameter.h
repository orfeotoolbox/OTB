// $Header$
/* ----------------------------------------------------------------------------
  parameters.h
  mbwall 14jul95
  Copyright (c) 1995 Massachusetts Institute of Technology 
                     all rights reserved

 DESCRIPTION:
  Header for the parameters object used by the GA objects.
---------------------------------------------------------------------------- */
#ifndef _ga_parameters_h_
#define _ga_parameters_h_

#include <ga/gaconfig.h>
#include <ga/gatypes.h>
#include <ga/std_stream.h>


/* ----------------------------------------------------------------------------
   This object is used for naming the parameters.  We associate a fullname, a
short name, and a value with each parameter.
---------------------------------------------------------------------------- */
class GAParameter {
public:
  enum Type {BOOLEAN, CHAR, STRING, INT, FLOAT, DOUBLE, POINTER};

public:
  GAParameter(const char* fn, const char* sn, Type tp, const void* v);
  GAParameter(const GAParameter& orig);
  GAParameter& operator=(const GAParameter& orig){ copy(orig); return *this; }
  virtual ~GAParameter();
  void copy(const GAParameter&);
  char* fullname() const { return fname; }
  char* shrtname() const { return sname; }
  const void* value() const 
    { return (t==STRING ? val.sval : (t==POINTER ? val.pval : &val)); }
  const void* value(const void* v) 
    {setvalue(v); return(t==STRING ? val.sval : (t==POINTER ? val.pval:&val));}
  Type type() const { return t; }

protected:
  char* fname;
  char* sname;
  union Value {
    int ival;
    char cval;
    char* sval;
    float fval;
    double dval;
    const void* pval;
  } val;
  Type t;
  void setvalue(const void*);
};






/* ----------------------------------------------------------------------------
   The parameter list is implemented as an array, but has the interface of a
list.  Don't ask.  You can traverse through the list to get the parameters that
you need.  Be sure to check the type before you try to extract the value for 
any specific parameter in the list.
---------------------------------------------------------------------------- */
class GAParameterList {
public:
  GAParameterList();
  GAParameterList(const GAParameterList&);
  GAParameterList& operator=(const GAParameterList&);
  virtual ~GAParameterList();

  int size() const { return n; }
  int get(const char*, void*) const;
  int set(const char*, const void*);
  int set(const char* s, int v) { return set(s, (void*)&v); }
  int set(const char* s, unsigned int v) { return set(s, (void*)&v); }
  int set(const char* s, char v) { return set(s, (void*)&v); }
  int set(const char* s, const char* v) { return set(s, (void*)v); }
  int set(const char* s, double v);
  int add(const char*, const char*, GAParameter::Type, const void*);
  int remove();
  GAParameter& operator[](unsigned int i) const {return *(p[i]);}
  GAParameter& next(){return *(p[((cur > n) ? cur=0 : ++cur)]);}
  GAParameter& prev(){return *(p[((cur == 0) ? cur=n-1 : --cur)]);}
  GAParameter& current() const {return *(p[cur]);}
  GAParameter& first(){return *(p[cur=0]);}
  GAParameter& last(){return *(p[cur=n-1]);}
  GAParameter* operator()(const char* name);
  int parse(int& argc, char **argv, GABoolean flag=gaTrue);

#ifdef GALIB_USE_STREAMS
  int write(const char* filename) const;
  int write(STD_OSTREAM & os) const;
  int read(const char* filename, GABoolean flag=gaTrue);
  int read(STD_ISTREAM & is, GABoolean flag=gaTrue);
#endif

protected:
  unsigned int n, N, cur;
  GAParameter **p;
};



#ifdef GALIB_USE_STREAMS
inline STD_OSTREAM & operator<< (STD_OSTREAM &os, const GAParameterList& plist)
{ plist.write(os); return os; }
inline STD_ISTREAM & operator>> (STD_ISTREAM& is, GAParameterList& plist)
{ plist.read(is); return is; }
#endif

#endif
