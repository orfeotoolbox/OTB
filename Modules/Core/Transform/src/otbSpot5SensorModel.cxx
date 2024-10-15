/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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
#include "float.h"


namespace otb
{

Spot5SensorModel::Spot5SensorModel(const std::string& /*productType*/,
                                   const Spot5Param& Spot5Param
                                  ):
                                  m_Spot5Param(Spot5Param),
                                  m_EcefToWorldTransform(otb::GeocentricTransform<otb::TransformDirection::INVERSE, double>::New()),
                                  m_WorldToEcefTransform(otb::GeocentricTransform<otb::TransformDirection::FORWARD, double>::New())

{
  InitBilinearTransform();
}

Spot5SensorModel::Spot5SensorModel(const ImageMetadata & imd)
          : Spot5SensorModel(imd.Has(MDStr::ProductType) ? imd[MDStr::ProductType] : "UNKNOWN",
                            boost::any_cast<Spot5Param>(imd[MDGeom::Spot5Geometry]))
{
}

void Spot5SensorModel::InitBilinearTransform(){
    
    Point2DType ul, ur, lr, ll;

    ul[0] = 0; ul[1] = 0;
    ur[0] = m_Spot5Param.ImageSize[0] - 1; ur[1] = 0;
    ll[0] = 0; ll[1] =  m_Spot5Param.ImageSize[1] - 1;
    lr[0] = m_Spot5Param.ImageSize[0] - 1; lr[1] = m_Spot5Param.ImageSize[1] - 1;

    m_ImageRect = PolygonType::New();
    m_ImageRect->AddVertex(Point2DToIndex(ul));
    m_ImageRect->AddVertex(Point2DToIndex(ur));
    m_ImageRect->AddVertex(Point2DToIndex(lr));
    m_ImageRect->AddVertex(Point2DToIndex(ll));

    const Point3DType ulg(LineSampleToWorld(ul));
    const Point3DType urg(LineSampleToWorld(ur));
    const Point3DType lrg(LineSampleToWorld(lr));
    const Point3DType llg(LineSampleToWorld(ll));

    m_GroundRect = PolygonType::New();
    m_GroundRect->AddVertex(Point3DToIndex(ulg));
    m_GroundRect->AddVertex(Point3DToIndex(urg));
    m_GroundRect->AddVertex(Point3DToIndex(lrg));
    m_GroundRect->AddVertex(Point3DToIndex(llg));

    std::vector<Point2DType> impts {ul,ur,lr,ll};
    std::vector<Point3DType> wrldpts {ulg,urg,lrg,llg};
    m_BilinearProj = BilinearProjection::New();
    m_BilinearProj->setLineSamplePoints(impts);
    m_BilinearProj->setWorldPoints(wrldpts);
    m_BilinearProj->computeLS();
}

Spot5SensorModel::ContinuousIndexType 
Spot5SensorModel::Point2DToIndex(const itk::Point<double, 2> point) const{
  ContinuousIndexType indexPoint;
  indexPoint[0] = point[0];
  indexPoint[1] = point[1];
  return indexPoint;
}

Spot5SensorModel::ContinuousIndexType 
Spot5SensorModel::Point3DToIndex(const itk::Point<double, 3> point) const{
  ContinuousIndexType indexPoint;
  indexPoint[0] = point[0];
  indexPoint[1] = point[1];
  return indexPoint;
}

itk::Point<double, 2> Spot5SensorModel::WorldToLineSample(const Point3DType& inGeoPoint) const
{
  
  static const double PIXEL_THRESHOLD    = .1; // acceptable pixel error
  static const int    MAX_NUM_ITERATIONS = 20;

        
  //***
  // First check if the world point is inside bounding rectangle:
  //***
  int iters = 0;
  Point3DType wdp (inGeoPoint);

  if (!(m_GroundRect->IsInside(Point3DToIndex(wdp)) ||
        m_GroundRect->IsOnEdge(Point3DToIndex(wdp))))
  {
    return m_BilinearProj->worldToLineSample(wdp);
  }


  double height = inGeoPoint[2];


  // Initialize with binilear interpolation
  Point2DType ip(m_BilinearProj->worldToLineSample(wdp));

  Point2DType ip_du;
  Point2DType ip_dv;

  Point3DType gp, gp_du, gp_dv;
  double dlat_du, dlat_dv, dlon_du, dlon_dv;
  double delta_lat, delta_lon, delta_u, delta_v;
  double inverse_norm;

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
    gp = LineSampleHeightToWorld(ip, height);
    gp_du = LineSampleHeightToWorld(ip_du, height);
    gp_dv = LineSampleHeightToWorld(ip_dv, height);

    dlat_du = gp_du[1] - gp[1]; //e
    dlon_du = gp_du[0] - gp[0]; //g
    dlat_dv = gp_dv[1] - gp[1]; //f
    dlon_dv = gp_dv[0] - gp[0]; //h
    

    //
    // Compute linearized estimate of image point given gp delta:
    //
    inverse_norm = dlat_dv*dlon_du - dlat_du*dlon_dv; // fg-eh

    if (std::abs(inverse_norm) > DBL_EPSILON)
    {
      //
      // Test for convergence:
      //
      delta_lat = inGeoPoint[1] - gp[1];
      delta_lon = inGeoPoint[0] - gp[0];
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
    if ((std::abs(delta_u) < PIXEL_THRESHOLD) &&
        (std::abs(delta_v) < PIXEL_THRESHOLD))
      break;
    iters++;
  } while (iters < MAX_NUM_ITERATIONS); /* && ((std::abs(delta_u) < PIXEL_THRESHOLD)
                                               && (std::abs(delta_v) < PIXEL_THRESHOLD)) */

  return ip - m_Spot5Param.SubImageOffset;

}

itk::Point<double, 3> Spot5SensorModel::LineSampleHeightToWorld(Point2DType imPt,
                                             double  heightAboveEllipsoid) const
{

  // Outside the image
  if (!insideImage(imPt, 1.0 - FLT_EPSILON))
  {
    return m_BilinearProj->lineSampleToWorld(imPt);
  }

  // Ecef coordinate point intersection
  // Conversion to lat long coordinates
  return EcefToWorld(NearestIntersection(ImagingRay(imPt), heightAboveEllipsoid));
}

itk::Point<double, 3> Spot5SensorModel::LineSampleToWorld(Point2DType imPt) const
{

  if (!insideImage(imPt, 2.0))
  {
    return m_BilinearProj->lineSampleToWorld(imPt);
  }

  // Determine ray and intersect with terrain model
  return IntersectRay(ImagingRay(imPt));
}




itk::Point<double, 3> Spot5SensorModel::GetBilinearInterpolation(
                                            const double& time,
                                            const std::vector<Point3DType>& V,
                                            const std::vector<double>& T) const
{
  // get iterator to first element >= time
  const auto first_sup_value = std::lower_bound(T.begin(), T.end(), time);

  if(first_sup_value == T.begin())
  {
    return V[0];
  }
  else if(first_sup_value == T.end())
  {
    return V[1];
  }
  else
  {
    const int32_t samp0 = std::distance(T.begin(), first_sup_value);
    double t = (*(first_sup_value-1)-time)/
               (*(first_sup_value-1) - (*first_sup_value));
    const auto& it_v = V.begin() + samp0;

    return *(it_v-1) + ((*it_v)-*(it_v-1))*t;
  }
}

itk::Point<double, 3> Spot5SensorModel::GetLagrangeInterpolation(
                                              const double& time,
                                              const std::vector<Point3DType>& V,
                                              const std::vector<double>& T) const
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
    otbGenericExceptionMacro(itk::ExceptionObject, <<"Invalid vector pass to LagrangeInterpolation");
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

