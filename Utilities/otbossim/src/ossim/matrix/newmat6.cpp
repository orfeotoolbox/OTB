//$$ newmat6.cpp            Operators, element access, submatrices

// Copyright (C) 1991,2,3,4: R B Davies

#include <ossim/matrix/include.h>

#include <ossim/matrix/newmat.h>
#include <ossim/matrix/newmatrc.h>

#ifdef use_namespace
namespace NEWMAT {
#endif



#ifdef DO_REPORT
#define REPORT { static ExeCounter ExeCount(__LINE__,6); ++ExeCount; }
#else
#define REPORT {}
#endif

/*************************** general utilities *************************/

static int tristore(int n)                      // els in triangular matrix
{ return (n*(n+1))/2; }


/****************************** operators *******************************/

Real& Matrix::operator()(int m, int n)
{
   REPORT
   if (m<=0 || m>nrows || n<=0 || n>ncols)
      Throw(IndexException(m,n,*this));
   return store[(m-1)*ncols+n-1];
}

Real& SymmetricMatrix::operator()(int m, int n)
{
   REPORT
   if (m<=0 || n<=0 || m>nrows || n>ncols)
      Throw(IndexException(m,n,*this));
   if (m>=n) return store[tristore(m-1)+n-1];
   else return store[tristore(n-1)+m-1];
}

Real& UpperTriangularMatrix::operator()(int m, int n)
{
   REPORT
   if (m<=0 || n<m || n>ncols)
      Throw(IndexException(m,n,*this));
   return store[(m-1)*ncols+n-1-tristore(m-1)];
}

Real& LowerTriangularMatrix::operator()(int m, int n)
{
   REPORT
   if (n<=0 || m<n || m>nrows)
      Throw(IndexException(m,n,*this));
   return store[tristore(m-1)+n-1];
}

Real& DiagonalMatrix::operator()(int m, int n)
{
   REPORT
   if (n<=0 || m!=n || m>nrows || n>ncols)
      Throw(IndexException(m,n,*this));
   return store[n-1];
}

Real& DiagonalMatrix::operator()(int m)
{
   REPORT
   if (m<=0 || m>nrows) Throw(IndexException(m,*this));
   return store[m-1];
}

Real& ColumnVector::operator()(int m)
{
   REPORT
   if (m<=0 || m> nrows) Throw(IndexException(m,*this));
   return store[m-1];
}

Real& RowVector::operator()(int n)
{
   REPORT
   if (n<=0 || n> ncols) Throw(IndexException(n,*this));
   return store[n-1];
}

Real& BandMatrix::operator()(int m, int n)
{
   REPORT
   int w = upper+lower+1; int i = lower+n-m;
   if (m<=0 || m>nrows || n<=0 || n>ncols || i<0 || i>=w)
      Throw(IndexException(m,n,*this));
   return store[w*(m-1)+i];
}

Real& UpperBandMatrix::operator()(int m, int n)
{
   REPORT
   int w = upper+1; int i = n-m;
   if (m<=0 || m>nrows || n<=0 || n>ncols || i<0 || i>=w)
      Throw(IndexException(m,n,*this));
   return store[w*(m-1)+i];
}

Real& LowerBandMatrix::operator()(int m, int n)
{
   REPORT
   int w = lower+1; int i = lower+n-m;
   if (m<=0 || m>nrows || n<=0 || n>ncols || i<0 || i>=w)
      Throw(IndexException(m,n,*this));
   return store[w*(m-1)+i];
}

Real& SymmetricBandMatrix::operator()(int m, int n)
{
   REPORT
   int w = lower+1;
   if (m>=n)
   {
      REPORT
      int i = lower+n-m;
      if ( m>nrows || n<=0 || i<0 )
         Throw(IndexException(m,n,*this));
      return store[w*(m-1)+i];
   }
   else
   {
      REPORT
      int i = lower+m-n;
      if ( n>nrows || m<=0 || i<0 )
         Throw(IndexException(m,n,*this));
      return store[w*(n-1)+i];
   }
}


Real Matrix::operator()(int m, int n) const
{
   REPORT
   if (m<=0 || m>nrows || n<=0 || n>ncols)
      Throw(IndexException(m,n,*this));
   return store[(m-1)*ncols+n-1];
}

Real SymmetricMatrix::operator()(int m, int n) const
{
   REPORT
   if (m<=0 || n<=0 || m>nrows || n>ncols)
      Throw(IndexException(m,n,*this));
   if (m>=n) return store[tristore(m-1)+n-1];
   else return store[tristore(n-1)+m-1];
}

Real UpperTriangularMatrix::operator()(int m, int n) const
{
   REPORT
   if (m<=0 || n<m || n>ncols)
      Throw(IndexException(m,n,*this));
   return store[(m-1)*ncols+n-1-tristore(m-1)];
}

Real LowerTriangularMatrix::operator()(int m, int n) const
{
   REPORT
   if (n<=0 || m<n || m>nrows)
      Throw(IndexException(m,n,*this));
   return store[tristore(m-1)+n-1];
}

Real DiagonalMatrix::operator()(int m, int n) const
{
   REPORT
   if (n<=0 || m!=n || m>nrows || n>ncols)
      Throw(IndexException(m,n,*this));
   return store[n-1];
}

Real DiagonalMatrix::operator()(int m) const
{
   REPORT
   if (m<=0 || m>nrows) Throw(IndexException(m,*this));
   return store[m-1];
}

Real ColumnVector::operator()(int m) const
{
   REPORT
   if (m<=0 || m> nrows) Throw(IndexException(m,*this));
   return store[m-1];
}

Real RowVector::operator()(int n) const
{
   REPORT
   if (n<=0 || n> ncols) Throw(IndexException(n,*this));
   return store[n-1];
}

Real BandMatrix::operator()(int m, int n) const
{
   REPORT
   int w = upper+lower+1; int i = lower+n-m;
   if (m<=0 || m>nrows || n<=0 || n>ncols || i<0 || i>=w)
      Throw(IndexException(m,n,*this));
   return store[w*(m-1)+i];
}

Real UpperBandMatrix::operator()(int m, int n) const
{
   REPORT
   int w = upper+1; int i = n-m;
   if (m<=0 || m>nrows || n<=0 || n>ncols || i<0 || i>=w)
      Throw(IndexException(m,n,*this));
   return store[w*(m-1)+i];
}

Real LowerBandMatrix::operator()(int m, int n) const
{
   REPORT
   int w = lower+1; int i = lower+n-m;
   if (m<=0 || m>nrows || n<=0 || n>ncols || i<0 || i>=w)
      Throw(IndexException(m,n,*this));
   return store[w*(m-1)+i];
}

Real SymmetricBandMatrix::operator()(int m, int n) const
{
   REPORT
   int w = lower+1;
   if (m>=n)
   {
      REPORT
      int i = lower+n-m;
      if ( m>nrows || n<=0 || i<0 )
         Throw(IndexException(m,n,*this));
      return store[w*(m-1)+i];
   }
   else
   {
      REPORT
      int i = lower+m-n;
      if ( n>nrows || m<=0 || i<0 )
         Throw(IndexException(m,n,*this));
      return store[w*(n-1)+i];
   }
}


Real BaseMatrix::AsScalar() const
{
   REPORT
   GeneralMatrix* gm = ((BaseMatrix&)*this).Evaluate();

   if (gm->nrows!=1 || gm->ncols!=1)
   {
      Tracer tr("AsScalar");
      Try
         { Throw(ProgramException("Cannot convert to scalar", *gm)); }
      CatchAll { gm->tDelete(); ReThrow; }
   }

   Real x = *(gm->store); gm->tDelete(); return x;
}


AddedMatrix BaseMatrix::operator+(const BaseMatrix& bm) const
{ REPORT return AddedMatrix(this, &bm); }

SPMatrix SP(const BaseMatrix& bm1,const BaseMatrix& bm2)
{ REPORT return SPMatrix(&bm1, &bm2); }

KPMatrix KP(const BaseMatrix& bm1,const BaseMatrix& bm2)
{ REPORT return KPMatrix(&bm1, &bm2); }

MultipliedMatrix BaseMatrix::operator*(const BaseMatrix& bm) const
{ REPORT return MultipliedMatrix(this, &bm); }

ConcatenatedMatrix BaseMatrix::operator|(const BaseMatrix& bm) const
{ REPORT return ConcatenatedMatrix(this, &bm); }

StackedMatrix BaseMatrix::operator&(const BaseMatrix& bm) const
{ REPORT return StackedMatrix(this, &bm); }

SolvedMatrix InvertedMatrix::operator*(const BaseMatrix& bmx) const
{ REPORT return SolvedMatrix(bm, &bmx); }

SubtractedMatrix BaseMatrix::operator-(const BaseMatrix& bm) const
{ REPORT return SubtractedMatrix(this, &bm); }

ShiftedMatrix BaseMatrix::operator+(Real f) const
{ REPORT return ShiftedMatrix(this, f); }

ShiftedMatrix operator+(Real f, const BaseMatrix& BM)
{ REPORT return ShiftedMatrix(&BM, f); }

NegShiftedMatrix operator-(Real f, const BaseMatrix& bm)
{ REPORT return NegShiftedMatrix(f, &bm); }

ScaledMatrix BaseMatrix::operator*(Real f) const
{ REPORT return ScaledMatrix(this, f); }

ScaledMatrix BaseMatrix::operator/(Real f) const
{ REPORT return ScaledMatrix(this, 1.0/f); }

ScaledMatrix operator*(Real f, const BaseMatrix& BM)
{ REPORT return ScaledMatrix(&BM, f); }

ShiftedMatrix BaseMatrix::operator-(Real f) const
{ REPORT return ShiftedMatrix(this, -f); }

TransposedMatrix BaseMatrix::t() const
{ REPORT return TransposedMatrix(this); }

NegatedMatrix BaseMatrix::operator-() const
{ REPORT return NegatedMatrix(this); }

ReversedMatrix BaseMatrix::Reverse() const
{ REPORT return ReversedMatrix(this); }

InvertedMatrix BaseMatrix::i() const
{ REPORT return InvertedMatrix(this); }


RowedMatrix BaseMatrix::AsRow() const
{ REPORT return RowedMatrix(this); }

ColedMatrix BaseMatrix::AsColumn() const
{ REPORT return ColedMatrix(this); }

DiagedMatrix BaseMatrix::AsDiagonal() const
{ REPORT return DiagedMatrix(this); }

MatedMatrix BaseMatrix::AsMatrix(int nrx, int ncx) const
{ REPORT return MatedMatrix(this,nrx,ncx); }


void GeneralMatrix::operator=(Real f)
{ REPORT int i=storage; Real* s=store; while (i--) { *s++ = f; } }

void Matrix::operator=(const BaseMatrix& X)
{
   REPORT //CheckConversion(X);
   // MatrixConversionCheck mcc;
   Eq(X,MatrixType::Rt);
} 

void SquareMatrix::operator=(const BaseMatrix& X)
{
   REPORT //CheckConversion(X);
   // MatrixConversionCheck mcc;
   Eq(X,MatrixType::Rt);
   if (nrows != ncols)
      { Tracer tr("SquareMatrix(=)"); Throw(NotSquareException(*this)); }
}

void SquareMatrix::operator=(const Matrix& m)
{
   REPORT
   if (m.nrows != m.ncols)
      { Tracer tr("SquareMatrix(=Matrix)"); Throw(NotSquareException(*this)); }
   Eq(m);
}

void RowVector::operator=(const BaseMatrix& X)
{
   REPORT  // CheckConversion(X);
   // MatrixConversionCheck mcc;
   Eq(X,MatrixType::RV);
   if (nrows!=1)
      { Tracer tr("RowVector(=)"); Throw(VectorException(*this)); }
}

void ColumnVector::operator=(const BaseMatrix& X)
{
   REPORT //CheckConversion(X);
   // MatrixConversionCheck mcc;
   Eq(X,MatrixType::CV);
   if (ncols!=1)
      { Tracer tr("ColumnVector(=)"); Throw(VectorException(*this)); }
}

void SymmetricMatrix::operator=(const BaseMatrix& X)
{
   REPORT // CheckConversion(X);
   // MatrixConversionCheck mcc;
   Eq(X,MatrixType::Sm);
}

void UpperTriangularMatrix::operator=(const BaseMatrix& X)
{
   REPORT //CheckConversion(X);
   // MatrixConversionCheck mcc;
   Eq(X,MatrixType::UT);
}

void LowerTriangularMatrix::operator=(const BaseMatrix& X)
{
   REPORT //CheckConversion(X);
   // MatrixConversionCheck mcc;
   Eq(X,MatrixType::LT);
}

void DiagonalMatrix::operator=(const BaseMatrix& X)
{
   REPORT // CheckConversion(X);
   // MatrixConversionCheck mcc;
   Eq(X,MatrixType::Dg);
}

void IdentityMatrix::operator=(const BaseMatrix& X)
{
   REPORT // CheckConversion(X);
   // MatrixConversionCheck mcc;
   Eq(X,MatrixType::Id);
}

void GeneralMatrix::operator<<(const Real* r)
{
   REPORT
   int i = storage; Real* s=store;
   while(i--) *s++ = *r++;
}


void GeneralMatrix::operator<<(const int* r)
{
   REPORT
   int i = storage; Real* s=store;
   while(i--) *s++ = *r++;
}


void GenericMatrix::operator=(const GenericMatrix& bmx)
{
   if (&bmx != this) { REPORT if (gm) delete gm; gm = bmx.gm->Image();}
   else { REPORT }
   gm->Protect();
}

void GenericMatrix::operator=(const BaseMatrix& bmx)
{
   if (gm)
   {
      int counter=bmx.search(gm);
      if (counter==0) { REPORT delete gm; gm=0; }
      else { REPORT gm->Release(counter); }
   }
   else { REPORT }
   GeneralMatrix* gmx = ((BaseMatrix&)bmx).Evaluate();
   if (gmx != gm) { REPORT if (gm) delete gm; gm = gmx->Image(); }
   else { REPORT }
   gm->Protect();
}


/*************************** += etc ***************************************/

// will also need versions for SubMatrix



// GeneralMatrix operators

void GeneralMatrix::operator+=(const BaseMatrix& X)
{
   REPORT
   Tracer tr("GeneralMatrix::operator+=");
   // MatrixConversionCheck mcc;
   Protect();                                   // so it cannot get deleted
						// during Evaluate
   GeneralMatrix* gm = ((BaseMatrix&)X).Evaluate();
   AddedMatrix am(this,gm);
   if (gm==this) Release(2); else Release();
   Eq2(am,Type());
}

void GeneralMatrix::operator-=(const BaseMatrix& X)
{
   REPORT
   Tracer tr("GeneralMatrix::operator-=");
   // MatrixConversionCheck mcc;
   Protect();                                   // so it cannot get deleted
						// during Evaluate
   GeneralMatrix* gm = ((BaseMatrix&)X).Evaluate();
   SubtractedMatrix am(this,gm);
   if (gm==this) Release(2); else Release();
   Eq2(am,Type());
}

void GeneralMatrix::operator*=(const BaseMatrix& X)
{
   REPORT
   Tracer tr("GeneralMatrix::operator*=");
   // MatrixConversionCheck mcc;
   Protect();                                   // so it cannot get deleted
						// during Evaluate
   GeneralMatrix* gm = ((BaseMatrix&)X).Evaluate();
   MultipliedMatrix am(this,gm);
   if (gm==this) Release(2); else Release();
   Eq2(am,Type());
}

void GeneralMatrix::operator|=(const BaseMatrix& X)
{
   REPORT
   Tracer tr("GeneralMatrix::operator|=");
   // MatrixConversionCheck mcc;
   Protect();                                   // so it cannot get deleted
						// during Evaluate
   GeneralMatrix* gm = ((BaseMatrix&)X).Evaluate();
   ConcatenatedMatrix am(this,gm);
   if (gm==this) Release(2); else Release();
   Eq2(am,Type());
}

void GeneralMatrix::operator&=(const BaseMatrix& X)
{
   REPORT
   Tracer tr("GeneralMatrix::operator&=");
   // MatrixConversionCheck mcc;
   Protect();                                   // so it cannot get deleted
						// during Evaluate
   GeneralMatrix* gm = ((BaseMatrix&)X).Evaluate();
   StackedMatrix am(this,gm);
   if (gm==this) Release(2); else Release();
   Eq2(am,Type());
}

void GeneralMatrix::operator+=(Real r)
{
   REPORT
   Tracer tr("GeneralMatrix::operator+=(Real)");
   // MatrixConversionCheck mcc;
   ShiftedMatrix am(this,r);
   Release(); Eq2(am,Type());
}

void GeneralMatrix::operator*=(Real r)
{
   REPORT
   Tracer tr("GeneralMatrix::operator*=(Real)");
   // MatrixConversionCheck mcc;
   ScaledMatrix am(this,r);
   Release(); Eq2(am,Type());
}


// Generic matrix operators

void GenericMatrix::operator+=(const BaseMatrix& X)
{
   REPORT
   Tracer tr("GenericMatrix::operator+=");
   if (!gm) Throw(ProgramException("GenericMatrix is null"));
   gm->Protect();            // so it cannot get deleted during Evaluate
   GeneralMatrix* gmx = ((BaseMatrix&)X).Evaluate();
   AddedMatrix am(gm,gmx);
   if (gmx==gm) gm->Release(2); else gm->Release();
   GeneralMatrix* gmy = am.Evaluate();
   if (gmy != gm) { REPORT delete gm; gm = gmy->Image(); }
   else { REPORT }
   gm->Protect();
}

void GenericMatrix::operator-=(const BaseMatrix& X)
{
   REPORT
   Tracer tr("GenericMatrix::operator-=");
   if (!gm) Throw(ProgramException("GenericMatrix is null"));
   gm->Protect();            // so it cannot get deleted during Evaluate
   GeneralMatrix* gmx = ((BaseMatrix&)X).Evaluate();
   SubtractedMatrix am(gm,gmx);
   if (gmx==gm) gm->Release(2); else gm->Release();
   GeneralMatrix* gmy = am.Evaluate();
   if (gmy != gm) { REPORT delete gm; gm = gmy->Image(); }
   else { REPORT }
   gm->Protect();
}

void GenericMatrix::operator*=(const BaseMatrix& X)
{
   REPORT
   Tracer tr("GenericMatrix::operator*=");
   if (!gm) Throw(ProgramException("GenericMatrix is null"));
   gm->Protect();            // so it cannot get deleted during Evaluate
   GeneralMatrix* gmx = ((BaseMatrix&)X).Evaluate();
   MultipliedMatrix am(gm,gmx);
   if (gmx==gm) gm->Release(2); else gm->Release();
   GeneralMatrix* gmy = am.Evaluate();
   if (gmy != gm) { REPORT delete gm; gm = gmy->Image(); }
   else { REPORT }
   gm->Protect();
}

void GenericMatrix::operator|=(const BaseMatrix& X)
{
   REPORT
   Tracer tr("GenericMatrix::operator|=");
   if (!gm) Throw(ProgramException("GenericMatrix is null"));
   gm->Protect();            // so it cannot get deleted during Evaluate
   GeneralMatrix* gmx = ((BaseMatrix&)X).Evaluate();
   ConcatenatedMatrix am(gm,gmx);
   if (gmx==gm) gm->Release(2); else gm->Release();
   GeneralMatrix* gmy = am.Evaluate();
   if (gmy != gm) { REPORT delete gm; gm = gmy->Image(); }
   else { REPORT }
   gm->Protect();
}

void GenericMatrix::operator&=(const BaseMatrix& X)
{
   REPORT
   Tracer tr("GenericMatrix::operator&=");
   if (!gm) Throw(ProgramException("GenericMatrix is null"));
   gm->Protect();            // so it cannot get deleted during Evaluate
   GeneralMatrix* gmx = ((BaseMatrix&)X).Evaluate();
   StackedMatrix am(gm,gmx);
   if (gmx==gm) gm->Release(2); else gm->Release();
   GeneralMatrix* gmy = am.Evaluate();
   if (gmy != gm) { REPORT delete gm; gm = gmy->Image(); }
   else { REPORT }
   gm->Protect();
}

void GenericMatrix::operator+=(Real r)
{
   REPORT
   Tracer tr("GenericMatrix::operator+= (Real)");
   if (!gm) Throw(ProgramException("GenericMatrix is null"));
   ShiftedMatrix am(gm,r);
   gm->Release();
   GeneralMatrix* gmy = am.Evaluate();
   if (gmy != gm) { REPORT delete gm; gm = gmy->Image(); }
   else { REPORT }
   gm->Protect();
}

void GenericMatrix::operator*=(Real r)
{
   REPORT
   Tracer tr("GenericMatrix::operator*= (Real)");
   if (!gm) Throw(ProgramException("GenericMatrix is null"));
   ScaledMatrix am(gm,r);
   gm->Release();
   GeneralMatrix* gmy = am.Evaluate();
   if (gmy != gm) { REPORT delete gm; gm = gmy->Image(); }
   else { REPORT }
   gm->Protect();
}


/************************* element access *********************************/

Real& Matrix::element(int m, int n)
{
   REPORT
   if (m<0 || m>= nrows || n<0 || n>= ncols)
      Throw(IndexException(m,n,*this,true));
   return store[m*ncols+n];
}

Real Matrix::element(int m, int n) const
{
   REPORT
   if (m<0 || m>= nrows || n<0 || n>= ncols)
      Throw(IndexException(m,n,*this,true));
   return store[m*ncols+n];
}

Real& SymmetricMatrix::element(int m, int n)
{
   REPORT
   if (m<0 || n<0 || m >= nrows || n>=ncols)
      Throw(IndexException(m,n,*this,true));
   if (m>=n) return store[tristore(m)+n];
   else return store[tristore(n)+m];
}

Real SymmetricMatrix::element(int m, int n) const
{
   REPORT
   if (m<0 || n<0 || m >= nrows || n>=ncols)
      Throw(IndexException(m,n,*this,true));
   if (m>=n) return store[tristore(m)+n];
   else return store[tristore(n)+m];
}

Real& UpperTriangularMatrix::element(int m, int n)
{
   REPORT
   if (m<0 || n<m || n>=ncols)
      Throw(IndexException(m,n,*this,true));
   return store[m*ncols+n-tristore(m)];
}

Real UpperTriangularMatrix::element(int m, int n) const
{
   REPORT
   if (m<0 || n<m || n>=ncols)
      Throw(IndexException(m,n,*this,true));
   return store[m*ncols+n-tristore(m)];
}

Real& LowerTriangularMatrix::element(int m, int n)
{
   REPORT
   if (n<0 || m<n || m>=nrows)
      Throw(IndexException(m,n,*this,true));
   return store[tristore(m)+n];
}

Real LowerTriangularMatrix::element(int m, int n) const
{
   REPORT
   if (n<0 || m<n || m>=nrows)
      Throw(IndexException(m,n,*this,true));
   return store[tristore(m)+n];
}

Real& DiagonalMatrix::element(int m, int n)
{
   REPORT
   if (n<0 || m!=n || m>=nrows || n>=ncols)
      Throw(IndexException(m,n,*this,true));
   return store[n];
}

Real DiagonalMatrix::element(int m, int n) const
{
   REPORT
   if (n<0 || m!=n || m>=nrows || n>=ncols)
      Throw(IndexException(m,n,*this,true));
   return store[n];
}

Real& DiagonalMatrix::element(int m)
{
   REPORT
   if (m<0 || m>=nrows) Throw(IndexException(m,*this,true));
   return store[m];
}

Real DiagonalMatrix::element(int m) const
{
   REPORT
   if (m<0 || m>=nrows) Throw(IndexException(m,*this,true));
   return store[m];
}

Real& ColumnVector::element(int m)
{
   REPORT
   if (m<0 || m>= nrows) Throw(IndexException(m,*this,true));
   return store[m];
}

Real ColumnVector::element(int m) const
{
   REPORT
   if (m<0 || m>= nrows) Throw(IndexException(m,*this,true));
   return store[m];
}

Real& RowVector::element(int n)
{
   REPORT
   if (n<0 || n>= ncols)  Throw(IndexException(n,*this,true));
   return store[n];
}

Real RowVector::element(int n) const
{
   REPORT
   if (n<0 || n>= ncols)  Throw(IndexException(n,*this,true));
   return store[n];
}

Real& BandMatrix::element(int m, int n)
{
   REPORT
   int w = upper+lower+1; int i = lower+n-m;
   if (m<0 || m>= nrows || n<0 || n>= ncols || i<0 || i>=w)
      Throw(IndexException(m,n,*this,true));
   return store[w*m+i];
}

Real BandMatrix::element(int m, int n) const
{
   REPORT
   int w = upper+lower+1; int i = lower+n-m;
   if (m<0 || m>= nrows || n<0 || n>= ncols || i<0 || i>=w)
      Throw(IndexException(m,n,*this,true));
   return store[w*m+i];
}

Real& UpperBandMatrix::element(int m, int n)
{
   REPORT
   int w = upper+1; int i = n-m;
   if (m<0 || m>= nrows || n<0 || n>= ncols || i<0 || i>=w)
      Throw(IndexException(m,n,*this,true));
   return store[w*m+i];
}

Real UpperBandMatrix::element(int m, int n) const
{
   REPORT
   int w = upper+1; int i = n-m;
   if (m<0 || m>= nrows || n<0 || n>= ncols || i<0 || i>=w)
      Throw(IndexException(m,n,*this,true));
   return store[w*m+i];
}

Real& LowerBandMatrix::element(int m, int n)
{
   REPORT
   int w = lower+1; int i = lower+n-m;
   if (m<0 || m>= nrows || n<0 || n>= ncols || i<0 || i>=w)
      Throw(IndexException(m,n,*this,true));
   return store[w*m+i];
}

Real LowerBandMatrix::element(int m, int n) const
{
   REPORT
   int w = lower+1; int i = lower+n-m;
   if (m<0 || m>= nrows || n<0 || n>= ncols || i<0 || i>=w)
      Throw(IndexException(m,n,*this,true));
   return store[w*m+i];
}

Real& SymmetricBandMatrix::element(int m, int n)
{
   REPORT
   int w = lower+1;
   if (m>=n)
   {
      REPORT
      int i = lower+n-m;
      if ( m>=nrows || n<0 || i<0 )
         Throw(IndexException(m,n,*this,true));
      return store[w*m+i];
   }
   else
   {
      REPORT
      int i = lower+m-n;
      if ( n>=nrows || m<0 || i<0 )
         Throw(IndexException(m,n,*this,true));
      return store[w*n+i];
   }
}

Real SymmetricBandMatrix::element(int m, int n) const
{
   REPORT
   int w = lower+1;
   if (m>=n)
   {
      REPORT
      int i = lower+n-m;
      if ( m>=nrows || n<0 || i<0 )
         Throw(IndexException(m,n,*this,true));
      return store[w*m+i];
   }
   else
   {
      REPORT
      int i = lower+m-n;
      if ( n>=nrows || m<0 || i<0 )
         Throw(IndexException(m,n,*this,true));
      return store[w*n+i];
   }
}

#ifdef use_namespace
}
#endif

