/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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


#ifndef otbVectorDataToLabelMapWithAttributesFilter_h
#define otbVectorDataToLabelMapWithAttributesFilter_h

#include "itkLabelObject.h"
#include "itkProgressReporter.h"
#include "itkBarrier.h"
#include "itkConceptChecking.h"
#include "itkContinuousIndex.h"
//#include "itkDataObject.h"

#include "otbLabelMapSource.h"
#include "otbCorrectPolygonFunctor.h"

#include <vector>
#include <map>

#include "otbVectorDataProperties.h"
// #include "itkImageToImageFilter.h"

namespace otb
{

/**
 * \class VectorDataToLabelMapWithAttributesFilter
 * \brief Converts a vector data into a LabelMap.
 *
 * VectorDataToLabelMapWithAttributesFilter convert a vector data in a LabelMap.
 * To each distinct object is assigned a unique label.
 * The first Label is set using SetInitialLabel (by default it is 0), and increases 1 by 1.
 * The vector data attributes are also assigned to the object in the label map.
 *
 * When AutomaticSizeComputation=True, the spacing from the vector data object
 * is used. Then the size and origin are computed so that the image extent
 * covers exactly the vector data bounding region.
 *
 * Note : the coordinates in the vector data are assumed to be physical coordinates.
 *
 * \sa LabelMapSource
 *
 * \ingroup OTBConversion
 */

template <class TVectorData, class TLabelMap>
class ITK_EXPORT VectorDataToLabelMapWithAttributesFilter :
  public LabelMapSource <TLabelMap>

{
public:
  /**
   * Standard "Self" & Superclass typedef.
   */
  typedef VectorDataToLabelMapWithAttributesFilter Self;
  typedef LabelMapSource<TLabelMap> Superclass;

  /** Some convenient typedefs. */
  typedef TVectorData InputVectorDataType;
  typedef TLabelMap OutputLabelMapType;
  typedef typename InputVectorDataType::Pointer InputVectorDataPointer;
  typedef typename InputVectorDataType::ConstPointer InputVectorDataConstPointer;

  typedef typename OutputLabelMapType::Pointer OutputLabelMapPointer;
  typedef typename OutputLabelMapType::ConstPointer OutputLabelMapConstPointer;

  typedef typename InputVectorDataType::DataTreeType::TreeNodeType InternalTreeNodeType;
  typedef typename InternalTreeNodeType::ChildrenListType ChildrenListType;
  typedef typename InputVectorDataType::DataNodeType DataNodeType;
  typedef typename DataNodeType::Pointer DataNodePointerType;
  typedef typename DataNodeType::PolygonType PolygonType;
  typedef typename PolygonType::Pointer PolygonPointerType;
  typedef typename OutputLabelMapType::LabelType LabelType;

  typedef typename OutputLabelMapType::IndexType IndexType;
  typedef typename OutputLabelMapType::PixelType OutputLabelMapPixelType;
  typedef typename OutputLabelMapType::PointType OriginType;
  typedef typename OutputLabelMapType::SpacingType SpacingType;
  typedef typename OutputLabelMapType::DirectionType DirectionType;
  typedef typename OutputLabelMapType::RegionType RegionType;
  typedef typename OutputLabelMapType::LabelObjectType LabelObjectType;
  typedef typename LabelObjectType::AttributesValueType AttributesValueType;

  typedef VectorDataProperties<InputVectorDataType> VectorDataPropertiesType;

  /** typedefs for correct polygon */
  typedef otb::CorrectPolygonFunctor<PolygonType> CorrectFunctorType;

  /** Number of dimensions. */
  itkStaticConstMacro(VectorDataDimension, unsigned int,
      TVectorData::Dimension);

  /** Image size typedef. */
  typedef itk::Size<itkGetStaticConstMacro(VectorDataDimension)> SizeType;

  typedef typename InputVectorDataType::PointType PointType;

  /**
   * Smart pointer typedef support
   */
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /**
   * Run-time type information (and related methods)
   */
  itkTypeMacro(VectorDataToLabelMapWithAttributesFilter, ImageToImageFilter)
;

  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self)
;

