//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Hicks
//
// Description: Evaluate geodetic functions.
//
//----------------------------------------------------------------------------

#include <ossim/base/ossimGeodeticEvaluator.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNotify.h>

#include <iostream>
#include <iomanip>

static ossimTrace traceDebug(ossimString("ossimGeodeticEvaluator:debug"));
static ossimTrace traceExec(ossimString("ossimGeodeticEvaluator:exec"));


//*****************************************************************************
//  METHOD: ossimGeodeticEvaluator::ossimGeodeticEvaluator()
//  
//  Constructor.
//  
//*****************************************************************************
ossimGeodeticEvaluator::ossimGeodeticEvaluator(const ossimEllipsoid& ell)
   :
      m_A(ell.getA()),
      m_B(ell.getB()),
      m_F(ell.getFlattening()),
      m_Ecc2(ell.eccentricitySquared())
{
   m_A2 = m_A*m_A;
   m_B2 = m_B*m_B;
   m_2ndEcc2 = (m_A2 - m_B2) / m_B2;
}


//*****************************************************************************
//  METHOD: ossimGeodeticEvaluator::ossimGeodeticEvaluator()
//  
//  Destructor.
//  
//*****************************************************************************
ossimGeodeticEvaluator::~ossimGeodeticEvaluator()
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG)
      << "DEBUG: ~ossimGeodeticEvaluator(): returning..." << std::endl;
}


//*****************************************************************************
//  METHOD: ossimGeodeticEvaluator::direct()
//  
//  Equation numbers reference Vincenty:
//    http://www.ngs.noaa.gov/PUBS_LIB/inverse.pdf
//
//    p2   = unknown point
//    az21 = azimuth 2->1  (back azimuth)
//  
//*****************************************************************************
bool ossimGeodeticEvaluator::direct(const ossimGpt& p1,
                                    const double& d,
                                    const double& az12,
                                    ossimGpt& p2,
                                    double& az21)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "\nossimGeodeticEvaluator::direct DEBUG:" << std::endl;
   }

   bool computationOK = false;

   double phi1 = p1.latr();
   // double lam1 = p1.lonr();

   double cosalpha1 = cos(az12);
   double sinalpha1 = sin(az12);
   double tanU1 = (1.0 - m_F) * tan(phi1);
   double cosU1 = 1.0 / sqrt( 1.0 + tanU1 * tanU1 );
   double sinU1 = tanU1 * cosU1;

   // eq. 1
   double sigma1 = atan2(tanU1, cosalpha1);

   // eq. 2
   double sinalpha = cosU1 * sinalpha1;

   double sin2alpha = sinalpha * sinalpha;
   double cos2alpha = 1.0 - sin2alpha;

   double u2 = cos2alpha * m_2ndEcc2;

   // Eq. 3
   double A = 1 + u2 / 16384 * (4096 + u2 * (-768 + u2 * (320 - 175*u2)));

   // Eq. 4
   double B = u2 / 1024 * (256 + u2 * (-128 + u2 * (74 - 47*u2)));

   // iterate until there is a negligible change in sigma
   double dSig;
   double d_bA = d / (m_B * A);
   double sigma = d_bA;
   double sinsig;
   double cossig;
   double prevSigma = M_PI;
   double sigmaM2;
   double cos2sigm;
   double cos2sigm2;

   while (abs(sigma - prevSigma) > 1.e-12)
   {
      // eq. 5
      sigmaM2 = 2.0*sigma1 + sigma;
      cos2sigm = cos(sigmaM2);
      cos2sigm2 = cos2sigm * cos2sigm;
      sinsig = sin(sigma);
      cossig = cos(sigma);
      double sin2sig = sinsig*sinsig;

      // Eq. 6
      dSig = B * sinsig * (cos2sigm + B / 4 * (cossig * (-1 + 2*cos2sigm2) - B / 6 * cos2sigm * (-3 + 4*sin2sig) * (-3 + 4*cos2sigm2)));

      // eq. 7
      prevSigma = sigma;
      sigma = d_bA + dSig;
   }

   sigmaM2 = 2.0*sigma1 + sigma;
   cos2sigm = cos(sigmaM2);
   cos2sigm2 = cos2sigm * cos2sigm;
   cossig = cos(sigma);
   sinsig = sin(sigma);

   // eq. 8
   double tmp = sinU1*sinsig - cosU1*cossig*cosalpha1;
   double phi2 = atan2( sinU1*cossig + cosU1*sinsig*cosalpha1, 
                        (1.0-m_F) * sqrt( sin2alpha + tmp*tmp));

   // eq. 9
   double lam = atan2(sinsig*sinalpha1, cosU1*cossig - sinU1*sinsig*cosalpha1);

   // eq. 10
   double C = m_F/16 * cos2alpha * (4 + m_F * (4 - 3*cos2alpha));

   // eq. 11
   double L = lam - (1-C) * m_F * sinalpha * (sigma + C * sinsig * (cos2sigm + C * cossig * (-1 + 2*cos2sigm2)));

   // eq. 12
   double alpha2 = atan2(sinalpha, -tmp);


   p2.latr(phi2);
   p2.lonr(p1.lonr() + L);
   az21 = alpha2 + M_PI;
   if (az21 < 0.0)
      az21 += 2.0*M_PI;
   if (az21 > 2.0*M_PI)
      az21 -= 2.0*M_PI;

   return computationOK;
}


