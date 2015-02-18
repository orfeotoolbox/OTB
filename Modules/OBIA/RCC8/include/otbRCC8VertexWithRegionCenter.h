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
#ifndef __otbRCC8VertexWithRegionCenter_h
#define __otbRCC8VertexWithRegionCenter_h

#include "otbRCC8VertexBase.h"

namespace otb
{
/** \class RCC8VertexWithRegionCenter
 *  \brief Class to represent a vertex in a RCC8 Graph with the compacity value of
 *  the represented region.
 *
 * \sa RCC8Graph, RCC8Edge, RCC8VertexBase
 */
template <class TLabel, class TPrecision = float>
class ITK_EXPORT RCC8VertexWithRegionCenter
  : public RCC8VertexBase<TLabel>
{
public:
  /** Standard class typedefs */
  typedef RCC8VertexWithRegionCenter    Self;
  typedef otb::RCC8VertexBase<TLabel>   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  /** Run-time type information (and related methods). */
  itkTypeMacro(RCC8VertexWithRegionCenter, RCC8VertexBase);
  /** Input image associated typedefs*/
  typedef TLabel LabelType;
  /** Precision  typedef */
  typedef TPrecision PrecisionType;
  /** char* vector attributes */
  typedef  typename Superclass::AttributesMapType AttributesMapType;

  itkGetMacro(RegionCenterXCoordinate, PrecisionType);
  itkSetMacro(RegionCenterXCoordinate, PrecisionType);
  itkGetMacro(RegionCenterYCoordinate, PrecisionType);
  itkSetMacro(RegionCenterYCoordinate, PrecisionType);
  /**
   * Set the VertexWithRegionCenter attributes from the attributes vector.
   * \param attributes The vector containing the parsed attributes.
   */
  void SetAttributesMap(AttributesMapType attributes);
  /**
   * Get an attributes vector representing the VertexWithRegionCenter attributes.
   * \return The attributes vector
   */
  AttributesMapType GetAttributesMap(void);

protected:
  /** Constructor */
  RCC8VertexWithRegionCenter();
  /** Desctructor */
  ~RCC8VertexWithRegionCenter() {}
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  /** The compacity */
  PrecisionType m_RegionCenterXCoordinate;
  PrecisionType m_RegionCenterYCoordinate;
};
} // end namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRCC8VertexWithRegionCenter.txx"
#endif

#endif