  /**
   * Set/Get the value used as "background" in the output image.
   * Defaults to NumericTraits<PixelType>::NonpositiveMin().
   */
  itkSetMacro(BackgroundValue, OutputLabelMapPixelType)
; itkGetConstMacro(BackgroundValue, OutputLabelMapPixelType)
;

  /** Set the size of the output image. */
  itkSetMacro(Size, SizeType)
;

  /** Get the size of the output image. */
  itkGetConstReferenceMacro(Size, SizeType)
;

  /** Set the origin of the vector data.
   * \sa GetOrigin() */
  itkSetMacro(Origin, OriginType)
;
  virtual void SetOrigin(const double origin[2]);
  virtual void SetOrigin(const float origin[2]);

  itkGetConstReferenceMacro(Origin, OriginType)
;

  /** Get/Set start index*/
  itkGetConstReferenceMacro(StartIndex, IndexType)
; itkSetMacro(StartIndex, IndexType)
;
  /** Set the spacing (size of a pixel) of the vector data.
   * \sa GetSignedSpacing() */
  virtual void SetSpacing(const SpacingType& spacing);
  virtual void SetSpacing(const double spacing[2]);
  virtual void SetSpacing(const float spacing[2]);

  itkGetConstReferenceMacro(Spacing, SpacingType)
;

  /** Set/Get the Vector data input of this process object.  */
  using Superclass::SetInput;
  virtual void SetInput(const InputVectorDataType *input);
  virtual void SetInput(unsigned int idx, const InputVectorDataType *input);
  const InputVectorDataType * GetInput(void);
  const InputVectorDataType * GetInput(unsigned int idx);

  /** Set/Get Automatic size computation mode*/
  itkSetMacro(AutomaticSizeComputation, bool)
; itkGetMacro(AutomaticSizeComputation, bool)
;

  /** Set/Get Initial label value */
  itkSetMacro(InitialLabel, LabelType)
; itkGetMacro(InitialLabel, LabelType)
;

  void GenerateOutputInformation() override;

protected:
  VectorDataToLabelMapWithAttributesFilter();
  ~VectorDataToLabelMapWithAttributesFilter() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /**
   * Standard pipeline method.
   */

  void GenerateData() override;

  /** VectorDataToLabelMapWithAttributesFilter needs the entire input. Therefore
   * it must provide an implementation GenerateInputRequestedRegion().
   * \sa ProcessObject::GenerateInputRequestedRegion(). */
  //   void GenerateInputRequestedRegion();

  /** VectorDataToLabelMapWithAttributesFilter will produce all of the output.
   * Therefore it must provide an implementation of
   * EnlargeOutputRequestedRegion().
   * \sa ProcessObject::EnlargeOutputRequestedRegion() */
  //   void EnlargeOutputRequestedRegion(itk::DataObject *itkNotUsed(output));

private:
  VectorDataToLabelMapWithAttributesFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  void ProcessNode(InternalTreeNodeType * source);

  /** Current label value incremented after the vectorization of a layer*/
  LabelType m_lab;
  /** Set to 0 by default.*/
  LabelType m_InitialLabel;

  //TODO don't need this attributes now compute with VectorDataProperties

  SpacingType   m_Spacing;
  OriginType    m_Origin;
  SizeType      m_Size;
  IndexType     m_StartIndex;
  DirectionType m_Direction;

  /** Background value, not use actually, background value=itk::NumericTraits<LabelType>::max()*/
  OutputLabelMapPixelType m_BackgroundValue;

  bool m_AutomaticSizeComputation;
  typename VectorDataPropertiesType::Pointer m_VectorDataProperties;
};

} // end namespace itk

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorDataToLabelMapWithAttributesFilter.hxx"
#endif

#endif
