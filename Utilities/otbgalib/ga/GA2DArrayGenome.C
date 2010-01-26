// $Header$
/* ----------------------------------------------------------------------------
  array2.C
  mbwall 25feb95
  Copyright (c) 1995 Massachusetts Institute of Technology
                     all rights reserved

 DESCRIPTION:
  Source file for the 2D array genome.
---------------------------------------------------------------------------- */
#ifndef _ga_array2_C_
#define _ga_array2_C_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ga/garandom.h>
#include <ga/GA2DArrayGenome.h>
#include <ga/GAMask.h>


/* ----------------------------------------------------------------------------
2DArrayGenome
---------------------------------------------------------------------------- */
template <class T> const char *
GA2DArrayGenome<T>::className() const {return "GA2DArrayGenome";}
template <class T> int
GA2DArrayGenome<T>::classID() const {return GAID::ArrayGenome2D;}

template <class T> 
GA2DArrayGenome<T>::
GA2DArrayGenome(unsigned int width, unsigned int height, 
		GAGenome::Evaluator f,
		void * u) :
GAArray<T>(width*height),
GAGenome(DEFAULT_2DARRAY_INITIALIZER, 
	 DEFAULT_2DARRAY_MUTATOR,
	 DEFAULT_2DARRAY_COMPARATOR)
{
  evaluator(f);
  userData(u);
  crossover(DEFAULT_2DARRAY_CROSSOVER);
  nx=minX=maxX=width; ny=minY=maxY=height;
}


template <class T> 
GA2DArrayGenome<T>::
GA2DArrayGenome(const GA2DArrayGenome<T> & orig) : GAArray<T>(orig.sz){
  GA2DArrayGenome<T>::copy(orig);
}


template <class T>
GA2DArrayGenome<T>::~GA2DArrayGenome() { }


template <class T> void
GA2DArrayGenome<T>::copy(const GAGenome & orig){
  if(&orig == this) return;
  const GA2DArrayGenome<T>* c = DYN_CAST(const GA2DArrayGenome<T>*, &orig);
  if(c) {
    GAGenome::copy(*c);
    GAArray<T>::copy(*c);
    nx = c->nx; ny = c->ny; 
    minX = c->minX; minY = c->minY; 
    maxX = c->maxX; maxY = c->maxY;
  } 
}


template <class T> GAGenome *
GA2DArrayGenome<T>::clone(GAGenome::CloneMethod flag) const {
  GA2DArrayGenome<T> *cpy = new GA2DArrayGenome<T>(nx,ny);
  if(flag == CONTENTS){
    cpy->copy(*this);
  }
  else{
    cpy->GAGenome::copy(*this);
    cpy->minX = minX; cpy->minY = minY; 
    cpy->maxX = maxX; cpy->maxY = maxY; 
  }
  return cpy;
}


template <class T> int
GA2DArrayGenome<T>::resize(int w, int h){
  if(w == STA_CAST(int,nx) && h == STA_CAST(int,ny)) return this->sz;

  if(w == GAGenome::ANY_SIZE)
    w = GARandomInt(minX, maxX);
  else if(w < 0)
    w = nx;		// do nothing
  else if(minX == maxX)
    minX=maxX = w;
  else{
    if(w < STA_CAST(int,minX)) w=minX;
    if(w > STA_CAST(int,maxX)) w=maxX;
  }

  if(h == GAGenome::ANY_SIZE)
    h = GARandomInt(minY, maxY);
  else if(h < 0)
    h = ny;		// do nothing
  else if(minY == maxY)
    minY=maxY = h;
  else{
    if(h < STA_CAST(int,minY)) h=minY;
    if(h > STA_CAST(int,maxY)) h=maxY;
  }

  if(w < STA_CAST(int,nx)){
    int y=GAMin(STA_CAST(int,ny),h);
    for(int j=0; j<y; j++)
      GAArray<T>::move(j*w,j*nx,w);
  }
  GAArray<T>::size(w*h);
  if(w > STA_CAST(int,nx)){		// adjust the existing chunks of bits
    int y=GAMin(STA_CAST(int,ny),h);
    for(int j=y-1; j>=0; j--)
      GAArray<T>::move(j*w,j*nx,nx);
  }

  nx = w; ny = h;
  _evaluated = gaFalse;
  return this->sz;
}


