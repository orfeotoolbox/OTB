/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbVectorDataProjectionFilter_h
#define otbVectorDataProjectionFilter_h

#include "otbVectorDataToVectorDataFilter.h"
#include "otbGenericRSTransform.h"
#include "otbImageKeywordlist.h"
#include <string>

namespace otb
{

/** \class VectorDataProjectionFilter
  * \brief Reproject vector data in a different coordinate system
  *
  * This class is used to reproject vector data into a different coordinate system.
  * the input and output coordinate system can be a map projection, a raw image from a
  * sensor (the sensor model will be used), or the local coordinate system of an image.
  *
  * This filter works on otb::VectorData as input and output.
  *
  * The process goes as follow:
  * - offset/scaling of the input coordinates
  * - transform to get the data in geographic coordinates (lon/lat)
  * - transform from geographic coordinates
  * - offset/scaling of the output coordinates
  *
  * Each of this step is optional and would default to an identity transform if nothing
  * is specified.
  *
  * The offset/scaling steps are necessary only when working with the local coordinate
  * system of the image (origin on the top left). The value need to be provided by the
  * SetInputSpacing, SetInputOrigin, SetOutputSpacing and SetOutputOrigin methods.
  *
  * The two transforms derived from itk::Transform and will be instantiated as
  * otb::GenericMapProjection or otb::InverseSensorModel or otb::ForwardSensorModel
  * (according to the available information).
  *
  * \ingroup VectorDataFilter
  * \ingroup Projection
  *
  * \example Projections/VectorDataProjectionExample.cxx
  *
 *
 * \ingroup OTBProjection
  */

template <class TInputVectorData, class TOutputVectorData>
class ITK_EXPORT VectorDataProjectionFilter : public otb::VectorDataToVectorDataFilter<TInputVectorData, TOutputVectorData>
{

public:
  /** Standard class typedefs. */
  typedef VectorDataProjectionFilter Self;
  typedef otb::VectorDataToVectorDataFilter<TInputVectorData, TOutputVectorData> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef TInputVectorData                        InputVectorDataType;
  typedef TOutputVectorData                       OutputVectorDataType;
  typedef typename TInputVectorData::ConstPointer InputVectorDataPointer;
  typedef typename TOutputVectorData::Pointer     OutputVectorDataPointer;

  /** Some typedefs. */
  typedef itk::Transform<double, 2, 2> GenericTransformType;
  typedef typename GenericTransformType::Pointer GenericTransformPointerType;
  //   typedef otb::CompositeTransform<GenericTransformType, GenericTransformType, double, 2, 2> InternalTransformType;
  typedef otb::GenericRSTransform<double, 2, 2> InternalTransformType;
  typedef typename InternalTransformType::Pointer InternalTransformPointerType;

  typedef itk::Vector<double, 2> SpacingType;
  typedef itk::Point<double, 2>  OriginType;

  typedef typename OutputVectorDataType::DataNodeType OutputDataNodeType;
  typedef typename InputVectorDataType::DataNodeType  InputDataNodeType;

  typedef typename InputVectorDataType::DataTreeType::TreeNodeType  InputInternalTreeNodeType;
  typedef typename OutputVectorDataType::DataTreeType::TreeNodeType OutputInternalTreeNodeType;

  typedef typename InputDataNodeType::PointType       InputPointType;
  typedef typename InputDataNodeType::LineType        InputLineType;
  typedef typename InputDataNodeType::PolygonType     InputPolygonType;
  typedef typename InputDataNodeType::PolygonListType InputPolygonListType;

  typedef typename InputLineType::Pointer        InputLinePointerType;
  typedef typename InputPolygonType::Pointer     InputPolygonPointerType;
  typedef typename InputPolygonListType::Pointer InputPolygonListPointerType;

  typedef typename OutputDataNodeType::PointType       OutputPointType;
  typedef typename OutputDataNodeType::LineType        OutputLineType;
  typedef typename OutputDataNodeType::PolygonType     OutputPolygonType;
  typedef typename OutputDataNodeType::PolygonListType OutputPolygonListType;

  typedef typename OutputLineType::Pointer        OutputLinePointerType;
  typedef typename OutputPolygonType::Pointer     OutputPolygonPointerType;
  typedef typename OutputPolygonListType::Pointer OutputPolygonListPointerType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorDataProjectionFilter, VectorDataToVectorDataFilter);

  /** Set/Get for input and output projections.  */
  itkSetStringMacro(InputProjectionRef);
  itkGetStringMacro(InputProjectionRef);

  itkSetStringMacro(OutputProjectionRef);
  itkGetStringMacro(OutputProjectionRef);

  /** Set the origin of the vector data.
   * \sa GetOrigin() */
  itkSetMacro(InputOrigin, OriginType);
  virtual void SetInputOrigin(const double origin[2]);
  virtual void SetInputOrigin(const float origin[2]);

  itkGetConstReferenceMacro(InputOrigin, OriginType);

  /** Set the spacing (size of a pixel) of the vector data.
   * \sa GetSignedSpacing() */
  virtual void SetInputSpacing(const SpacingType& spacing);
  virtual void SetInputSpacing(const double spacing[2]);
  virtual void SetInputSpacing(const float spacing[2]);

  itkGetConstReferenceMacro(InputSpacing, SpacingType);

  /** Set the origin of the vector data.
   * \sa GetOrigin() */
  itkSetMacro(OutputOrigin, OriginType);
  virtual void SetOutputOrigin(const double origin[2]);
  virtual void SetOutputOrigin(const float origin[2]);

  itkGetConstReferenceMacro(OutputOrigin, OriginType);

  /** Set the spacing (size of a pixel) of the vector data.
   * \sa GetSignedSpacing() */
  virtual void SetOutputSpacing(const SpacingType& spacing);
  virtual void SetOutputSpacing(const double spacing[2]);
  virtual void SetOutputSpacing(const float spacing[2]);

  itkGetConstReferenceMacro(OutputSpacing, SpacingType);

  /**\name ImageMetadata accessors and mutators */
  //@{
  const ImageMetadata* GetInputImageMetadata() const
  {
    return m_InputImageMetadata;
  }
  void SetInputImageMetadata(const ImageMetadata* imd)
  {
    m_InputImageMetadata = imd;
    this->Modified();
  }

  const ImageMetadata* GetOutputImageMetadata() const
  {
    return m_OutputImageMetadata;
  }
  void SetOutputImageMetadata(const ImageMetadata* imd)
    {
      m_OutputImageMetadata = imd;
      this->Modified();
    }
  //@}

protected:
  VectorDataProjectionFilter();
  ~VectorDataProjectionFilter() override
  {
  }

  OutputPointType ProcessPoint(InputPointType point) const override;
  OutputLinePointerType ProcessLine(InputLinePointerType line) const override;
  OutputPolygonPointerType ProcessPolygon(InputPolygonPointerType polygon) const override;
  OutputPolygonListPointerType ProcessPolygonList(InputPolygonListPointerType polygonList) const override;

  virtual void InstantiateTransform(void);

  void GenerateOutputInformation(void) override;
  void GenerateData(void) override;

private:
  VectorDataProjectionFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  InternalTransformPointerType m_Transform;
  std::string                  m_InputProjectionRef;
  std::string                  m_OutputProjectionRef;
  const ImageMetadata*   m_InputImageMetadata = nullptr;
  const ImageMetadata*   m_OutputImageMetadata = nullptr;

  SpacingType m_InputSpacing;
  OriginType  m_InputOrigin;
  SpacingType m_OutputSpacing;
  OriginType  m_OutputOrigin;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorDataProjectionFilter.hxx"
#endif

#endif
