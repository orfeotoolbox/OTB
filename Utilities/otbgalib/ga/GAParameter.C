// $Header$
/* ----------------------------------------------------------------------------
  parameters.C
  mbwall 28jul94
  Copyright (c) 1995 Massachusetts Institute of Technology
                     all rights reserved

 DESCRIPTION:
  Definition of the parameter object and a container list for it.  I did this
as a separate list implementation because I don't want all of the overhead of
a fullblown list.  The parameter list is a special purpose, stripped down list
implementation.
---------------------------------------------------------------------------- */
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <ga/gaconfig.h>
#include <ga/gaerror.h>
#include <ga/GAParameter.h>

#define PRM_CHUNKSIZE 10
#define BUFSIZE       1024	// size of buffer for reading pairs
#define MAX_PAIRS     5000	// max number of name-value pairs in stream
#define NAMESIZE      128	// max length of name in name-value pair

extern char _gaerrbuf1[];
extern char _gaerrbuf2[];
static int IsNumeric(const char*);

GAParameter::GAParameter(const char* fn, const char* sn, 
			 Type tp, const void* v){
  if(fn){
    fname = new char[strlen(fn)+1];
    strcpy(fname, fn);
  }
  else fname = (char*)0;

  if(sn){
    sname = new char[strlen(sn)+1];
    strcpy(sname, sn);
  }
  else sname = (char*)0;

  t = tp;
  memset(&val, 0, sizeof(Value));
  setvalue(v);
}

GAParameter::GAParameter(const GAParameter& orig){
  fname=sname = (char*)0; 
  memset(&val, 0, sizeof(Value));
  copy(orig);
}

void
GAParameter::copy(const GAParameter& orig){
  if(&orig == this) return;

  delete [] fname;
  delete [] sname;
  if(orig.fname){
    fname = new char[strlen(orig.fname)+1];
    strcpy(fname, orig.fname);
  }
  else fname = (char *)0;
  if(orig.sname){
    sname = new char[strlen(orig.sname)+1];
    strcpy(sname, orig.sname);
  }
  else sname = (char *)0;

  t = orig.t;
  setvalue(orig.value());	// do this directly...
}    

GAParameter::~GAParameter(){
  delete [] fname;
  delete [] sname;
  if(t == STRING) delete [] val.sval;
}


void
GAParameter::setvalue(const void* v){
  switch(t){
  case BOOLEAN:
  case INT:
    val.ival = *((int*)v);
    break;
  case CHAR:
    val.cval = *((char*)v);
    break;
  case STRING:
    if(v != val.sval) {
      char* ptr=0;
      if(strlen((char*)v) > 0){
	ptr = new char[strlen((char*)v)+1];
	strcpy(ptr, (char*)v);
      }
      delete [] val.sval;
      val.sval = ptr;
    }
    break;
  case FLOAT:
    val.fval = *((float*)v);
    break;
  case DOUBLE:
    val.dval = *((double*)v);
    break;
  case POINTER:
  default:
    val.pval = v;
    break;
  }
}






// The default parameter list is empty.  Allocate space for the pointers, but
// don't allocate any parameters at this point.
GAParameterList::GAParameterList(){
  N = n = cur = 0;
  p = (GAParameter**)0;
}

GAParameterList::GAParameterList(const GAParameterList& list){
  N = list.N;
  n = list.n;
  cur = list.cur;
  p = new GAParameter * [N];
  for(unsigned int i=0; i<n; i++)
    p[i] = new GAParameter(*(list.p[i]));
}

// This is a rather stupid operator= implementation.  Instead of doing a copy
// we just nuke everything the reallocate new stuff.  If this were called a lot
// then we could end up with some fragmentation this way (rather than just 
// doing copies on already-allocated memory).
GAParameterList&
GAParameterList::operator=(const GAParameterList& list){
  if(&list == this) return *this;

  unsigned int i;
  for(i=0; i<n; i++)
    delete p[i];
  delete [] p;

  N = list.N;
  n = list.n;
  cur = list.cur;
  p = new GAParameter * [N];
  for(i=0; i<n; i++)
    p[i] = new GAParameter(*(list.p[i]));

  return *this;
}

GAParameterList::~GAParameterList(){
  for(unsigned int i=0; i<n; i++)
    delete p[i];
  delete [] p;
}


// Set the specified parameter (if we have it).  If we don't recognize the name
// (ie it has not been added to the list) then we return the error code.
int
GAParameterList::set(const char* name, const void* v){
  int found = 0;
  for(unsigned int i=0; i<n && !found; i++){
    if(strcmp(name, p[i]->fullname()) == 0 || 
       strcmp(name, p[i]->shrtname()) == 0){
      p[i]->value(v);
      found = 1;
    }
  }
  return found ? 0 : -1;
}


