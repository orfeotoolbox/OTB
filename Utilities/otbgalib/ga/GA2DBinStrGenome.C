// $Header$
/* ----------------------------------------------------------------------------
  binstr2.C
  mbwall 19apr95
  Copyright (c) 1995 Massachusetts Institute of Technology
                     all rights reserved

 DESCRIPTION:
  Source file for the 2D binary string genome.  See the 1D genome for comments.
---------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <ga/gaerror.h>
#include <ga/garandom.h>
#include <ga/GA2DBinStrGenome.h>
#include <ga/GAMask.h>

/* ----------------------------------------------------------------------------
   Genome class definition
---------------------------------------------------------------------------- */
GA2DBinaryStringGenome::
GA2DBinaryStringGenome(unsigned int width, unsigned int height, 
		       GAGenome::Evaluator f, void * u) :
GABinaryString(width*height),
GAGenome(DEFAULT_2DBINSTR_INITIALIZER,
	 DEFAULT_2DBINSTR_MUTATOR,
	 DEFAULT_2DBINSTR_COMPARATOR) {
  evaluator(f);
  userData(u);
  crossover(DEFAULT_2DBINSTR_CROSSOVER);
  nx=minX=maxX=0; ny=minY=maxY=0;
  resize(width, height);
}


GA2DBinaryStringGenome::
GA2DBinaryStringGenome(const GA2DBinaryStringGenome & orig) :
GABinaryString(orig.GABinaryString::size()),
GAGenome() {
  nx=minX=maxX=0; ny=minY=maxY=0;
  GA2DBinaryStringGenome::copy(orig);
}

GA2DBinaryStringGenome::~GA2DBinaryStringGenome() {
}

