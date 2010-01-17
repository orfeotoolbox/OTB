// $Header$
/* ----------------------------------------------------------------------------
  binstr3.C
  mbwall 19apr95
  Copyright (c) 1995 Massachusetts Institute of Technology
                     all rights reserved

 DESCRIPTION:
  Source file for the 3D binary string genome.  See the 1D genome for comments.
---------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <ga/gaerror.h>
#include <ga/garandom.h>
#include <ga/GA3DBinStrGenome.h>
#include <ga/GAMask.h>


/* ----------------------------------------------------------------------------
   Genome class definition
---------------------------------------------------------------------------- */
GA3DBinaryStringGenome::
GA3DBinaryStringGenome(unsigned int width,
		       unsigned int height,
		       unsigned int depth,
		       GAGenome::Evaluator f, void * u) :
GABinaryString(width*height*depth),
GAGenome(DEFAULT_3DBINSTR_INITIALIZER,
	 DEFAULT_3DBINSTR_MUTATOR,
	 DEFAULT_3DBINSTR_COMPARATOR) {
  evaluator(f);
  userData(u);
  crossover(DEFAULT_3DBINSTR_CROSSOVER);
  nx=minX=maxX=0; ny=minY=maxY=0; nz=minZ=maxZ=0;
  resize(width, height, depth);
}


GA3DBinaryStringGenome::
GA3DBinaryStringGenome(const GA3DBinaryStringGenome & orig) :
GABinaryString(orig.GABinaryString::size()),
GAGenome() {
  nx=minX=maxX=0; ny=minY=maxY=0; nz=minZ=maxZ=0;
  GA3DBinaryStringGenome::copy(orig);
}

GA3DBinaryStringGenome::~GA3DBinaryStringGenome() { }

GAGenome *
GA3DBinaryStringGenome::clone(GAGenome::CloneMethod flag) const {
  GA3DBinaryStringGenome *cpy = new GA3DBinaryStringGenome(nx,ny,nz);
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

void
GA3DBinaryStringGenome::copy(const GAGenome & orig)
{
  if(&orig == this) return;
  const GA3DBinaryStringGenome* c = 
    DYN_CAST(const GA3DBinaryStringGenome*, &orig);
  if(c) {
    GAGenome::copy(*c);
    GABinaryString::copy(*c);
    nx = c->nx; ny = c->ny; nz = c->nz;
    minX = c->minX; minY = c->minY; minZ = c->minZ;
    maxX = c->maxX; maxY = c->maxY; maxZ = c->maxZ;
  }
}


int
GA3DBinaryStringGenome::resize(int w, int h, int d)
{
  if(w == STA_CAST(int,nx) && h == STA_CAST(int,ny) && d == STA_CAST(int,nz))
    return sz;

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
	GABinaryString::move(k*h*w+j*w,k*ny*nx+j*nx,w);
  }
  else if(w < STA_CAST(int,nx)){
    int z=GAMin(STA_CAST(int,nz),d);
    for(int k=0; k<z; k++)
      for(int j=0; j<STA_CAST(int,ny); j++)
	GABinaryString::move(k*ny*w+j*w,k*ny*nx+j*nx,w);
  }
  else if(h < STA_CAST(int,ny)){
    int z=GAMin(STA_CAST(int,nz),d);
    for(int k=0; k<z; k++)
      for(int j=0; j<h; j++)
	GABinaryString::move(k*h*nx+j*nx,k*ny*nx+j*nx,nx);
  }

  GABinaryString::resize(w*h*d);

  if(w > STA_CAST(int,nx) && h > STA_CAST(int,ny)){ // adjust the existing bits
    int z=GAMin(STA_CAST(int,nz),d);
    for(int k=z-1; k>=0; k--){
      int j;
      for(j=ny-1; j>=0; j--){
	GABinaryString::move(k*h*w+j*w,k*ny*nx+j*nx,nx);
	for(int i=nx; i<w; i++)
	  bit(k*h*w+j*w+i, GARandomBit());
      }
      for(j=ny; j<h; j++)
	for(int i=0; i<w; i++)
	  bit(k*h*w+j*w+i, GARandomBit());
    }
  }
  else if(w > STA_CAST(int,nx)){
    int z=GAMin(STA_CAST(int,nz),d);
    for(int k=z-1; k>=0; k--){
      for(int j=h-1; j>=0; j--){
	GABinaryString::move(k*h*w+j*w,k*h*nx+j*nx,nx);
	for(int i=nx; i<w; i++)
	  bit(k*h*w+j*w+i, GARandomBit());
      }
    }
  }
  else if(h > STA_CAST(int,ny)){
    int z=GAMin(STA_CAST(int,nz),d);
    for(int k=z-1; k>=0; k--){
      int j;
      for(j=ny-1; j>=0; j--)
	GABinaryString::move(k*h*w+j*w,k*ny*w+j*w,w);
      for(j=ny; j<h; j++)
	for(int i=0; i<w; i++)
	  bit(k*h*w+j*w+i, GARandomBit());
    }
  }
  if(d > STA_CAST(int,nz)){		// change in depth is always new bits
    for(int i=w*h*nz; i<w*h*d; i++)
      bit(i, GARandomBit());
  }

  nx = w; ny = h; nz = d;
  _evaluated = gaFalse;
  return sz;
}

