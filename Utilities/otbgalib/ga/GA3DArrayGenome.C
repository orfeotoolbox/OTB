// $Header$
/* ----------------------------------------------------------------------------
  array3.C
  mbwall 25feb95
  Copyright (c) 1995 Massachusetts Institute of Technology
                     all rights reserved

 DESCRIPTION:
  Source file for the 3D array genome.
---------------------------------------------------------------------------- */
#ifndef _ga_array3_C_
#define _ga_array3_C_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ga/garandom.h>
#include <ga/GA3DArrayGenome.h>
#include <ga/GAMask.h>


/* ----------------------------------------------------------------------------
3DArrayGenome
---------------------------------------------------------------------------- */
template <class T> const char *
GA3DArrayGenome<T>::className() const {return "GA3DArrayGenome";}
template <class T> int
GA3DArrayGenome<T>::classID() const {return GAID::ArrayGenome3D;}

template <class T> 
GA3DArrayGenome<T>::
GA3DArrayGenome(unsigned int w, unsigned int h, unsigned int d,
		GAGenome::Evaluator f,
		void * u) :
GAArray<T>(w*h*d),
GAGenome(DEFAULT_3DARRAY_INITIALIZER, 
	 DEFAULT_3DARRAY_MUTATOR,
	 DEFAULT_3DARRAY_COMPARATOR)
{
  evaluator(f);
  userData(u);
  crossover(DEFAULT_3DARRAY_CROSSOVER);
  nx=minX=maxX=w; ny=minY=maxY=h; nz=minZ=maxZ=d;
}


template <class T> 
GA3DArrayGenome<T>::
GA3DArrayGenome(const GA3DArrayGenome<T> & orig) :
GAArray<T>(orig.sz), GAGenome(){
  GA3DArrayGenome<T>::copy(orig);
}


template <class T>
GA3DArrayGenome<T>::~GA3DArrayGenome(){ }


template <class T> void
GA3DArrayGenome<T>::copy(const GAGenome & orig){
  if(&orig == this) return;
  const GA3DArrayGenome<T>* c = DYN_CAST(const GA3DArrayGenome<T>*, &orig);
  if(c) {
    GAGenome::copy(*c);
    GAArray<T>::copy(*c);
    nx = c->nx; ny = c->ny; nz = c->nz;
    minX = c->minX; minY = c->minY; minZ = c->minZ;
    maxX = c->maxX; maxY = c->maxY; maxZ = c->maxZ;
  }
}


template <class T> GAGenome *
GA3DArrayGenome<T>::clone(GAGenome::CloneMethod flag) const {
  GA3DArrayGenome<T> *cpy = new GA3DArrayGenome<T>(nx,ny,nz);
  if(flag == CONTENTS){
    cpy->copy(*this);
  }
  else{
    cpy->GAGenome::copy(*this);
    cpy->minX = minX; cpy->minY = minY; cpy->minZ = minZ; 
    cpy->maxX = maxX; cpy->maxY = maxY; cpy->maxZ = maxZ;
  }
  return cpy;
}


template <class T> int
GA3DArrayGenome<T>::resize(int w, int h, int d)
{
  if(w == STA_CAST(int,nx) && h == STA_CAST(int,ny) && d == STA_CAST(int,nz))
    return this->sz;

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

  if(d == GAGenome::ANY_SIZE)
    d = GARandomInt(minZ, maxZ);
  else if(d < 0)
    d = nz;		// do nothing
  else if(minZ == maxZ)
    minZ=maxZ = d;
  else{
    if(d < STA_CAST(int,minZ)) d=minZ;
    if(d > STA_CAST(int,maxZ)) d=maxZ;
  }

  if(w < STA_CAST(int,nx) && h < STA_CAST(int,ny)){
    int z=GAMin(STA_CAST(int,nz),d);
    for(int k=0; k<z; k++)
      for(int j=0; j<h; j++)
	GAArray<T>::move(k*h*w+j*w,k*ny*nx+j*nx,w);
  }
  else if(w < STA_CAST(int,nx)){
    int z=GAMin(STA_CAST(int,nz),d);
    for(int k=0; k<z; k++)
      for(int j=0; j<STA_CAST(int,ny); j++)
	GAArray<T>::move(k*ny*w+j*w,k*ny*nx+j*nx,w);
  }
  else if(h < STA_CAST(int,ny)){
    int z=GAMin(STA_CAST(int,nz),d);
    for(int k=0; k<z; k++)
      for(int j=0; j<h; j++)
	GAArray<T>::move(k*h*nx+j*nx,k*ny*nx+j*nx,nx);
  }

  GAArray<T>::size(w*h*d);

  if(w > STA_CAST(int,nx) && h > STA_CAST(int,ny)){ 
    int z=GAMin(STA_CAST(int,nz),d);
    for(int k=z-1; k>=0; k--)
      for(int j=ny-1; j>=0; j--)
	GAArray<T>::move(k*h*w+j*w,k*ny*nx+j*nx,nx);
  }
  else if(w > STA_CAST(int,nx)){
    int z=GAMin(STA_CAST(int,nz),d);
    for(int k=z-1; k>=0; k--)
      for(int j=h-1; j>=0; j--)
	GAArray<T>::move(k*h*w+j*w,k*h*nx+j*nx,nx);
  }
  else if(h > STA_CAST(int,ny)){
    int z=GAMin(STA_CAST(int,nz),d);
    for(int k=z-1; k>=0; k--)
      for(int j=ny-1; j>=0; j--)
	GAArray<T>::move(k*h*w+j*w,k*ny*w+j*w,w);
  }

  nx = w; ny = h; nz = d;
  _evaluated = gaFalse;
  return this->sz;
}


