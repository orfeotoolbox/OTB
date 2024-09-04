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

#ifndef otbBilinearProjection_h
#define otbBilinearProjection_h

#include <vector>
#include "itkObject.h"
#include "itkObjectFactory.h"
#include "otbLeastSquareBilinearTransformEstimator.h"

namespace otb
{
/**
 * \class BilinearProjection
 * \brief Computes a bilinear projection after init with reference points
 *
 *
 * \ingroup OTBTransform
 */
class ITK_EXPORT BilinearProjection : public itk::Object
{
public:
  /** Standard class usings. */
  using Self = BilinearProjection;
  using SuperClass = itk::Object;
  using Pointer = itk::SmartPointer<Self>;
  using ConstPointer = itk::SmartPointer<const Self>;
  using Point2DType = itk::Point<double, 2>;
  using Point3DType = itk::Point<double, 3>;
  using LSQREstimatorType = otb::LeastSquareBilinearTransformEstimator<Point2DType>;
  using MatrixType = itk::Matrix<double, 3, 1>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(BilinearProjection, Object);

  Point2DType worldToLineSample(const Point3DType& worldPoint) const;
  /*!
  * METHOD: lineSampleToWorld()
  * Performs the inverse projection from line, sample to ground (world):
  * @return line sample to 3D pos at height 0.0
  */
  Point3DType lineSampleToWorld(Point2DType lineSampPt) const;
   
  /*!
  * METHOD: lineSampleHeightToWorld
  * This projects the image point to the given
  * elevation above ellipsoid, thereby bypassing reference to a DEM. Useful
  * for projections that are sensitive to elevation (such as sensor models).
  * 
  * @return image point to world point at given height
  */
  Point3DType lineSampleHeightToWorld(Point2DType lineSampPt,
                                      double heightAboveEllipsoid) const;

  const std::vector<Point2DType>& getLineSamplePoints() const;

  void setLineSamplePoints(const std::vector<Point2DType>& lsPt);

  const std::vector<Point3DType>& getWorldPoints() const;

  void setWorldPoints(const std::vector<Point3DType>& wPt);

  /** Return true if there is any nan in the points coordinates */
  bool imgPointsHaveNan();

  /** Return true if there is any nan in the points coordinates */
  bool worldPointsHaveNan();

  /** Resolve the bilinear system */
  void computeLS();

protected:
  BilinearProjection();
  BilinearProjection(const Point2DType& ul,
                     const Point2DType& ur,
                     const Point2DType& lr,
                     const Point2DType& ll,
                     const Point3DType& ulg,
                     const Point3DType& urg,
                     const Point3DType& lrg,
                     const Point3DType& llg);
  virtual ~BilinearProjection() = default;

private:
  BilinearProjection(const Self&) = delete;
  void operator=(const Self&) = delete;

  std::vector<Point2DType> m_LineSamplePoints;
  std::vector<Point3DType> m_worldPoints;
  LSQREstimatorType::Pointer m_LonFit;
  LSQREstimatorType::Pointer m_LatFit;
  LSQREstimatorType::Pointer m_XFit;
  LSQREstimatorType::Pointer m_YFit;
};

} // end namespace otb

#endif