#ifdef GALIB_USE_STREAMS
template <class T> int
GA2DArrayGenome<T>::read(STD_ISTREAM &) {
  GAErr(GA_LOC, className(), "read", gaErrOpUndef);
  return 1;
}


template <class T> int
GA2DArrayGenome<T>::write(STD_OSTREAM & os) const 
{
  for(unsigned int j=0; j<ny; j++){
    for(unsigned int i=0; i<nx; i++){
      os << gene(i,j) << " ";
    }
    os << "\n";
  }
  return 0;
}
#endif


template <class T> int
GA2DArrayGenome<T>::resizeBehaviour(GAGenome::Dimension which) const {
  int val = 0;
  if(which == WIDTH) {
    if(maxX == minX) val = FIXED_SIZE;
    else val = maxX;
  }
  else if(which == HEIGHT) {
    if(maxY == minY) val = FIXED_SIZE;
    else val = maxY;
  }
  return val;
}


template <class T> int
GA2DArrayGenome<T>::
resizeBehaviour(GAGenome::Dimension which, 
		unsigned int lower, unsigned int upper)
{
  if(upper < lower){
    GAErr(GA_LOC, className(), "resizeBehaviour", gaErrBadResizeBehaviour);
    return resizeBehaviour(which);
  }

  switch(which){
  case WIDTH:
    minX = lower; maxX = upper;
    if(nx > upper) GA2DArrayGenome<T>::resize(upper,ny);
    if(nx < lower) GA2DArrayGenome<T>::resize(lower,ny);
    break;

  case HEIGHT:
    minY = lower; maxY = upper;
    if(ny > upper) GA2DArrayGenome<T>::resize(nx,upper);
    if(ny < lower) GA2DArrayGenome<T>::resize(nx,lower);
    break;

  default:
    break;
  }

  return resizeBehaviour(which);
}


template <class T> void
GA2DArrayGenome<T>::copy(const GA2DArrayGenome<T> & orig,
			 unsigned int r, unsigned int s,
			 unsigned int x, unsigned int y,
			 unsigned int w, unsigned int h)
{
  if(w == 0 || x >= orig.nx || r >= nx ||
     h == 0 || y >= orig.ny || s >= ny) return;
  if(x + w > orig.nx) w = orig.nx - x;
  if(y + h > orig.ny) h = orig.ny - y;
  if(r + w > nx) w = nx - r;
  if(s + h > ny) h = ny - s;

  for(unsigned int j=0; j<h; j++)
    GAArray<T>::copy(orig, (s+j)*nx+r, (y+j)*orig.nx+x, w);

  _evaluated = gaFalse; 
}



template <class T> int 
GA2DArrayGenome<T>::equal(const GAGenome & c) const
{
  if(this == &c) return 1;
  GA2DArrayGenome<T> & b = (GA2DArrayGenome<T> &)c;
  if(nx != b.nx || ny != b.ny) return 0;
  int val=0;
  for(unsigned int j=0; j<ny && val==0; j++)
    val = GAArray<T>::equal(b,j*nx,j*nx,nx) ? 0 : 1;
  return(val ? 0 : 1);
}























/* ----------------------------------------------------------------------------
2DArrayAlleleGenome
---------------------------------------------------------------------------- */
template <class T> const char *
GA2DArrayAlleleGenome<T>::className() const {return "GA2DArrayAlleleGenome";}
template <class T> int
GA2DArrayAlleleGenome<T>::classID() const {return GAID::ArrayAlleleGenome2D;}

template <class T> 
GA2DArrayAlleleGenome<T>::
GA2DArrayAlleleGenome(unsigned int width, unsigned int height, 
		      const GAAlleleSet<T> & s,
		      GAGenome::Evaluator f, void * u) :
GA2DArrayGenome<T>(width,height,f,u){
  naset = 1;
  aset = new GAAlleleSet<T>[1];
  aset[0] = s;

  initializer(GA2DArrayAlleleGenome<T>::DEFAULT_2DARRAY_ALLELE_INITIALIZER);
  mutator(GA2DArrayAlleleGenome<T>::DEFAULT_2DARRAY_ALLELE_MUTATOR);
  comparator(GA2DArrayAlleleGenome<T>::DEFAULT_2DARRAY_ALLELE_COMPARATOR);
  crossover(GA2DArrayAlleleGenome<T>::DEFAULT_2DARRAY_ALLELE_CROSSOVER);
}