    S[0] += p[0];
    S[1] += p[1];
    S[2] += p[2];
  }

  return S;
}


itk::Point<double, 3> Spot5SensorModel::GetPositionEcf(double time)  const
{
  if((m_Spot5Param.EcefPosSamples.size() < 8)||
    (m_Spot5Param.EcefTimeSamples.size() < 8))
  {
    return GetBilinearInterpolation(time, m_Spot5Param.EcefPosSamples, m_Spot5Param.EcefTimeSamples);
  }
  else
  {
    return GetLagrangeInterpolation(time, m_Spot5Param.EcefPosSamples, m_Spot5Param.EcefTimeSamples);
  }                                       
}


itk::Point<double, 3> Spot5SensorModel::GetVelocityEcf(double time)  const
{
  if((m_Spot5Param.EcefVelSamples.size() < 8)||
    (m_Spot5Param.EcefTimeSamples.size() < 8))
  {
    return GetBilinearInterpolation(time, m_Spot5Param.EcefVelSamples, m_Spot5Param.EcefTimeSamples);
  }
  else
  {
    return GetLagrangeInterpolation(time, m_Spot5Param.EcefVelSamples, m_Spot5Param.EcefTimeSamples);
  }
}

void Spot5SensorModel::GetPixelLookAngleXY(unsigned int line,
                                           double& psiX, double& psiY) const
{
  if (line >= m_Spot5Param.PixelLookAngleX.size())
  {
    otbGenericExceptionMacro(itk::ExceptionObject, <<"Line > number of element in PixelLookAngleX");
  }
  if (line >= m_Spot5Param.PixelLookAngleY.size())
  {
    otbGenericExceptionMacro(itk::ExceptionObject, <<"Line > number of element in PixelLookAngleY");
  }
  psiX = m_Spot5Param.PixelLookAngleX[line];
  psiY = m_Spot5Param.PixelLookAngleY[line];

}

itk::Point<double, 3> Spot5SensorModel::GetAttitude(double time)  const
{
  return GetBilinearInterpolation(time, m_Spot5Param.AttitudesSamples, m_Spot5Param.AttitudesSamplesTimes);
}