//*****************************************************************************
//  METHOD: ossimGeodeticEvaluator::inverse()
//  
//  Equation numbers reference Vincenty:
//    http://www.ngs.noaa.gov/PUBS_LIB/inverse.pdf
//
//    d    = distance (m)
//    az12 = azimuth 1->2
//    az21 = azimuth 2->1  (back azimuth)
//  
//*****************************************************************************
bool ossimGeodeticEvaluator::inverse(const ossimGpt& p1,
                                     const ossimGpt& p2,
                                     double& d,
                                     double& az12,
                                     double& az21)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "\nossimGeodeticEvaluator::inverse DEBUG:" << std::endl;
   }

   bool computationOK = false;

   double phi1 = p1.latr();
   double lam1 = p1.lonr();
   double phi2 = p2.latr();
   double lam2 = p2.lonr();

   double U1 = atan((1.0-m_F) * tan(phi1));
   double sinU1 = sin(U1);
   double cosU1 = cos(U1);

   double U2 = atan((1.0-m_F) * tan(phi2));
   double sinU2 = sin(U2);
   double cosU2 = cos(U2);

   double sinU1sinU2 = sinU1 * sinU2;
   double cosU1sinU2 = cosU1 * sinU2;
   double sinU1cosU2 = sinU1 * cosU2;
   double cosU1cosU2 = cosU1 * cosU2;

   // Eq. 13
   // 1st approximation
   double L = lam2 - lam1;
   double lam = L;


   // Auxiliaries
   double sig = 0.0;
   double dSig = 0.0;
   double lam0;
   double cos2alpha;
   double cos2sigm;
   double cos2sigm2;
   double sinsig;
   double sin2sig;
   double cossig;
   bool converged = false;


   int iter = 0;
   int iterMax = 100;
   int iterMin = 4;

   // Iterative loop
   do 
   {
      iter++;

      double sinlam = sin(lam);
      double coslam = cos(lam);

      // Eq. 14
      double tmp = cosU1sinU2 - sinU1cosU2 * coslam;
      sin2sig = cosU2*sinlam * cosU2*sinlam + tmp * tmp;
      sinsig = sqrt(sin2sig);

      // Eq. 15
      cossig = sinU1sinU2 + (cosU1cosU2 * coslam);

      // Eq. 16
      sig = atan2(sinsig, cossig);

      // Eq. 17
      double sinalpha;
      if (sin2sig == 0.0)
         sinalpha = 0.0;
      else
         sinalpha = cosU1cosU2 * sinlam / sinsig;
      cos2alpha = 1.0 - sinalpha*sinalpha;

      // Eq. 18
      if (cos2alpha == 0.0)
         cos2sigm = 0.0;
      else
         cos2sigm = cossig - 2 * sinU1sinU2 / cos2alpha;
      cos2sigm2 = cos2sigm * cos2sigm;

      // Eq. 10
      double C = m_F/16 * cos2alpha * (4 + m_F * (4 - 3*cos2alpha));

      // Eq. 11
      lam0 = lam;
      lam = L + (1-C) * m_F * sinalpha * (sig + C * sinsig * (cos2sigm + C * cossig * (-1 + 2*cos2sigm2)));


      // Check delta lambda for convergence
      double delta = abs(lam - lam0);
      if (delta < 1e-13 && iter>=iterMin)
      {
        converged = true;
      }

   } while (!converged && iter<=iterMax);


   double u2 = cos2alpha * m_2ndEcc2;

   // Eq. 3
   double A = 1 + u2 / 16384 * (4096 + u2 * (-768 + u2 * (320 - 175*u2)));

   // Eq. 4
   double B = u2 / 1024 * (256 + u2 * (-128 + u2 * (74 - 47*u2)));

   // Eq. 6
   dSig = B * sinsig * (cos2sigm + B / 4 * (cossig * (-1 + 2*cos2sigm2) - B / 6 * cos2sigm * (-3 + 4*sin2sig) * (-3 + 4*cos2sigm2)));


   // Compute distance
   //  Eq. 19
   d = m_B * A * (sig - dSig);


   // Compute azimuths
   //  Diverged, same meridian or antipodal
   if (!converged)
   {
      if (phi1 > phi2)
      {
         az12 = M_PI;
         az21 = 0.0;
      }
      else if (phi1 < phi2)
      {
         az12 = 0.0;
         az21 = M_PI;
      }
      else
      {
         az12 = ossim::nan();
         az21 = ossim::nan();
      }
   }

   // Converged
   else
   {
      // Eq. 20
      az12 = atan2(cosU2 * sin(lam), (cosU1sinU2 - sinU1cosU2 * cos(lam)));
      if (az12 < 0.0) az12 += 2.0*M_PI;

      // Eq. 21
      az21 = atan2(cosU1 * sin(lam), (-sinU1cosU2 + cosU1sinU2 * cos(lam))) + M_PI;
      if (az21 < 0.0) az21 += 2.0*M_PI;

      computationOK = true;
   }

   return computationOK;
 }