template <class T> 
GA2DArrayAlleleGenome<T>::
GA2DArrayAlleleGenome(unsigned int width, unsigned int height, 
		      const GAAlleleSetArray<T> & sa,
		      GAGenome::Evaluator f, void * u) :
GA2DArrayGenome<T>(width,height, f, u) {
  naset = sa.size();
  aset = new GAAlleleSet<T>[naset];
  for(int i=0; i<naset; i++)
    aset[i] = sa.set(i);

  initializer(GA2DArrayAlleleGenome<T>::DEFAULT_2DARRAY_ALLELE_INITIALIZER);
  mutator(GA2DArrayAlleleGenome<T>::DEFAULT_2DARRAY_ALLELE_MUTATOR);
  comparator(GA2DArrayAlleleGenome<T>::DEFAULT_2DARRAY_ALLELE_COMPARATOR);
  crossover(GA2DArrayAlleleGenome<T>::DEFAULT_2DARRAY_ALLELE_CROSSOVER);
}


template <class T> 
GA2DArrayAlleleGenome<T>::
GA2DArrayAlleleGenome(const GA2DArrayAlleleGenome<T> & orig) : 
GA2DArrayGenome<T>(orig.nx, orig.ny) {
  naset = 0;
  aset = (GAAlleleSet<T>*)0;
  GA2DArrayAlleleGenome<T>::copy(orig);
}


template <class T>
GA2DArrayAlleleGenome<T>::~GA2DArrayAlleleGenome(){
  delete [] aset;
}


template <class T> GAGenome *
GA2DArrayAlleleGenome<T>::clone(GAGenome::CloneMethod) const {
  return new GA2DArrayAlleleGenome<T>(*this);
}


template <class T> void 
GA2DArrayAlleleGenome<T>::copy(const GAGenome& orig){
  if(&orig == this) return;
  const GA2DArrayAlleleGenome<T>* c = 
    DYN_CAST(const GA2DArrayAlleleGenome<T>*, &orig);
  if(c) {
    GA2DArrayGenome<T>::copy(*c);
    if(naset != c->naset){
      delete [] aset;
      naset = c->naset;
      aset = new GAAlleleSet<T>[c->naset];
    }
    for(int i=0; i<naset; i++)
      aset[i].link(c->aset[i]);
  }
}


template <class T> int
GA2DArrayAlleleGenome<T>::resize(int x, int y){
  unsigned int oldx = this->nx;
  unsigned int oldy = this->ny;
  GA2DArrayGenome<T>::resize(x,y);

  if(this->nx > oldx){		// adjust the existing chunks of bits
    int y=GAMin(oldy,this->ny);
    for(int j=y-1; j>=0; j--){
      for(unsigned int i=oldx; i<this->nx; i++)
	this->a[j * this->nx+i] = aset[(j * this->nx+i) % naset].allele();
    }
  }
  if(this->ny > oldy){		// change in height is always new bits
    for(unsigned int i=this->nx*oldy; i<this->nx * this->ny; i++)
      this->a[i] = aset[i % naset].allele();
  }

  return this->sz;
}


#ifdef GALIB_USE_STREAMS
template <class T> int
GA2DArrayAlleleGenome<T>::read(STD_ISTREAM& is){
  return GA2DArrayGenome<T>::read(is);
}

template <class T> int
GA2DArrayAlleleGenome<T>::write(STD_OSTREAM& os) const {
  return GA2DArrayGenome<T>::write(os);
}
#endif

template <class T> int
GA2DArrayAlleleGenome<T>::equal(const GAGenome & c) const {
  return GA2DArrayGenome<T>::equal(c);
}











/* ----------------------------------------------------------------------------
   Operator definitions
---------------------------------------------------------------------------- */
// this does not handle genomes with multiple allele sets!
template <class ARRAY_TYPE> void 
GA2DArrayAlleleGenome<ARRAY_TYPE>::UniformInitializer(GAGenome & c)
{
  GA2DArrayAlleleGenome<ARRAY_TYPE> &child=
    DYN_CAST(GA2DArrayAlleleGenome<ARRAY_TYPE> &, c);
  child.resize(GAGenome::ANY_SIZE,GAGenome::ANY_SIZE); 
  for(int i=child.width()-1; i>=0; i--)
    for(int j=child.height()-1; j>=0; j--)
      child.gene(i, j, child.alleleset().allele());
}


