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


#ifndef otbVectorDataToLabelMapFilter_h
#define otbVectorDataToLabelMapFilter_h

#include "itkLabelObject.h"
#include "itkProgressReporter.h"
#include "itkBarrier.h"
#include "itkConceptChecking.h"
#include "itkContinuousIndex.h"
//#include "itkDataObject.h"

#include "otbLabelMapSource.h"
#include "otbVectorData.h"
#include "otbCorrectPolygonFunctor.h"

#include <vector>
#include <map>

// #include "itkImageToImageFilter.h"

namespace otb
{

/**
 * \class VectorDataToLabelMapFilter
 * \brief Convert a vector data and produce a collection of label objects for each node
 *
 * VectorDataToLabelMapFilter convert a vector data in a LabelMap.
 * Each distinct object is assigned a unique label.
 * The final object labels start with 1 and are consecutive (depraced +10).
 *
 * \sa LabelMapSource
 *
 * \ingroup OTBConversion
 */

template <class TVectorData, class TLabelMap>
class ITK_EXPORT VectorDataToLabelMapFilter :
  public LabelMapSource <TLabelMap>

{
public:
  /**
   * Standard "Self" & Superclass typedef.
   */
  typedef VectorDataToLabelMapFilter Self;
  typedef LabelMapSource <TLabelMap> Superclass;

  /** Some convenient typedefs. */
  typedef TVectorData                                InputVectorDataType;
  typedef TLabelMap                                  OutputLabelMapType;
  typedef typename InputVectorDataType::Pointer      InputVectorDataPointer;
  typedef typename InputVectorDataType::ConstPointer InputVectorDataConstPointer;

  typedef typename OutputLabelMapType::Pointer      OutputLabelMapPointer;
  typedef typename OutputLabelMapType::ConstPointer OutputLabelMapConstPointer;

  typedef typename InputVectorDataType::DataTreeType::TreeNodeType InternalTreeNodeType;
  typedef typename InternalTreeNodeType::ChildrenListType          ChildrenListType;
  typedef typename InputVectorDataType::DataNodeType               DataNodeType;
  typedef typename DataNodeType::Pointer                           DataNodePointerType;
  typedef typename DataNodeType::PolygonType                       PolygonType;
  typedef typename PolygonType::Pointer                            PolygonPointerType;
  typedef typename OutputLabelMapType::LabelType                   LabelType;

  typedef typename OutputLabelMapType::IndexType     IndexType;
  typedef typename OutputLabelMapType::PixelType     OutputLabelMapPixelType;
  typedef typename OutputLabelMapType::PointType     OriginType;
  typedef typename OutputLabelMapType::SpacingType   SpacingType;
  typedef typename OutputLabelMapType::DirectionType DirectionType;

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
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Set the size of the output image. */
//   itkSetMacro( Size, SizeType );

  /** Set the starting index of the output image. */
//   itkSetMacro( StartIndex, IndexType );
/**
 * Run-time type information (and related methods)
 */
  itkTypeMacro(VectorDataToLabelMapFilter, ImageToImageFilter);

  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);

  /**
   * Set/Get the value used as "background" in the output image.
   * Defaults to NumericTraits<PixelType>::NonpositiveMin().
   */
  itkSetMacro(BackgroundValue, OutputLabelMapPixelType);
  itkGetConstMacro(BackgroundValue, OutputLabelMapPixelType);

  /** Set the size of the output image. */
  itkSetMacro(Size, SizeType);

  /** Get the size of the output image. */
  itkGetConstReferenceMacro(Size, SizeType);

  /** Set the origin of the vector data.
   * \sa GetOrigin() */
  itkSetMacro(Origin, OriginType);
  virtual void SetOrigin(const double origin[2]);
  virtual void SetOrigin(const float origin[2]);

  itkGetConstReferenceMacro(Origin, OriginType);

  /** Get/Set start index*/
  itkGetConstReferenceMacro(StartIndex, IndexType);
  itkSetMacro(StartIndex, IndexType);
  /** Set the spacing (size of a pixel) of the vector data.
   * \sa GetSignedSpacing() */
  virtual void SetSpacing(const SpacingType& spacing);
  virtual void SetSpacing(const double spacing[2]);
  virtual void SetSpacing(const float spacing[2]);

  itkGetConstReferenceMacro(Spacing, SpacingType);

  /** Set/Get the Vector data input of this process object.  */
  using Superclass::SetInput;
  virtual void SetInput(const InputVectorDataType *input);
  virtual void SetInput(unsigned int idx, const InputVectorDataType *input);
  const InputVectorDataType * GetInput(void);
  const InputVectorDataType * GetInput(unsigned int idx);

  void GenerateOutputInformation() override;

protected:
  VectorDataToLabelMapFilter();
  ~VectorDataToLabelMapFilter() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /**
   * Standard pipeline method.
   */

  void GenerateData() override;

  /** VectorDataToLabelMapFilter needs the entire input. Therefore
   * it must provide an implementation GenerateInputRequestedRegion().
   * \sa ProcessObject::GenerateInputRequestedRegion(). */
//   void GenerateInputRequestedRegion();

  /** VectorDataToLabelMapFilter will produce all of the output.
   * Therefore it must provide an implementation of
   * EnlargeOutputRequestedRegion().
   * \sa ProcessObject::EnlargeOutputRequestedRegion() */
//   void EnlargeOutputRequestedRegion(itk::DataObject *itkNotUsed(output));

private:
  VectorDataToLabelMapFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  void ProcessNode(InternalTreeNodeType * source);

  /** Current label value incremented after the vectorization of a layer*/
  LabelType m_lab;

  //TODO donc need this attribute now compute with VectorDataProperties
  SpacingType   m_Spacing;
  OriginType    m_Origin;
  SizeType      m_Size;
  IndexType     m_StartIndex;
  DirectionType m_Direction;

  /** Background value, not use actually, background value=itk::NumericTraits<LabelType>::max()*/
  OutputLabelMapPixelType m_BackgroundValue;
};

} // end namespace itk

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorDataToLabelMapFilter.hxx"
#endif

#endif
