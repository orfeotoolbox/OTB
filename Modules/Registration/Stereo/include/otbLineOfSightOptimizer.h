/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbLineOfSightOptimizer_h
#define otbLineOfSightOptimizer_h

#include "itkObject.h"

#include "itkPointSet.h"

namespace otb
{

/** \class LineOfSightOptimizer
 *  \brief Compute the position of a 3D point from a set of N lines of sight
 *
 *  This filter can use at least 2 lines of sight to estimate the position of
 *  the intersection using a Mean Square optimization method.
 *  This filter is templated over the precision of 3 points coordinates
 *
 *
 * \ingroup OTBStereo
 */
template <class TPrecision = float, class TLabel = int>
class ITK_EXPORT LineOfSightOptimizer : public itk::Object
{
public:
  /** Standard class typedef */
  typedef LineOfSightOptimizer          Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(LineOfSightOptimizer, itk::Object);

  /** Useful typedefs */
  typedef TPrecision PrecisionType;
  typedef TLabel     LabelType;
  typedef itk::DefaultStaticMeshTraits<TLabel, 3, 3, TPrecision> MeshType;
  typedef itk::PointSet<TLabel, 3, MeshType> PointSetType;
  typedef typename PointSetType::PointType                    PointType;
  typedef typename PointSetType::Pointer                      PointSetPointerType;
  typedef typename PointSetType::PointsContainerConstIterator PointSetConstIteratorType;
  typedef typename PointSetType::PointDataContainerIterator   LabelIteratorType;

  typedef std::vector<TPrecision> ResidueType;

  /** Compute the best intersection between N lines of sight.
   *  Starting points of every line of sight are stored in the point set 'pointA'
   *  ending points are stored in 'pointB' (however, the computation is symmetrical)*/
  PointType Compute(PointSetPointerType pointA, PointSetPointerType pointB);

  /** Get the residues from last computation */
  // itkGetMacro(Residues,ResidueType);
  ResidueType GetResidues()
  {
    return m_Residues;
  }

  /** Get the global residue from last computation */
  itkGetMacro(GlobalResidue, PrecisionType);

protected:
  /** Constructor */
  LineOfSightOptimizer();

  /** Destructor */
  ~LineOfSightOptimizer() override{};


private:
  LineOfSightOptimizer(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** residues from the last computation on each line of sight */
  ResidueType m_Residues;

  /** global residu from last computation */
  PrecisionType m_GlobalResidue;

  /** Internal matrices for computation */
  vnl_matrix<PrecisionType> m_InvCumul;
  vnl_matrix<PrecisionType> m_Identity;
  vnl_vector<PrecisionType> m_SecCumul;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLineOfSightOptimizer.hxx"
#endif

#endif
