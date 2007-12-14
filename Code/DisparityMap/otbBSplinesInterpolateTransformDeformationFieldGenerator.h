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
#ifndef _otbBSplinesInterpolateTransformDeformationFieldGenerator_h
#define _otbBSplinesInterpolateTransformDeformationFieldGenerator_h

#include "otbPointSetWithTransformToDeformationFieldGenerator.h"
#include "ijBSplineScatteredDataPointSetToImageFilter.h"
#include "otbImage.h"
#include "otbMath.h"
#include <complex>

namespace otb
{
/** \class BSplinesInterpolateTransformDeformationFieldGenerator
 *  \brief This class generate the deformation field by using spline interpolation on the parameters of the transform.
 *
 *  Spline interpolation of non regularly scattered data is provided by a filter from the insight journal, 
 *  ij::BSplineScatteredDataPointSetToImageFilter. It allows interpolation using any spline order and implements a multi-level approach.
 *  
 *  This filter is used for each parameter. One can also specify the indices of the angular parameters. Angular parameters are first 
 *  converted to complex exponential, the interpolated and converted back to the angular space. This is done to avoid interpolating angular discontinuities,
 *  which is a non-sense.
 *
 *  \sa ij::BSplineScatteredDataPointSetToImageFilter
 */
template <class TPointSet, class TDeformationField>
class ITK_EXPORT BSplinesInterpolateTransformDeformationFieldGenerator
  : public PointSetWithTransformToDeformationFieldGenerator<TPointSet, TDeformationField>
{
 public:
  /** Standard typedefs */
  typedef BSplinesInterpolateTransformDeformationFieldGenerator Self;
  typedef PointSetWithTransformToDeformationFieldGenerator<TPointSet,TDeformationField> Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;
  
  /** Type macro */
  itkNewMacro(Self);
  
  /** Creation through object factory macro */
  itkTypeMacro(BSplinesInterpolateTransformDeformationFieldGenerator,PointSetWithTransformsToDeformationFieldGenerator);
  
  /** Template parameters typedefs */
  typedef typename Superclass::PointSetType PointSetType;
  typedef typename Superclass::PointSetPointerType PointSetPointerType;
  typedef typename Superclass::DeformationFieldType DeformationFieldType;
  typedef typename Superclass::DeformationFieldPointerType DeformationFieldPointerType;
  typedef typename Superclass::IndexType IndexType;
  typedef typename DeformationFieldType::PixelType PixelType;
  typedef typename Superclass::ValueType ValueType;
  typedef typename Superclass::PointType PointType;
  typedef typename Superclass::IndexVectorType IndexVectorType;
  typedef typename Superclass::DistanceVectorType DistanceVectorType;
  typedef typename Superclass::ParametersType ParametersType;

  /** Filters definition */
  typedef itk::Vector<ValueType,2> PointSetDataType;
  typedef otb::Image<PointSetDataType,DeformationFieldType::ImageDimension> InternalImageType;
  typedef itk::PointSet<PointSetDataType,PointSetType::PointDimension> InternalPointSetType;
  typedef ij::BSplineScatteredDataPointSetToImageFilter<InternalPointSetType,InternalImageType>
    SPlineInterpolateFilterType;
  typedef typename SPlineInterpolateFilterType::Pointer SPlineInterpolateFilterPointerType;

  /**
   * Add an angular parameter. Angular parameters are transformed by complex exponential during interpolation so as to avoid discontinuity.
   * \param index The index of the angular parameter.
   */
  void AddAngularParameter(unsigned int index);
  /** 
   * Remove an angular parameter.
   * \param index The index of the angular parameter.
   */  
  void RemoveAngularParameter(unsigned int index);
  
  itkSetMacro(SplineOrder,unsigned int);
  itkGetMacro(SplineOrder,unsigned int);
  itkSetMacro(NumberOfControlPoints,unsigned int);
  itkGetMacro(NumberOfControlPoints,unsigned int);
  itkSetMacro(NumberOfLevels,unsigned int);
  itkGetMacro(NumberOfLevels,unsigned int);
  
protected:
  /** Constructor */
  BSplinesInterpolateTransformDeformationFieldGenerator();
  /** Destructor */
  virtual ~BSplinesInterpolateTransformDeformationFieldGenerator() {};
 /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;
  /** Main computation method */
  virtual void GenerateData();

  virtual void GenerateOutputInformation();

  /**
   * Test wether a parameter is angular or not.
   * \param index The index of the parameter to test.
   */
  bool IsAngular(unsigned int index);

private:
  BSplinesInterpolateTransformDeformationFieldGenerator(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  IndexVectorType m_AngularParameters;

  unsigned int m_SplineOrder;
  unsigned int m_NumberOfControlPoints;
  unsigned int m_NumberOfLevels;
};
}// End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbBSplinesInterpolateTransformDeformationFieldGenerator.txx"
#endif

#endif
