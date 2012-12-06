//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Hicks
//
// Description: Weighted least squares bundle solution.
//
//----------------------------------------------------------------------------

#include <ossim/base/ossimWLSBundleSolution.h>
#include <ossim/base/ossimAdjSolutionAttributes.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNotify.h>

#include <iostream>
#include <iomanip>

static ossimTrace traceDebug(ossimString("ossimWLSBundleSolution:debug"));
static ossimTrace traceExec(ossimString("ossimWLSBundleSolution:exec"));


//*****************************************************************************
//  METHOD: ossimWLSBundleSolution::ossimWLSBundleSolution()
//  
//  Constructor.
//  
//*****************************************************************************
ossimWLSBundleSolution::ossimWLSBundleSolution()
{
}


//*****************************************************************************
//  METHOD: ossimWLSBundleSolution::ossimWLSBundleSolution()
//  
//  Destructor.
//  
//*****************************************************************************
ossimWLSBundleSolution::~ossimWLSBundleSolution()
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG)
      << "DEBUG: ~ossimWLSBundleSolution(): returning..." << std::endl;
}


//*****************************************************************************
//  METHOD: ossimWLSBundleSolution::run()
//  
//  Execute solution.
//  
//*****************************************************************************
bool ossimWLSBundleSolution::run(ossimAdjSolutionAttributes* solAttributes)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "\nossimWLSBundleSolution::run DEBUG:" << std::endl;
   }
   
   theSolValid = false;


   // Initialize traits
   int numObs    = solAttributes->numObjObs();
   int numImages = solAttributes->numImages();

   int Nrank = solAttributes->theImgNumparXref[0];
   //int NdIndex[numImages];
   std::vector<int> NdIndex(numImages);
   NdIndex[0] = 1;
   for (int n=1; n<numImages; ++n)
   {
      NdIndex[n] = NdIndex[n-1] + solAttributes->theImgNumparXref[n-1];
      Nrank += solAttributes->theImgNumparXref[n];
   }

   // N-dot rank before adding ground partition
   int Nd_rank = Nrank;

   // Full rank
   Nrank += numObs*3;

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         <<"\n Bundle........"
         <<"\n   numObs    = "<<numObs
         <<"\n   numImages = "<<numImages
         <<"\n   Nd_rank   = "<<Nd_rank
         <<"\n   Nrank     = "<<Nrank;
      for (int n=0; n<numImages; ++n)
         ossimNotify(ossimNotifyLevel_DEBUG)
            <<"\n    NdIndex     = "<<n<<" "<<NdIndex[n];
      ossimNotify(ossimNotifyLevel_DEBUG)<<std::endl;
   }


   // NORMAL EQUATION ARRAYS
   NEWMAT::UpperTriangularMatrix N(Nrank);//coefficient matrix
   NEWMAT::ColumnVector C(Nrank);         // normal constant vector
	NEWMAT::ColumnVector D(Nrank);         // solution vector 

   // IMAGE PARTITION ARRAYS (for image having "p" parameters)
   NEWMAT::Matrix Bd;                     // [B-dot] matrix            (2Xp)
   NEWMAT::Matrix Bdt_w;                  // [B-dot(t) * w] matrix     (2Xp)
   NEWMAT::UpperTriangularMatrix Nd;      // [N-dot] matrix            (pXp)
   NEWMAT::Matrix Cd;                     // [C-dot] matrix            (pX1)
   NEWMAT::Matrix Nb;                     // [N-bar] matrix            (pX3)
   
   NEWMAT::ColumnVector eps(2);           // image pt residual matrix  (2X1)
   NEWMAT::Matrix w(2,2);                 // image pt weight matrix    (2X2)

   // GROUND PARTITION ARRAYS
   NEWMAT::Matrix Bdd(2,3);               // [B-dbl-dot] matrix        (2X3)
   NEWMAT::Matrix Bddt_w(2,3);            // [B-dbl-dot(t) * w] matrix (2X3)
   NEWMAT::UpperTriangularMatrix Ndd(3);  // [N-dbl-dot] matrix        (3X3)
   NEWMAT::ColumnVector Cdd(3);           // [C-dbl_dot] matrix        (3X1)
   NEWMAT::Matrix Wdd(3,3);               // [W-dbl-dot] matrix        (3X3)


	// initialize N and C partions with weights
   N = 0.0;
   C = 0.0;

   for (int img=0; img<numImages; img++)
   {
      int size = solAttributes->theImgNumparXref[img];
      int rcBeg = NdIndex[img];
      int rcEnd = rcBeg+size-1;

      //  TODO....  theAdjParCov is not stacked because npar/image may vary.
      //            However, it's not treated as a full matrix in the solution due to
      //            to current use of simple partitioning, assuming no correlation.
      NEWMAT::Matrix Wd(size,size);
      Wd = solAttributes->theAdjParCov.SubMatrix(rcBeg,rcEnd,rcBeg,rcEnd).i();

      NEWMAT::ColumnVector Ed(size);
      Ed = solAttributes->theTotalCorrections.Rows(rcBeg,rcEnd);

      N.SymSubMatrix(rcBeg,rcEnd) << Wd;
      C.Rows(rcBeg,rcEnd) = Wd * Ed;
   }

   //*******************
   // object point loop 
   //*******************
   int cMeas = 1;
   int cImgIdx = 1;
   int cObjIdx = 1;

   for (int obs=0; obs<numObs; obs++)
   {
      // Initialize Ndd & Cdd partitions with weight matrix
      int idx = obs*3 + 1;
      Wdd = solAttributes->theObjectPtCov.Rows(idx,idx+2).i();
      int NddIdx = Nd_rank + idx;
      N.SymSubMatrix(NddIdx, NddIdx+2) << Wdd;

      NEWMAT::ColumnVector Edd(3);
      Edd = solAttributes->theTotalCorrections.Rows(NddIdx, NddIdx+2);
      C.Rows(NddIdx, NddIdx+2) = Wdd * Edd;


      //*******************************************
      // image point loop for current object point 
      //*******************************************
      int nMeasOnObs = (int) solAttributes->theObjImgXref.count(obs);
      ObjImgMapIterPair_t imgRng;
      imgRng = solAttributes->theObjImgXref.equal_range(obs);
      ObjImgMapIter_t currImg = imgRng.first;

      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            <<"\n=========obs, nMeasOnObs ========="<<obs<<"  "<<nMeasOnObs;
      }

      for (int meas=0; meas<nMeasOnObs; meas++)
      {

         // object point partials
         Bdd = solAttributes->theObjPartials.Rows(cObjIdx,cObjIdx+2).t();
         cObjIdx += 3;
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)<<"\n cObjIdx "<<cObjIdx;
         }

         //image parameter partials
         int cNumPar = solAttributes->theImgNumparXref[currImg->second];
         Bd = solAttributes->theParPartials.Rows(cImgIdx,cImgIdx+cNumPar-1).t();
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)<<"\n cImgIdx,cNumPar "<<cImgIdx<<"  "<<cNumPar;
         }

         //***********************************************************
         // form all image parameter & object pt parameter partitions 
         //***********************************************************
         int NdIdx = NdIndex[currImg->second];
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)<<"\n NdIdx,NddIdx "<<NdIdx<<"  "<<NddIdx;
         }

         // residuals
		   eps = solAttributes->theMeasResiduals.Row(cMeas).t();
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)<<"\n resid ("<<eps(1)<<","<<eps(2)<<")";
         }

         // compute N-dot & C-dot contributions
         int start = (cMeas-1)*2 + 1;
         w = solAttributes->theImagePtCov.Rows(start,start+1).i();
         Bdt_w << Bd.t() * w;
         Nd    << Bdt_w * Bd;
         Cd    << Bdt_w * eps;

         // compute N-dd & C-dd contributions
         Bddt_w << Bdd.t() * w;
         Ndd    << Bddt_w * Bdd;
         Cdd    << Bddt_w * eps;

         // compute N-bar for PT "obs" & IMAGE "meas"
         Nb << Bdt_w * Bdd;

         // SUM Nd into N
         N.SymSubMatrix(NdIdx,NdIdx+cNumPar-1) += Nd;

         // SUM Ndd into N
         N.SymSubMatrix(NddIdx,NddIdx+2) += Ndd;

         // INSERT Nb into N
         N.SubMatrix(NdIdx,NdIdx+cNumPar-1,NddIdx,NddIdx+2) = Nb;

         // SUM Cd into C
         C.Rows(NdIdx,NdIdx+cNumPar-1) += Cd;

         // SUM Cdd into C
         C.Rows(NddIdx,NddIdx+2) += Cdd;

         // Increment index counters
         cImgIdx += cNumPar;
         cMeas++;
         currImg++;

      }
      //**********************
      // END image point loop 
      //**********************

	}
   //***********************
   // END object point loop 
   //***********************


   //******************************
   // solve normal equation system 
   //******************************
   NEWMAT::LowerTriangularMatrix Nl = N.t();

   // Solve
   //   Note: solveSystem uses 1-based indexing
	if (!solveSystem(Nl.Store()-1, C.Store()-1, D.Store()-1, Nrank))
   {
      theSolValid = false;
   }
   else
   {
      theSolValid = true;

      //******************
      // load corrections 
      //******************
      solAttributes->theLastCorrections = -D;
      solAttributes->theTotalCorrections -= D;


      //************************
      // load covariance matrix 
      //   Note: recurBack uses 1-based indexing
      //************************
      if (recurBack(Nl.Store()-1, Nrank))
      {
         solAttributes->theFullCovMatrix = Nl.t();
      }
      else
      {
         theSolValid = false;
      }
   }

   return theSolValid;
}


