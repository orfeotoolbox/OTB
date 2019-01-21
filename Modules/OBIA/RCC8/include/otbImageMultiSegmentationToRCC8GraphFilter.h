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

#ifndef otbImageMultiSegmentationToRCC8GraphFilter_h
#define otbImageMultiSegmentationToRCC8GraphFilter_h

#include "otbImageListToRCC8GraphFilter.h"

namespace otb
{
/**
 * \class ImageMultiSegmentationToRCC8GraphFilter
 * \brief This class takes a list of labelled segmentation images
 * and build the RCC8 graph of the set of regions it represents.
 *
 * \ingroup OTBRCC8
 */
template <class TInputImage, class TOutputGraph>
class ITK_EXPORT ImageMultiSegmentationToRCC8GraphFilter
  : public ImageListToRCC8GraphFilter<TInputImage, TOutputGraph>
{
public:
  /** Standard class typedefs. */
  typedef ImageMultiSegmentationToRCC8GraphFilter               Self;
  typedef ImageListToRCC8GraphFilter<TInputImage, TOutputGraph> Superclass;
  typedef itk::SmartPointer<Self>                               Pointer;
  typedef itk::SmartPointer<const Self>                         ConstPointer;
  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageMultiSegmentationToRCC8GraphFilter, ImageListToRCC8GraphFilter);
  /** Input related typedefs */
  typedef TInputImage                                InputImageType;
  typedef typename InputImageType::PixelType         PixelType;
  typedef typename InputImageType::Pointer           InputImagePointerType;
  typedef typename Superclass::InputImageListType    InputImageListType;
  typedef typename InputImageListType::Pointer       InputImageListPointerType;
  typedef typename InputImageListType::ConstIterator ConstListIteratorType;
  /** Output related typedefs */
  typedef TOutputGraph                                   OutputGraphType;
  typedef typename OutputGraphType::Pointer              OutputGraphPointerType;
  typedef typename OutputGraphType::VertexType           VertexType;
  typedef typename VertexType::Pointer                   VertexPointerType;
  typedef typename VertexType::PathType                  PathType;
  typedef typename OutputGraphType::VertexDescriptorType VertexDescriptorType;
  /** Knowledge enum typedef */
  typedef typename OutputGraphType::RCC8ValueType      RCC8ValueType;
  typedef enum {NO_INFO, LEVEL_1, LEVEL_3, FULL}       KnowledgeValueType;
  typedef std::pair<KnowledgeValueType, RCC8ValueType> KnowledgeStateType;
  /** Toogle optimisation flag */
  itkBooleanMacro(Optimisation);
  itkSetMacro(Optimisation, bool);

  /**
   * Get the number of occurrences of the given value
   * \return The value.
   */
  unsigned int GetRelationsCount(RCC8ValueType val);

  /**
   * Get number of relations
   * \return The number of relations.
   */
  unsigned int GetNumberOfRelations(void);

protected:
  /** Constructor */
  ImageMultiSegmentationToRCC8GraphFilter();
  /** Destructor */
  ~ImageMultiSegmentationToRCC8GraphFilter() override;
  /** Main computation method */
  void GenerateData() override;
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;
  /**
   * Get the composition knowledge.
   * \param r1 First RCC8 relation value,
   * \param r2 Second RCC8 relation value,
   * \return The knowledge associated with the composition.
   */
  KnowledgeStateType GetKnowledge(RCC8ValueType r1, RCC8ValueType r2);

private:
  /** Optimisation flag */
  bool         m_Optimisation;
  unsigned int m_Accumulator[8];

};
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageMultiSegmentationToRCC8GraphFilter.hxx"
#endif

#endif