// Must do a special case for double/float.  Any floats that get passed to this
// routine will be cast to doubles, but then we need to force them back to a
// float if FLOAT is the type that is expected.  Kind of sucks, eh?
//   We could check the parameter type against the type here, but we don't. 
// (could do it for all of the 'set' members).  Maybe in a later release.
int
GAParameterList::set(const char* name, double v) {
  int found = 0;
  for(unsigned int i=0; i<n; i++){
    if(strcmp(name, p[i]->fullname()) == 0 ||
       strcmp(name, p[i]->shrtname()) == 0){
      if(p[i]->type() == GAParameter::FLOAT){
	float fval = (float)v;
	p[i]->value((void*)&fval);
      }
      else if(p[i]->type() == GAParameter::DOUBLE)
	p[i]->value((void*)&v);
      else
	GAErr(GA_LOC, "GAParameterList", "set", gaErrBadTypeIndicator);
      found = 1;
    }
  }
  return found ? 0 : -1;
}

// This allocates space for strings, so be sure to free it!
int
GAParameterList::get(const char* name, void* value) const {
  int status = 1;
  for(unsigned int i=0; i<n; i++){
    if(strcmp(name, p[i]->fullname()) == 0 ||
       strcmp(name, p[i]->shrtname()) == 0){
      switch(p[i]->type()){
      case GAParameter::BOOLEAN:
      case GAParameter::INT:
	*((int*)value) = *((int*)p[i]->value());
	break;
      case GAParameter::CHAR:
	*((char*)value) = *((char*)p[i]->value());
	break;
      case GAParameter::STRING:
	break;
      case GAParameter::FLOAT:
	*((float*)value) = *((float*)p[i]->value());
	break;
      case GAParameter::DOUBLE:
	*((double*)value) = *((double*)p[i]->value());
	break;
      case GAParameter::POINTER:
      default:
	break;
      }
      status = 0;
    }
  }
  return status;
}


// Add the item to the list if it does not already exist.  Return 0 if the add
// was OK, -1 if there was a problem.
int
GAParameterList::add(const char* fn, const char* sn,
		     GAParameter::Type t, const void* v) { 
  int status = -1;
  if(n == N){
    N += PRM_CHUNKSIZE;
    GAParameter **tmp = p;
    p = new GAParameter* [N];
    memcpy(p, tmp, n * sizeof(GAParameter*));
    delete [] tmp;
  }
  int found = 0;
  for(unsigned int i=0; i<n && !found; i++){
    if(strcmp(fn, p[i]->fullname()) == 0 &&
       strcmp(sn, p[i]->shrtname()) == 0)
      found = 1;
  }
  if(!found){
    cur = n;
    p[n++] = new GAParameter(fn, sn, t, v);
  }
  return status;
}


// When you remove a parameter from the list, the iterator is left pointing
// at the same location.  If the item was the last in the list, then the 
// iterator moves to the new last item in the list.
//   Return 0 if everything was OK, -1 if error.
int
GAParameterList::remove(){
  int status = -1;
  if(cur > n) return status;
  delete p[cur];
  memmove(&(p[cur]), &(p[cur+1]), (n-cur-1)*sizeof(GAParameter*));
  n--;
  if(cur > n) cur = n;
  status = 0;
  return status;
}

GAParameter* 
GAParameterList::operator()(const char* name){
  for(unsigned int i=0; i<n; i++)
    if(strcmp(name, p[i]->fullname())==0 || strcmp(name, p[i]->shrtname())==0)
      return p[i];
  return (GAParameter*)0;
}


#ifdef GALIB_USE_STREAMS
// Dump the parameters to the specified stream.  Just name-value pairs with a
// tab delimiter and newline separating pairs.
//   If there is an error, return 1, otherwise return 0.
int
GAParameterList::write(STD_OSTREAM& os) const {
  for(unsigned int i=0; i<n; i++){
    int ival;
    float fval;
    double dval;
    char cval;
    char* sval;

    os << p[i]->fullname() << "\t";

    switch(p[i]->type()){
    case GAParameter::BOOLEAN:
      ival = *((int*)(p[i]->value()));
      if(ival) os << "1\n";
      else os << "0\n";
      break;
    case GAParameter::INT:
      ival = *((int*)(p[i]->value()));
      os << ival << "\n";
      break;
    case GAParameter::CHAR:
      cval = *((char*)(p[i]->value()));
      os << cval << "\n";
      break;
    case GAParameter::STRING:
      sval = ((char*)(p[i]->value()));
      os << sval << "\n";
      break;
    case GAParameter::FLOAT:
      fval = *((float*)(p[i]->value()));
      os << fval << "\n";
      break;
    case GAParameter::DOUBLE:
      dval = *((double*)(p[i]->value()));
      os << dval << "\n";
      break;
    case GAParameter::POINTER:
    default:
      os << "(pointer)\n";
//      os << p[i]->value() << "\n";
      break;
    }
  }
  return 0;
}