template <class ARRAY_TYPE> int 
GA2DArrayAlleleGenome<ARRAY_TYPE>::FlipMutator(GAGenome & c, float pmut)
{
  GA2DArrayAlleleGenome<ARRAY_TYPE> &child=
    DYN_CAST(GA2DArrayAlleleGenome<ARRAY_TYPE> &, c);
  register int n, m, i, j;
  if(pmut <= 0.0) return(0);

  float nMut = pmut * STA_CAST(float,child.size());
  if(nMut < 1.0){		// we have to do a flip test on each bit
    nMut = 0;
    for(i=child.width()-1; i>=0; i--){
      for(j=child.height()-1; j>=0; j--){
	if(GAFlipCoin(pmut)){
	  child.gene(i, j, child.alleleset().allele());
	  nMut++;
	}
      }
    }
  }
  else{				// only flip the number of bits we need to flip
    for(n=0; n<nMut; n++){
      m = GARandomInt(0, child.size()-1);
      i = m % child.width();
      j = m / child.width();
      child.gene(i, j, child.alleleset().allele());
    }
  }
  return(STA_CAST(int,nMut));
}


template <class ARRAY_TYPE> int 
GA2DArrayGenome<ARRAY_TYPE>::SwapMutator(GAGenome & c, float pmut)
{
  GA2DArrayGenome<ARRAY_TYPE> &child=DYN_CAST(GA2DArrayGenome<ARRAY_TYPE>&, c);
  register int n, i;
  if(pmut <= 0.0) return(0);

  float nMut = pmut * STA_CAST(float,child.size());
  int size = child.size()-1;
  if(nMut < 1.0){		// we have to do a flip test on each bit
    nMut = 0;
    for(i=size; i>=0; i--){
      if(GAFlipCoin(pmut)){
	child.GAArray<ARRAY_TYPE>::swap(i, GARandomInt(0, size));
	nMut++;
      }
    }
  }
  else{				// only flip the number of bits we need to flip
    for(n=0; n<nMut; n++)
      child.GAArray<ARRAY_TYPE>::swap(GARandomInt(0,size),GARandomInt(0,size));
  }
  return(STA_CAST(int,nMut));
}


template <class ARRAY_TYPE> float
GA2DArrayGenome<ARRAY_TYPE>::
ElementComparator(const GAGenome& a, const GAGenome& b)
{
  const GA2DArrayGenome<ARRAY_TYPE>& sis=
    DYN_CAST(const GA2DArrayGenome<ARRAY_TYPE>&, a);
  const GA2DArrayGenome<ARRAY_TYPE>& bro=
    DYN_CAST(const GA2DArrayGenome<ARRAY_TYPE>&, b);
  if(sis.size() != bro.size()) return -1;
  if(sis.size() == 0) return 0;
  float count = 0.0;
  for(int i=sis.width()-1; i>=0; i--)
    for(int j=sis.height()-1; j>=0; j--)
      count += ((sis.gene(i,j) == bro.gene(i,j)) ? 0 : 1);
  return count/sis.size();
}