//*****************************************************************************
// method: recursive forward solution
//
// output: d = folded matrix
//*****************************************************************************
bool ossimWLSBundleSolution::recurFwd(double* d, double* c, std::vector<double>& tmp, std::vector<int>& nz, int rank)
{
   int i,j,k,jj,jk,ik,nsize,jcol,kcol,ksize,nbr,ncol;
   double r,q,diag;

   nsize = rank*(rank+1)/2;

   for (jcol=rank; jcol>=2; --jcol)
   {
      diag = d[nsize];

      if(diag!=0.0)
      {
         r = 1.0/diag;
         tmp[jcol] = r*c[jcol];
         kcol = jcol-1;
         k = nsize-jcol;
         ksize = k;
         nbr = 0;
         for (i=1;i<=kcol;++i)
         {
            k++;
            if(d[k]!=0.0)
            {
               ++nbr;
               nz[nbr] = k;
               c[i] -= d[k]*tmp[jcol];
            }
         }

         if(nbr!=0.0)
         {
            for (jk=nbr; jk>=1; --jk)
            {
               j = nz[jk];
               q = r*d[j];
               ncol = j-ksize;
               jj = ncol*(ncol-1)/2 - ksize;
               for (ik=jk;ik>=1;--ik)
               {
                  i = nz[ik];
                  k = jj+i;
                  d[k] -= d[i]*q;
               }
               d[j] = q;
            }
         }
         d[nsize] = r;
         nsize -= jcol;
      }
      else
      {
         return false;
      }
   }

   return true;
}


