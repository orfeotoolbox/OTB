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
#ifndef otbRCC8VertexBase_txx
#define otbRCC8VertexBase_txx

#include "otbRCC8VertexBase.h"

namespace otb
{
/**
 * Constructor
 */
template <class TPath>
RCC8VertexBase<TPath>
::RCC8VertexBase()
{
  m_SegmentationLevel = 0;
  m_SegmentationType = true;
  m_Path = PathType::New();
  m_Path->Initialize();
}
/**
 * Set the VertexBase attributes from the attributes vector.
 * \param attributes The vector containing the parsed attributes.
 */
template <class TPath>
void
RCC8VertexBase<TPath>
::SetAttributesMap(AttributesMapType attributes)
{
  m_SegmentationLevel = static_cast<unsigned int>(atoi(attributes["SegmentationLevel"].c_str()));
  m_SegmentationType = static_cast<bool>(atoi(attributes["SegmentationType"].c_str()));
  unsigned int nbPoints = static_cast<unsigned int>(atoi(attributes["NumberOfPointsInPath"].c_str()));

  std::ostringstream oss;

  m_Path = PathType::New();
  m_Path->Initialize();

  for (unsigned int i = 0; i < nbPoints; ++i)
    {
    ContinuousIndexType newPoint;
    oss << "P" << i << "x";
    newPoint[0] = atof(attributes[oss.str()].c_str());
    oss.str("");
    oss << "P" << i << "y";
    newPoint[1] = atof(attributes[oss.str()].c_str());
    oss.str("");
    m_Path->AddVertex(newPoint);
    }
}
/**
 * Get an attributes vector representing the VertexBase attributes.
 * \return The attributes vector
 */
template <class TPath>
typename RCC8VertexBase<TPath>::AttributesMapType
RCC8VertexBase<TPath>
::GetAttributesMap(void)
{
  std::ostringstream oss;
  AttributesMapType  results;
  oss << m_SegmentationLevel;
  results["SegmentationLevel"] = oss.str();
  oss.str("");
  oss << m_SegmentationType;
  results["SegmentationType"] = oss.str();
  oss.str("");
  oss << m_Path->GetVertexList()->Size();
  results["NumberOfPointsInPath"] = oss.str();
  oss.str("");

  typename PathType::VertexListType::ConstIterator it;
  unsigned int                                     index = 0;

  for (it = m_Path->GetVertexList()->Begin(); it != m_Path->GetVertexList()->End(); ++it, ++index)
    {
    ContinuousIndexType point = it.Value();
    oss << "P" << index << "x";
    std::string key = oss.str();
    oss.str("");
    oss << point[0];
    results[key] = oss.str();
    oss.str("");
    oss << "P" << index << "y";
    key = oss.str();
    oss.str("");
    oss << point[1];
    results[key] = oss.str();
    oss.str("");
    }
  return results;
}
/**
 * PrintSelf method
 */
template <class TPath>
void
RCC8VertexBase<TPath>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "SegmentationLevel: " << m_SegmentationLevel << std::endl;
  os << indent << "SegmentationType: " << m_SegmentationType << std::endl;
}
} // end namespace otb
#endif