template <class T> int
GA2DArrayGenome<T>::
UniformCrossover(const GAGenome& p1, const GAGenome& p2,
		 GAGenome* c1, GAGenome* c2){
  const GA2DArrayGenome<T> &mom=DYN_CAST(const GA2DArrayGenome<T> &, p1);
  const GA2DArrayGenome<T> &dad=DYN_CAST(const GA2DArrayGenome<T> &, p2);

  int nc=0;
  int i,j;

  if(c1 && c2){
    GA2DArrayGenome<T> &sis=DYN_CAST(GA2DArrayGenome<T> &, *c1);
    GA2DArrayGenome<T> &bro=DYN_CAST(GA2DArrayGenome<T> &, *c2);
    
    if(sis.width() == bro.width() && sis.height() == bro.height() &&
       mom.width() == dad.width() && mom.height() == dad.height() &&
       sis.width() == mom.width() && sis.height() == mom.height()){
      for(i=sis.width()-1; i>=0; i--){
	for(j=sis.height()-1; j>=0; j--){
	  if(GARandomBit()){
	    sis.gene(i,j, mom.gene(i,j));
	    bro.gene(i,j, dad.gene(i,j));
	  }
	  else{
	    sis.gene(i,j, dad.gene(i,j));
	    bro.gene(i,j, mom.gene(i,j));
	  }
	}
      }
    }
    else{
      GAMask mask;
      int startx, starty;
      int maxx = (sis.width() > bro.width()) ? sis.width() : bro.width();
      int minx = (mom.width() < dad.width()) ? mom.width() : dad.width();
      int maxy = (sis.height() > bro.height()) ? sis.height() : bro.height();
      int miny = (mom.height() < dad.height()) ? mom.height() : dad.height();
      mask.size(maxx*maxy);
      for(i=0; i<maxx; i++)
	for(j=0; j<maxy; j++)
	  mask[i*maxy+j] = GARandomBit();
      startx = (sis.width() < minx) ? sis.width() : minx;
      starty = (sis.height() < miny) ? sis.height() : miny;
      for(i=startx-1; i>=0; i--)
	for(j=starty-1; j>=0; j--)
	  sis.gene(i,j, (mask[i*starty+j] ? mom.gene(i,j) : dad.gene(i,j)));
      startx = (bro.width() < minx) ? bro.width() : minx;
      starty = (bro.height() < miny) ? bro.height() : miny;
      for(i=startx-1; i>=0; i--)
	for(j=starty-1; j>=0; j--)
	  bro.gene(i,j, (mask[i*starty+j] ? dad.gene(i,j) : mom.gene(i,j)));
    }

    nc = 2;
  }
  else if(c1){
    GA2DArrayGenome<T> &sis=DYN_CAST(GA2DArrayGenome<T> &, *c1);

    if(mom.width() == dad.width() && mom.height() == dad.height() &&
       sis.width() == mom.width() && sis.height() == mom.height()){
      for(i=sis.width()-1; i>=0; i--)
	for(j=sis.height()-1; j>=0; j--)
	  sis.gene(i,j, (GARandomBit() ? mom.gene(i,j) : dad.gene(i,j)));
    }
    else{
      int minx = (mom.width() < dad.width()) ? mom.width() : dad.width();
      int miny = (mom.height() < dad.height()) ? mom.height() : dad.height();
      minx = (sis.width() < minx) ? sis.width() : minx;
      miny = (sis.height() < miny) ? sis.height() : miny;
      for(i=minx-1; i>=0; i--)
	for(j=miny-1; j>=0; j--)
	  sis.gene(i,j, (GARandomBit() ? mom.gene(i,j) : dad.gene(i,j)));
    }

    nc = 1;
  }

  return nc;
}




