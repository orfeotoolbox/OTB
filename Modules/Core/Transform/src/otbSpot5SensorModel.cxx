/*
 * Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "otbSpot5SensorModel.h"
#include "otbDEMHandler.h"

#include "itkVector.hxx"

#include <numeric>

namespace otb
{

/*double DotProduct(const itk::Point<double, 3> & pt1, const itk::Point<double, 3> & pt2)
{
  return std::inner_product(pt1.Begin(), pt1.End(), pt2.Begin(), 0.);
}*/

Spot5SensorModel::Spot5SensorModel( const std::string & productType,
                                const Spot5Param & Spot5Param,
                                const Projection::GCPParam & gcps)
                            : m_ProductType(productType),
                              m_GCP(gcps),
                              m_Spot5Param(Spot5Param),
                              m_AzimuthTimeOffset(MetaData::Duration::Seconds(0)),
                              m_RangeTimeOffset(0.),
                              m_EcefToWorldTransform(otb::GeocentricTransform<otb::TransformDirection::INVERSE, double>::New()),
                              m_WorldToEcefTransform(otb::GeocentricTransform<otb::TransformDirection::FORWARD, double>::New())

{

}

Spot5SensorModel::Spot5SensorModel(const ImageMetadata & imd)
          : Spot5SensorModel(imd.Has(MDStr::ProductType) ? imd[MDStr::ProductType] : "UNKNOWN",
                            boost::any_cast<Spot5Param>(imd[MDGeom::Spot5Geometry]),
                            imd.GetGCPParam())
{
}

void Spot5SensorModel::WorldToLineSample(const Point3DType& inGeoPoint, Point2DType& outLineSample) const
{
  
  static const double PIXEL_THRESHOLD    = .1; // acceptable pixel error
  static const int    MAX_NUM_ITERATIONS = 20;

        
  //***
  // First check if the world point is inside bounding rectangle:
  //***
  int iters = 0;
  Point3DType wdp (inGeoPoint);
  //   if ((!recursionFlag)&&!(theBoundGndPolygon.pointWithin(wdp)))

  //  if((theBoundGndPolygon.getNumberOfVertices() > 0)&&
  //     (!theBoundGndPolygon.hasNans()))
  //  {
  //     if (!(theBoundGndPolygon.pointWithin(wdp)))
  //     {
  //        if(theSeedFunction.valid())
  //        {
  //           theSeedFunction->worldToLineSample(worldPoint, ip);
  //        }
  //        else if(!theExtrapolateGroundFlag) // if I am not already in the extrapolation routine

  //        {
  //        //      recursionFlag = true;
  //           ip = extrapolate(worldPoint);
  //        //      recursionFlag = false;
  //        }
  //        return;
  //     }         
  //  }

  double height = inGeoPoint[2];


  // TODO seed function if else
  Point2DType ip;
  ip[0] = m_Spot5Param.ImageSize[0] / 2;
  ip[1] = m_Spot5Param.ImageSize[1] / 2;

  ip[0] = 97.737153095193207;
  ip[1] = 99.127843534966814;


  Point2DType ip_du;
  Point2DType ip_dv;

  Point3DType gp, gp_du, gp_dv;
  double dlat_du, dlat_dv, dlon_du, dlon_dv;
  double delta_lat, delta_lon, delta_u, delta_v;
  double inverse_norm;
  bool done = false;

  //***
  // Begin iterations:
  //***
  do
  {
    //***
    // establish perturbed image points about the guessed point:
    //***
    ip_du[0] = ip[0] + 1.0;
    ip_du[1] = ip[1];
    ip_dv[0] = ip[0];
    ip_dv[1] = ip[1] + 1.0;

    //***
    // Compute numerical partials at current guessed point:
    //***
    LineSampleHeightToWorld(ip,    height, gp);
    LineSampleHeightToWorld(ip_du, height, gp_du);
    LineSampleHeightToWorld(ip_dv, height, gp_dv);

    // TODO test si nan

    dlat_du = gp_du[1] - gp[1]; //e
    dlon_du = gp_du[0] - gp[0]; //g
    dlat_dv = gp_dv[1] - gp[1]; //f
    dlon_dv = gp_dv[0] - gp[0]; //h
    
    //
    // Test for convergence:
    //
    delta_lat = inGeoPoint[1] - gp[1];
    delta_lon = inGeoPoint[0] - gp[0];    

    //
    // Compute linearized estimate of image point given gp delta:
    //
    inverse_norm = dlat_dv*dlon_du - dlat_du*dlon_dv; // fg-eh

    if (! inverse_norm <= DBL_EPSILON)      
    {
      delta_u = (-dlon_dv*delta_lat + dlat_dv*delta_lon)/inverse_norm;
      delta_v = ( dlon_du*delta_lat - dlat_du*delta_lon)/inverse_norm;
      ip[0] += delta_u;
      ip[1] += delta_v;
    }
    else
    {
      delta_u = 0;
      delta_v = 0;
    }
    done = ((fabs(delta_u) < PIXEL_THRESHOLD)&&
            (fabs(delta_v) < PIXEL_THRESHOLD));
    iters++;
  } while ((!done) && (iters < MAX_NUM_ITERATIONS));

  outLineSample = ip - m_Spot5Param.SubImageOffset;





  

}

