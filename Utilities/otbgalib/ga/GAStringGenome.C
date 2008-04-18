// $Header$
/* ----------------------------------------------------------------------------
  string.C
  mbwall 21mar95
  Copyright (c) 1995-1996 Massachusetts Institute of Technology
                          all rights reserved

 DESCRIPTION:
   Source file for the string specialization of the array genome.
---------------------------------------------------------------------------- */
#include <ga/GAStringGenome.h>

template <> const char * 
GA1DArrayAlleleGenome<char>::className() const {return "GAStringGenome";}
template <> int 
GA1DArrayAlleleGenome<char>::classID() const {return GAID::StringGenome;}

template <> GA1DArrayAlleleGenome<char>::
GA1DArrayAlleleGenome(unsigned int length, const GAAlleleSet<char> & s,
		      GAGenome::Evaluator f, void * u) :
GA1DArrayGenome<char>(length, f, u){
  naset = 1;
  aset = new GAAlleleSet<char>[1];
  aset[0] = s;

  initializer(DEFAULT_STRING_INITIALIZER);
  mutator(DEFAULT_STRING_MUTATOR);
  comparator(DEFAULT_STRING_COMPARATOR);
  crossover(DEFAULT_STRING_CROSSOVER);
}

template <> GA1DArrayAlleleGenome<char>::
GA1DArrayAlleleGenome(const GAAlleleSetArray<char> & sa,
		      GAGenome::Evaluator f, void * u) :
GA1DArrayGenome<char>(sa.size(), f, u){
  naset = sa.size();
  aset = new GAAlleleSet<char>[naset];
  for(int i=0; i<naset; i++)
    aset[i] = sa.set(i);

  initializer(DEFAULT_STRING_INITIALIZER);
  mutator(DEFAULT_STRING_MUTATOR);
  comparator(DEFAULT_STRING_COMPARATOR);
  crossover(DEFAULT_STRING_CROSSOVER);
}

template <> 
GA1DArrayAlleleGenome<char>::~GA1DArrayAlleleGenome(){
  delete [] aset;
}


#ifdef GALIB_USE_STREAMS
// The read specialization takes in each character whether it is whitespace or
// not and stuffs it into the genome.  This is unlike the default array read.
template <> int
GA1DArrayAlleleGenome<char>::read(STD_ISTREAM & is)
{
  unsigned int i=0;
  char c;
  do{
    is.get(c);
    if(!is.fail()) gene(i++, c);
  } while(!is.fail() && !is.eof() && i < nx);

  if(is.eof() && i < nx){
    GAErr(GA_LOC, className(), "read", gaErrUnexpectedEOF);
    is.clear(STD_IOS_BADBIT | is.rdstate());
    return 1;
  }
  return 0;
}

// Unlike the base array genome, here when we write out we don't put any
// whitespace between genes.  No newline at end of it all.
template <> int
GA1DArrayAlleleGenome<char>::write(STD_OSTREAM & os) const
{
  for(unsigned int i=0; i<nx; i++)
    os << gene(i);
  return 0;
}
#endif



// force instantiations of this genome type.
//
// These must be included _after_ the specializations because some compilers
// get all wigged out about the declaration/specialization order.  Note that
// some compilers require a syntax different than others when forcing the 
// instantiation (i.e. GNU wants the 'template class', borland does not).
#ifndef GALIB_USE_AUTO_INST
#include <ga/GAAllele.C>
#include <ga/GA1DArrayGenome.C>

#if defined(__BORLANDC__)
#define GALIB_STRINGGENOME_TEMPLATE_PREFACE
#else
#define GALIB_STRINGGENOME_TEMPLATE_PREFACE template class
#endif

GALIB_STRINGGENOME_TEMPLATE_PREFACE GAAlleleSet<char>;
GALIB_STRINGGENOME_TEMPLATE_PREFACE GAAlleleSetCore<char>;
GALIB_STRINGGENOME_TEMPLATE_PREFACE GAAlleleSetArray<char>;

GALIB_STRINGGENOME_TEMPLATE_PREFACE GAArray<char>;
GALIB_STRINGGENOME_TEMPLATE_PREFACE GA1DArrayGenome<char>;
GALIB_STRINGGENOME_TEMPLATE_PREFACE GA1DArrayAlleleGenome<char>;

#endif
