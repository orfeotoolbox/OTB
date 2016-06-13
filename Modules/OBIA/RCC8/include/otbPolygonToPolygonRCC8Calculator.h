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
#ifndef otbPolygonToPolygonRCC8Calculator_h
#define otbPolygonToPolygonRCC8Calculator_h

#include "itkObject.h"
#include "otbRCC8Value.h"
#include "otbImage.h"

namespace otb
{
/**
 * \class PolygonToPolygonRCC8Calculator
 * \brief This class compute the RCC8 relation between the regions from two segmentation images.
 *
 * The RCC8 system comes from qualitative spatial reasoning.
 * It is a set of pairwise disjoint exhaustive relation between two closed region of space.
 * There are 8 possible relations :
 * DC: Disconnected
 * EC: Externaly connected
 * PO: Partial overlap
 * TPP: Tangential proper part
 * NTPP: Non tangential proper part
 * TPPI: Tangential proper part inverse
 * NTPPI: Non tangential proper part inverse
 * EQ: Equivalence
 *
 * The goal of this class is to determine which of these 8 relations link the two inputs regions represented
 * by the closed input path. Since this class will further be used iteratively on a possibly large set
 * of regiosn, it is optimised : the decision is managed by a decision tree.
 *
 * \ingroup OTBRCC8
 */
template <class TInputPolygon>
class ITK_EXPORT PolygonToPolygonRCC8Calculator : public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef PolygonToPolygonRCC8Calculator Self;
  typedef itk::Object                    Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;
  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  /** Run-time type information (and related methods). */
  itkTypeMacro(PolygonToPolygonRCC8Calculator, Object);
  /** Types definitions for the input image. */
  typedef TInputPolygon                                     PolygonType;
  typedef typename PolygonType::Pointer                     PolygonPointerType;
  typedef typename PolygonType::ConstPointer                PolygonConstPointerType;
  typedef typename PolygonType::VertexListType              VertexListType;
  typedef typename PolygonType::ContinuousIndexType         ContinuousIndexType;
  typedef typename PolygonType::RegionType::ImageRegionType RegionType;
  typedef typename VertexListType::ConstIterator            VertexListConstIteratorType;
  typedef std::vector<bool>                                 BoolVectorType;

  /** RCC8 values type */
  typedef RCC8Value RCC8ValueType;

  /**
   * Get the RCC8 relation.
   * \return The RCC8 relation value.
   */
  RCC8ValueType GetValue(void);

  /** Set external knowledge to help the decision process */
  itkSetMacro(Level1APrioriKnowledge, bool);
  itkSetMacro(Level3APrioriKnowledge, bool);
  itkGetMacro(Level1APrioriKnowledge, bool);
  itkGetMacro(Level3APrioriKnowledge, bool);

  itkSetObjectMacro(Polygon1, PolygonType);
  itkSetObjectMacro(Polygon2, PolygonType);
  itkGetObjectMacro(Polygon1, PolygonType);
  itkGetObjectMacro(Polygon2, PolygonType);

  /** Main computation method */
  void Compute(void);

  virtual bool ComputeRelation(bool edgeEdgeBool, bool interExterBool, bool exterInterBool);

  virtual bool ComputeInterExter(PolygonPointerType path1, PolygonPointerType path2);

  virtual bool ComputeEdgeEdge(PolygonPointerType path1, PolygonPointerType path2);

  virtual bool ComputeInterInter(PolygonPointerType path1, PolygonPointerType path2);

  virtual bool RegionsIntersectionIsNull(const RegionType& region1, const RegionType& region2);

protected:
  /** Constructor */
  PolygonToPolygonRCC8Calculator();
  /** Destructor */
  ~PolygonToPolygonRCC8Calculator() ITK_OVERRIDE {}
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

private:
  /** The RCC8 relation value */
  RCC8ValueType m_Value;
  /**  Decision tree Level 1 A priori knowledge */
  bool m_Level1APrioriKnowledge;
  /**  Decision tree Level 3 A priori knowledge */
  bool m_Level3APrioriKnowledge;
  /** Polygon of region 1 */
  PolygonPointerType m_Polygon1;
  /** Polygon of region 2 */
  PolygonPointerType m_Polygon2;
  /** Epsilon */
  double m_Epsilon;

};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPolygonToPolygonRCC8Calculator.txx"
#endif

#endif