GAGenome *
GA2DBinaryStringGenome::clone(GAGenome::CloneMethod flag) const {
  GA2DBinaryStringGenome *cpy = new GA2DBinaryStringGenome(nx,ny);
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

void
GA2DBinaryStringGenome::copy(const GAGenome & orig)
{
  if(&orig == this) return;
  const GA2DBinaryStringGenome* c =
    DYN_CAST(const GA2DBinaryStringGenome*, &orig);
  if(c) {
    GAGenome::copy(*c);
    GABinaryString::copy(*c);
    nx = c->nx; ny = c->ny; 
    minX = c->minX; minY = c->minY; 
    maxX = c->maxX; maxY = c->maxY;
  } 
}


int
GA2DBinaryStringGenome::resize(int w, int h)
{
  if((unsigned int)w == nx && (unsigned int)h == ny) return sz;

  if(w == GAGenome::ANY_SIZE)
    w = GARandomInt(minX, maxX);
  else if(w < 0)
    w = nx;		// do nothing
  else if(minX == maxX)
    minX=maxX = w;
  else{
    if(w < STA_CAST(int, minX)) w=minX;
    if(w > STA_CAST(int, maxX)) w=maxX;
  }

  if(h == GAGenome::ANY_SIZE)
    h = GARandomInt(minY, maxY);
  else if(h < 0)
    h = ny;		// do nothing
  else if(minY == maxY)
    minY=maxY = h;
  else{
    if(h < STA_CAST(int, minY)) h=minY;
    if(h > STA_CAST(int, maxY)) h=maxY;
  }

// Move the bits into the right position.  If we're smaller, then shift to
// the smaller size before we do the resize (the resize method maintains bit
// integrety).  If we're larger, do the move after the resize.  If we're the 
// same size the we don't do anything.  When we're adding more bits, the new
// bits get set randomly to 0 or 1.

  if(w < STA_CAST(int,nx)){
    int y=GAMin(STA_CAST(int,ny),h);
    for(int j=0; j<y; j++)
      GABinaryString::move(j*w,j*nx,w);
  }
  GABinaryString::resize(w*h);
  if(w > STA_CAST(int,nx)){		// adjust the existing chunks of bits
    int y=GAMin(STA_CAST(int,ny),h);
    for(int j=y-1; j>=0; j--){
      GABinaryString::move(j*w,j*nx,nx);
      for(int i=nx; i<w; i++)
	bit(j*w+i, GARandomBit());
    }
  }
  if(h > STA_CAST(int,ny)){		// change in height is always new bits
    for(int i=w*ny; i<w*h; i++)
      bit(i, GARandomBit());
  }

  nx = w; ny = h;
  _evaluated = gaFalse;
  return sz;
}


#ifdef GALIB_USE_STREAMS
int
GA2DBinaryStringGenome::read(STD_ISTREAM & is)
{
  static char c;
  unsigned int i=0, j=0;
  while(!is.fail() && !is.eof() && j < ny) {
    is >> c;
    if(isdigit(c)){
      gene(i, j, ((c == '0') ? 0 : 1));
      if(++i >= nx){		// ready for next row
	i=0;
	j++;
      }
    }
  }

  _evaluated = gaFalse;

  if(is.eof() && 
     ((j < ny) ||	     // didn't get some lines
      (i < nx && i != 0))){   // stopped early on a row
    GAErr(GA_LOC, className(), "read", gaErrUnexpectedEOF);
    is.clear(STD_IOS_BADBIT | is.rdstate());
    return 1;
  }

  return 0;
}


// Dump the digits to the stream with a newline between each row.  No newline
// at the end of the whole thing.
int
GA2DBinaryStringGenome::write(STD_OSTREAM & os) const 
{
  for(unsigned int j=0; j<ny; j++){
    for(unsigned int i=0; i<nx; i++)
      os << gene(i,j);
    os << "\n";
  }
  return 0;
}
#endif


int 
GA2DBinaryStringGenome::resizeBehaviour(GAGenome::Dimension which) const {
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


int
GA2DBinaryStringGenome::
resizeBehaviour(Dimension which, unsigned int lower, unsigned int upper)
{
  if(upper < lower){
    GAErr(GA_LOC, className(), "resizeBehaviour", gaErrBadResizeBehaviour);
    return resizeBehaviour(which);
  }

  switch(which){
  case WIDTH:
    minX = lower; maxX = upper;
    if(nx > upper) resize(upper,ny);
    if(nx < lower) resize(lower,ny);
    break;

  case HEIGHT:
    minY = lower; maxY = upper;
    if(ny > upper) resize(nx,upper);
    if(ny < lower) resize(nx,lower);
    break;

  default:
    break;
  }

  return resizeBehaviour(which);
}


void
GA2DBinaryStringGenome::copy(const GA2DBinaryStringGenome & orig,
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
    GABinaryString::copy(orig, (s+j)*nx+r, (y+j)*orig.nx+x, w);
  _evaluated = gaFalse;
}


void
GA2DBinaryStringGenome::set(unsigned int x, unsigned int y,
			    unsigned int w, unsigned int h)
{
  if(x + w > nx) w = nx - x;
  if(y + h > ny) h = ny - y;

  for(unsigned int j=0; j<h; j++)
    GABinaryString::set((y+j)*nx+x, w);
  _evaluated = gaFalse;
}


void
GA2DBinaryStringGenome::unset(unsigned int x, unsigned int y,
			      unsigned int w, unsigned int h)
{
  if(x + w > nx) w = nx - x;
  if(y + h > ny) h = ny - y;

  for(unsigned int j=0; j<h; j++)
    GABinaryString::unset((y+j)*nx+x, w);
  _evaluated = gaFalse;
}


void
GA2DBinaryStringGenome::randomize(unsigned int x, unsigned int y,
				  unsigned int w, unsigned int h)
{
  if(x + w > nx) w = nx - x;
  if(y + h > ny) h = ny - y;

  for(unsigned int j=0; j<h; j++)
    GABinaryString::randomize((y+j)*nx+x, w);
  _evaluated = gaFalse;
}


void
GA2DBinaryStringGenome::move(unsigned int x, unsigned int y,
			     unsigned int srcx, unsigned int srcy,
			     unsigned int w, unsigned int h)
{
  if(srcx + w > nx) w = nx - srcx;
  if(x + w > nx) w = nx - x;
  if(srcy + h > ny) h = ny - srcy;
  if(y + h > ny) h = ny - y;

  if(srcy<y){
    for(int j=h-1; j>=0; j--)
      GABinaryString::move((y+j)*nx+x, (srcy+j)*nx+srcx, w);
  }
  else{
    for(unsigned int j=0; j<h; j++)
      GABinaryString::move((y+j)*nx+x, (srcy+j)*nx+srcx, w);
  }
  _evaluated = gaFalse;
}


int
GA2DBinaryStringGenome::equal(const GA2DBinaryStringGenome& orig,
			      unsigned int x, unsigned int y,
			      unsigned int srcx, unsigned int srcy,
			      unsigned int w, unsigned int h) const
{
  unsigned int eq = 0;
  for(unsigned int j=0; j<h; j++)
    eq += GABinaryString::equal(orig, (y+j)*nx+x, (srcy+j)*nx+srcx, w);
  return eq==h ? 1 : 0;
}


int 
GA2DBinaryStringGenome::equal(const GAGenome & c) const
{
  if(this == &c) return 1;
  GA2DBinaryStringGenome & b = (GA2DBinaryStringGenome &)c;
  if(nx != b.nx || ny != b.ny) return 0;
  int val=0;
  for(unsigned int j=0; j<ny && val==0; j++)
    val = GABinaryString::equal(b,j*nx,j*nx,nx) ? 0 : 1;
  return(val ? 0 : 1);
}







/* ----------------------------------------------------------------------------
   Operators
---------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------
2D Binary String Genome
  The order for looping through indices is height-then-width (ie height loops
before a single width increment)
---------------------------------------------------------------------------- */
void 
GA2DBinaryStringGenome::UniformInitializer(GAGenome & c)
{
  GA2DBinaryStringGenome &child=DYN_CAST(GA2DBinaryStringGenome &, c);
  child.resize(GAGenome::ANY_SIZE, GAGenome::ANY_SIZE);
  for(int i=child.width()-1; i>=0; i--)
    for(int j=child.height()-1; j>=0; j--)
      child.gene(i, j, GARandomBit());
}


void 
GA2DBinaryStringGenome::UnsetInitializer(GAGenome & c)
{
  GA2DBinaryStringGenome &child=DYN_CAST(GA2DBinaryStringGenome &, c);
  child.resize(GAGenome::ANY_SIZE, GAGenome::ANY_SIZE);
  child.unset(0, 0, child.width(), child.height());
}


void 
GA2DBinaryStringGenome::SetInitializer(GAGenome & c)
{
  GA2DBinaryStringGenome &child=DYN_CAST(GA2DBinaryStringGenome &, c);
  child.resize(GAGenome::ANY_SIZE, GAGenome::ANY_SIZE);	
  child.set(0, 0, child.width(), child.height());
}


int 
GA2DBinaryStringGenome::FlipMutator(GAGenome & c, float pmut)
{
  GA2DBinaryStringGenome &child=DYN_CAST(GA2DBinaryStringGenome &, c);
  register int n, m, i, j;
  if(pmut <= 0.0) return(0);

  float nMut = pmut * STA_CAST(float, child.size());
  if(nMut < 1.0){		// we have to do a flip test on each bit
    nMut = 0;
    for(i=child.width()-1; i>=0; i--){
      for(j=child.height()-1; j>=0; j--){
	if(GAFlipCoin(pmut)){
	  child.gene(i, j, ((child.gene(i,j) == 0) ? 1 : 0));
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
      child.gene(i, j, ((child.gene(i,j) == 0) ? 1 : 0));
    }
  }
  return(STA_CAST(int,nMut));
}


float
GA2DBinaryStringGenome::BitComparator(const GAGenome& a, const GAGenome& b){
  const GA2DBinaryStringGenome &sis=
    DYN_CAST(const GA2DBinaryStringGenome &, a);
  const GA2DBinaryStringGenome &bro=
    DYN_CAST(const GA2DBinaryStringGenome &, b);
  if(sis.size() != bro.size()) return -1;
  if(sis.size() == 0) return 0;
  float count = 0.0;
  for(int i=sis.width()-1; i>=0; i--)
    for(int j=sis.height()-1; j>=0; j--)
      count += ((sis.gene(i,j) == bro.gene(i,j)) ? 0 : 1);
  return count/sis.size();
}











int
GA2DBinaryStringGenome::
UniformCrossover(const GAGenome& p1, const GAGenome& p2, 
		 GAGenome* c1, GAGenome* c2){
  const GA2DBinaryStringGenome &mom=
    DYN_CAST(const GA2DBinaryStringGenome &, p1);
  const GA2DBinaryStringGenome &dad=
    DYN_CAST(const GA2DBinaryStringGenome &, p2);

  int nc=0;
  int i,j;

  if(c1 && c2){
    GA2DBinaryStringGenome &sis=DYN_CAST(GA2DBinaryStringGenome &, *c1);
    GA2DBinaryStringGenome &bro=DYN_CAST(GA2DBinaryStringGenome &, *c2);

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
      int maxx = GAMax(sis.width(), bro.width());
      int minx = GAMin(mom.width(), dad.width());
      int maxy = GAMax(sis.height(), bro.height());
      int miny = GAMin(mom.height(), dad.height());
      mask.size(maxx*maxy);
      for(i=0; i<maxx; i++)
	for(j=0; j<maxy; j++)
	  mask[i*maxy+j] = GARandomBit();
      minx = GAMin(sis.width(), minx);
      miny = GAMin(sis.height(), miny);
      for(i=0; i<minx; i++)
	for(j=0; j<miny; j++)
	  sis.gene(i,j, (mask[i*miny+j] ? mom.gene(i,j) : dad.gene(i,j)));
      minx = GAMin(bro.width(), minx);
      miny = GAMin(bro.height(), miny);
      for(i=0; i<minx; i++)
	for(j=0; j<miny; j++)
	  bro.gene(i,j, (mask[i*miny+j] ? dad.gene(i,j) : mom.gene(i,j)));
    }

    nc = 2;
  }
  else if(c1 || c2){
    GA2DBinaryStringGenome &sis = (c1 ? 
				   DYN_CAST(GA2DBinaryStringGenome&, *c1) : 
				   DYN_CAST(GA2DBinaryStringGenome&, *c2));

    if(mom.width() == dad.width() && mom.height() == dad.height() &&
       sis.width() == mom.width() && sis.height() == mom.height()){
      for(i=sis.width()-1; i>=0; i--)
	for(j=sis.height()-1; j>=0; j--)
	  sis.gene(i,j, (GARandomBit() ? mom.gene(i,j) : dad.gene(i,j)));
    }
    else{
      int minx = GAMin(mom.width(), dad.width());
      int miny = GAMin(mom.height(), dad.height());
      minx = GAMin(sis.width(), minx);
      miny = GAMin(sis.height(), miny);
      for(i=0; i<minx; i++)
	for(j=0; j<miny; j++)
	  sis.gene(i,j, (GARandomBit() ? mom.gene(i,j) : dad.gene(i,j)));
    }

    nc = 1;
  }

  return nc;
}


//   When we do single point crossover on resizable 2D genomes we can either
// clip or pad to make the mismatching geometries work out.  Either way, both
// children end up with the same dimensions (the children have the same 
// dimensions as each other, not the same as if they were clipped/padded).
//   When we pad, the extra space is filled with random bits.  This
// implementation does only clipping, no padding!
int
GA2DBinaryStringGenome::
OnePointCrossover(const GAGenome& p1, const GAGenome& p2, 
		  GAGenome* c1, GAGenome* c2){
  const GA2DBinaryStringGenome &mom=
    DYN_CAST(const GA2DBinaryStringGenome &, p1);
  const GA2DBinaryStringGenome &dad=
    DYN_CAST(const GA2DBinaryStringGenome &, p2);

  int nc=0;
  unsigned int momsitex, momlenx, momsitey, momleny;
  unsigned int dadsitex, dadlenx, dadsitey, dadleny;
  unsigned int sitex, lenx, sitey, leny;

  if(c1 && c2){
    GA2DBinaryStringGenome &sis=DYN_CAST(GA2DBinaryStringGenome &, *c1);
    GA2DBinaryStringGenome &bro=DYN_CAST(GA2DBinaryStringGenome &, *c2);

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
  else if(c1 || c2){
    GA2DBinaryStringGenome &sis = (c1 ?
				   DYN_CAST(GA2DBinaryStringGenome&, *c1) :
				   DYN_CAST(GA2DBinaryStringGenome&, *c2));

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



int
GA2DBinaryStringGenome::
EvenOddCrossover(const GAGenome& p1, const GAGenome& p2, 
		 GAGenome* c1, GAGenome* c2){
  const GA2DBinaryStringGenome &mom=
    DYN_CAST(const GA2DBinaryStringGenome &, p1);
  const GA2DBinaryStringGenome &dad=
    DYN_CAST(const GA2DBinaryStringGenome &, p2);

  int nc=0;
  int i,j;

  if(c1 && c2){
    GA2DBinaryStringGenome &sis=DYN_CAST(GA2DBinaryStringGenome &, *c1);
    GA2DBinaryStringGenome &bro=DYN_CAST(GA2DBinaryStringGenome &, *c2);
    
    if(sis.width() == bro.width() && sis.height() == bro.height() &&
       mom.width() == dad.width() && mom.height() == dad.height() &&
       sis.width() == mom.width() && sis.height() == mom.height()){
      int count=0;
      for(i=sis.width()-1; i>=0; i--){
	for(j=sis.height()-1; j>=0; j--){
	  if(count%2 == 0){
	    sis.gene(i,j, mom.gene(i,j));
	    bro.gene(i,j, dad.gene(i,j));
	  }
	  else{
	    sis.gene(i,j, dad.gene(i,j));
	    bro.gene(i,j, mom.gene(i,j));
	  }
	  count++;
	}
      }
    }
    else{
      int count;
      int minx = GAMin(mom.width(), dad.width());
      int miny = GAMin(mom.height(), dad.height());
      count = 0;
      minx = GAMin(sis.width(), minx);
      miny = GAMin(sis.height(), miny);
      for(i=0; i<minx; i++)
	for(j=0; j<miny; j++)
	  sis.gene(i,j, (((count++)%2 == 0) ? mom.gene(i,j):dad.gene(i,j)));
      count = 0;
      minx = GAMin(bro.width(), minx);
      miny = GAMin(bro.height(), miny);
      for(i=0; i<minx; i++)
	for(j=0; j<miny; j++)
	  bro.gene(i,j, (((count++)%2 == 0) ? dad.gene(i,j):mom.gene(i,j)));
    }

    nc = 2;
  }
  else if(c1 || c2){
    GA2DBinaryStringGenome &sis = (c1 ? 
				   DYN_CAST(GA2DBinaryStringGenome&, *c1) :
				   DYN_CAST(GA2DBinaryStringGenome&, *c2));
    
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
      int minx = GAMin(mom.width(), dad.width());
      int miny = GAMin(mom.height(), dad.height());
      minx = GAMin(sis.width(), minx);
      miny = GAMin(sis.height(), miny);
      for(i=minx-1; i>=0; i--)
	for(j=miny-1; j>=0; j--)
	  sis.gene(i,j, (((i*miny+j)%2 == 0) ? mom.gene(i,j) : dad.gene(i,j)));
    }

    nc = 1;
  }

  return nc;
}