void Spot5SensorModel::LineSampleHeightToWorld(const Point2DType& imPt,
                                             double  heightAboveEllipsoid,
                                             Point3DType& worldPt) const
{
  // ossimDrect(0.0, 0.0, theImageSize.x-1.0, theImageSize.y-1.0);
  //  	ul_corner_x, ul_corner_y,	lr_corner_x,	lr_corner_y, 
  // mode = OSSIM_LEFT_HANDED 
  Ephemeris ray;

  double epsilon = 1.0 - FLT_EPSILON;
  // En dehors de l'image
  if (!((imPt[0] >= -epsilon) &&
      (imPt[0] <= (m_Spot5Param.ImageSize[0]+epsilon-1)) &&
      (imPt[1] >= -epsilon) &&
      (imPt[1] <= (m_Spot5Param.ImageSize[1]+epsilon-1))))
  {
    // Compute LineSampleToWorld with a BilinearProjection

    // Init 
    Point2DType ul, ur, lr, ll;
    Point3DType ulg, urg, lrg, llg;

    ul[0] = 0; ul[1] = 0;
    ur[0] = m_Spot5Param.ImageSize[0]; ur[1] = 0;
    ll[0] = 0; ll[1] =  m_Spot5Param.ImageSize[1];
    lr = m_Spot5Param.ImageSize;

    // ??????
    // interp bilinear oui
    LineSampleToWorld(ul, ulg);
    LineSampleToWorld(ur, urg);
    LineSampleToWorld(ll, llg);
    LineSampleToWorld(lr, lrg);

    // ALEX PAS FINI

    // if (theInterpolationPointsHaveNanFlag)
    // {
    //     return;
    // }

    
    // worldPt.lat = theLatFit.lsFitValue(lineSampPt.x, lineSampPt.y);
    // worldPt.lon = theLonFit.lsFitValue(lineSampPt.x, lineSampPt.y);

    // if (theGeographicPt.size())
    // {
    //     worldPt.datum(theGeographicPt[0].datum());
    // }
  }

  ImagingRay(imPt, ray);

  // Ecef coordinate point intersection
  Point3DType intersectPt;

  NearestIntersection(ray, heightAboveEllipsoid, intersectPt);

  // Conversion to lat long coordinates
  worldPt = EcefToWorld(intersectPt);

  


}

void Spot5SensorModel::LineSampleToWorld(const Point2DType& imPt,
                                             Point3DType& worldPt) const
{

  //***
  // Extrapolate if image point is outside image:
  //***
  // TODO ALEX
  // if (!insideImage(image_point)&&(!theExtrapolateImageFlag))
  // {
  //   gpt = extrapolate(image_point);
  //   return;
  // }

  //***
  // Determine imaging ray and invoke elevation source object's services to
  // intersect ray with terrain model:
  //***
  Ephemeris ray;
  ImagingRay(imPt, ray);

  // ray.position[0] = -3095369.7618377847;
  // ray.position[1] = -4697042.7939339997;
  // ray.position[2] = 4493592.0991315609;

  // ray.velocity[0] = 0.51252240549774686;
  // ray.velocity[1] = 0.57856884187777025;
  // ray.velocity[2] = -0.63449103939379559;


  IntersectRay(ray, worldPt);

}