#ifdef GALIB_USE_STREAMS
template <class T> int
GA3DArrayGenome<T>::read(STD_ISTREAM &) {
  GAErr(GA_LOC, className(), "read", gaErrOpUndef);
  return 1;
}


template <class T> int
GA3DArrayGenome<T>::write(STD_OSTREAM & os) const 
{
  for(unsigned int k=0; k<nz; k++){
    for(unsigned int j=0; j<ny; j++){
      for(unsigned int i=0; i<nx; i++){
	os << gene(i,j,k) << " ";
      }
      os << "\n";
    }
    os << "\n";
  }
  return 0;
}
#endif


template <class T> int
GA3DArrayGenome<T>::resizeBehaviour(GAGenome::Dimension which) const {
  int val = 0;
  if(which == WIDTH) {
    if(maxX == minX) val = FIXED_SIZE;
    else val = maxX;
  }
  else if(which == HEIGHT) {
    if(maxY == minY) val = FIXED_SIZE;
    else val = maxY;
  }
  else if(which == DEPTH) {
    if(maxZ == minZ) val = FIXED_SIZE;
    else val = maxZ;
  }
  return val;
}


template <class T> int
GA3DArrayGenome<T>::
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
    if(nx > upper) GA3DArrayGenome<T>::resize(upper,ny,nz);
    if(nx < lower) GA3DArrayGenome<T>::resize(lower,ny,nz);
    break;

  case HEIGHT:
    minY = lower; maxY = upper;
    if(ny > upper) GA3DArrayGenome<T>::resize(nx,upper,nz);
    if(ny < lower) GA3DArrayGenome<T>::resize(nx,lower,nz);
    break;

  case DEPTH:
    minZ = lower; maxZ = upper;
    if(nz > upper) GA3DArrayGenome<T>::resize(nx,ny,upper);
    if(nz < lower) GA3DArrayGenome<T>::resize(nx,ny,lower);
    break;

  default:
    break;
  }

  return resizeBehaviour(which);
}


template <class T> void
GA3DArrayGenome<T>::
copy(const GA3DArrayGenome<T> & orig,
     unsigned int r, unsigned int s, unsigned int t,
     unsigned int x, unsigned int y, unsigned int z,
     unsigned int w, unsigned int h, unsigned int d)
{
  if(w == 0 || x >= orig.nx || r >= nx ||
     h == 0 || y >= orig.ny || s >= ny ||
     d == 0 || z >= orig.nz || t >= nz) return;
  if(x + w > orig.nx) w = orig.nx - x;
  if(y + h > orig.ny) h = orig.ny - y;
  if(z + d > orig.nz) d = orig.nz - z;
  if(r + w > nx) w = nx - r;
  if(s + h > ny) h = ny - s;
  if(t + d > nz) d = nz - t;

  for(unsigned int k=0; k<d; k++)
    for(unsigned int j=0; j<h; j++)
      GAArray<T>::copy(orig,
		       (t+k)*ny*nx + (s+j)*nx + r,
		       (z+k)*orig.ny*orig.nx + (y+j)*orig.nx + x, w);

  _evaluated = gaFalse;
}


template <class T> int 
GA3DArrayGenome<T>::equal(const GAGenome & c) const
{
  if(this == &c) return 1;
  GA3DArrayGenome<T> & b = (GA3DArrayGenome<T> &)c;
  if(nx != b.nx || ny != b.ny || nz != b.nz) return 0;
  int val=0;
  for(unsigned int k=0; k<nz && val==0; k++)
    for(unsigned int j=0; j<ny && val==0; j++)
      val = GAArray<T>::equal(b,k*ny*nx,k*ny*nx,nx) ? 0 : 1;
  return(val ? 0 : 1);
}














/* ----------------------------------------------------------------------------
3DArrayAlleleGenome
---------------------------------------------------------------------------- */
template <class T> const char *
GA3DArrayAlleleGenome<T>::className() const {return "GA3DArrayAlleleGenome";}
template <class T> int
GA3DArrayAlleleGenome<T>::classID() const {return GAID::ArrayAlleleGenome3D;}

