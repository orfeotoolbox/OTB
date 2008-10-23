//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
// Contains class declaration for locpt.  This uses geotrans
// local cartesian implementation.
//*******************************************************************
//  $Id: ossimMatrix4x4.cpp 11856 2007-10-12 15:21:17Z gpotts $

#include <ossim/base/ossimMatrix4x4.h>
#include <ossim/matrix/newmatap.h>
#include <ossim/base/ossimConstants.h>

ossimMatrix4x4::ossimMatrix4x4(const NEWMAT::Matrix& m)
   :theData(4,4)
{
   if((m.Nrows() == 4) &&
      (m.Ncols() == 4))
   {
      theData = m;
   }
   else if((m.Nrows()==3)&&
           (m.Ncols()==3))
   {
      theData[0][0] = m[0][0];
      theData[0][1] = m[0][1];
      theData[0][2] = m[0][2];
      theData[0][3] = 0.0;
      theData[1][0] = m[1][0];
      theData[1][1] = m[1][1];
      theData[1][2] = m[1][2];
      theData[1][3] = 0.0;
      theData[2][0] = m[2][0];
      theData[2][1] = m[2][1];
      theData[2][2] = m[2][2];
      theData[2][3] = 0.0;
      theData[3][0] = 0.0;
      theData[3][1] = 0.0;
      theData[3][2] = 0.0;
      theData[3][3] = 1.0;
   }
   else
   {
      theData[0][0] = 1.0;
      theData[0][1] = 0.0;
      theData[0][2] = 0.0;
      theData[0][3] = 0.0;
      
      theData[1][0] = 0.0;
      theData[1][1] = 1.0;
      theData[1][2] = 0.0;
      theData[1][3] = 0.0;
      
      theData[2][0] = 0.0;
      theData[2][1] = 0.0;
      theData[2][2] = 1.0;
      theData[2][3] = 0.0;
      
      theData[3][0] = 0.0;
      theData[3][1] = 0.0;
      theData[3][2] = 0.0;
      theData[3][3] = 1.0;
   }
}

ossimMatrix4x4::ossimMatrix4x4()
  :theData(4, 4)
{
  theData[0][0] = 1.0;
  theData[0][1] = 0.0;
  theData[0][2] = 0.0;
  theData[0][3] = 0.0;

  theData[1][0] = 0.0;
  theData[1][1] = 1.0;
  theData[1][2] = 0.0;
  theData[1][3] = 0.0;

  theData[2][0] = 0.0;
  theData[2][1] = 0.0;
  theData[2][2] = 1.0;
  theData[2][3] = 0.0;

  theData[3][0] = 0.0;
  theData[3][1] = 0.0;
  theData[3][2] = 0.0;
  theData[3][3] = 1.0;
}

ossimMatrix4x4::ossimMatrix4x4(double v00, double v01, double v02, double v03,
                               double v10, double v11, double v12, double v13,
                               double v20, double v21, double v22, double v23,
                               double v30, double v31, double v32, double v33)
  :theData(4, 4)
{
  theData[0][0] = v00;
  theData[0][1] = v01;
  theData[0][2] = v02;
  theData[0][3] = v03;

  theData[1][0] = v10;
  theData[1][1] = v11;
  theData[1][2] = v12;
  theData[1][3] = v13;

  theData[2][0] = v20;
  theData[2][1] = v21;
  theData[2][2] = v22;
  theData[2][3] = v23;

  theData[3][0] = v30;
  theData[3][1] = v31;
  theData[3][2] = v32;
  theData[3][3] = v33;
}
#define QX  q.theVector[0]
#define QY  q.theVector[1]
#define QZ  q.theVector[2]
#define QW  q.theVector[3]
void ossimMatrix4x4::makeRotate( const ossim::Quaternion& quat)
{
   setIdentity();
   setRotate(quat);
}