#ifdef GALIB_USE_STREAMS
int
GA3DBinaryStringGenome::read(STD_ISTREAM & is)
{
  static char c;
  unsigned int i=0, j=0, k=0;
  while(!is.fail() && !is.eof() && k < nz) {
    is >> c;
    if(isdigit(c)){
      gene(i++, j, k, ((c == '0') ? 0 : 1));
      if(i >= nx){
	i=0;
	j++;
      }
      if(j >= ny){
	j=0;
	k++;
      }
    }
  }

  _evaluated = gaFalse;

  if(is.eof() && 
     ((k < nz) ||		// didn't get some lines
      (j < ny && j != 0) ||	// didn't get some lines
      (i < nx && i != 0))){	// didn't get some lines
    GAErr(GA_LOC, className(), "read", gaErrUnexpectedEOF);
    is.clear(STD_IOS_BADBIT | is.rdstate());
    return 1;
  }

  return 0;
}


// Dump the bits to the stream with a newline at the end of each row and 
// another at the end of each layer.  No newline at the end of the block.
int
GA3DBinaryStringGenome::write(STD_OSTREAM & os) const 
{
  for(unsigned int k=0; k<nz; k++){
    for(unsigned int j=0; j<ny; j++){
      for(unsigned int i=0; i<nx; i++){
	os << gene(i,j,k);
      }
      os << "\n";
    }
    os << "\n";
  }
  return 0;
}
#endif