void Spot5SensorModel::GetBilinearInterpolation(
      const double& time,
      const std::vector<Point3DType>& V,
      const std::vector<double>& T,
      Point3DType& li) const
{
  unsigned int samp0 = 0;
  while ((samp0 < T.size()) && (T[samp0] < time)) ++samp0;

  if(samp0==0)
  {
    li = V[0];
  }
  else if(samp0 == T.size())
  {
    li = V[1];
  }
  else
  {
    double t = (T[samp0-1]-time)/(T[samp0-1] - T[samp0]);

    li = V[samp0-1] + (V[samp0]-V[samp0-1])*t;
  }
}

void Spot5SensorModel::GetLagrangeInterpolation(
      const double& time,
      const std::vector<Point3DType>& V,
      const std::vector<double>& T,
      Point3DType& li) const
{
  //
  // Verify that t is within allowable range:
  //
  unsigned int filter_size = 8;
  unsigned int lagrange_half_filter = 4;

  if(T.size() <= filter_size)
  {
    filter_size = (unsigned int)T.size()/2;
    lagrange_half_filter = filter_size/2;
  }
  if ((time <  T[lagrange_half_filter]) ||
      (time >= T[T.size()-lagrange_half_filter] ))
  {
    // Mettre erreur
    return;
  }

  //***
  // Search the sampling time array for surrounding samples:
  //***
  unsigned int samp0 = lagrange_half_filter;
  while ((samp0 < T.size()) && (T[samp0] < time)) ++samp0;

  //***
  // Do not use sample if it falls in neighborhood of desired time:
  //***
  unsigned int bump = 0;
  if (fabs(T[samp0] - time) < m_Spot5Param.LineSamplingPeriod/2.0)
    bump = 1;

  samp0 -= lagrange_half_filter; // adjust to first sample in window

  //***
  // Outer summation loop:
  //***
  Point3DType S;
  S[0] = 0;
  S[1] = 0;
  S[2] = 0;

  for (unsigned int j=samp0; j<(samp0+filter_size+bump); ++j)
  {
    double numerator   = 1.0;
    double denominator = 1.0;

    //***
    // Skip this sample if too close to desired time:
    //***
    if (bump && (j == (samp0+lagrange_half_filter) ))
        ++j;

    //***
    // Inner loop for product series:
    //***
    for (unsigned int i=samp0; i<(samp0+filter_size+bump); ++i)
    {
        //***
        // Skip this sample if too close to desired time:
        //***
        if (bump && (i == (samp0+lagrange_half_filter) ))
          ++i;

        if (i != j)
        {
          numerator   *= time - T[i];
          denominator *= T[j] - T[i];
        }
    }

    Point3DType p = V[j];
    p[0] = p[0] * numerator / denominator;
    p[1] = p[1] * numerator / denominator;
    p[2] = p[2] * numerator / denominator;
    // p = p * numerator;
    // p = p / denominator;

    S[0] += p[0];
    S[1] += p[1];
    S[2] += p[2];
  }

  li = S;

}


void Spot5SensorModel::GetPositionEcf(const double& time,
                                               Point3DType& ecef)  const
{
  //Point3DType tempPt;

  if((m_Spot5Param.EcefPosSamples.size() < 8)||
    (m_Spot5Param.EcefTimeSamples.size() < 8))
  {
    GetBilinearInterpolation(time, m_Spot5Param.EcefPosSamples, m_Spot5Param.EcefTimeSamples, ecef);
  }
  else{
    GetLagrangeInterpolation(time, m_Spot5Param.EcefPosSamples, m_Spot5Param.EcefTimeSamples, ecef);
  }
  // TODO Lagrange ?
  // ossim do 3d point -> ecef conversion, necesary?
                   
}


