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

#ifndef otbRCC8VertexBase_h
#define otbRCC8VertexBase_h

#include "itkDataObject.h"
#include "itkObjectFactory.h"
#include <cstdlib>

namespace otb
{
/** \class RCC8VertexBase
 *  \brief Base class to represent a vertex in a RCC8 Graph.
 *
 *  This class can be derived to add attributes to the vertices of the
 *  RCC8 graph.
 *
 * \sa RCC8Graph, RCC8Edge
 *
 * \ingroup OTBRCC8
 */
template <class TPath>
class ITK_EXPORT RCC8VertexBase
  : public itk::DataObject
{
public:
  /** Standard class typedefs */
  typedef RCC8VertexBase                Self;
  typedef itk::DataObject               Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  /** Run-time type information (and related methods). */
  itkTypeMacro(RCC8VertexBase, DataObject);
  /** Input image associated typedefs*/
  typedef TPath                                  PathType;
  typedef typename PathType::Pointer             PathPointerType;
  typedef typename PathType::ContinuousIndexType ContinuousIndexType;

  /** char* vector attributes */
  typedef std::map<std::string, std::string> AttributesMapType;

  /** Segmentation image index accessors */
  itkGetMacro(SegmentationLevel, unsigned int);
  itkSetMacro(SegmentationLevel, unsigned int);
  itkGetMacro(SegmentationType, bool);
  itkSetMacro(SegmentationType, bool);
  /** Object label in image accessor */
  itkGetObjectMacro(Path, PathType);
  itkSetObjectMacro(Path, PathType);
  /**
   * Set the VertexBase attributes from the attributes vector.
   * \param attributes The vector containing the parsed attributes.
   */
  virtual void SetAttributesMap(AttributesMapType attributes);
  /**
   * Get an attributes vector representing the VertexBase attributes.
   * \return The attributes vector
   */
  virtual AttributesMapType GetAttributesMap(void);

protected:
  /** Constructor */
  RCC8VertexBase();
  /** Desctructor */
  ~RCC8VertexBase() override {}
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  /** The segmentation level */
  unsigned int m_SegmentationLevel;
  /** True if bright details, false otherwise */
  bool m_SegmentationType;
  /** The path of the edge of the region */
  PathPointerType m_Path;
};
} // end namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRCC8VertexBase.hxx"
#endif

#endif
