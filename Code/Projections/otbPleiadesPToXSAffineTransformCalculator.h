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
#ifndef __otbPleiadesPToXSAffineTransformCalculator__h
#define __otbPleiadesPToXSAffineTransformCalculator__h

#include "itkLightObject.h"
#include "itkObjectFactory.h"
#include "itkScalableAffineTransform.h"
#include "itkImageBase.h"

namespace otb {
/**
 * \class PleiadesPToXSAffineTransformCalculator
 * \brief Compute the affine transform linking P and XS pixel position for Pleiades sensor bundles
 * 
 * Pleiades sensor bundle are exactly homotetic, it is therefore
 * possible to corregister the pancrhomatic and multispectral images
 * with a simple affine transform without using any sensor
 * modelling. This yelds a very accurate corregistration and avoid the
 * use of a DSM which may cause registration errors due to height errors.
 * 
 * This calculator is a helper class to build the affine transform. It
 * consists in only two static methods: one to check if the transform
 * calculation applies to the given pair of images, the other to
 * actually estimate the transfrom.
 * 
 * The estimated transform returned by the latter transforms
 * pancrhomatic image positions to multispectral image positions. If
 * the inverse transform is needed, one can call the GetInverse()
 * method of the transform to retrieve it.
 * 
 */
class ITK_EXPORT PleiadesPToXSAffineTransformCalculator
  : public itk::LightObject
{
public:
  typedef PleiadesPToXSAffineTransformCalculator Self;
  typedef itk::LightObject                       Superclass;
  typedef itk::SmartPointer<Self>                Pointer;
  typedef itk::SmartPointer<const Self>          ConstPointer;

  itkTypeMacro(PleiadesPToXSAffineTransformCalculator,itk::LightObject);
  
  typedef itk::ScalableAffineTransform<double,2> TransformType;
  
  /** 
   * This function checks if the transform calculation applies to the
   * given pair of images. Checked items are:
   * - Both images are sucessfully undertood by OTB as Pléiades images,
   * - Both images processing level is SENSOR",
   * - XS and Pan ids (except last 4 letters) are equal.
   * \return True if the calculation applies
   */
  static bool CanCompute(const itk::ImageBase<2> * panchromaticImage, const itk::ImageBase<2> * xsImage);
     

  /**
   * This function computes the transform for a pair of image. Note
   * that the CanCompute() method is first called, and that an
   * exception will be raised if computation can not be done.
   * 
   * This function reads both images support data and builds a
   * transform that will exactly coregister the images.
   * 
   * \return The computed transform
   */
  static TransformType::Pointer Compute(const itk::ImageBase<2> * panchromaticImage, const itk::ImageBase<2> * xsImage);
                  
private:
  PleiadesPToXSAffineTransformCalculator(); // purposely not implemented
  PleiadesPToXSAffineTransformCalculator(const Self &); // purposely not implemented
  void operator =(const Self&); // purposely not implemented
};


} // End namespace otb


#endif