void Spot5SensorModel::GetVelocityEcf(const double& time,
                                               Point3DType& ecef)  const
{
  if((m_Spot5Param.EcefVelSamples.size() < 8)||
    (m_Spot5Param.EcefTimeSamples.size() < 8))
  {
    GetBilinearInterpolation(time, m_Spot5Param.EcefVelSamples, m_Spot5Param.EcefTimeSamples, ecef);
  }
  else {
    GetLagrangeInterpolation(time, m_Spot5Param.EcefVelSamples, m_Spot5Param.EcefTimeSamples, ecef);
  }
  // TODO Lagrange?                   
}

void Spot5SensorModel::GetPixelLookAngleXY(unsigned int line,
                                          double& psiX, double& psiY) const
{
  if (line >= m_Spot5Param.PixelLookAngleX.size())
  {
    /*setErrorStatus();
    pa = ossim::nan();

    return;*/
  }

  psiX = m_Spot5Param.PixelLookAngleX[line];
  psiY = m_Spot5Param.PixelLookAngleY[line];

  // double line0 = floor(line);
  // double line1 = ceil(line);
  // if (line0 == line1){
  //   psiX = m_Spot5Param.PixelLookAngleX[line];
  //   psiY = m_Spot5Param.PixelLookAngleY[line];
  // }
  // else{

  //   double angleX0 = m_Spot5Param.PixelLookAngleX[line0];
  //   double angleX1 = m_Spot5Param.PixelLookAngleX[line1];
  //   psiX = (angleX0*(line1-line) + angleX1*(line-line0))/(line1-line0);

  //   double angleY0 = m_Spot5Param.PixelLookAngleY[line0];
  //   double angleY1 = m_Spot5Param.PixelLookAngleY[line1];
  //   psiY = (angleY0*(line1-line) + angleY1*(line-line0))/(line1-line0);
  // }
  
}

// Alex from SpotDimapSupport
void Spot5SensorModel::GetAttitude(const double& time,
                                            Point3DType& at)  const
{
   if (m_Spot5Param.AttitudesSamplesTimes.empty())
   {
     // Error
   }

  // TODO
  //  if ((time <  AttitudesSamplesTimes.front()) ||
  //      (time >= AttitudesSamplesTimes.back() ))
  //  {
  //     extrapolateAttitude(time, at);
  //     return;
  //  }

   /* Search the attitude sampling time array for surrounding samples: */
  GetBilinearInterpolation(time, m_Spot5Param.AttitudesSamples, m_Spot5Param.AttitudesSamplesTimes, at);
}

bool Spot5SensorModel::NearestIntersection(const Ephemeris& imRay, const double& offset, Point3DType& worldPt) const
{
  // WGS 84 parameters
  double wgsA = 6378137.000;
  double wgsB = 6356752.3142;
  double flattening = (wgsA * wgsB) / wgsA;
  double eccentricitySquared = 2*flattening - flattening * flattening;

  double wgsASquared = (wgsA + offset)*(wgsA + offset);
  double wgsBSquared = (wgsB + offset)*(wgsB + offset);

  Point3DType start = imRay.position;
  Vector3DType direction = imRay.velocity;

  double a = ((direction[0] * direction[0])/wgsASquared) +
             ((direction[1] * direction[1])/wgsASquared) +
             ((direction[2] * direction[2])/wgsBSquared);

  double b = 2.0*(((start[0]*direction[0])/wgsASquared) +
                  ((start[1]*direction[1])/wgsASquared) +
                  ((start[2]*direction[2])/wgsBSquared) );

  double c =  ((start[0]*start[0])/wgsASquared) +
              ((start[1]*start[1])/wgsASquared) +
              ((start[2]*start[2])/wgsBSquared) - 1.0;
   
  //***
  // solve the quadratic
  //***
  double root = b*b - 4*a*c;
  double t;
  if(root < 0.0)
  {
    return false;
  }
  else
  {
    double squareRoot = sqrt(root);
    double t1 = (-b + squareRoot ) / (2.0*a);
    double t2 = (-b - squareRoot ) / (2.0*a);

    //***
    // sort t1 and t2 and take the nearest intersection if they
    // are in front of the ray.
    //***
    if(t2 < t1)
    {
        double temp = t1;
        t1 = t2;
        t2 = temp;
    }     
    if(t1 > 0.0)
      t = t1;
    else
      t = t2;
  }

  worldPt  = imRay.position + imRay.velocity * t; 

  return true; 

}