void ossimMatrix4x4::setRotate(const ossim::Quaternion& quat)
{
    ossim::Quaternion q(quat);
    double length2 = q.length2();
    if (length2!=1.0 && length2!=0)
    {
        // normalize quat if required.
        q /= sqrt(length2);
    }

    // Source: Gamasutra, Rotating Objects Using Quaternions
    //
    //http://www.gamasutra.com/features/19980703/quaternions_01.htm

    double wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

    // calculate coefficients
    x2 = QX + QX;
    y2 = QY + QY;
    z2 = QZ + QZ;

    xx = QX * x2;
    xy = QX * y2;
    xz = QX * z2;

    yy = QY * y2;
    yz = QY * z2;
    zz = QZ * z2;

    wx = QW * x2;
    wy = QW * y2;
    wz = QW * z2;

    theData[0][0] = 1.0 - (yy + zz);
    theData[0][1] = xy - wz;
    theData[0][2] = xz + wy;
    
    theData[1][0] = xy + wz;
    theData[1][1] = 1.0 - (xx + zz);
    theData[1][2] = yz - wx;

    theData[2][0] = xz - wy;
    theData[2][1] = yz + wx;
    theData[2][2] = 1.0 - (xx + yy);
}

ossim::Quaternion ossimMatrix4x4::getRotate()const
{
   ossim::Quaternion quat;

   getRotate(quat);
   
   return quat;
}

void ossimMatrix4x4::getRotate(ossim::Quaternion& q)const
{
   ossim_float64 tr, s;
   ossim_float64 tq[4];
   int    i, j, k;
   
   int nxt[3] = {1, 2, 0};
   
   tr = theData[0][0] + theData[1][1] + theData[2][2]+1.0;
   
   // check the diagonal
   if (tr > 0.0)
   {
      s = (ossim_float64)std::sqrt (tr);
      QW = s / 2.0;
      s = 0.5 / s;
      QX = (theData[2][1] - theData[1][2]) * s;
      QY = (theData[0][2] - theData[2][0]) * s;
      QZ = (theData[1][0] - theData[0][1]) * s;
   }
   else
   {
      // diagonal is negative
      i = 0;
      if (theData[1][1] > theData[0][0])
         i = 1;
      if (theData[2][2] > theData[i][i])
         i = 2;
      j = nxt[i];
      k = nxt[j];
      
      s = (ossim_float64)std::sqrt((theData[i][i] - (theData[j][j] + theData[k][k])) + 1.0);
      
      tq[i] = s * 0.5;
      
      if (s != 0.0)
         s = 0.5 / s;
      
      tq[3] = (theData[k][j] - theData[j][k]) * s;
      tq[j] = (theData[j][i] + theData[i][j]) * s;
      tq[k] = (theData[k][i] + theData[i][k]) * s;
      
      QX = tq[0];
      QY = tq[1];
      QZ = tq[2];
      QW = tq[3];
   }
}

ossimColumnVector3d ossimMatrix4x4::getEigenValues()const
{
  NEWMAT::DiagonalMatrix d;
  NEWMAT::SymmetricMatrix s;

  s << theData;

  NEWMAT::EigenValues(s, d);

  return ossimColumnVector3d(d[0], d[1], d[2]);
}

void ossimMatrix4x4::setZero()
{
  theData[0][0] = 0.0;
  theData[0][1] = 0.0;
  theData[0][2] = 0.0;
  theData[0][3] = 0.0;

  theData[1][0] = 0.0;
  theData[1][1] = 0.0;
  theData[1][2] = 0.0;
  theData[1][3] = 0.0;

  theData[2][0] = 0.0;
  theData[2][1] = 0.0;
  theData[2][2] = 0.0;
  theData[2][3] = 0.0;

  theData[3][0] = 0.0;
  theData[3][1] = 0.0;
  theData[3][2] = 0.0;
  theData[3][3] = 0.0;
}

void ossimMatrix4x4::setIdentity()
{
   setZero();
   theData[0][0] = 1.0;
   theData[1][1] = 1.0;
   theData[2][2] = 1.0;
   theData[3][3] = 1.0;
}

NEWMAT::Matrix ossimMatrix4x4::createIdentity()
{
  ossimMatrix4x4 m(1.0, 0.0, 0.0, 0.0, 
                   0.0, 1.0, 0.0, 0.0,
                   0.0, 0.0, 1.0, 0.0,
                   0.0, 0.0, 0.0, 1.0);
    
  return m.getData();
}

NEWMAT::Matrix ossimMatrix4x4::createRotateOnly(const ossimMatrix4x4 &aMatrix)
{
  ossimMatrix4x4 m = aMatrix;

  m.theData[0][3] = 0;
  m.theData[1][3] = 0;
  m.theData[2][3] = 0;
  m.theData[3][3] = 1.0;

  m.theData[3][0] = 0.0;
  m.theData[3][1] = 0.0;
  m.theData[3][2] = 0.0;
  m.theData[3][3] = 1.0;

  return m.getData();
}

