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

#ifndef otbLabelImageToVectorDataFilter_h
#define otbLabelImageToVectorDataFilter_h

#include "otbVectorDataSource.h"
#include "otbVectorData.h"
#include "otbOGRDataSourceWrapper.h"
#include <string>

namespace otb
{
/** \class LabelImageToVectorDataFilter
 *  \brief this class uses GDALPolygonize method to transform a Label image into a VectorData.
 *  An optional input mask can be used to exclude pixels from vectorization process.
 *  All pixels with a value of 0 in the input mask image will not be suitable for vectorization.
 * \note The Use8Connected parameter can be turn on and it will be used in \c GDALPolygonize(). But be carreful, it
 * can create cross polygons !
 * \note It is a non-streamed version.
 *  \ingroup OBIA
 *
 * \ingroup OTBConversion
 */

template <class TInputImage, class TPrecision = double>
class ITK_EXPORT LabelImageToVectorDataFilter : public VectorDataSource<otb::VectorData<TPrecision>>
{
public:
  /** typedef for the classes standards. */
  typedef LabelImageToVectorDataFilter             Self;
  typedef VectorDataSource<VectorData<TPrecision>> Superclass;
  typedef itk::SmartPointer<Self>                  Pointer;
  typedef itk::SmartPointer<const Self>            ConstPointer;

  /** Method for management of the object factory. */
  itkNewMacro(Self);

  /** Return the name of the class. */
  itkTypeMacro(LabelImageToVectorDataFilter, VectorDataSource);

  /** Definition of the input image */
  typedef TInputImage                          InputImageType;
  typedef typename InputImageType::PixelType   InputPixelType;
  typedef typename InputImageType::IndexType   InputIndexType;
  typedef typename InputImageType::SizeType    SizeType;
  typedef typename InputImageType::RegionType  RegionType;
  typedef typename InputImageType::SpacingType SpacingType;
  typedef typename InputImageType::PointType   OriginType;
  typedef typename InputImageType::IndexType   IndexType;

  /** Definition of the output vector data. */
  typedef VectorData<TPrecision>                VectorDataType;
  typedef typename VectorDataType::Pointer      VectorDataPointerType;
  typedef typename VectorDataType::DataTreeType DataTreeType;
  typedef typename DataTreeType::Pointer        DataTreePointerType;
  typedef typename DataTreeType::TreeNodeType   InternalTreeNodeType;
  typedef typename VectorDataType::DataNodeType DataNodeType;
  typedef typename DataNodeType::Pointer        DataNodePointerType;
  typedef typename VectorDataType::LineType     LineType;
  typedef typename VectorDataType::PointType    PointType;
  typedef typename LineType::VertexType         VertexType;

  typedef ogr::DataSource                     OGRDataSourceType;
  typedef typename OGRDataSourceType::Pointer OGRDataSourcePointerType;
  typedef ogr::Layer                          OGRLayerType;


  /** Set/Get the input image of this process object.  */
  using Superclass::SetInput;
  virtual void SetInput(const InputImageType* input);
  virtual const InputImageType* GetInput(void);

  /** Set the input mask image.
   * All pixels in the mask with a value of 0 will not be considered
   * suitable for vectorization.
   */
  virtual void SetInputMask(const InputImageType* input);
  virtual const InputImageType* GetInputMask(void);

  itkSetMacro(FieldName, std::string);
  itkGetMacro(FieldName, std::string);

  itkSetMacro(Use8Connected, bool);
  itkGetMacro(Use8Connected, bool);

protected:
  LabelImageToVectorDataFilter();
  ~LabelImageToVectorDataFilter() override
  {
  }

  void GenerateInputRequestedRegion() override;

  /** Generate Data method*/
  void GenerateData() override;

private:
  LabelImageToVectorDataFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  std::string m_FieldName;
  bool        m_Use8Connected;
};


} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLabelImageToVectorDataFilter.hxx"
#endif

#endif