int
GAParameterList::write(const char* filename) const {
  STD_OFSTREAM outfile(filename, (STD_IOS_OUT | STD_IOS_TRUNC));
// should be done this way, but SGI systems (and others?) don't do it right...
//  if(! outfile.is_open()){
  if(outfile.fail()){
    GAErr(GA_LOC, "GAParameterList", "write", gaErrWriteError, filename);
    return 1;
  }
  int status = write(outfile);
  outfile.close();
  return status;
}

// Read name-value pairs from the stream.  If the first item is a number, then
// we expect that many name-value pairs.  If not, then we read until the end of
// the stream.  Be sure not to duplicate the last one, and be sure to dump any
// items from a # to end-of-line.
//   Parse for name-value pairs, where the pairs are separated by whitespace.
// *Every* parameter must be specified as a name-value pair (no single name 
// with no value allowed).  If we get a single name with no value that will
// screw up the name-value pairing for any remaining pairs.  We could check for
// this situation, but for now we just let it happen and dump the errors. (i'll
// do a better parsing algorithm at some later point).
//   We return the number of items that we were able to read from the stream.
// If we come across an item that is unrecognized, we dump an error message.
//   The buffer length is limited here, so don't try to read in any really long
// lines.
//   We parse only for items that we know about - if we come across a pair
// whose name we do not know about, we ignore the pair and go on to the next.
// There's no global list to tell us what type things are, and we don't assume.
//   We don't allow setting pointers using this method.
int
GAParameterList::read(STD_ISTREAM& is, GABoolean flag){
  int nfound = 0;
  if(n == 0) return nfound;

  char buf[BUFSIZE];
  char name[NAMESIZE];
  int toggle = 0, count = 0, npairs = -1;

  is.width(sizeof(buf));	// don't allow to overrun buffer

  do{
    is >> buf;
    if(npairs == -1){
      if(IsNumeric(buf)){
	npairs = atoi(buf);
	is >> buf;
      }
      else{
	npairs = MAX_PAIRS;
      }
    }
    if(is.eof()){
      break;
    }
    else if(buf[0] == '#'){		// dump to end-of-line
      is.get(buf, BUFSIZE);		// fails on lines longer than BUFSIZE
    }
    else if(toggle == 0){
      strcpy(name, buf);
      toggle = 1;
    }
    else if(toggle == 1){
      int found = 0;

      count += 1;
      toggle = 0;

      for(unsigned int i=0; i<n && !found; i++){
	if(strcmp(name, p[i]->fullname()) == 0 ||
	   strcmp(name, p[i]->shrtname()) == 0){
	  found = 1;

	  int ival;
	  float fval;
	  double dval;
	  
	  switch(p[i]->type()){
	  case GAParameter::BOOLEAN:
	  case GAParameter::INT:
	    ival = atoi(buf);
	    set(name, (void*)&ival);
	    nfound += 1;
	    break;
	  case GAParameter::CHAR:
	  case GAParameter::STRING:
	    set(name, (void*)buf);
	    nfound += 1;
	    break;
	  case GAParameter::FLOAT:
	    fval = (float)atof(buf);
	    set(name, (void*)&fval);
	    nfound += 1;
	    break;
	  case GAParameter::DOUBLE:
	    dval = (double)atof(buf);
	    set(name, (void*)&dval);
	    nfound += 1;
	    break;
	  case GAParameter::POINTER:
	  default:
	    break;
	  }

// Move this parameter to the front of the list

//	  if(i > 0) {
//	    GAParameter *tmpptr = p[i];
//	    memmove(&(p[1]), &(p[0]), i * sizeof(GAParameter*));
//	    p[0] = tmpptr;
//	  }
	  if(i < n-1) {
	    GAParameter *tmpptr = p[i];
	    memmove(&(p[i]), &(p[i+1]), (n-i-1) * sizeof(GAParameter*));
	    p[n-1] = tmpptr;
	  }
	}
      }

      if(!found && flag == gaTrue){
	strcpy(_gaerrbuf1, "");
	strcat(_gaerrbuf1, "unrecognized variable name '");
	strcat(_gaerrbuf1, name);
	strcat(_gaerrbuf1, "'");
	GAErr(GA_LOC, "GAParameterList", "read", _gaerrbuf1);
      }
    }
  } while(!is.eof() && count < npairs);

  if(toggle == 1){
    strcpy(_gaerrbuf1, "");
    strcat(_gaerrbuf1, "variable ");
    strcat(_gaerrbuf1, name);
    strcat(_gaerrbuf1, " has no value");
    strcat(_gaerrbuf2, "be sure there is a newline at end of the file");
    GAErr(GA_LOC, "GAParameterList", "read", _gaerrbuf1, _gaerrbuf2);

    is.clear(STD_IOS_BADBIT | is.rdstate());
  }

  return nfound;
}