// This crossover does clipping (no padding) for resizables.  Notice that this
// means that any resizable children of two parents will have identical 
// dimensions no matter what.
template <class T> int
GA2DArrayGenome<T>::
OnePointCrossover(const GAGenome& p1, const GAGenome& p2,
		  GAGenome* c1, GAGenome* c2){
  const GA2DArrayGenome<T> &mom=DYN_CAST(const GA2DArrayGenome<T> &, p1);
  const GA2DArrayGenome<T> &dad=DYN_CAST(const GA2DArrayGenome<T> &, p2);

  int nc=0;
  unsigned int momsitex, momlenx, momsitey, momleny;
  unsigned int dadsitex, dadlenx, dadsitey, dadleny;
  unsigned int sitex, lenx, sitey, leny;

  if(c1 && c2){
    GA2DArrayGenome<T> &sis=DYN_CAST(GA2DArrayGenome<T> &, *c1);
    GA2DArrayGenome<T> &bro=DYN_CAST(GA2DArrayGenome<T> &, *c2);

    if(sis.resizeBehaviour(GAGenome::WIDTH) == GAGenome::FIXED_SIZE &&
       bro.resizeBehaviour(GAGenome::WIDTH) == GAGenome::FIXED_SIZE){
      if(mom.width() != dad.width() || 
	 sis.width() != bro.width() || 
	 sis.width() != mom.width()){
	GAErr(GA_LOC, mom.className(), "one-point cross", gaErrSameLengthReqd);
	return nc;
      }
      sitex = momsitex = dadsitex = GARandomInt(0, mom.width());
      lenx = momlenx = dadlenx = mom.width() - momsitex;
    }
    else if(sis.resizeBehaviour(GAGenome::WIDTH) == GAGenome::FIXED_SIZE ||
	    bro.resizeBehaviour(GAGenome::WIDTH) == GAGenome::FIXED_SIZE){
      GAErr(GA_LOC, mom.className(), "one-point cross", gaErrSameBehavReqd);
      return nc;
    }
    else{
      momsitex = GARandomInt(0, mom.width());
      dadsitex = GARandomInt(0, dad.width());
      momlenx = mom.width() - momsitex;
      dadlenx = dad.width() - dadsitex;
      sitex = GAMin(momsitex, dadsitex);
      lenx = GAMin(momlenx, dadlenx);
    }

    if(sis.resizeBehaviour(GAGenome::HEIGHT) == GAGenome::FIXED_SIZE &&
       bro.resizeBehaviour(GAGenome::HEIGHT) == GAGenome::FIXED_SIZE){
      if(mom.height() != dad.height() || 
	 sis.height() != bro.height() || 
	 sis.height() != mom.height()){
	GAErr(GA_LOC, mom.className(), "one-point cross", gaErrSameLengthReqd);
	return nc;
      }
      sitey = momsitey = dadsitey = GARandomInt(0, mom.height());
      leny = momleny = dadleny = mom.height() - momsitey;
    }
    else if(sis.resizeBehaviour(GAGenome::HEIGHT) == GAGenome::FIXED_SIZE ||
	    bro.resizeBehaviour(GAGenome::HEIGHT) == GAGenome::FIXED_SIZE){
      GAErr(GA_LOC, mom.className(), "one-point cross", gaErrSameBehavReqd);
      return nc;
    }
    else{
      momsitey = GARandomInt(0, mom.height());
      dadsitey = GARandomInt(0, dad.height());
      momleny = mom.height() - momsitey;
      dadleny = dad.height() - dadsitey;
      sitey = GAMin(momsitey, dadsitey);
      leny = GAMin(momleny, dadleny);
    }

    sis.resize(sitex+lenx, sitey+leny);
    bro.resize(sitex+lenx, sitey+leny);
    
    sis.copy(mom, 0, 0, momsitex-sitex, momsitey-sitey, sitex, sitey);
    sis.copy(dad, sitex, 0, dadsitex, dadsitey-sitey, lenx, sitey);
    sis.copy(dad, 0, sitey, dadsitex-sitex, dadsitey, sitex, leny);
    sis.copy(mom, sitex, sitey, momsitex, momsitey, lenx, leny);
    
    bro.copy(dad, 0, 0, dadsitex-sitex, dadsitey-sitey, sitex, sitey);
    bro.copy(mom, sitex, 0, momsitex, momsitey-sitey, lenx, sitey);
    bro.copy(mom, 0, sitey, momsitex-sitex, momsitey, sitex, leny);
    bro.copy(dad, sitex, sitey, dadsitex, dadsitey, lenx, leny);

    nc = 2;
  }
  else if(c1){
    GA2DArrayGenome<T> &sis=DYN_CAST(GA2DArrayGenome<T> &, *c1);
    
    if(sis.resizeBehaviour(GAGenome::WIDTH) == GAGenome::FIXED_SIZE){
      if(mom.width() != dad.width() || sis.width() != mom.width()){
	GAErr(GA_LOC, mom.className(), "one-point cross", gaErrSameLengthReqd);
	return nc;
      }
      sitex = momsitex = dadsitex = GARandomInt(0, mom.width());
      lenx = momlenx = dadlenx = mom.width() - momsitex;
    }
    else{
      momsitex = GARandomInt(0, mom.width());
      dadsitex = GARandomInt(0, dad.width());
      momlenx = mom.width() - momsitex;
      dadlenx = dad.width() - dadsitex;
      sitex = GAMin(momsitex, dadsitex);
      lenx = GAMin(momlenx, dadlenx);
    }
    
    if(sis.resizeBehaviour(GAGenome::HEIGHT) == GAGenome::FIXED_SIZE){
      if(mom.height() != dad.height() || sis.height() != mom.height()){
	GAErr(GA_LOC, mom.className(), "one-point cross", gaErrSameLengthReqd);
	return nc;
      }
      sitey = momsitey = dadsitey = GARandomInt(0, mom.height());
      leny = momleny = dadleny = mom.height() - momsitey;
    }
    else{
      momsitey = GARandomInt(0, mom.height());
      dadsitey = GARandomInt(0, dad.height());
      momleny = mom.height() - momsitey;
      dadleny = dad.height() - dadsitey;
      sitey = GAMin(momsitey, dadsitey);
      leny = GAMin(momleny, dadleny);
    }
    
    sis.resize(sitex+lenx, sitey+leny);
    
    if(GARandomBit()){
      sis.copy(mom, 0, 0, momsitex-sitex, momsitey-sitey, sitex, sitey);
      sis.copy(dad, sitex, 0, dadsitex, dadsitey-sitey, lenx, sitey);
      sis.copy(dad, 0, sitey, dadsitex-sitex, dadsitey, sitex, leny);
      sis.copy(mom, sitex, sitey, momsitex, momsitey, lenx, leny);
    }
    else{
      sis.copy(dad, 0, 0, dadsitex-sitex, dadsitey-sitey, sitex, sitey);
      sis.copy(mom, sitex, 0, momsitex, momsitey-sitey, lenx, sitey);
      sis.copy(mom, 0, sitey, momsitex-sitex, momsitey, sitex, leny);
      sis.copy(dad, sitex, sitey, dadsitex, dadsitey, lenx, leny);
    }

    nc = 1;
  }

  return nc;
}