template <class T> 
GA3DArrayAlleleGenome<T>::
GA3DArrayAlleleGenome(unsigned int w, unsigned int h, unsigned int d,
		      const GAAlleleSet<T> & s,
		      GAGenome::Evaluator f, void * u) :
GA3DArrayGenome<T>(w,h,d,f,u) {
  naset = 1;
  aset = new GAAlleleSet<T>[1];
  aset[0] = s;

  initializer(GA3DArrayAlleleGenome<T>::DEFAULT_3DARRAY_ALLELE_INITIALIZER);
  mutator(GA3DArrayAlleleGenome<T>::DEFAULT_3DARRAY_ALLELE_MUTATOR);
  comparator(GA3DArrayAlleleGenome<T>::DEFAULT_3DARRAY_ALLELE_COMPARATOR);
  crossover(GA3DArrayAlleleGenome<T>::DEFAULT_3DARRAY_ALLELE_CROSSOVER);
}

template <class T> 
GA3DArrayAlleleGenome<T>::
GA3DArrayAlleleGenome(unsigned int w, unsigned int h, unsigned int d,
		      const GAAlleleSetArray<T> & sa,
		      GAGenome::Evaluator f, void * u) :
GA3DArrayGenome<T>(w,h,d, f, u) {
  naset = sa.size();
  aset = new GAAlleleSet<T>[naset];
  for(int i=0; i<naset; i++)
    aset[i] = sa.set(i);

  initializer(GA3DArrayAlleleGenome<T>::DEFAULT_3DARRAY_ALLELE_INITIALIZER);
  mutator(GA3DArrayAlleleGenome<T>::DEFAULT_3DARRAY_ALLELE_MUTATOR);
  comparator(GA3DArrayAlleleGenome<T>::DEFAULT_3DARRAY_ALLELE_COMPARATOR);
  crossover(GA3DArrayAlleleGenome<T>::DEFAULT_3DARRAY_ALLELE_CROSSOVER);
}


template <class T> 
GA3DArrayAlleleGenome<T>::
GA3DArrayAlleleGenome(const GA3DArrayAlleleGenome<T> & orig) : 
GA3DArrayGenome<T>(orig.nx, orig.ny, orig.nz) {
  naset = 0;
  aset = (GAAlleleSet<T>*)0;
  GA3DArrayAlleleGenome<T>::copy(orig);
}


template <class T>
GA3DArrayAlleleGenome<T>::~GA3DArrayAlleleGenome(){
  delete [] aset;
}


template <class T> GAGenome *
GA3DArrayAlleleGenome<T>::clone(GAGenome::CloneMethod) const {
  return new GA3DArrayAlleleGenome<T>(*this);
}


