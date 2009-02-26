//
// Copyright (C) 1991 Texas Instruments Incorporated.
//
// Permission is granted to any individual or institution to use, copy, modify,
// and distribute this software, provided that this complete copyright and
// permission notice is maintained, intact, in all copies and supporting
// documentation.
//
// Texas Instruments Incorporated provides this software "as is" without
// express or implied warranty.
//
// .NAME CoolRegExp - Pattern matching with regular expressions.
// .LIBRARY cool
// .HEADER String Classes
// .INCLUDE cool/RegExp.h
// .FILE cool/RegExp.C
// .FILE cool/RegExp.h
// .EXAMPLE examples/ex_RegExp.C
//
//
// .SECTION Description
//  A regular expression allows a programmer to specify  complex
//  patterns  that  can  be searched for and matched against the
//  character string of a string object. In its simplest form, a
//  regular  expression  is  a  sequence  of  characters used to
//  search for exact character matches. However, many times  the
//  exact  sequence to be found is not known, or only a match at
//  the beginning or end of a string is desired. The cool  regu-
//  lar  expression  class implements regular expression pattern
//  matching as is found and implemented in many  UNIX  commands
//  and utilities.
//
//  The regular expression class provides a convenient mechanism
//  for  specifying  and  manipulating  regular expressions. The
//  regular expression object allows specification of such  pat-
//  terns  by using the following regular expression metacharac-
//  ters:
// 
//   ^        Matches at beginning of a line
//
//   $        Matches at end of a line
//
//  .         Matches any single character
//
//  [ ]       Matches any character(s) inside the brackets
//
//  [^ ]      Matches any character(s) not inside the brackets
//
//   -        Matches any character in range on either side of a dash
//
//   *        Matches preceding pattern zero or more times
//
//   +        Matches preceding pattern one or more times
//
//   ?        Matches preceding pattern zero or once only
//
//  ()        Saves a matched expression and uses it in a  later match
// 
//  Note that more than one of these metacharacters can be  used
//  in  a  single  regular expression in order to create complex
//  search patterns. For example, the pattern [^ab1-9]  says  to
//  match  any  character  sequence that does not begin with the
//  characters "ab"  followed  by  numbers  in  the  series  one
//  through nine.
 

#ifndef ossimRegExph
#define ossimRegExph

#include <ossim/base/ossimString.h>

const int NSUBEXP = 10;


class OSSIMDLLEXPORT ossimRegExp {
public:
  inline ossimRegExp ();			// ossimRegExp with program=NULL
  inline ossimRegExp (const char*);	// ossimRegExp with compiled char*
  ossimRegExp (const ossimRegExp&);	// Copy constructor
  inline ~ossimRegExp();			// Destructor 

  void compile (const char*);		// Compiles char* --> regexp
  bool find (const char*);		// true if regexp in char* arg
  inline ossim_uint32 start() const;	// Index to start of first find
  inline ossim_uint32 end() const;	// Index to end of first find

  bool operator== (const ossimRegExp&) const;	// Equality operator
  inline bool operator!= (const ossimRegExp&) const; // Inequality operator
  bool deep_equal (const ossimRegExp&) const;	// Same regexp and state?
  
  inline bool is_valid() const;		// true if compiled regexp
  inline void set_invalid();		// Invalidates regexp

  // awf added
  ossim_uint32 start(ossim_uint32 n) const;
  ossim_uint32 end(ossim_uint32 n) const;
  std::string match(ossim_uint32 n) const;
  
private: 
  const char* startp[NSUBEXP];
  const char* endp[NSUBEXP];
  char  regstart;			// Internal use only
  char  reganch;			// Internal use only
  const char* regmust;			// Internal use only
  ossim_uint32 regmlen;			// Internal use only
  char* program;   
  ossim_uint32 progsize;
  const char* searchstring;

 // work variables
  mutable const char* regparse;
  mutable int   regnpar;	// () count.
  mutable char  regdummy;
  mutable char* regcode;	// Code-emit pointer; &regdummy = don't.
  mutable long  regsize;	// Code size.
  const char*  reginput;	// String-input pointer.
  const char*  regbol;	// Beginning of input, for ^ check.
  const char* *regstartp;	// Pointer to startp array.
  const char* *regendp;	// Ditto for endp.

  char* reg (int, int*);
  char* regbranch (int*);
  char* regpiece (int*);
  char* regatom (int*);
  char* regnode (char);
  const char* regnext (register const char*);
  char* regnext (register char*);
  void        regc (unsigned char);
  void        reginsert (char, char*);
  void        regtail (char*, const char*);
  void        regoptail (char*, const char*);
  int regtry (const char*, const char* *,
              const char* *, const char*);
  int regmatch (const char*);
  int regrepeat (const char*);
#ifdef STRCSPN
  int strcspn ();
#endif
  
}; 

// ossimRegExp -- Creates an empty regular expression.

inline ossimRegExp::ossimRegExp () { 
  this->program = NULL;
}


// ossimRegExp -- Creates a regular expression from string s, and
// compiles s.


inline ossimRegExp::ossimRegExp (const char* s) {  
  this->program = NULL;
  compile(s);
}

// ~ossimRegExp -- Frees space allocated for regular expression.

inline ossimRegExp::~ossimRegExp () {
//#ifndef WIN32
   if(program)
   {
      delete [] this->program;
      this->program = 0;
   }
//#endif
}

// Start -- 

inline ossim_uint32 ossimRegExp::start () const {
  return(this->startp[0] - searchstring);
}


// End -- Returns the start/end index of the last item found.


inline ossim_uint32 ossimRegExp::end () const {
  return(this->endp[0] - searchstring);
}


// operator!= //

inline bool ossimRegExp::operator!= (const ossimRegExp& r) const {
  return(!(*this == r));
}


// is_valid -- Returns true if a valid regular expression is compiled
// and ready for pattern matching.

inline bool ossimRegExp::is_valid () const {
  return (this->program != NULL);
}


// set_invalid -- Invalidates regular expression.

inline void ossimRegExp::set_invalid () {
//#ifndef WIN32
  delete [] this->program;
//#endif
  this->program = NULL;
}

// -- Return start index of nth submatch. start(0) is the start of the full match.
inline ossim_uint32 ossimRegExp::start(ossim_uint32 n) const
{
  return this->startp[n] - searchstring;
}

// -- Return end index of nth submatch. end(0) is the end of the full match.
inline ossim_uint32 ossimRegExp::end(ossim_uint32 n) const
{
  return this->endp[n] - searchstring;
}

// -- Return nth submatch as a string.
inline std::string ossimRegExp::match(ossim_uint32 n) const
{
	return std::string(this->startp[n], this->endp[n] - this->startp[n]);
}

#endif // CoolRegExph
