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
#ifndef __otbPolygonToPolygonRCC8Calculator_txx
#define __otbPolygonToPolygonRCC8Calculator_txx

#include "otbPolygonToPolygonRCC8Calculator.h"
#include "otbMacro.h"

namespace otb
{
/**
 * Constructor
 */
template<class TInputPolygon>
PolygonToPolygonRCC8Calculator<TInputPolygon>
::PolygonToPolygonRCC8Calculator()
{
  m_Value        = OTB_RCC8_DC;
  m_Level1APrioriKnowledge = false;
  m_Level3APrioriKnowledge = false;
}
/**
 * Get the RCC8 relation.
 * \return The RCC8 relation value.
 */
template <class TInputPolygon>
typename PolygonToPolygonRCC8Calculator<TInputPolygon>
::RCC8ValueType
PolygonToPolygonRCC8Calculator<TInputPolygon>
::GetValue(void)
{
  return m_Value;
}

template<class TInputPolygon>
void
PolygonToPolygonRCC8Calculator<TInputPolygon>
::Compute(void)
{
  // First check if bounding regions are disjoints
  RegionType region1 = m_Polygon1->GetBoundingRegion().GetImageRegion();
  RegionType region2 = m_Polygon2->GetBoundingRegion().GetImageRegion();

  // If intersection is not null, we have to do the computation
  if (!RegionsIntersectionIsNull(region1, region2))
    {

    bool edgeEdgeBool, interExterBool, exterInterBool, interInterBool;
    /// The boolean edgeEdge is needed in each case, so it si computed
    /// now
    edgeEdgeBool = ComputeEdgeEdge(m_Polygon1, m_Polygon2);

    //std::cout<<"EdgeEdge: "<<edgeEdgeBool<<std::endl;

    if (this->GetLevel1APrioriKnowledge())
      {
      interExterBool = true;
      }
    else
      {
      /// Else it must be computed
      interExterBool = ComputeInterExter(m_Polygon1, m_Polygon2);
      }

    //std::cout<<"InterExter: "<<interExterBool<<std::endl;

    /// At this stage we can determine if the relation is of type NTPP
    if ((!interExterBool) && (!edgeEdgeBool))
      {
      m_Value = OTB_RCC8_NTPP;
      }
    else
      {
      /// If not, we must consider the intersection between exterior
      if (this->GetLevel3APrioriKnowledge())
        {
        /// If the Level3APRioriKnowledge flag is set, this boolean
        /// can be determined from the two others
        exterInterBool = true;
        }
      else
        {
        /// Else it must be computed
        exterInterBool = ComputeInterExter(m_Polygon2, m_Polygon1);
        }

      //std::cout<<"ExterInter: "<<exterInterBool<<std::endl;

      /// If it is not sufficient to compute the relation
      if (!ComputeRelation(edgeEdgeBool, interExterBool, exterInterBool))
        {
        /// Compute the last boolean
        interInterBool = ComputeInterInter(m_Polygon1, m_Polygon2);

        //std::cout<<"InterInter: "<<interInterBool<<std::endl;

        /// Which allow the full determination
        if ((interExterBool) && (edgeEdgeBool) && (exterInterBool) && (!interInterBool))
          {
          m_Value = OTB_RCC8_EC;
          }
        else
          {
          m_Value = OTB_RCC8_PO;
          }
        }
      }
    }
}

template<class TInputPolygon>
bool
PolygonToPolygonRCC8Calculator<TInputPolygon>
::RegionsIntersectionIsNull(const RegionType& region1, const RegionType& region2)
{
  for (unsigned int dim = 0; dim < RegionType::ImageDimension; ++dim)
    {
    if (region1.GetIndex()[dim] + static_cast<int>(region1.GetSize()[dim]) < region2.GetIndex()[dim])
      {
      return true;
      }
    else if (region2.GetIndex()[dim] + static_cast<int>(region2.GetSize()[dim]) < region1.GetIndex()[dim])
      {
      return true;
      }
    }
  return false;
}

template<class TInputPolygon>
bool
PolygonToPolygonRCC8Calculator<TInputPolygon>
::ComputeRelation(bool edgeEdgeBool, bool interExterBool, bool exterInterBool)
{
  // This decision process is based on a decision tree
  if ((!interExterBool) && (edgeEdgeBool) && (!exterInterBool))
    {
    m_Value = OTB_RCC8_EQ;
    return true;
    }
  else if ((!interExterBool) && (edgeEdgeBool) && (exterInterBool))
    {
    m_Value = OTB_RCC8_TPP;
    return true;
    }
  else if ((interExterBool) && (!edgeEdgeBool) && (!exterInterBool))
    {
    m_Value = OTB_RCC8_NTPPI;
    return true;
    }
  else if ((interExterBool) && (!edgeEdgeBool) && (exterInterBool))
    {
    m_Value = OTB_RCC8_DC;
    return true;
    }
  else if ((interExterBool) && (edgeEdgeBool) && (!exterInterBool))
    {
    m_Value = OTB_RCC8_TPPI;
    return true;
    }
  else
    {
    return false;
    }
}

template<class TInputPolygon>
bool
PolygonToPolygonRCC8Calculator<TInputPolygon>
::ComputeInterExter(PolygonPointerType polygon1, PolygonPointerType polygon2)
{
  bool                        resp = false;
  VertexListConstIteratorType it = polygon1->GetVertexList()->Begin();
  VertexListConstIteratorType it_end = polygon1->GetVertexList()->End();

  ContinuousIndexType current = it.Value();
  ContinuousIndexType first = current;
  bool                isInside = polygon2->IsInside(current);
  bool                firstIsInside = isInside;
  bool                isExterior = !isInside && !polygon2->IsOnEdge(current);

  //std::cout<<current<<" is inside: "<<isInside<<std::endl;
  //std::cout<<current<<" is on edge: "<<polygon2->IsOnEdge(current)<<std::endl;

  unsigned int index = 0;
  if (isExterior)
    {
    resp = true;
    }
  ++it;
  while (!resp && it != it_end)
    {
    bool nextIsInside = polygon2->IsInside(it.Value());

    if (isInside && nextIsInside)
      {
      //std::cout<<current<<" is inside and "<<it.Value()<<" is inside, nb crossings: "<<polygon2->NbCrossing(current, it.Value())<<std::endl;
      resp = polygon2->NbCrossing(current, it.Value()) > 0;
      }
    current = it.Value();
    isInside  = nextIsInside;
    isExterior = !isInside && !polygon2->IsOnEdge(current);
    //std::cout<<current<<" is inside: "<<isInside<<std::endl;
    //std::cout<<current<<" is on edge: "<<polygon2->IsOnEdge(current)<<std::endl;
    if (isExterior)
      {
      resp = true;
      }

    ++index;
    ++it;
    }

  if (!resp && isInside && firstIsInside)
    {
    resp = polygon2->NbCrossing(current, first) > 0;
    //std::cout<<current<<" is inside and "<<first<<" is inside, nb crossings: "<<polygon2->NbCrossing(current, first)<<std::endl;
    }

  return resp;
}

template<class TInputPolygon>
bool
PolygonToPolygonRCC8Calculator<TInputPolygon>
::ComputeInterInter(PolygonPointerType polygon1, PolygonPointerType polygon2)
{
  bool                        resp = false;
  VertexListConstIteratorType it = polygon1->GetVertexList()->Begin();
  VertexListConstIteratorType it_end = polygon1->GetVertexList()->End();
  ContinuousIndexType         current = it.Value();
  ContinuousIndexType         first = current;
  bool                        currentIsInside = polygon2->IsInside(current);
  bool                        firstIsInside = currentIsInside;

  if (currentIsInside)
    {
    resp = true;
    }
  ++it;
  while (!resp && it != it_end)
    {
    bool nextIsInside = polygon2->IsInside(it.Value());

    if (!currentIsInside && !nextIsInside && !polygon2->IsOnEdge(current) && !polygon2->IsOnEdge(it.Value()))
      {
      unsigned int nbCrossings = polygon2->NbCrossing(current, it.Value());

      resp = nbCrossings > 0;
      }
    currentIsInside = nextIsInside;
    current = it.Value();

    if (currentIsInside)
      {
      resp = true;
      }
    ++it;
    }

  if (!resp && !currentIsInside && !firstIsInside && !polygon2->IsOnEdge(current) && !polygon2->IsOnEdge(first))
    {
    unsigned int nbCrossings = polygon2->NbCrossing(current, first);

    resp = nbCrossings > 0;
    }
  return resp;
}
template<class TInputPolygon>
bool
PolygonToPolygonRCC8Calculator<TInputPolygon>
::ComputeEdgeEdge(PolygonPointerType polygon1, PolygonPointerType polygon2)
{
  bool                        resp = false;
  VertexListConstIteratorType it = polygon1->GetVertexList()->Begin();
  VertexListConstIteratorType it_end = polygon1->GetVertexList()->End();

  ContinuousIndexType current = it.Value();
  resp = polygon2->IsOnEdge(current);
  //std::cout<<"IsOnEdge: "<<current<<": "<<polygon2->IsOnEdge(current)<<std::endl;
  ContinuousIndexType first = current;
  ++it;

  while (!resp && it != it_end)
    {
    if (polygon2->NbTouching(current, it.Value()) > 0)
      {
      resp = true;
      //std::cout<<"NbCrossing: "<<current<<" -> "<<it.Value()<<": "<<polygon2->NbCrossing(current, it.Value())<<std::endl;
      }
    if (polygon2->NbCrossing(current, it.Value()) > 0)
      {
      resp = true;
      //std::cout<<"NbTouching: "<<current<<" -> "<<it.Value()<<": "<<polygon2->NbTouching(current, it.Value())<<std::endl;
      }
    current = it.Value();

    if (polygon2->IsOnEdge(current))
      {
      resp = true;
      //std::cout<<"IsOnEdge: "<<current<<": "<<polygon2->IsOnEdge(current)<<std::endl;
      }
    ++it;
    }
  if (!resp && polygon2->NbTouching(current, first) > 0)
    {
    resp = true;
    //std::cout<<"NbCrossing: "<<current<<" -> "<<first<<": "<<polygon2->NbCrossing(current, first)<<std::endl;
    }
  if (polygon2->NbCrossing(current, first) > 0)
    {
    resp = true;
    //std::cout<<"NbTouching: "<<current<<" -> "<<first<<": "<<polygon2->NbTouching(current, first)<<std::endl;
    }

  return resp;
}
/**
 * PrintSelf method
 */
template<class TInputPolygon>
void
PolygonToPolygonRCC8Calculator<TInputPolygon>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // end namespace otb

#endif
