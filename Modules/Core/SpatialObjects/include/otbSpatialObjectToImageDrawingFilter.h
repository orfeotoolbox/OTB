/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbSpatialObjectToImageDrawingFilter_h
#define otbSpatialObjectToImageDrawingFilter_h

#include "itkImageSource.h"
#include "itkConceptChecking.h"
#include "itkSpatialObject.h"

namespace otb
{

/** \class SpatialObjectToImageDrawingFilter
 * \brief Base class for filters that take a SpatialObject
 *        as input and produce an image as output.
 *  By default, if the user does not specify the size of the output image,
 *  the maximum size of the object's bounding box is used.
 *  The spacing of the image is given by the spacing of the input
 *  Spatial object.
 *
 * \ingroup OTBSpatialObjects
 */
template <class TInputSpatialObject, class TOutputImage>
class ITK_EXPORT SpatialObjectToImageDrawingFilter : public itk::ImageSource<TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef SpatialObjectToImageDrawingFilter     Self;
  typedef itk::ImageSource<TOutputImage>        Superclass;
  typedef itk::SmartPointer<Self>               Pointer;
  typedef itk::SmartPointer<const Self>         ConstPointer;
  typedef typename TOutputImage::SizeType       SizeType;
  typedef typename TOutputImage::PointType      PointType;
  typedef TOutputImage                          OutputImageType;
  typedef typename OutputImageType::Pointer     OutputImagePointer;
  typedef typename OutputImageType::ValueType   ValueType;
  typedef typename OutputImageType::SpacingType SpacingType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SpatialObjectToImageDrawingFilter, itk::ImageSource);

  /** Superclass typedefs. */
  typedef typename Superclass::OutputImageRegionType OutputImageRegionType;

  /** Some convenient typedefs. */
  typedef TInputSpatialObject                            InputSpatialObjectType;
  typedef typename InputSpatialObjectType::Pointer       InputSpatialObjectPointer;
  typedef typename InputSpatialObjectType::ConstPointer  InputSpatialObjectConstPointer;
  typedef typename TInputSpatialObject::ChildrenListType ChildrenListType;

  /** ImageDimension constants */
  itkStaticConstMacro(ObjectDimension, unsigned int,
                      InputSpatialObjectType::ObjectDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  /** Set/Get the image input of this process object.  */
  using Superclass::SetInput;
  virtual void SetInput(const InputSpatialObjectType *object);
  virtual void SetInput(unsigned int, const InputSpatialObjectType * object);
  const InputSpatialObjectType * GetInput(void);
  const InputSpatialObjectType * GetInput(unsigned int idx);

  /** Spacing (size of a pixel) of the output image. The
   * spacing is the geometric distance between image samples.
   * It is stored internally as double, but may be set from
   * float. \sa GetSpacing() */
  virtual void SetSpacing(const SpacingType& spacing);
  virtual void SetSpacing(const double* spacing);
  virtual void SetSpacing(const float* spacing);
  virtual const double* GetSpacing() const;

  /** Set/Get the value for pixels inside the spatial object.
  * By default, this filter will return an image
  * that contains values from the spatial object specified as input.
  * If this "inside" value is changed to a non-null value,
  * the output produced by this filter will be a mask with inside/outside values
  * specified by the user. */
  itkSetMacro(InsideValue, ValueType);
  itkGetMacro(InsideValue, ValueType);

  /** Set/Get the value for pixels outside the spatial object.
  * By default, this filter will return an image
  * that contains values from the spatial object specified as input.
  * If this "outside" value is changed to a non-null value,
  * the output produced by this filter will be a mask with inside/outside values
  * specified by the user. */
  itkSetMacro(OutsideValue, ValueType);
  itkGetMacro(OutsideValue, ValueType);

  /** The origin of the output image. The origin is the geometric
   * coordinates of the index (0, 0, ..., 0).  It is stored internally
   * as double but may be set from float.
   * \sa GetOrigin() */
  virtual void SetOrigin(const PointType& origin);
  virtual void SetOrigin(const double* origin);
  virtual void SetOrigin(const float* origin);
  virtual const double * GetOrigin() const;

  /** The spatial object being transformed can be part of a hierarchy.
   * How deep in the hierarchy should we descend in generating the
   * image?  A ChildrenDepth of 0 means to only include the object
   * itself. */
  itkSetMacro(ChildrenDepth, unsigned int);
  itkGetMacro(ChildrenDepth, unsigned int);

  /** Set/Get Size */
  itkSetMacro(Size, SizeType);
  itkGetMacro(Size, SizeType);

  /** If UseObjectValue is set to true, then the filter uses
   *  the ValueAt() function instead of IsInside() */
  itkSetMacro(UseObjectValue, bool);
  itkGetMacro(UseObjectValue, bool);

  typedef itk::SpatialObject<InputSpatialObjectType::ObjectDimension> SpatialObjectType;
  typedef typename SpatialObjectType::ChildrenListType*               ChildrenType;
  typedef typename SpatialObjectType::ChildrenListType::iterator      IteratorType;

protected:
  SpatialObjectToImageDrawingFilter();
  ~SpatialObjectToImageDrawingFilter() override;

  void GenerateOutputInformation() override; // do nothing
  void GenerateData() override;

  SizeType     m_Size;
  double       m_Spacing[OutputImageDimension];
  double       m_Origin[OutputImageDimension];
  unsigned int m_ChildrenDepth;
  ValueType    m_InsideValue;
  ValueType    m_OutsideValue;
  bool         m_UseObjectValue;

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  SpatialObjectToImageDrawingFilter(const Self &) = delete;
  void operator =(const Self&) = delete;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSpatialObjectToImageDrawingFilter.hxx"
#endif

#endif