void Spot5SensorModel::IntersectRay(const Ephemeris& imRay, Point3DType& worldPt, double defaultElevation) const 
{

  static const double CONVERGENCE_THRESHOLD = 0.001; // meters
  static const int    MAX_NUM_ITERATIONS    = 50;
  
  double          h_ellips; // height above ellipsoid
  bool            intersected;
  Point3DType     prev_intersect_pt (imRay.position);
  Point3DType     new_intersect_pt;
  double          distance;
  bool            done = false;
  int             iteration_count = 0;


  //***
  // Set the initial guess for horizontal intersect position as the ray's
  // origin, and establish the datum and ellipsoid:
  //***
  // const ossimDatum*     datum     = gpt.datum();

  
  // gpt = ossimGpt(prev_intersect_pt, datum);
  worldPt = EcefToWorld(prev_intersect_pt);

  //
  // Loop to iterate on ray intersection with variable elevation surface:
  //
  do
  {
    //
    // Intersect ray with ellipsoid inflated by h_ellips:
    //
    h_ellips = DEMHandler::GetInstance().GetHeightAboveEllipsoid(worldPt[0], worldPt[1]);
    

    intersected = NearestIntersection(imRay,
                                      h_ellips,
                                      new_intersect_pt);
    if (!intersected)
    {
        //
        // No intersection (looking over the horizon), so set ground point
        // to NaNs:
        //
        // ALEX j'ai mis commentaire
        // gpt.makeNan();
        done = true;
    }
    else
    {
        //
        // Assign the ground point to the latest iteration's intersection
        // point:
        //
        worldPt = EcefToWorld(new_intersect_pt);
        
        //
        // Determine if convergence achieved:
        //
        distance = (new_intersect_pt - prev_intersect_pt).GetNorm();
        if (distance < CONVERGENCE_THRESHOLD)
          done = true;
        else
        {
          prev_intersect_pt = new_intersect_pt;
        }
    }

    iteration_count++;

  } while ((!done) && (iteration_count < MAX_NUM_ITERATIONS));

  if (iteration_count == MAX_NUM_ITERATIONS)
  {
    otbLogMacro(Warning, << "WARNING ossimElevSource::intersectRay: Max number of iterations reached solving for ground "
                                    << "point. Result is probably inaccurate." << std::endl);
    
  }
  
}

void Spot5SensorModel::ComputeSatToOrbRotation(MatrixType& result, double t) const
{

  /* Linearly interpolate attitudes angles: */
  
  Point3DType att;
  GetAttitude(t, att);
  // att[0] = -0.0002686775900083758;
  // att[1] = 0.00015700296460361587;
  // att[2] = 0.0010522677392487997;


  //---
  // Apply the attitude adjustable parameters:
  //---
  double dt = m_Spot5Param.RefLineTime - t;
   
  // TODO offset and rate to implement in OTB?
  //  att.x     += thePitchOffset + dt*thePitchRate;
  //  att.y     += theRollOffset  + dt*theRollRate;
  //  att.z     += theYawOffset   + dt*theYawRate;

  //---
  // Compute trig functions to populate rotation matrices: ANGLES IN RADIANS
  //---
  double cp = cos(att[0]);
  double sp = sin(att[0]);
  double cr = cos(att[1]);
  double sr = sin(att[1]);
  double cy = cos(att[2]);
  double sy = sin(att[2]);

  //---
  // Populate rotation matrix:
  //---
  result(0, 0) = (cr*cy);
  result(0, 1) = (-cr*sy);
  result(0, 2) = (-sr);
  result(1, 0) = (cp*sy+sp*sr*cy);
  result(1, 1) = (cp*cy-sp*sr*sy);
  result(1, 2) = (sp*cr);
  result(2, 0) = (-sp*sy+cp*sr*cy);
  result(2, 1) = (-sp*cy-cp*sr*sy);
  result(2, 2) =  cp*cr;
}