template <class T> int
GA2DArrayGenome<T>::
EvenOddCrossover(const GAGenome& p1, const GAGenome& p2,
		 GAGenome* c1, GAGenome* c2){
  const GA2DArrayGenome<T> &mom=DYN_CAST(const GA2DArrayGenome<T> &, p1);
  const GA2DArrayGenome<T> &dad=DYN_CAST(const GA2DArrayGenome<T> &, p2);

  int nc=0;
  int i,j;

  if(c1 && c2){
    GA2DArrayGenome<T> &sis=DYN_CAST(GA2DArrayGenome<T> &, *c1);
    GA2DArrayGenome<T> &bro=DYN_CAST(GA2DArrayGenome<T> &, *c2);

    if(sis.width() == bro.width() && sis.height() == bro.height() &&
       mom.width() == dad.width() && mom.height() == dad.height() &&
       sis.width() == mom.width() && sis.height() == mom.height()){
      int count=0;
      for(i=sis.width()-1; i>=0; i--){
	for(j=sis.height()-1; j>=0; j--){
	  sis.gene(i,j, ((count%2 == 0) ? mom.gene(i,j) : dad.gene(i,j)));
	  bro.gene(i,j, ((count%2 == 0) ? dad.gene(i,j) : mom.gene(i,j)));
	  count++;
	}
      }
    }
    else{
      int startx, starty;
      int minx = (mom.width() < dad.width()) ? mom.width() : dad.width();
      int miny = (mom.height() < dad.height()) ? mom.height() : dad.height();
      startx = (sis.width() < minx) ? sis.width() : minx;
      starty = (sis.height() < miny) ? sis.height() : miny;
      for(i=startx-1; i>=0; i--)
	for(j=starty-1; j>=0; j--)
	  sis.gene(i,j, (((i*starty+j)%2 == 0) ? mom.gene(i,j):dad.gene(i,j)));
      startx = (bro.width() < minx) ? bro.width() : minx;
      starty = (bro.height() < miny) ? bro.height() : miny;
      for(i=startx-1; i>=0; i--)
	for(j=starty-1; j>=0; j--)
	  bro.gene(i,j, (((i*starty+j)%2 == 0) ? dad.gene(i,j):mom.gene(i,j)));
    }

    nc = 2;
  }
  else if(c1){
    GA2DArrayGenome<T> &sis=DYN_CAST(GA2DArrayGenome<T> &, *c1);
    
    if(mom.width() == dad.width() && mom.height() == dad.height() &&
       sis.width() == mom.width() && sis.height() == mom.height()){
      int count=0;
      for(i=sis.width()-1; i>=0; i--){
	for(j=sis.height()-1; j>=0; j--){
	  sis.gene(i,j, ((count%2 == 0) ? mom.gene(i,j) : dad.gene(i,j)));
	  count++;
	}
      }
    }
    else{
      int minx = (mom.width() < dad.width()) ? mom.width() : dad.width();
      int miny = (mom.height() < dad.height()) ? mom.height() : dad.height();
      minx = (sis.width() < minx) ? sis.width() : minx;
      miny = (sis.height() < miny) ? sis.height() : miny;
      for(i=minx-1; i>=0; i--)
	for(j=miny-1; j>=0; j--)
	  sis.gene(i,j, (((i*miny+j)%2 == 0) ? mom.gene(i,j) : dad.gene(i,j)));
    }

    nc = 1;
  }

  return nc;
}

#endif
