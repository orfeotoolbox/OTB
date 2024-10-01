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

#ifndef otbPleiadesPToXSAffineTransformCalculator__h
#define otbPleiadesPToXSAffineTransformCalculator__h

#include "itkLightObject.h"
#include "itkScalableAffineTransform.h"
#include "itkImageBase.h"
#include "OTBProjectionExport.h"
#include "otbImageMetadata.h"

namespace otb
{
/**
 * \class PleiadesPToXSAffineTransformCalculator
 * \brief Compute the affine transform linking P and XS pixel position for Pleiades sensor bundles
 *
 * Pleiades sensor bundle are exactly homotetic, it is therefore
 * possible to corregister the panchromatic and multispectral images
 * with a simple affine transform without using any sensor
 * modelling. This yields a very accurate corregistration and avoid the
 * use of a DSM which may cause registration errors due to height errors.
 *
 * This calculator is a helper class to build the affine transform. It
 * consists in only two static methods: one to check if the transform
 * calculation applies to the given pair of images, the other to
 * actually estimate the transform.
 *
 * The estimated transform returned by the latter transforms
 * pancrhomatic image positions to multispectral image positions. If
 * the inverse transform is needed, one can call the GetInverse()
 * method of the transform to retrieve it.
 *
 *
 * \ingroup OTBProjection
 */
class OTBProjection_EXPORT PleiadesPToXSAffineTransformCalculator : public itk::LightObject
{
public:
  typedef PleiadesPToXSAffineTransformCalculator Self;
  typedef itk::LightObject                       Superclass;
  typedef itk::SmartPointer<Self>                Pointer;
  typedef itk::SmartPointer<const Self>          ConstPointer;

  itkTypeMacro(PleiadesPToXSAffineTransformCalculator, itk::LightObject);

  typedef itk::ScalableAffineTransform<double, 2> TransformType;
  typedef TransformType::OutputVectorType OffsetType;


  /**
   * This function checks if the transform calculation applies to the
   * given pair of ImageMetadata. Checked items are:
   * - Both ImageMetadata are identified as Pleiades images,
   * - Both images processing level is SENSOR",
   * - The ImageMetadata contains all required metadata
   * - XS and Pan ids (except last 4 letters) are equal.
   * \return True if the calculation applies
   */
  static bool CanCompute(const ImageMetadata & panchromaticImd, const ImageMetadata & xsImd);

  /**
     * This function computes the offset in pan pixels for a pair of
     * ImageMetadata. Note that the CanCompute() method is first called, and
     * that an exception will be raised if computation can not be done.
     *
     * \return The computed transform
     */
  static OffsetType ComputeOffset(const ImageMetadata & panchromaticImd, const ImageMetadata & xsImd);


  /**
   * This function computes the transform for a pair of ImageMetadata. Note
   * that the CanCompute() method is first called, and that an
   * exception will be raised if computation can not be done.
   *
   * This function reads both ImageMetadata support data and builds a
   * transform that will exactly coregister the corresponding images.
   *
   * \return The computed transform
   */
  static TransformType::Pointer Compute(const ImageMetadata & panchromaticImd, const ImageMetadata & xsImd);

private:
  PleiadesPToXSAffineTransformCalculator()            = delete;
  PleiadesPToXSAffineTransformCalculator(const Self&) = delete;
  void operator=(const Self&) = delete;
};


} // End namespace otb


#endif