void Spot5SensorModel::ImagingRay(const Point2DType& imPt, Ephemeris& imRay)  const
{
  // TimeType azimuthTime;
  // double rangeTime;
  Point3DType sensorPos; 
  Point3DType sensorVel;
  Point2DType imPtTmp;

  imPtTmp[0] = imPt[0] + m_Spot5Param.SubImageOffset[0];
  imPtTmp[1] = imPt[1] + m_Spot5Param.SubImageOffset[1];

  /* 1. Establish time of line imaging: */
  double test = 3281886594.13235;
  double timeLine = m_Spot5Param.RefLineTime + m_Spot5Param.LineSamplingPeriod*(imPtTmp[0] - m_Spot5Param.RefLineTimeLine);

  /* 2. Interpolate ephemeris position and velocity (in ECF): */

  Point3DType  ecfPos;
  Point3DType  ecfVel;

  GetPositionEcf(timeLine, ecfPos);
  GetVelocityEcf(timeLine, ecfVel);

  /* 3. Establish the look direction in Vehicle LSR space (S_sat). ANGLES IN RADIANS*/
  double psiX, psiY;
  GetPixelLookAngleXY(imPtTmp[0], psiX, psiY);

  // Offset theFocalLenOffset à mettre ?
  // ossim ordre sur ce vecteur y,x,z? 
  Vector3DType u_sat;
  u_sat[0] = -tan(psiY);
  u_sat[1] =  tan(psiX);
  u_sat[2] = -1.0;


  /* 4. Transform vehicle LSR space look direction vector to orbital LSR space (S_orb): */
  MatrixType satToOrbit;
  ComputeSatToOrbRotation(satToOrbit, timeLine);

  Vector3DType uOrb = satToOrbit*u_sat;
  uOrb.Normalize(); // == ossim même valeur

  //
  // 5. Transform orbital LSR space look direction vector to ECF.
  //
  //   a. S_orb space Z-axis (zOrb) is || to the ECF radial vector (P_ecf),
  //   b. xOrb axis is computed as cross-product between velocity and radial,
  //   c. yOrb completes the orthogonal S_orb coordinate system.
  //
  Vector3DType zOrb, xOrb, yOrb, tmpVec;
  zOrb[0] = ecfPos[0];
  zOrb[1] = ecfPos[1];
  zOrb[2] = ecfPos[2];
  zOrb.Normalize();

  tmpVec[0] = ecfVel[0];
  tmpVec[1] = ecfVel[1];
  tmpVec[2] = ecfVel[2];

  xOrb = CrossProduct(tmpVec, zOrb); 
  xOrb.Normalize();
  
  yOrb = CrossProduct(zOrb, xOrb);

  MatrixType orbToEcfRotation;
  orbToEcfRotation(0, 0) = xOrb[0]; 
  orbToEcfRotation(0, 1) = yOrb[0];
  orbToEcfRotation(0, 2) = zOrb[0];
  orbToEcfRotation(1, 0) = xOrb[1];
  orbToEcfRotation(1, 1) = yOrb[1];
  orbToEcfRotation(1, 2) = zOrb[1];
  orbToEcfRotation(2, 0) = xOrb[2];
  orbToEcfRotation(2, 1) = yOrb[2];
  orbToEcfRotation(2, 2) = zOrb[2];


  Vector3DType uEcf  = (orbToEcfRotation*uOrb);

  imRay.position = ecfPos;
  imRay.velocity = uEcf;


  /* Establish the imaging ray given direction and origin: */
  //image_ray = ossimEcefRay(P_ecf, ossimEcefVector(u_ecf[0], u_ecf[1], u_ecf[2]));

}


itk::Point<double, 3> Spot5SensorModel::EcefToWorld(const itk::Point<double, 3> & ecefPoint) const
{
  return m_EcefToWorldTransform->TransformPoint(ecefPoint);
}

itk::Point<double, 3> Spot5SensorModel::WorldToEcef(const itk::Point<double, 3> & worldPoint) const
{
  return m_WorldToEcefTransform->TransformPoint(worldPoint);
}



}