int
GAParameterList::read(const char* filename, GABoolean flag){
  STD_IFSTREAM infile(filename, STD_IOS_IN);
  if(!infile){
    GAErr(GA_LOC, "GAParameterList", "read", gaErrReadError, filename);
    return 1;
  }
  int status = read(infile, flag);
  infile.close();
  return status;
}
#endif


// Parse the arglist for any recognized arguments.  If we find a string we
// know, then we set the value.  If we find a string we don't know then we
// don't do anything unless the flag is set.  If the flag is set then we
// complain about unknown arguments.
//   You should set up the list before you do the parsing in order to grab
// arguments from the arglist.
//   When we encounter names we know with valid values, we put the list into
// order as we get the new values.  When we recognize a pair, we pull the
// parameter from the list, set its value, then stick it at the end of the
// parameter list.  So if something turns up more than once, we'll remove-
// then-append it more than once and the ordering from argv will be properly
// maintained.
//   We assume that argv[0] is the name of the program, so we don't barf on 
// it if it is not a recognized name.
int
GAParameterList::parse(int& argc, char *argv[], GABoolean flag){
  int nfound = 0;
  if(n == 0) return nfound;

  char **argvout = new char* [argc];
  int argcu = argc-1;
  int argcl = 0;

  for(int i=0; i<argc; i++){
    int found = 0;

// Loop through all of the parameters to see if we got a match.  If there is
// no value for the name, complain.  Otherwise, set the value.

    for(unsigned int j=0; j<n && found == 0; j++){
      if(strcmp(p[j]->shrtname(), argv[i]) == 0 ||
	 strcmp(p[j]->fullname(), argv[i]) == 0){
	found = 1;
	argvout[argcu] = argv[i]; argcu--;
	if(++i >= argc){
	  GAErr(GA_LOC, argv[0], argv[i-1], " needs a value");
	}
	else{
	  int ival;
	  float fval;
	  double dval;
	  
	  switch(p[j]->type()){
	  case GAParameter::BOOLEAN:
	    if(IsNumeric(argv[i])){
	      ival = atoi(argv[i]);
	      ival = (ival == 0) ? 0 : 1;
	    }
	    else {
	      if(strcmp(argv[i], "true") == 0 ||
		 strcmp(argv[i], "True") == 0 ||
		 strcmp(argv[i], "TRUE") == 0 ||
		 strcmp(argv[i], "t") == 0 ||
		 strcmp(argv[i], "T") == 0)
		ival = 1;
	    }
	    set(argv[i-1], (void*)&ival);
	    nfound += 1;
	    break;
	  case GAParameter::INT:
	    ival = atoi(argv[i]);
	    set(argv[i-1], (void*)&ival);
	    nfound += 1;
	    break;
	  case GAParameter::CHAR:
	  case GAParameter::STRING:
	    set(argv[i-1], (void*)argv[i]);
	    nfound += 1;
	    break;
	  case GAParameter::FLOAT:
	    fval = (float)atof(argv[i]);
	    set(argv[i-1], (void*)&fval);
	    nfound += 1;
	    break;
	  case GAParameter::DOUBLE:
	    dval = (double)atof(argv[i]);
	    set(argv[i-1], (void*)&dval);
	    nfound += 1;
	    break;
	  case GAParameter::POINTER:
	  default:
	    break;
	  }

// Move this parameter to the front of the list

//	  if(j > 0) {
//	    GAParameter *tmpptr = p[j];
//	    memmove(&(p[1]), &(p[0]), j * sizeof(GAParameter*));
//	    p[0] = tmpptr;
//	  }
	  if(j < n-1) {
	    GAParameter *tmpptr = p[j];
	    memmove(&(p[j]), &(p[j+1]), (n-j-1) * sizeof(GAParameter*));
	    p[n-1] = tmpptr;
	  }

// Now update the argv array and argc count to indicate we understood this one

	  argvout[argcu] = argv[i]; argcu--;
	  continue;
	}
      }
    }
    if(!found){
      if(flag && i!=0){
	_gaerrbuf1[0] = '\0';
	strcat(_gaerrbuf1, "unrecognized name ");
	strcat(_gaerrbuf1, argv[i]);
	GAErr(GA_LOC, "GAParameterList", "parse", _gaerrbuf1);
      }
      argvout[argcl] = argv[i];
      argcl++;
    }
  }

//  bcopy(argvout, argv, argc * sizeof(char*));
  memcpy(argv, argvout, argc*sizeof(char*));
  argc = argcl;
  delete [] argvout;

  return nfound;
}
 


static int
IsNumeric(const char* str){
  for(int i=strlen(str)-1; i>=0; i--)
    if(! isdigit(str[i]) && str[i] != '.') return 0;
  return 1;
}
