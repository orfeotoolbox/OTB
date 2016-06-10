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
#ifndef __otbVectorDataToRandomLineGenerator_h
#define __otbVectorDataToRandomLineGenerator_h

#include "otbVectorDataSource.h"
#include "otbVectorData.h"
#include "itkPreOrderTreeIterator.h"
#include "itkMersenneTwisterRandomVariateGenerator.h"

namespace otb
{
/** \class VectorDataToRandomLineGenerator
 *  \brief Produces a VectorData Containing Random Lines
 *
 *  This generator produces a unique vector data containing
 *  randomly generated lines inside polygons contained in the input
 *  vectorData.
 *
 *  The user can set the range of point per line through
 *  SetMinLineSize() and SetMaxLineSize().
 *  The generated line number per polygon can be set through
 *  SetNumberOfOutputLine
 *
 *  \sa VectorDataSource
 *
 * \ingroup OTBVectorDataManipulation
 */
template <class TVectorData>
class ITK_EXPORT VectorDataToRandomLineGenerator :
    public VectorDataSource<TVectorData>
{
public:
  /** Standard class typedefs */
  typedef VectorDataToRandomLineGenerator              Self;
  typedef VectorDataSource<TVectorData>                Superclass;
  typedef itk::SmartPointer<Self>                      Pointer;
  typedef itk::SmartPointer<const Self>                ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorDataToRandomLineGenerator, VectorDataSource);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  typedef TVectorData                                           VectorDataType;
  typedef typename VectorDataType::Pointer                      VectorDataPointerType;
  typedef typename VectorDataType::DataNodeType                 DataNodeType;
  typedef typename DataNodeType::PolygonType::RegionType        RegionType;
  typedef typename DataNodeType::PointType                      PointType;
  typedef typename DataNodeType::LineType                       LineType;
  typedef typename DataNodeType::LineType::VertexType           VertexType;
  typedef typename std::vector<PointType>                       PointVectorType;

  typedef itk::PreOrderTreeIterator<typename VectorDataType::DataTreeType> TreeIteratorType;

  typedef itk::Statistics::MersenneTwisterRandomVariateGenerator RandomGeneratorType;

  /** Connects the VectorData from which the extend will be extracted. */
  using Superclass::SetInput;
  void SetInput(const VectorDataType *);
  const VectorDataType * GetInput() const;

  itkGetConstMacro(NumberOfOutputLine, unsigned int);
  itkSetMacro(NumberOfOutputLine, unsigned int);

  itkGetConstMacro(MinLineSize, unsigned int);
  itkSetMacro(MinLineSize, unsigned int);

  itkGetConstMacro(MaxLineSize, unsigned int);
  itkSetMacro(MaxLineSize, unsigned int);

  /** Set the seed for random number generator */
  void SetSeed(unsigned int seed)
  {
    m_RandomGenerator->SetSeed(seed);
    m_RandomSizeGenerator->SetSeed(seed);
  }

protected:
  VectorDataToRandomLineGenerator();
  ~VectorDataToRandomLineGenerator() ITK_OVERRIDE {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

  /** Triggers the Computation of the sample list */
  void GenerateData(void) ITK_OVERRIDE;

  PointVectorType RandomPointsGenerator(DataNodeType * node);

private:
  VectorDataToRandomLineGenerator(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  std::string GetNextID()
    {
      std::ostringstream oss;
      oss << m_CurrentID++;
      return oss.str();
    }

  RandomGeneratorType::Pointer m_RandomGenerator;
  RandomGeneratorType::Pointer m_RandomSizeGenerator;

  unsigned int                 m_NumberOfOutputLine;
  unsigned int                 m_MinLineSize;
  unsigned int                 m_MaxLineSize;
  unsigned int                 m_CurrentID;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorDataToRandomLineGenerator.txx"
#endif

#endif