itk::Point<double, 3> Spot5SensorModel::NearestIntersection(const Ephemeris& imRay, double offset) const
{
  // WGS 84 parameters conversion
  double wgsA = 6378137.000;
  double wgsB = 6356752.3142;

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
   
  // solve the quadratic
  double root = b*b - 4*a*c;
  double t;
  if(root < 0.0)
  {
    otbGenericExceptionMacro(itk::ExceptionObject, <<"No intersection found for the point, point not valid");
  }
  else
  {
    double squareRoot = sqrt(root);
    double t1 = (-b + squareRoot ) / (2.0*a);
    double t2 = (-b - squareRoot ) / (2.0*a);

    // sort t1 and t2 and take the nearest intersection if they are in front of the ray.
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

  return imRay.position + imRay.velocity * t; 

}

itk::Point<double, 3> Spot5SensorModel::IntersectRay(const Ephemeris& imRay) const 
{

  static const double CONVERGENCE_THRESHOLD = 0.001; // meters
  static const int    MAX_NUM_ITERATIONS    = 50;
  
  double          h_ellips; // height above ellipsoid
  Point3DType     prev_intersect_pt (imRay.position);
  Point3DType     new_intersect_pt;
  double          distance;
  int             iteration_count = 0;

  Point3DType worldPt(EcefToWorld(prev_intersect_pt));

  // Loop to iterate on ray intersection with variable elevation surface:
  do
  {

    // Intersect ray with ellipsoid inflated by h_ellips:
    h_ellips = DEMHandler::GetInstance().GetHeightAboveEllipsoid(worldPt[0], worldPt[1]);
    

    new_intersect_pt = NearestIntersection(imRay, h_ellips);

    // Assign the ground point to the latest iteration's intersection point:
    worldPt = EcefToWorld(new_intersect_pt);
    
    // Determine if convergence achieved:
    //
    distance = (new_intersect_pt - prev_intersect_pt).GetNorm();
    if (distance < CONVERGENCE_THRESHOLD)
      break;
    else
      prev_intersect_pt = new_intersect_pt;

    iteration_count++;

  } while (iteration_count < MAX_NUM_ITERATIONS /* && distance >= CONVERGENCE_THRESHOLD */);

  if (iteration_count == MAX_NUM_ITERATIONS)
  {
    otbLogMacro(Warning, << "WARNING Spot5SensorModel::IntersectRay: Max number of iterations reached solving for ground "
                                    << "point. Result is probably inaccurate." << std::endl);
  }
  
  return worldPt;
}

itk::Matrix<double, 3, 3> Spot5SensorModel::ComputeSatToOrbRotation(double t) const
{
  itk::Matrix<double, 3, 3> result;
  /* Linearly interpolate attitudes angles: */
  Point3DType att(GetAttitude(t));

  /* Compute trig functions to populate rotation matrices: ANGLES IN RADIANS */
  double cp = cos(att[0]);
  double sp = sin(att[0]);
  double cr = cos(att[1]);
  double sr = sin(att[1]);
  double cy = cos(att[2]);
  double sy = sin(att[2]);

  /* Populate rotation matrix: */
  result(0, 0) = (cr*cy);
  result(0, 1) = (-cr*sy);
  result(0, 2) = (-sr);
  result(1, 0) = (cp*sy+sp*sr*cy);
  result(1, 1) = (cp*cy-sp*sr*sy);
  result(1, 2) = (sp*cr);
  result(2, 0) = (-sp*sy+cp*sr*cy);
  result(2, 1) = (-sp*cy-cp*sr*sy);
  result(2, 2) =  cp*cr;

  return result;
}

Ephemeris Spot5SensorModel::ImagingRay(Point2DType imPt)  const
{
  Point2DType imPtTmp;

  imPtTmp[0] = imPt[0] + m_Spot5Param.SubImageOffset[0];
  imPtTmp[1] = imPt[1] + m_Spot5Param.SubImageOffset[1];

  /* 1. Establish time of line imaging: */
  double timeLine = m_Spot5Param.RefLineTime + m_Spot5Param.LineSamplingPeriod*(imPtTmp[1] - m_Spot5Param.RefLineTimeLine);

  /* 2. Interpolate ephemeris position and velocity (in ECF): */
  Point3DType  ecfPos(GetPositionEcf(timeLine));
  Point3DType  ecfVel(GetVelocityEcf(timeLine));

  /* 3. Establish the look direction in Vehicle LSR space (S_sat). ANGLES IN RADIANS*/
  double psiX, psiY;
  GetPixelLookAngleXY(imPtTmp[0], psiX, psiY);

  Vector3DType u_sat;
  u_sat[0] = -tan(psiY);
  u_sat[1] =  tan(psiX);
  u_sat[2] = -1.0;

  /* 4. Transform vehicle LSR space look direction vector to orbital LSR space (S_orb): */
  Vector3DType uOrb = ComputeSatToOrbRotation(timeLine) * u_sat;
  uOrb.Normalize();

  // 5. Transform orbital LSR space look direction vector to ECF.
  //
  //   a. S_orb space Z-axis (zOrb) is || to the ECF radial vector (P_ecf),
  //   b. xOrb axis is computed as cross-product between velocity and radial,
  //   c. yOrb completes the orthogonal S_orb coordinate system.
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

  /* Establish the imaging ray given direction and origin: */
  // Ephemeris.time , Ephemeris.position, Ephemeris.velocity
  return {0, ecfPos, uEcf};
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