//*****************************************************************************
// method: recursive back solution
//
// output: d = unfolded matrix
//*****************************************************************************
bool ossimWLSBundleSolution::recurBack(double* d, int rank)
{
   int jcol,iq,jcol1,nsize,i,j;
   double sum;
   std::vector<double> tmp(rank+1);//workspace vector

   if(d[1]==0.0)
   {
      return false;
   }
   else
      d[1] = 1.0/d[1];

   for (jcol=2; jcol<=rank; jcol++)
   {
      nsize = jcol*(jcol+1)/2;
      iq = nsize-jcol+1;
      jcol1 = jcol-1;
      trimv(d,d,1,iq,jcol1,tmp);
      sum = 0.0;
      for (i=iq,j=1; i<=iq+jcol1-1; i++,j++)
      {
         if (d[i] != 0.0)
            sum += d[i]*tmp[j];
      }
      moveAndNegate(tmp, d, 1, iq, jcol1);
      d[nsize] += sum;
   }

   tmp.clear();

   return true;
}


//*****************************************************************************
// method: triangular move
//
//*****************************************************************************
void ossimWLSBundleSolution::trimv(double* pc, double* h, int pcIndex, int hIndex, int mr, std::vector<double>& sum)
{
   for (int k=1; k<=mr; k++)
      sum[k]=0.0;

   for (int j=1; j<=mr; j++)
   {
      if (h[j+hIndex-1]!=0.0)
      {
         int k = j*(j-1)/2+1;
         int jadd = j;
         double hj = h[j+hIndex-1];
         for (int i=1; i<=mr; i++)
         {
            sum[i] += pc[k+pcIndex-1]*hj;
            if (i<j)
               k++;
            else
            {
               k += jadd;
               jadd++;
            }
         }
      }
   }
}


//*****************************************************************************
// method: block move and negate
//
// output: to = pointer to new location
//*****************************************************************************
void ossimWLSBundleSolution::moveAndNegate
   (std::vector<double>& from, double* to, int indexFrom, int indexTo, int nElements)
{
   for (int i=indexFrom,j=indexTo; i<=indexFrom+nElements-1; i++,j++)
     to[j] = -from[i]; 
}


/*******************************************************************************
*  method: linear system solution
*
*  input:  d     = upper triangular coeff matrix (rankXrank)
*          c     = constant vector (rank)
*          delta = solution vector (rank)
*
*  output: delta = solution vector
*******************************************************************************/
bool ossimWLSBundleSolution::solveSystem(double* d, double* c, double* delta, int rank)
{
   std::vector<int> nz(rank+1);    //indexing vector
   int i,j,jcol1,k;
   std::vector<double> tmp(rank+1);//workspace vector
   double sum;


   if (!recurFwd(d, c, tmp, nz, rank))
      return false;
   else if (d[1]==0.0)
   {
      return false;
   }
   else
      delta[1] = c[1]/d[1];

   for (j=2; j<=rank; j++)
   {
      jcol1 = j-1;
      k = j*(j-1)/2;
      sum = 0.0;
      for (i=1; i<=jcol1; i++)
      {
         k++;
         sum += d[k]*delta[i];
      }
      delta[j] = tmp[j]-sum;
   }

   nz.clear();
   tmp.clear();

   return true;
}