template <class T> void 
GA3DArrayAlleleGenome<T>::copy(const GAGenome& orig){
  if(&orig == this) return;
  const GA3DArrayAlleleGenome<T>* c = 
    DYN_CAST(const GA3DArrayAlleleGenome<T>*, &orig);
  if(c) {
    GA3DArrayGenome<T>::copy(*c);
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
GA3DArrayAlleleGenome<T>::resize(int w, int h, int d){
  unsigned int oldx = this->nx;
  unsigned int oldy = this->ny;
  unsigned int oldz = this->nz;
  GA3DArrayGenome<T>::resize(w,h,d);

// set new elements to proper randomly selected values

  if(this->nx > oldx && this->ny > oldy){
    int z=GAMin(oldz,this->nz);
    for(int k=z-1; k>=0; k--){
      int j;
      for(j=oldy-1; j>=0; j--)
	for(unsigned int i=oldx; i<this->nx; i++)
	  this->a[k * this->ny * this->nx + j * this->nx + i] =
	    aset[(k*this->ny*this->nx+j*this->nx+i) % naset].allele();
      for(j=oldy; j<STA_CAST(int,this->ny); j++)
	for(unsigned int i=0; i<this->nx; i++)
	  this->a[k * this->ny * this->nx + j * this->nx + i] =
	    aset[(k * this->ny * this->nx + j * this->nx + i) % naset].allele();
    }
  }
  else if(this->nx > oldx){
    int z=GAMin(oldz,this->nz);
    for(int k=z-1; k>=0; k--)
      for(int j=this->ny-1; j>=0; j--)
	for(unsigned int i=oldx; i<this->nx; i++)
	  this->a[k*this->ny*this->nx+j*this->nx+i] = 
	    aset[(k*this->ny*this->nx+j*this->nx+i) % naset].allele();
  }
  else if(this->ny > oldy){
    int z=GAMin(oldz,this->nz);
    for(int k=z-1; k>=0; k--)
      for(unsigned int j=oldy; j<this->ny; j++)
	for(unsigned int i=0; i<this->nx; i++)
	  this->a[k*this->ny*this->nx+j*this->nx+i] =
	    aset[(k*this->ny*this->nx+j*this->nx+i) % naset].allele();
  }
  if(this->nz > oldz){		// change in depth is always new elements
    for(unsigned int i=this->nx*this->ny*oldz;
	i<this->nx*this->ny*this->nz; i++)
      this->a[i] = aset[i % naset].allele();
  }

  return this->sz;
}


#ifdef GALIB_USE_STREAMS
template <class T> int
GA3DArrayAlleleGenome<T>::read(STD_ISTREAM& is){
  return GA3DArrayGenome<T>::read(is);
}

template <class T> int
GA3DArrayAlleleGenome<T>::write(STD_OSTREAM& os) const {
  return GA3DArrayGenome<T>::write(os);
}
#endif

template <class T> int
GA3DArrayAlleleGenome<T>::equal(const GAGenome & c) const {
  return GA3DArrayGenome<T>::equal(c);
}








/* ----------------------------------------------------------------------------
   Operator definitions
---------------------------------------------------------------------------- */
// this does not handle genomes with multiple allele sets!
template <class ARRAY_TYPE> void 
GA3DArrayAlleleGenome<ARRAY_TYPE>::UniformInitializer(GAGenome & c)
{
  GA3DArrayAlleleGenome<ARRAY_TYPE> &child=
    DYN_CAST(GA3DArrayAlleleGenome<ARRAY_TYPE> &, c);
  child.resize(GAGenome::ANY_SIZE,GAGenome::ANY_SIZE,GAGenome::ANY_SIZE);
  for(int i=child.width()-1; i>=0; i--)
    for(int j=child.height()-1; j>=0; j--)
      for(int k=child.depth()-1; k>=0; k--)
	child.gene(i, j, k, child.alleleset().allele());
}


template <class ARRAY_TYPE> int 
GA3DArrayAlleleGenome<ARRAY_TYPE>::FlipMutator(GAGenome & c, float pmut)
{
  GA3DArrayAlleleGenome<ARRAY_TYPE> &child=
    DYN_CAST(GA3DArrayAlleleGenome<ARRAY_TYPE> &, c);
  register int n, m, d, i, j, k;
  if(pmut <= 0.0) return(0);

  float nMut = pmut * STA_CAST(float,child.size());
  if(nMut < 1.0){		// we have to do a flip test on each bit
    nMut = 0;
    for(i=child.width()-1; i>=0; i--){
      for(j=child.height()-1; j>=0; j--){
        for(k=child.depth()-1; k>=0; k--){
	  if(GAFlipCoin(pmut)){
	    child.gene(i, j, k, child.alleleset().allele());
	    nMut++;
	  }
	}
      }
    }
  }
  else{				// only flip the number of bits we need to flip
    for(n=0; n<nMut; n++){
      m = GARandomInt(0, child.size()-1);
      d = child.height() * child.depth();
      i = m / d;
      j = (m % d) / child.depth();
      k = (m % d) % child.depth();
      child.gene(i, j, k, child.alleleset().allele());
    }
  }
  return(STA_CAST(int,nMut));
}


template <class ARRAY_TYPE> int 
GA3DArrayGenome<ARRAY_TYPE>::SwapMutator(GAGenome & c, float pmut)
{
  GA3DArrayGenome<ARRAY_TYPE> &child=DYN_CAST(GA3DArrayGenome<ARRAY_TYPE>&, c);
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
      child.GAArray<ARRAY_TYPE>::swap(GARandomInt(0, size),
				      GARandomInt(0, size));
  }
  return(STA_CAST(int,nMut));
}


template <class ARRAY_TYPE> float
GA3DArrayGenome<ARRAY_TYPE>::
ElementComparator(const GAGenome& a, const GAGenome& b)
{
  const GA3DArrayGenome<ARRAY_TYPE>& sis=
    DYN_CAST(const GA3DArrayGenome<ARRAY_TYPE>&, a);
  const GA3DArrayGenome<ARRAY_TYPE>& bro=
    DYN_CAST(const GA3DArrayGenome<ARRAY_TYPE>&, b);
  if(sis.size() != bro.size()) return -1;
  if(sis.size() == 0) return 0;
  float count = 0.0;
  for(int i=sis.width()-1; i>=0; i--)
    for(int j=sis.height()-1; j>=0; j--)
      for(int k=sis.depth()-1; k>=0; k--)
	count += ((sis.gene(i,j,k) == bro.gene(i,j,k)) ? 0 : 1);
  return count/sis.size();
}












// Make sure our bitmask is big enough, generate a mask, then use it to 
// extract the information from each parent to stuff the two children.
// We don't deallocate any space for the masks under the assumption that we'll
// have to use them again in the future.
//   For now we'll implement this only for fixed length genomes.  If you use
// this crossover method on genomes of different sizes it might break!
template <class T> int
GA3DArrayGenome<T>::
UniformCrossover(const GAGenome& p1, const GAGenome& p2,
		 GAGenome* c1, GAGenome* c2){
  const GA3DArrayGenome<T> &mom=DYN_CAST(const GA3DArrayGenome<T> &, p1);
  const GA3DArrayGenome<T> &dad=DYN_CAST(const GA3DArrayGenome<T> &, p2);

  int nc=0;
  int i,j,k;

  if(c1 && c2){
    GA3DArrayGenome<T> &sis=DYN_CAST(GA3DArrayGenome<T> &, *c1);
    GA3DArrayGenome<T> &bro=DYN_CAST(GA3DArrayGenome<T> &, *c2);

    if(sis.width() == bro.width() && sis.height() == bro.height() &&
       sis.depth() == bro.depth() &&
       mom.width() == dad.width() && mom.height() == dad.height() &&
       mom.depth() == dad.depth() &&
       sis.width() == mom.width() && sis.height() == mom.height() &&
       sis.depth() == mom.depth()){
      for(i=sis.width()-1; i>=0; i--){
	for(j=sis.height()-1; j>=0; j--){
	  for(k=sis.depth()-1; k>=0; k--){
	    if(GARandomBit()){
	      sis.gene(i,j,k, mom.gene(i,j,k));
	      bro.gene(i,j,k, dad.gene(i,j,k));
	    }
	    else{
	      sis.gene(i,j,k, dad.gene(i,j,k));
	      bro.gene(i,j,k, mom.gene(i,j,k));
	    }
	  }
	}
      }
    }
    else{
      GAMask mask;
      int startx, starty, startz;
      int maxx = (sis.width() > bro.width()) ? sis.width() : bro.width();
      int minx = (mom.width() < dad.width()) ? mom.width() : dad.width();
      int maxy = (sis.height() > bro.height()) ? sis.height() : bro.height();
      int miny = (mom.height() < dad.height()) ? mom.height() : dad.height();
      int maxz = (sis.depth() > bro.depth()) ? sis.depth() : bro.depth();
      int minz = (mom.depth() < dad.depth()) ? mom.depth() : dad.depth();
      mask.size(maxx*maxy*maxz);
      for(i=0; i<maxx; i++)
	for(j=0; j<maxy; j++)
	  for(k=0; k<maxz; k++)
	    mask[i*maxy*maxz+j*maxz+k] = GARandomBit();
      startx = (sis.width() < minx) ? sis.width() : minx;
      starty = (sis.height() < miny) ? sis.height() : miny;
      startz = (sis.depth() < minz) ? sis.depth() : minz;
      for(i=startx-1; i>=0; i--)
	for(j=starty-1; j>=0; j--)
	  for(k=startz-1; k>=0; k--)
	    sis.gene(i,j,k, (mask[i*starty*startz+j*startz+k] ?
			     mom.gene(i,j,k) : dad.gene(i,j,k)));
      startx = (bro.width() < minx) ? bro.width() : minx;
      starty = (bro.height() < miny) ? bro.height() : miny;
      startz = (bro.depth() < minz) ? bro.depth() : minz;
      for(i=startx-1; i>=0; i--)
	for(j=starty-1; j>=0; j--)
	  for(k=startz-1; k>=0; k--)
	    bro.gene(i,j,k, (mask[i*starty*startz+j*startz+k] ?
			     dad.gene(i,j,k) : mom.gene(i,j,k)));
    }

    nc = 2;
  }
  else if(c1){
    GA3DArrayGenome<T> &sis=DYN_CAST(GA3DArrayGenome<T> &, *c1);

    if(mom.width() == dad.width() && mom.height() == dad.height() &&
       mom.depth() == dad.depth() &&
       sis.width() == mom.width() && sis.height() == mom.height() &&
       sis.depth() == mom.depth()){
      for(i=sis.width()-1; i>=0; i--)
	for(j=sis.height()-1; j>=0; j--)
	  for(k=sis.depth()-1; k>=0; k--)
	    sis.gene(i,j,k, (GARandomBit() ? mom.gene(i,j,k):dad.gene(i,j,k)));
    }
    else{
      int minx = (mom.width() < dad.width()) ? mom.width() : dad.width();
      int miny = (mom.height() < dad.height()) ? mom.height() : dad.height();
      int minz = (mom.depth() < dad.depth()) ? mom.depth() : dad.depth();
      minx = (sis.width() < minx) ? sis.width() : minx;
      miny = (sis.height() < miny) ? sis.height() : miny;
      minz = (sis.depth() < minz) ? sis.depth() : minz;
      for(i=minx-1; i>=0; i--)
	for(j=miny-1; j>=0; j--)
	  for(k=minz-1; k>=0; k--)
	    sis.gene(i,j,k, (GARandomBit() ? mom.gene(i,j,k):dad.gene(i,j,k)));
    }

    nc = 1;
  }

  return nc;
}


// For even crossover we take the even bits from the mother (for the daughter)
// and the odd bits from the father.  Just the opposite for the son.
//   This is designed only for genomes that are the same length.  If the child
// is not the same length as the parent, or if the children are not the same
// size, we don't do the crossover.
//   In the interest of speed we do not do any checks for size.  Do not use 
// this crossover method when the parents and children may be different sizes.
// It might break!
template <class T> int
GA3DArrayGenome<T>::
EvenOddCrossover(const GAGenome& p1, const GAGenome& p2, 
GAGenome* c1, GAGenome* c2){
  const GA3DArrayGenome<T> &mom=DYN_CAST(const GA3DArrayGenome<T> &, p1);
  const GA3DArrayGenome<T> &dad=DYN_CAST(const GA3DArrayGenome<T> &, p2);

  int nc=0;
  int i,j,k;

  if(c1 && c2){
    GA3DArrayGenome<T> &sis=DYN_CAST(GA3DArrayGenome<T> &, *c1);
    GA3DArrayGenome<T> &bro=DYN_CAST(GA3DArrayGenome<T> &, *c2);
    
    if(sis.width() == bro.width() && sis.height() == bro.height() &&
       sis.depth() == bro.depth() &&
       mom.width() == dad.width() && mom.height() == dad.height() &&
       mom.depth() == dad.depth() &&
       sis.width() == mom.width() && sis.height() == mom.height() &&
       sis.depth() == mom.depth()){
      int count=0;
      for(i=sis.width()-1; i>=0; i--){
	for(j=sis.height()-1; j>=0; j--){
	  for(k=sis.depth()-1; k>=0; k--){
	    sis.gene(i,j,k,((count%2 == 0) ? mom.gene(i,j,k):dad.gene(i,j,k)));
	    bro.gene(i,j,k,((count%2 == 0) ? dad.gene(i,j,k):mom.gene(i,j,k)));
	    count++;
	  }
	}
      }
    }
    else{
      int startx, starty, startz;
      int maxx = (sis.width() > bro.width()) ? sis.width() : bro.width();
      int minx = (mom.width() < dad.width()) ? mom.width() : dad.width();
      int maxy = (sis.height() > bro.height()) ? sis.height() : bro.height();
      int miny = (mom.height() < dad.height()) ? mom.height() : dad.height();
      int maxz = (sis.depth() > bro.depth()) ? sis.depth() : bro.depth();
      int minz = (mom.depth() < dad.depth()) ? mom.depth() : dad.depth();
      startx = (sis.width() < minx) ? sis.width() : minx;
      starty = (sis.height() < miny) ? sis.height() : miny;
      startz = (sis.depth() < minz) ? sis.depth() : minz;
      for(i=startx-1; i>=0; i--)
	for(j=starty-1; j>=0; j--)
	  for(k=startz-1; k>=0; k--)
	    sis.gene(i,j,k, (((i*starty*startz+j*startz+k)%2 == 0) ?
			     mom.gene(i,j,k) : dad.gene(i,j,k)));
      startx = (bro.width() < minx) ? bro.width() : minx;
      starty = (bro.height() < miny) ? bro.height() : miny;
      startz = (bro.depth() < minz) ? bro.depth() : minz;
      for(i=startx-1; i>=0; i--)
	for(j=starty-1; j>=0; j--)
	  for(k=startz-1; k>=0; k--)
	    bro.gene(i,j,k, (((i*starty*startz+j*startz+k)%2 == 0) ?
			     dad.gene(i,j,k) : mom.gene(i,j,k)));
    }

    nc = 2;
  }
  else if(c1){
    GA3DArrayGenome<T> &sis=DYN_CAST(GA3DArrayGenome<T> &, *c1);

    if(mom.width() == dad.width() && mom.height() == dad.height() &&
       mom.depth() == dad.depth() &&
       sis.width() == mom.width() && sis.height() == mom.height() &&
       sis.depth() == mom.depth()){
      int count=0;
      for(i=sis.width()-1; i>=0; i--){
	for(j=sis.height()-1; j>=0; j--){
	  for(k=sis.depth()-1; k>=0; k--){
	    sis.gene(i,j,k,((count%2 == 0) ? mom.gene(i,j,k):dad.gene(i,j,k)));
	    count++;
	  }
	}
      }
    }
    else{
      int minx = (mom.width() < dad.width()) ? mom.width() : dad.width();
      int miny = (mom.height() < dad.height()) ? mom.height() : dad.height();
      int minz = (mom.depth() < dad.depth()) ? mom.depth() : dad.depth();
      minx = (sis.width() < minx) ? sis.width() : minx;
      miny = (sis.height() < miny) ? sis.height() : miny;
      minz = (sis.depth() < minz) ? sis.depth() : minz;
      for(i=minx-1; i>=0; i--)
	for(j=miny-1; j>=0; j--)
	  for(k=minz-1; k>=0; k--)
	    sis.gene(i,j, (((i*miny*minz+j*minz+k)%2 == 0) ?
			   mom.gene(i,j,k) : dad.gene(i,j,k)));
    }

    nc = 1;
  }

  return nc;
}








// Pick a single point in the 3D block and grab alternating quadrants for each
// child.  If the children are resizable, this crossover does clipping.
template <class T> int
GA3DArrayGenome<T>::
OnePointCrossover(const GAGenome& p1, const GAGenome& p2,
		  GAGenome* c1, GAGenome* c2){
  const GA3DArrayGenome<T> &mom=DYN_CAST(const GA3DArrayGenome<T> &, p1);
  const GA3DArrayGenome<T> &dad=DYN_CAST(const GA3DArrayGenome<T> &, p2);

  int nc=0;
  unsigned int momsitex, momlenx, momsitey, momleny, momsitez, momlenz;
  unsigned int dadsitex, dadlenx, dadsitey, dadleny, dadsitez, dadlenz;
  unsigned int sitex, lenx, sitey, leny, sitez, lenz;

  if(c1 && c2){
    GA3DArrayGenome<T> &sis=DYN_CAST(GA3DArrayGenome<T> &, *c1);
    GA3DArrayGenome<T> &bro=DYN_CAST(GA3DArrayGenome<T> &, *c2);

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

    if(sis.resizeBehaviour(GAGenome::DEPTH) == GAGenome::FIXED_SIZE &&
       bro.resizeBehaviour(GAGenome::DEPTH) == GAGenome::FIXED_SIZE){
      if(mom.depth() != dad.depth() || 
	 sis.depth() != bro.depth() || 
	 sis.depth() != mom.depth()){
	GAErr(GA_LOC, mom.className(), "one-point cross", gaErrSameLengthReqd);
	return nc;
      }
      sitez = momsitez = dadsitez = GARandomInt(0, mom.depth());
      lenz = momlenz = dadlenz = mom.depth() - momsitez;
    }
    else if(sis.resizeBehaviour(GAGenome::DEPTH) == GAGenome::FIXED_SIZE ||
	    bro.resizeBehaviour(GAGenome::DEPTH) == GAGenome::FIXED_SIZE){
      GAErr(GA_LOC, mom.className(), "one-point cross", gaErrSameBehavReqd);
      return nc;
    }
    else{
      momsitez = GARandomInt(0, mom.depth());
      dadsitez = GARandomInt(0, dad.depth());
      momlenz = mom.depth() - momsitez;
      dadlenz = dad.depth() - dadsitez;
      sitez = GAMin(momsitez, dadsitez);
      lenz = GAMin(momlenz, dadlenz);
    }

    sis.resize(sitex+lenx, sitey+leny, sitez+lenz);
    bro.resize(sitex+lenx, sitey+leny, sitez+lenz);

    sis.copy(mom, 
	     0, 0, 0, 
	     momsitex-sitex, momsitey-sitey, momsitez-sitez,
	     sitex, sitey, sitez);
    sis.copy(dad,
	     sitex, 0, 0,
	     dadsitex, dadsitey-sitey, dadsitez-sitez,
	     lenx, sitey, sitez);
    sis.copy(dad,
	     0, sitey, 0,
	     dadsitex-sitex, dadsitey, dadsitez-sitez,
	     sitex, leny, sitez);
    sis.copy(mom,
	     sitex, sitey, 0,
	     momsitex, momsitey, momsitez-sitez,
	     lenx, leny, sitez);
    sis.copy(dad, 
	     0, 0, sitez, 
	     dadsitex-sitex, dadsitey-sitey, dadsitez,
	     sitex, sitey, lenz);
    sis.copy(mom,
	     sitex, 0, sitez,
	     momsitex, momsitey-sitey, momsitez,
	     lenx, sitey, lenz);
    sis.copy(mom,
	     0, sitey, sitez,
	     momsitex-sitex, momsitey, momsitez,
	     sitex, leny, lenz);
    sis.copy(dad,
	     sitex, sitey, sitez,
	     dadsitex, dadsitey, dadsitez,
	     lenx, leny, lenz);
    
    bro.copy(dad, 
	     0, 0, 0, 
	     dadsitex-sitex, dadsitey-sitey, dadsitez-sitez,
	     sitex, sitey, sitez);
    bro.copy(mom,
	     sitex, 0, 0,
	     momsitex, momsitey-sitey, momsitez-sitez,
	     lenx, sitey, sitez);
    bro.copy(mom,
	     0, sitey, 0,
	     momsitex-sitex, momsitey, momsitez-sitez,
	     sitex, leny, sitez);
    bro.copy(dad,
	     sitex, sitey, 0,
	     dadsitex, dadsitey, dadsitez-sitez,
	     lenx, leny, sitez);
    bro.copy(mom, 
	     0, 0, sitez, 
	     momsitex-sitex, momsitey-sitey, momsitez,
	     sitex, sitey, lenz);
    bro.copy(dad,
	     sitex, 0, sitez,
	     dadsitex, dadsitey-sitey, dadsitez,
	     lenx, sitey, lenz);
    bro.copy(dad,
	     0, sitey, sitez,
	     dadsitex-sitex, dadsitey, dadsitez,
	     sitex, leny, lenz);
    bro.copy(mom,
	     sitex, sitey, sitez,
	     momsitex, momsitey, momsitez,
	     lenx, leny, lenz);

    nc = 2;
  }
  else if(c1){
    GA3DArrayGenome<T> &sis=DYN_CAST(GA3DArrayGenome<T> &, *c1);

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

    if(sis.resizeBehaviour(GAGenome::DEPTH) == GAGenome::FIXED_SIZE){
      if(mom.depth() != dad.depth() || sis.depth() != mom.depth()){
	GAErr(GA_LOC, mom.className(), "one-point cross", gaErrSameLengthReqd);
	return nc;
      }
      sitez = momsitez = dadsitez = GARandomInt(0, mom.depth());
      lenz = momlenz = dadlenz = mom.depth() - momsitez;
    }
    else{
      momsitez = GARandomInt(0, mom.depth());
      dadsitez = GARandomInt(0, dad.depth());
      momlenz = mom.depth() - momsitez;
      dadlenz = dad.depth() - dadsitez;
      sitez = GAMin(momsitez, dadsitez);
      lenz = GAMin(momlenz, dadlenz);
    }
    
    sis.resize(sitex+lenx, sitey+leny, sitez+lenz);
    
    if(GARandomBit()){
      sis.copy(mom, 
	       0, 0, 0, 
	       momsitex-sitex, momsitey-sitey, momsitez-sitez,
	       sitex, sitey, sitez);
      sis.copy(dad,
	       sitex, 0, 0,
	       dadsitex, dadsitey-sitey, dadsitez-sitez,
	       lenx, sitey, sitez);
      sis.copy(dad,
	       0, sitey, 0,
	       dadsitex-sitex, dadsitey, dadsitez-sitez,
	       sitex, leny, sitez);
      sis.copy(mom,
	       sitex, sitey, 0,
	       momsitex, momsitey, momsitez-sitez,
	       lenx, leny, sitez);
      sis.copy(dad, 
	       0, 0, sitez, 
	       dadsitex-sitex, dadsitey-sitey, dadsitez,
	       sitex, sitey, lenz);
      sis.copy(mom,
	       sitex, 0, sitez,
	       momsitex, momsitey-sitey, momsitez,
	       lenx, sitey, lenz);
      sis.copy(mom,
	       0, sitey, sitez,
	       momsitex-sitex, momsitey, momsitez,
	       sitex, leny, lenz);
      sis.copy(dad,
	       sitex, sitey, sitez,
	       dadsitex, dadsitey, dadsitez,
	       lenx, leny, lenz);
    }
    else{
      sis.copy(dad, 
	       0, 0, 0, 
	       dadsitex-sitex, dadsitey-sitey, dadsitez-sitez,
	       sitex, sitey, sitez);
      sis.copy(mom,
	       sitex, 0, 0,
	       momsitex, momsitey-sitey, momsitez-sitez,
	       lenx, sitey, sitez);
      sis.copy(mom,
	       0, sitey, 0,
	       momsitex-sitex, momsitey, momsitez-sitez,
	       sitex, leny, sitez);
      sis.copy(dad,
	       sitex, sitey, 0,
	       dadsitex, dadsitey, dadsitez-sitez,
	       lenx, leny, sitez);
      sis.copy(mom, 
	       0, 0, sitez, 
	       momsitex-sitex, momsitey-sitey, momsitez,
	       sitex, sitey, lenz);
      sis.copy(dad,
	       sitex, 0, sitez,
	       dadsitex, dadsitey-sitey, dadsitez,
	       lenx, sitey, lenz);
      sis.copy(dad,
	       0, sitey, sitez,
	       dadsitex-sitex, dadsitey, dadsitez,
	       sitex, leny, lenz);
      sis.copy(mom,
	       sitex, sitey, sitez,
	       momsitex, momsitey, momsitez,
	       lenx, leny, lenz);
    }

    nc = 1;
  }

  return nc;
}

#endif
