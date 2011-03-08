/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbHermitianEigenAnalysis_txx
#define __otbHermitianEigenAnalysis_txx

#include "otbHermitianEigenAnalysis.h"
#include "vnl/vnl_math.h"

namespace otb
{

template< class TMatrix, class TVector, class TEigenMatrix >
unsigned int
HermitianEigenAnalysis< TMatrix, TVector, TEigenMatrix >::
ComputeEigenValues(const TMatrix  & A,
                         TVector  & D) const
{
  double *workArea1 = new double[ m_Dimension ];

  // Copy the input matrix
  double *inputMatrix = new double [ m_Dimension * m_Dimension ];
  
  unsigned int k = 0;

  for( unsigned int row=0; row < m_Dimension; row++ )
    {
    for( unsigned int col=0; col < m_Dimension; col++ )
      {
      inputMatrix[k++] = A(row, col);
      }
    }

  ReduceToTridiagonalMatrix( inputMatrix, D, workArea1, workArea1 );
  const unsigned int eigenErrIndex =
          ComputeEigenValuesUsingQL( D, workArea1 );
  
  delete[] workArea1;
  delete[] inputMatrix;

  return eigenErrIndex; //index of eigen value that could not be computed
}


/*******************************************************************************
Routine  : diagonalisation
Authors  : Eric POTTIER, Laurent FERRO-FAMIL
Creation : 01/2002
Update   :
*-------------------------------------------------------------------------------
Description :  Computes the eigenvectors and eigenvalues of a N*N hermitian
matrix (with N < 10)
*-------------------------------------------------------------------------------
Inputs arguments :
MatrixDim : Dimension of the Hermitian Matrix (N)
HermitianMatrix : N*N*2 complex hermitian matrix
Returned values  :
EigenVect : N*N*2 complex eigenvector matrix
EigenVal  : N elements eigenvalue real vector
*******************************************************************************/

static void HermitianDiagonalisation( float HM[3][3][2], float EigenVect[3][3][2], float EigenVal[3])
{

    double a[10][10][2], v[10][10][2], d[10], b[10], z[10];
    double w[2], s[2], c[2], titi[2], gc[2], hc[2];
    double sm, tresh, x, toto, e, f, g, h, r, d1, d2;
    int n, p, q, ii, i, j, k;

    //n = MatrixDim;
    n=3;
   
  

    for (i = 1; i < n + 1; i++) 
      {
        for (j = 1; j < n + 1; j++) 
          {
            a[i][j][0] = HM[i - 1][j - 1][0];
            a[i][j][1] = HM[i - 1][j - 1][1];
            v[i][j][0] = 0.;
            v[i][j][1] = 0.;
          }
        v[i][i][0] = 1.;
        v[i][i][1] = 0.;
      }

    for (p = 1; p < n + 1; p++) 
      {
        d[p] = a[p][p][0];
        b[p] = d[p];
        z[p] = 0.;
      }

    for (ii = 1; ii < 1000 * n * n; ii++) 
      {
        sm = 0.;
        for (p = 1; p < n; p++) 
          {
            for (q = p + 1; q < n + 1; q++) 
              {
                sm = sm + 2. * sqrt(a[p][q][0] * a[p][q][0] + a[p][q][1] * a[p][q][1]);
              }
          }
        sm = sm / (n * (n - 1));
        if (sm < 1.E-16)
          {
            goto Sortie;
          }
        tresh = 1.E-17;
        if (ii < 4) 
          {
            tresh = (long) 0.2 *sm / (n * n);
          }
        x = -1.E-15;
        for (i = 1; i < n; i++) 
          {
            for (j = i + 1; j < n + 1; j++) 
              {
                toto = sqrt(a[i][j][0] * a[i][j][0] + a[i][j][1] * a[i][j][1]);
                if (x < toto) 
                  {
                    x = toto;
                    p = i;
                    q = j;
                  }
              }
          }
        toto = sqrt(a[p][q][0] * a[p][q][0] + a[p][q][1] * a[p][q][1]);
        if (toto > tresh) 
          {
            e = d[p] - d[q];
            w[0] = a[p][q][0];
            w[1] = a[p][q][1];
            g = sqrt(w[0] * w[0] + w[1] * w[1]);
            g = g * g;
            f = sqrt(e * e + 4. * g);
            d1 = e + f;
            d2 = e - f;
            if (fabs(d2) > fabs(d1)) 
              {
                d1 = d2;
              }
            r = fabs(d1) / sqrt(d1 * d1 + 4. * g);
            s[0] = r;
            s[1] = 0.;
            titi[0] = 2. * r / d1;
            titi[1] = 0.;
            c[0] = titi[0] * w[0] - titi[1] * w[1];
            c[1] = titi[0] * w[1] + titi[1] * w[0];
            r = sqrt(s[0] * s[0] + s[1] * s[1]);
            r = r * r;
            h = (d1 / 2. + 2. * g / d1) * r;
            d[p] = d[p] - h;
            z[p] = z[p] - h;
            d[q] = d[q] + h;
            z[q] = z[q] + h;
            a[p][q][0] = 0.;
            a[p][q][1] = 0.;
            
            for (j = 1; j < p; j++) 
              {
                gc[0] = a[j][p][0];
                gc[1] = a[j][p][1];
                hc[0] = a[j][q][0];
                hc[1] = a[j][q][1];
                a[j][p][0] = c[0] * gc[0] - c[1] * gc[1] - s[0] * hc[0] - s[1] * hc[1];
                a[j][p][1] = c[0] * gc[1] + c[1] * gc[0] - s[0] * hc[1] + s[1] * hc[0];
                a[j][q][0] = s[0] * gc[0] - s[1] * gc[1] + c[0] * hc[0] + c[1] * hc[1];
                a[j][q][1] = s[0] * gc[1] + s[1] * gc[0] + c[0] * hc[1] - c[1] * hc[0];
              }
            for (j = p + 1; j < q; j++) 
              {
                gc[0] = a[p][j][0];
                gc[1] = a[p][j][1];
                hc[0] = a[j][q][0];
                hc[1] = a[j][q][1];
                a[p][j][0] = c[0] * gc[0] + c[1] * gc[1] - s[0] * hc[0] - s[1] * hc[1];
                a[p][j][1] = c[0] * gc[1] - c[1] * gc[0] + s[0] * hc[1] - s[1] * hc[0];
                a[j][q][0] = s[0] * gc[0] + s[1] * gc[1] + c[0] * hc[0] + c[1] * hc[1];
                a[j][q][1] = -s[0] * gc[1] + s[1] * gc[0] + c[0] * hc[1] - c[1] * hc[0];
              }
            for (j = q + 1; j < n + 1; j++) 
              {
                gc[0] = a[p][j][0];
                gc[1] = a[p][j][1];
                hc[0] = a[q][j][0];
                hc[1] = a[q][j][1];
                a[p][j][0] = c[0] * gc[0] + c[1] * gc[1] - s[0] * hc[0] + s[1] * hc[1];
                a[p][j][1] = c[0] * gc[1] - c[1] * gc[0] - s[0] * hc[1] - s[1] * hc[0];
                a[q][j][0] = s[0] * gc[0] + s[1] * gc[1] + c[0] * hc[0] - c[1] * hc[1];
                a[q][j][1] = s[0] * gc[1] - s[1] * gc[0] + c[0] * hc[1] + c[1] * hc[0];
              }
            for (j = 1; j < n + 1; j++) 
              {
                gc[0] = v[j][p][0];
                gc[1] = v[j][p][1];
                hc[0] = v[j][q][0];
                hc[1] = v[j][q][1];
                v[j][p][0] = c[0] * gc[0] - c[1] * gc[1] - s[0] * hc[0] - s[1] * hc[1];
                v[j][p][1] = c[0] * gc[1] + c[1] * gc[0] - s[0] * hc[1] + s[1] * hc[0];
                v[j][q][0] = s[0] * gc[0] - s[1] * gc[1] + c[0] * hc[0] + c[1] * hc[1];
                v[j][q][1] = s[0] * gc[1] + s[1] * gc[0] + c[0] * hc[1] - c[1] * hc[0];
              }
          }
      }
    
 Sortie:
    
    for (k = 1; k < n + 1; k++) 
      {
        d[k] = 0;
        for (i = 1; i < n + 1; i++) 
          {
            for (j = 1; j < n + 1; j++) 
              {
                d[k] = d[k] + v[i][k][0] * (HM[i - 1][j - 1][0] * v[j][k][0] - HM[i - 1][j - 1][1] * v[j][k][1]);
                d[k] = d[k] + v[i][k][1] * (HM[i - 1][j - 1][0] * v[j][k][1] + HM[i - 1][j - 1][1] * v[j][k][0]);
              }
          }
      }
    
    for (i = 1; i < n + 1; i++) 
      {
        for (j = i + 1; j < n + 1; j++) 
          {
            if (d[j] > d[i]) 
              {
                x = d[i];
                d[i] = d[j];
                d[j] = x;
                for (k = 1; k < n + 1; k++) 
                  {
                    c[0] = v[k][i][0];
                    c[1] = v[k][i][1];
                    v[k][i][0] = v[k][j][0];
                    v[k][i][1] = v[k][j][1];
                    v[k][j][0] = c[0];
                    v[k][j][1] = c[1];
                  }
              }
          }
      }
    
    for (i = 0; i < n; i++) 
      {
        EigenVal[i] = d[i + 1];
        for (j = 0; j < n; j++) 
          {
            EigenVect[i][j][0] = v[i + 1][j + 1][0];
            EigenVect[i][j][1] = v[i + 1][j + 1][1];
          }
      }
    
}


template< class TMatrix, class TVector, class TEigenMatrix >
unsigned int
HermitianEigenAnalysis< TMatrix, TVector, TEigenMatrix >::
ComputeEigenValuesAndVectors(
            const TMatrix  & T,
            TVector        & EigenValues,
            TEigenMatrix   & EigenVectors ) const
{
  // double *workArea1 = new double[ m_Dimension ];
//   double *workArea2 = new double [ m_Dimension * m_Dimension ];

  // Copy the input matrix
 
  unsigned int k = 0;
  
  float HM[3][3][2];


// Passage de la matrice T T1....T9  une matrice 3*3*2 compatible avec la methode HermitianDiagonalisation


  HM[0][0][0]=T[0];    HM[0][0][1]=0.;
  HM[0][1][0]=T[3];    HM[0][1][1]=-T[4];
  HM[0][2][0]=T[5];    HM[0][2][1]=-T[6];
  HM[1][0][0]=T[3];    HM[1][0][1]=T[4];
  HM[1][1][0]=T[1];    HM[1][1][1]=0.;
  HM[1][2][0]=T[7];    HM[1][2][1]=-T[8];
  HM[2][0][0]=T[5];    HM[2][0][1]=T[6];
  HM[2][1][0]=T[7];    HM[2][1][1]=T[8];
  HM[2][2][0]=T[2];    HM[2][2][1]=0.;

 


  //Acrobatie pour convectir EigenVectors en un float *** et EigenValues en float*

  float eigenVect[3][3][2];
 float eigenVal[3];

  for (int i=0; i<3; i++)
    for (int j=0; j<3; j++)
      {
       eigenVect[i][j][0]=0.;
       eigenVect[i][j][1]=0.;
      }

  for (int i=0; i<3; i++)
    eigenVal[i]=0.;
    
  
  
  HermitianDiagonalisation( HM, eigenVect, eigenVal);

  
  //std::cout << HM[0][0][0] << " " << HM[0][1][0]<<"+j"<<HM[0][1][1] << " " << HM[0][2][0]<<"+j"<<HM[0][2][1]<<std::endl;
  //std::cout << HM[1][0][0]<<"+j"<<HM[1][0][1] << " " << HM[1][1][0] << " " << HM[1][2][0]<<"+j"<<HM[1][2][1]<<std::endl;
  //std::cout << HM[2][0][0]<<"+j"<<HM[2][0][1] << " " << HM[2][1][0]<<"+j"<< HM[2][1][1] << " " <<HM[2][2][0]<<std::endl;
  //std::cout << "Valeurs propres : " << eigenVal[0] << "...." << eigenVal[1] <<  "...." << eigenVal[2] << std::endl<< std::endl;
   
  
  // Recuperation des sorties
  for (int i=0; i<3; i++)
    for (int j=0; j<3; j++)
       {
          EigenVectors[i][2*j]=eigenVect[i][j][0];
          EigenVectors[i][2*j+1]=eigenVect[i][j][1];
       }
  

  for (int i=0; i<3; i++)
    EigenValues[i]=eigenVal[i];
   
  
  

 
  int eigenErrIndex=0;
  return eigenErrIndex; //index of eigen value that could not be computed
}


template< class TMatrix, class TVector, class TEigenMatrix >
void
HermitianEigenAnalysis< TMatrix, TVector, TEigenMatrix >::
ReduceToTridiagonalMatrix(double * a, VectorType &d,
                          double *e, double *e2) const
{
  double d__1;

  /* Local variables */
  double f, g, h;
  int i, j, k, l;
  double scale;
  
  for (i = 0; i < static_cast< int >(m_Order); ++i)
    {
    d[i] = a[m_Order-1 + i * m_Dimension];
    a[m_Order-1 + i * m_Dimension] = a[i + i * m_Dimension];
    }


  for (i = m_Order-1; i >= 0; --i)
    {
    l = i - 1;
    h = 0.;
    scale = 0.;

    /*     .......... scale row (algol tol then not needed) .......... */
    for (k = 0; k <= l; ++k)
      {
      scale += vnl_math_abs(d[k]);
      }
    if (scale == 0.)
      {
      for (j = 0; j <= l; ++j)
        {
        d[j] = a[l + j * m_Dimension];
        a[l + j * m_Dimension] = a[i + j * m_Dimension];
        a[i + j * m_Dimension] = 0.;
        }
        e[i] = 0.;
        e2[i] = 0.;
        continue;
      }
    for (k = 0; k <= l; ++k)
      {
      d[k] /= scale;
      h += d[k] * d[k];
      }

    e2[i] = scale * scale * h;
    f = d[l];
    d__1 = vcl_sqrt(h);
    g = (-1.0) * vnl_math_sgn0(f) * vnl_math_abs(d__1);
    e[i] = scale * g;
    h -= f * g;
    d[l] = f - g;
    if (l != 0)
      {

      /*     .......... form a*u .......... */
      for (j = 0; j <= l; ++j)
        {
        e[j] = 0.;
        }

      for (j = 0; j <= l; ++j)
        {
        f = d[j];
        g = e[j] + a[j + j * m_Dimension] * f;

        for (k = j+1; k <= l; ++k)
          {
          g += a[k + j * m_Dimension] * d[k];
          e[k] += a[k + j * m_Dimension] * f;
          }
        e[j] = g;
        }
    
      /*     .......... form p .......... */
      f = 0.;

      for (j = 0; j <= l; ++j)
        {
        e[j] /= h;
        f += e[j] * d[j];
        }

      h = f / (h + h);
      /*     .......... form q .......... */
      for (j = 0; j <= l; ++j)
        {
        e[j] -= h * d[j];
        }

      /*     .......... form reduced a .......... */
      for (j = 0; j <= l; ++j)
        {
        f = d[j];
        g = e[j];

        for (k = j; k <= l; ++k)
          {
          a[k + j * m_Dimension] = a[k + j * m_Dimension] - f * e[k] - g * d[k];
          }
        }
      }

    for (j = 0; j <= l; ++j)
      {
      f = d[j];
      d[j] = a[l + j * m_Dimension];
      a[l + j * m_Dimension] = a[i + j * m_Dimension];
      a[i + j * m_Dimension] = f * scale;
      }
    }
}


template< class TMatrix, class TVector, class TEigenMatrix >
void
HermitianEigenAnalysis< TMatrix, TVector, TEigenMatrix >::
ReduceToTridiagonalMatrixAndGetTransformation(double * a, VectorType &d,
                                              double *e, double *z) const
{
  double d__1;

  /* Local variables */
  double f, g, h;
  unsigned int i, j, k, l;
  double scale, hh;

  for (i = 0; i < m_Order; ++i)
    {
    for (j = i; j < m_Order; ++j)
      {
      z[j + i * m_Dimension] = a[j + i * m_Dimension];
      }
    d[i] = a[m_Order -1 + i * m_Dimension];
    }

  for (i = m_Order-1; i > 0; --i)
    {
    l = i - 1;
    h = 0.0;
    scale = 0.0;
    
    /*     .......... scale row (algol tol then not needed) .......... */
    for (k = 0; k <= l; ++k)
      {
      scale += vnl_math_abs(d[k]);
      }
    if (scale == 0.0)
      {
      e[i] = d[l];

      for (j = 0; j <= l; ++j)
        {
        d[j] = z[l + j * m_Dimension];
        z[i + j * m_Dimension] = 0.0;
        z[j + i * m_Dimension] = 0.0;
        }
      }
    else
      {
      for (k = 0; k <= l; ++k)
        {
        d[k] /= scale;
        h += d[k] * d[k];
        }

      f = d[l];
      d__1 = vcl_sqrt(h);
      g = (-1.0) * vnl_math_sgn0(f) * vnl_math_abs(d__1);
      e[i] = scale * g;
      h -= f * g;
      d[l] = f - g;
      
      /*     .......... form a*u .......... */
      for (j = 0; j <= l; ++j)
        {
        e[j] = 0.0;
        }

      for (j = 0; j <= l; ++j)
        {
        f = d[j];
        z[j + i * m_Dimension] = f;
        g = e[j] + z[j + j * m_Dimension] * f;

        for (k = j+1; k <= l; ++k)
          {
          g += z[k + j * m_Dimension] * d[k];
          e[k] += z[k + j * m_Dimension] * f;
          }

          e[j] = g;
        }
      
      /*     .......... form p .......... */
      f = 0.0;

      for (j = 0; j <= l; ++j)
        {
        e[j] /= h;
        f += e[j] * d[j];
        }

      hh = f / (h + h);
      
      /*     .......... form q .......... */
      for (j = 0; j <= l; ++j)
        {
        e[j] -= hh * d[j];
        }

      /*     .......... form reduced a .......... */
      for (j = 0; j <= l; ++j)
        {
        f = d[j];
        g = e[j];

        for (k = j; k <= l; ++k)
          {
          z[k + j * m_Dimension] = z[k + j * m_Dimension] - f * e[k] - g * d[k];
          }

        d[j] = z[l + j * m_Dimension];
        z[i + j * m_Dimension] = 0.0;
        }
      }

    d[i] = h;
    }

  /*     .......... accumulation of transformation matrices .......... */
  for (i = 1; i < m_Order; ++i)
    {
    l = i - 1;
    z[m_Order-1 + l * m_Dimension] = z[l + l * m_Dimension];
    z[l + l * m_Dimension] = 1.0;
    h = d[i];
    if (h != 0.0)
      {
      for (k = 0; k <= l; ++k)
        {
        d[k] = z[k + i * m_Dimension] / h;
        }

      for (j = 0; j <= l; ++j)
        {
        g = 0.0;

        for (k = 0; k <= l; ++k)
          {
          g += z[k + i * m_Dimension] * z[k + j * m_Dimension];
          }

        for (k = 0; k <= l; ++k)
          {
          z[k + j * m_Dimension] -= g * d[k];
          }
        }
      }
    
    for (k = 0; k <= l; ++k)
      {
      z[k + i * m_Dimension] = 0.0;
      }
    
    }
    
  for (i = 0; i < m_Order; ++i)
    {
    d[i] = z[m_Order-1 + i * m_Dimension];
    z[m_Order-1 + i * m_Dimension] = 0.0;
    }

  z[m_Order-1 + (m_Order-1) * m_Dimension] = 1.0;
  e[0] = 0.0;

}


template< class TMatrix, class TVector, class TEigenMatrix >
unsigned int
HermitianEigenAnalysis< TMatrix, TVector, TEigenMatrix >::
ComputeEigenValuesUsingQL(VectorType &d, double *e) const
{
  
  const double c_b10 = 1.0;

  /* Local variables */
  double c, f, g, h;
  unsigned int i, j, l, m;
  double p, r, s, c2, c3=0.0;
  double s2=0.0;
  double dl1, el1;
  double tst1, tst2;

  unsigned int ierr = 0;
  if (m_Order == 1) {
       return 1;
  }

  for (i = 1; i < m_Order; ++i) {
      e[i-1] = e[i];
  }

  f = 0.;
  tst1 = 0.;
  e[m_Order-1] = 0.;

  for (l = 0; l < m_Order; ++l)
    {
    j = 0;
    h = vnl_math_abs(d[l]) + vnl_math_abs(e[l]);
    if (tst1 < h)
      {
      tst1 = h;
      }
    /*     .......... look for small sub-diagonal element .......... */
    for (m = l; m < m_Order; ++m)
      {
      tst2 = tst1 + vnl_math_abs(e[m]);
      if (tst2 == tst1)
        {
        break;
        }
      /*     .......... e(n) is always zero, so there is no exit */
      /*                through the bottom of the loop .......... */
      }

    if (m != l)
      {
    
      do
        {
        if (j == 30)
          {
          /*     .......... set error -- no convergence to an */
          /*                eigenvalue after 30 iterations .......... */
          ierr = l+1;
          return ierr;
          }
        ++j;
        /*     .......... form shift .......... */
        g = d[l];
        p = (d[l+1] - g) / (e[l] * 2.);
        r = vnl_math_hypot(p, c_b10);
        d[l] = e[l] / (p + vnl_math_sgn0(p) * vnl_math_abs(r));
        d[l+1] = e[l] * (p + vnl_math_sgn0(p) * vnl_math_abs(r));
        dl1 = d[l+1];
        h = g - d[l];

        for (i = l+2; i < m_Order; ++i)
        {
            d[i] -= h;
        }

        f += h;
        /*     .......... ql transformation .......... */
        p = d[m];
        c = 1.;
        c2 = c;
        el1 = e[l+1];
        s = 0.;
        for (i = m-1; i >= l; --i)
          {
          c3 = c2;
          c2 = c;
          s2 = s;
          g = c * e[i];
          h = c * p;
          r = vnl_math_hypot(p, e[i]);
          e[i+1] = s * r;
          s = e[i] / r;
          c = p / r;
          p = c * d[i] - s * g;
          d[i+1] = h + s * (c * g + s * d[i]);
          if( i == l )
            {
            break;
            }
          }

        p = -s * s2 * c3 * el1 * e[l] / dl1;
        e[l] = s * p;
        d[l] = c * p;
        tst2 = tst1 + vnl_math_abs(e[l]);
        } while (tst2 > tst1);
      }

    p = d[l] + f;
 
    if( m_OrderEigenValues == OrderByValue )
      {
      // Order by value
      for (i = l; i > 0; --i)
        {
        if (p >= d[i-1])
          break;
        d[i] = d[i-1];
        }
      d[i] = p;
      }
    else if( m_OrderEigenValues == OrderByMagnitude )
      {
      // Order by magnitude.. make eigen values positive
      for (i = l; i > 0; --i)
        {
        if (vnl_math_abs(p) >= vnl_math_abs(d[i-1]))
          break;
        d[i] = vnl_math_abs(d[i-1]);
        }
      d[i] = vnl_math_abs(p);
      }
    else
      {
      d[l] = p;
      }
    }

  return ierr;    //ierr'th eigen value that couldn't be computed
  
}


template< class TMatrix, class TVector, class TEigenMatrix >
unsigned int
HermitianEigenAnalysis< TMatrix, TVector, TEigenMatrix >::
ComputeEigenValuesAndVectorsUsingQL(VectorType &d, double *e, double *z) const
{
  
  const double c_b10 = 1.0;

  /* Local variables */
  double c, f, g, h;
  unsigned int i, j, k, l, m;
  double p, r, s, c2, c3=0.0;
  double s2=0.0;
  double dl1, el1;
  double tst1, tst2;

  unsigned int ierr = 0;
  if (m_Order == 1)
    {
    return 1;
    }

  for (i = 1; i < m_Order; ++i)
    {
    e[i - 1] = e[i];
    }

  f = 0.0;
  tst1 = 0.;
  e[m_Order-1] = 0.;

  for (l = 0; l < m_Order; ++l)
    {
    j = 0;
    h = vnl_math_abs(d[l]) + vnl_math_abs(e[l]);
    if (tst1 < h)
      {
      tst1 = h;
      }
    
    /*     .......... look for small sub-diagonal element .......... */
    for (m = l; m < m_Order; ++m)
      {
      tst2 = tst1 + vnl_math_abs(e[m]);
      if (tst2 == tst1)
        {
        break;
        }
      
      /*     .......... e(n) is always zero, so there is no exit */
      /*                through the bottom of the loop .......... */
      }

    if (m != l)
      {
      do
        {
          
        if (j == 1000)
          {
          /*     .......... set error -- no convergence to an */
          /*                eigenvalue after 1000 iterations .......... */
          ierr = l+1;
          return ierr;
          }
        ++j;
        /*     .......... form shift .......... */
        g = d[l];
        p = (d[l+1] - g) / (e[l] * 2.);
        r = vnl_math_hypot(p, c_b10);
        d[l] = e[l] / (p + vnl_math_sgn0(p) * vnl_math_abs(r));
        d[l+1] = e[l] * (p + vnl_math_sgn0(p) * vnl_math_abs(r));
        dl1 = d[l+1];
        h = g - d[l];

        for (i = l+2; i < m_Order; ++i)
          {
          d[i] -= h;
        }

        f += h;
        /*     .......... ql transformation .......... */
        p = d[m];
        c = 1.0;
        c2 = c;
        el1 = e[l+1];
        s = 0.;
        
        for (i = m-1; i >= l; --i)
          {
          c3 = c2;
          c2 = c;
          s2 = s;
          g = c * e[i];
          h = c * p;
          r = vnl_math_hypot(p, e[i]);
          e[i + 1] = s * r;
          s = e[i] / r;
          c = p / r;
          p = c * d[i] - s * g;
          d[i + 1] = h + s * (c * g + s * d[i]);
          
          /*     .......... form vector .......... */
          for (k = 0; k < m_Order; ++k)
            {
            h = z[k + (i + 1) * m_Dimension];
            z[k + (i + 1) * m_Dimension] = s * z[k + i * m_Dimension] + c * h;
            z[k + i * m_Dimension] = c * z[k + i * m_Dimension] - s * h;
            }
          if( i == l )
            {
            break;
            }
          }

        p = -s * s2 * c3 * el1 * e[l] / dl1;
        e[l] = s * p;
        d[l] = c * p;
        tst2 = tst1 + vnl_math_abs(e[l]);
        } while (tst2 > tst1);
  
      }

    d[l] += f;
    }
  
  /*     .......... order eigenvalues and eigenvectors .......... */
  if( m_OrderEigenValues == OrderByValue )
    {
    // Order by value
    for (i = 0; i < m_Order-1; ++i)
      {
      k = i;
      p = d[i];

      for (j = i+1; j < m_Order; ++j)
        {
        if (d[j] >= p)
          {
          continue;
          }
        k = j;
        p = d[j];
      }

      if (k == i)
        {
        continue;
        }
      d[k] = d[i];
      d[i] = p;

      for (j = 0; j < m_Order; ++j)
        {
        p = z[j + i * m_Dimension];
        z[j + i * m_Dimension] = z[j + k * m_Dimension];
        z[j + k * m_Dimension] = p;
        }
      }
    }
  else if( m_OrderEigenValues == OrderByMagnitude )
    {
    // Order by magnitude
    for (i = 0; i < m_Order-1; ++i)
      {
      k = i;
      p = d[i];

      for (j = i+1; j < m_Order; ++j)
        {
        if (vnl_math_abs(d[j]) >= vnl_math_abs(p))
          {
          continue;
          }
        k = j;
        p = d[j];
      }

      if (k == i)
        {
        continue;
        }
      d[k] = vnl_math_abs(d[i]);
      d[i] = vnl_math_abs(p);

      for (j = 0; j < m_Order; ++j)
        {
        p = z[j + i * m_Dimension];
        z[j + i * m_Dimension] = z[j + k * m_Dimension];
        z[j + k * m_Dimension] = p;
        }
      }
    }
 
  
  return ierr;
}


}  // end namespace otb
 
#endif
 