int
GA3DBinaryStringGenome::resizeBehaviour(GAGenome::Dimension which) const {
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


int
GA3DBinaryStringGenome::
resizeBehaviour(Dimension which, unsigned int lower, unsigned int upper)
{
  if(upper < lower){
    GAErr(GA_LOC, className(), "resizeBehaviour", gaErrBadResizeBehaviour);
    return resizeBehaviour(which);
  }

  switch(which){
  case WIDTH:
    minX = lower; maxX = upper;
    if(nx > upper) resize(upper,ny,nz);
    if(nx < lower) resize(lower,ny,nz);
    break;

  case HEIGHT:
    minY = lower; maxY = upper;
    if(ny > upper) resize(nx,upper,nz);
    if(ny < lower) resize(nx,lower,nz);
    break;

  case DEPTH:
    minZ = lower; maxZ = upper;
    if(nz > upper) resize(nx,ny,upper);
    if(nz < lower) resize(nx,ny,lower);
    break;

  default:
    break;
  }

  return resizeBehaviour(which);
}


void
GA3DBinaryStringGenome::
copy(const GA3DBinaryStringGenome & orig,
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
      GABinaryString::copy(orig,
			   (t+k)*ny*nx + (s+j)*nx + r,
			   (z+k)*orig.ny*orig.nx + (y+j)*orig.nx + x, w);
  _evaluated = gaFalse;
}


void
GA3DBinaryStringGenome::
set(unsigned int x, unsigned int y, unsigned int z,
    unsigned int w, unsigned int h, unsigned int d)
{
  if(x + w > nx) w = nx - x;
  if(y + h > ny) h = ny - y;
  if(z + d > nz) d = nz - z;

  for(unsigned int k=0; k<d; k++)
    for(unsigned int j=0; j<h; j++)
      GABinaryString::set((z+k)*ny*nx + (y+j)*nx + x, w);
  _evaluated = gaFalse;
}


void
GA3DBinaryStringGenome::
unset(unsigned int x, unsigned int y, unsigned int z,
      unsigned int w, unsigned int h, unsigned int d)
{
  if(x + w > nx) w = nx - x;
  if(y + h > ny) h = ny - y;
  if(z + d > nz) d = nz - z;

  for(unsigned int k=0; k<d; k++)
    for(unsigned int j=0; j<h; j++)
      GABinaryString::unset((z+k)*ny*nx + (y+j)*nx + x, w);
  _evaluated = gaFalse;
}


void
GA3DBinaryStringGenome::
randomize(unsigned int x, unsigned int y, unsigned int z,
	  unsigned int w, unsigned int h, unsigned int d)
{
  if(x + w > nx) w = nx - x;
  if(y + h > ny) h = ny - y;
  if(z + d > nz) d = nz - z;

  for(unsigned int k=0; k<d; k++)
    for(unsigned int j=0; j<h; j++)
      GABinaryString::randomize((z+k)*ny*nx + (y+j)*nx + x, w);
  _evaluated = gaFalse;
}


void
GA3DBinaryStringGenome::
move(unsigned int x, unsigned int y, unsigned int z,
     unsigned int srcx, unsigned int srcy, unsigned int srcz,
     unsigned int w, unsigned int h, unsigned int d)
{
  if(srcx + w > nx) w = nx - srcx;
  if(x + w > nx) w = nx - x;
  if(srcy + h > ny) h = ny - srcy;
  if(y + h > ny) h = ny - y;
  if(srcz + d > nz) d = nz - srcz;
  if(z + d > nz) d = nz - z;

  if(srcz<z){
    if(srcy<y){
      for(int k=d-1; k>=0; k--)
	for(int j=h-1; j>=0; j--)
	  GABinaryString::move((z+k)*ny*nx + (y+j)*nx + x,
			       (srcz+k)*ny*nx + (srcy+j)*nx + srcx, w);
    }
    else{
      for(int k=d-1; k>=0; k--)
	for(unsigned int j=0; j<h; j++)
	  GABinaryString::move((z+k)*ny*nx + (y+j)*nx + x,
			       (srcz+k)*ny*nx + (srcy+j)*nx + srcx, w);
    }
  }
  else{
    if(srcy<y){
      for(unsigned int k=0; k<d; k++)
	for(int j=h-1; j>=0; j--)
	  GABinaryString::move((z+k)*ny*nx + (y+j)*nx + x,
			       (srcz+k)*ny*nx + (srcy+j)*nx + srcx, w);
    }
    else{
      for(unsigned int k=0; k<d; k++)
	for(unsigned int j=0; j<h; j++)
	  GABinaryString::move((z+k)*ny*nx + (y+j)*nx + x,
			       (srcz+k)*ny*nx + (srcy+j)*nx + srcx, w);
    }
  }
  _evaluated = gaFalse;
}


int
GA3DBinaryStringGenome::
equal(const GA3DBinaryStringGenome& orig,
      unsigned int x, unsigned int y, unsigned int z,
      unsigned int srcx, unsigned int srcy, unsigned int srcz,
      unsigned int w, unsigned int h, unsigned int d) const
{
  unsigned int eq = 0;
  for(unsigned int k=0; k<d; k++)
    for(unsigned int j=0; j<h; j++)
      eq += GABinaryString::equal(orig,
				  (z+k)*ny*nx + (y+j)*nx + x,
				  (srcz+k)*ny*nx + (srcy+j)*nx + srcx, 
				  w);
  return eq==d*h ? 1 : 0;
}


int 
GA3DBinaryStringGenome::equal(const GAGenome & c) const
{
  if(this == &c) return 1;
  const GA3DBinaryStringGenome& b = DYN_CAST(const GA3DBinaryStringGenome&, c);
  if(nx != b.nx || ny != b.ny || nz != b.nz) return 0;
  int val=0;
  for(unsigned int k=0; k<nz && val==0; k++)
    for(unsigned int j=0; j<ny && val==0; j++)
      val = GABinaryString::equal(b,k*ny*nx,k*ny*nx,nx) ? 0 : 1;
  return(val ? 0 : 1);
}








/* ----------------------------------------------------------------------------
   Operators
---------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------
3D Binary String Genome
  The order for looping through indices is depth-then-height-then-width
(ie depth loops before a single height increment, height loops before a single
width increment)
---------------------------------------------------------------------------- */
void 
GA3DBinaryStringGenome::UniformInitializer(GAGenome & c)
{
  GA3DBinaryStringGenome &child=DYN_CAST(GA3DBinaryStringGenome &, c);
  child.resize(GAGenome::ANY_SIZE,GAGenome::ANY_SIZE,GAGenome::ANY_SIZE);
  for(int i=child.width()-1; i>=0; i--)
    for(int j=child.height()-1; j>=0; j--)
      for(int k=child.depth()-1; k>=0; k--)
	child.gene(i,j,k, GARandomBit());
}


void 
GA3DBinaryStringGenome::UnsetInitializer(GAGenome & c)
{
  GA3DBinaryStringGenome &child=DYN_CAST(GA3DBinaryStringGenome &, c);
  child.resize(GAGenome::ANY_SIZE,GAGenome::ANY_SIZE,GAGenome::ANY_SIZE);
  child.unset(0, 0, 0, child.width(), child.height(), child.depth());
}


void 
GA3DBinaryStringGenome::SetInitializer(GAGenome & c)
{
  GA3DBinaryStringGenome &child=DYN_CAST(GA3DBinaryStringGenome &, c);
  child.resize(GAGenome::ANY_SIZE,GAGenome::ANY_SIZE,GAGenome::ANY_SIZE);
  child.set(0, 0, 0, child.width(), child.height(), child.depth());
}


int 
GA3DBinaryStringGenome::FlipMutator(GAGenome & c, float pmut)
{
  GA3DBinaryStringGenome &child=DYN_CAST(GA3DBinaryStringGenome &, c);
  register int n, m, i, j, k, d;
  if(pmut <= 0.0) return(0);

  float nMut = pmut * STA_CAST(float,child.size());
  if(nMut < 1.0){		// we have to do a flip test on each bit
    nMut = 0;
    for(i=child.width()-1; i>=0; i--){
      for(j=child.height()-1; j>=0; j--){
	for(k=child.depth()-1; k>=0; k--){
	  if(GAFlipCoin(pmut)){
	    child.gene(i, j, k, ((child.gene(i,j,k) == 0) ? 1 : 0));
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
      child.gene(i, j, k, ((child.gene(i,j,k) == 0) ? 1 : 0));
    }
  }
  return(STA_CAST(int,nMut));
}


float
GA3DBinaryStringGenome::BitComparator(const GAGenome& a, const GAGenome& b){
  const GA3DBinaryStringGenome &sis=DYN_CAST(const GA3DBinaryStringGenome&, a);
  const GA3DBinaryStringGenome &bro=DYN_CAST(const GA3DBinaryStringGenome&, b);
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
int
GA3DBinaryStringGenome::
UniformCrossover(const GAGenome& p1, const GAGenome& p2,
		 GAGenome* c1, GAGenome* c2){
  const GA3DBinaryStringGenome &mom=
    DYN_CAST(const GA3DBinaryStringGenome &, p1);
  const GA3DBinaryStringGenome &dad=
    DYN_CAST(const GA3DBinaryStringGenome &, p2);

  int i,j,k, nc=0;;

  if(c1 && c2){
    GA3DBinaryStringGenome &sis=DYN_CAST(GA3DBinaryStringGenome &, *c1);
    GA3DBinaryStringGenome &bro=DYN_CAST(GA3DBinaryStringGenome &, *c2);

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
      int maxx = GAMax(sis.width(), bro.width());
      int minx = GAMin(mom.width(), dad.width());
      int maxy = GAMax(sis.height(), bro.height());
      int miny = GAMin(mom.height(), dad.height());
      int maxz = GAMax(sis.depth(), bro.depth());
      int minz = GAMin(mom.depth(), dad.depth());
      mask.size(maxx*maxy*maxz);
      for(i=0; i<maxx; i++)
	for(j=0; j<maxy; j++)
	  for(k=0; k<maxz; k++)
	    mask[i*maxy*maxz+j*maxz+k] = GARandomBit();
      minx = GAMin(sis.width(), minx);
      miny = GAMin(sis.height(), miny);
      minz = GAMin(sis.depth(), minz);
      for(i=minx-1; i>=0; i--)
	for(j=miny-1; j>=0; j--)
	  for(k=minz-1; k>=0; k--)
	    sis.gene(i,j,k, (mask[i*miny*minz+j*minz+k] ?
			     mom.gene(i,j,k) : dad.gene(i,j,k)));
      minx = GAMin(bro.width(), minx);
      miny = GAMin(bro.height(), miny);
      minz = GAMin(bro.depth(), minz);
      for(i=minx-1; i>=0; i--)
	for(j=miny-1; j>=0; j--)
	  for(k=minz-1; k>=0; k--)
	    bro.gene(i,j,k, (mask[i*miny*minz+j*minz+k] ?
			     dad.gene(i,j,k) : mom.gene(i,j,k)));
    }

    nc = 2;
  }
  else if(c1 || c2){
    GA3DBinaryStringGenome &sis = (c1 ? 
				   DYN_CAST(GA3DBinaryStringGenome&, *c1) : 
				   DYN_CAST(GA3DBinaryStringGenome&, *c2));
    
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
      int minx = GAMin(mom.width(), dad.width());
      int miny = GAMin(mom.height(), dad.height());
      int minz = GAMin(mom.depth(), dad.depth());
      minx = GAMin(sis.width(), minx);
      miny = GAMin(sis.height(), miny);
      minz = GAMin(sis.depth(), minz);
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
int
GA3DBinaryStringGenome::
EvenOddCrossover(const GAGenome& p1, const GAGenome& p2,
		 GAGenome* c1, GAGenome* c2){
  const GA3DBinaryStringGenome &mom=
    DYN_CAST(const GA3DBinaryStringGenome &, p1);
  const GA3DBinaryStringGenome &dad=
    DYN_CAST(const GA3DBinaryStringGenome &, p2);

  int nc=0;
  int i,j,k;

  if(c1 && c2){
    GA3DBinaryStringGenome &sis=DYN_CAST(GA3DBinaryStringGenome &, *c1);
    GA3DBinaryStringGenome &bro=DYN_CAST(GA3DBinaryStringGenome &, *c2);

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
	    if(count%2 == 0){
	      sis.gene(i,j,k, mom.gene(i,j,k));
	      bro.gene(i,j,k, dad.gene(i,j,k));
	    }
	    else{
	      sis.gene(i,j,k, dad.gene(i,j,k));
	      bro.gene(i,j,k, mom.gene(i,j,k));
	    }
	    count++;
	  }
	}
      }
    }
    else{
      int minx = GAMin(mom.width(), dad.width());
      int miny = GAMin(mom.height(), dad.height());
      int minz = GAMin(mom.depth(), dad.depth());
      minx = GAMin(sis.width(), minx);
      miny = GAMin(sis.height(), miny);
      minz = GAMin(sis.depth(), minz);
      for(i=minx-1; i>=0; i--)
	for(j=miny-1; j>=0; j--)
	  for(k=minz-1; k>=0; k--)
	    sis.gene(i,j,k, (((i*miny*minz+j*minz+k)%2 == 0) ?
			     mom.gene(i,j,k) : dad.gene(i,j,k)));
      minx = (bro.width() < minx) ? bro.width() : minx;
      miny = (bro.height() < miny) ? bro.height() : miny;
      minz = (bro.depth() < minz) ? bro.depth() : minz;
      for(i=minx-1; i>=0; i--)
	for(j=miny-1; j>=0; j--)
	  for(k=minz-1; k>=0; k--)
	    bro.gene(i,j,k, (((i*miny*minz+j*minz+k)%2 == 0) ?
			     dad.gene(i,j,k) : mom.gene(i,j,k)));
    }

    nc = 2;
  }
  else if(c1 || c2){
    GA3DBinaryStringGenome &sis = (c1 ?
				   DYN_CAST(GA3DBinaryStringGenome &, *c1) : 
				   DYN_CAST(GA3DBinaryStringGenome &, *c2));

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
      int minx = GAMin(mom.width(), dad.width());
      int miny = GAMin(mom.height(), dad.height());
      int minz = GAMin(mom.depth(), dad.depth());
      minx = GAMin(sis.width(), minx);
      miny = GAMin(sis.height(), miny);
      minz = GAMin(sis.depth(), minz);
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
// child.  If the children are resizable, this crossover does clipping or 
// padding depending on the setting of the clip flag.  If we pad, we fill the
// additional bits with random contents.
int
GA3DBinaryStringGenome::
OnePointCrossover(const GAGenome& p1, const GAGenome& p2,
		  GAGenome* c1, GAGenome* c2){
  const GA3DBinaryStringGenome &mom=
    DYN_CAST(const GA3DBinaryStringGenome &, p1);
  const GA3DBinaryStringGenome &dad=
    DYN_CAST(const GA3DBinaryStringGenome &, p2);

  int nc=0;
  unsigned int momsitex, momlenx, momsitey, momleny, momsitez, momlenz;
  unsigned int dadsitex, dadlenx, dadsitey, dadleny, dadsitez, dadlenz;
  unsigned int sitex, lenx, sitey, leny, sitez, lenz;

  if(c1 && c2){
    GA3DBinaryStringGenome &sis=DYN_CAST(GA3DBinaryStringGenome &, *c1);
    GA3DBinaryStringGenome &bro=DYN_CAST(GA3DBinaryStringGenome &, *c2);

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
  else if(c1 || c2){
    GA3DBinaryStringGenome &sis = (c1 ?
				   DYN_CAST(GA3DBinaryStringGenome &, *c1) :
				   DYN_CAST(GA3DBinaryStringGenome &, *c2));

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
