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
#ifndef __otbRCC8VertexBase_h
#define __otbRCC8VertexBase_h

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
  virtual ~RCC8VertexBase() {}
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

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
#include "otbRCC8VertexBase.txx"
#endif

#endif