NEWMAT::Matrix ossimMatrix4x4::createZero()
{
  ossimMatrix4x4 m;

  m.getData() = 0.0;

  return m.getData();
}

NEWMAT::Matrix ossimMatrix4x4::createTranslationMatrix(double x,
                                                       double y,
                                                       double z) 
{
   ossimMatrix4x4 m(1.0, 0.0, 0.0, x,
                    0.0, 1.0, 0.0, y,
                    0.0, 0.0, 1.0, z,
                    0.0, 0.0, 0.0, 1.0);
   
   return m.getData();
}

NEWMAT::Matrix ossimMatrix4x4::createRotationMatrix(double angleX,
                                                    double angleY,
                                                    double angleZ,
                                                    ossimCoordSysOrientMode orientationMode)
{
  return (createRotationZMatrix(angleZ,
                                orientationMode)*
	  createRotationYMatrix(angleY,
                                orientationMode)*
	  createRotationXMatrix(angleX,
                                orientationMode));
}

NEWMAT::Matrix ossimMatrix4x4::createRotationXMatrix(double angle,
                                                     ossimCoordSysOrientMode orientationMode)
{
   NEWMAT::Matrix m(4,4);
   
   double Cosine = cos(angle*RAD_PER_DEG);
   double Sine   = sin(angle*RAD_PER_DEG);
   
   if(orientationMode == OSSIM_RIGHT_HANDED)
   {
      m << 1.0 << 0.0 << 0.0 << 0.0
        << 0.0 << Cosine << Sine << 0.0
        << 0.0 << -Sine  << Cosine << 0.0
        << 0.0 << 0.0 << 0.0 << 1.0;
   }
   else
   {
      m << 1.0 << 0.0 << 0.0 << 0.0
        << 0.0 << Cosine << -Sine << 0.0
        << 0.0 << Sine  << Cosine << 0.0
        << 0.0 << 0.0 << 0.0 << 1.0;
   }
  
   return m;
}

NEWMAT::Matrix ossimMatrix4x4::createRotationYMatrix(double angle,
                                                     ossimCoordSysOrientMode orientationMode)
{
   NEWMAT::Matrix m(4,4);
   
   double Cosine = cos(angle*RAD_PER_DEG);
   double Sine   = sin(angle*RAD_PER_DEG);
   
   if(orientationMode == OSSIM_RIGHT_HANDED)
   {
      m << Cosine << 0.0 << -Sine  << 0.0
        << 0.0    << 1.0 << 0.0    << 0.0
        << Sine   << 0.0 << Cosine << 0.0
        << 0.0    << 0.0 << 0.0    << 1.0;
   }
   else
   {
      m << Cosine << 0.0 << Sine   << 0.0
        << 0.0    << 1.0 << 0.0    << 0.0
        << -Sine  << 0.0 << Cosine << 0.0
        << 0.0    << 0.0 << 0.0    << 1.0;
   }
   
  return m;
}


NEWMAT::Matrix ossimMatrix4x4::createRotationZMatrix(double angle,
                                                     ossimCoordSysOrientMode orientationMode)
{
   NEWMAT::Matrix m(4,4);
   
   double Cosine = cos(angle*RAD_PER_DEG);
   double Sine   = sin(angle*RAD_PER_DEG);
   
   if(orientationMode == OSSIM_RIGHT_HANDED)
   {
      m << Cosine << Sine   << 0.0 << 0.0
        << -Sine  << Cosine << 0.0 << 0.0
        << 0.0    << 0.0    << 1.0 << 0.0
        << 0.0    << 0.0    << 0.0 << 1.0;
   }
   else
   {
      m << Cosine << -Sine   << 0.0 << 0.0
        << Sine   << Cosine << 0.0 << 0.0
        << 0.0    << 0.0    << 1.0 << 0.0
        << 0.0    << 0.0    << 0.0 << 1.0;
   }
   
   return m;
}


NEWMAT::Matrix ossimMatrix4x4::createScaleMatrix(double x, double y, double z)
{
   NEWMAT::Matrix m(4, 4);
   
      m << x   << 0.0 << 0.0 << 0.0
        << 0.0 << y   << 0.0 << 0.0
        << 0.0 << 0.0 << z   << 0.0
        << 0.0 << 0.0 << 0.0 << 1.0;

    return m;
}
