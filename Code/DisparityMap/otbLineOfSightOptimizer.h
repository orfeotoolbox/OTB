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
#ifndef __otbLineOfSightOptimizer_h
#define __otbLineOfSightOptimizer_h

#include "itkObject.h"
#include "itkDefaultStaticMeshTraits.h"

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
 */
template <class TPrecision = float, class TLabel = int>
class ITK_EXPORT LineOfSightOptimizer :
  public itk::Object
{
public:
  /** Standard class typedef */
  typedef LineOfSightOptimizer                      Self;
  typedef itk::Object                               Superclass;
  typedef itk::SmartPointer<Self>                   Pointer;
  typedef itk::SmartPointer<const Self>             ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(LineOfSightOptimizer, itk::Object);

  /** Usefull typedefs */
  typedef TPrecision  PrecisionType;
  typedef TLabel      LabelType;
  typedef itk::DefaultStaticMeshTraits<TLabel,3,3,TPrecision> MeshType;
  typedef itk::PointSet<TLabel,3, MeshType >        PointSetType;
  typedef typename PointSetType::PointType        PointType;
  typedef typename PointSetType::Pointer          PointSetPointerType;
  typedef typename PointSetType::PointsContainerConstIterator PointSetConstIteratorType;
  typedef typename PointSetType::PointDataContainerIterator LabelIteratorType;

  typedef std::vector<TPrecision>                 ResidueType;

  /** Compute the best intersection between N lines of sight.
   *  Starting points of every line of sight are stored in the point set 'pointA'
   *  ending points are stored in 'pointB' (however, the computation is symetrical)*/
  PointType Compute(PointSetPointerType pointA, PointSetPointerType pointB);

  /** Get the residues from last computation */
  //itkGetMacro(Residues,ResidueType);
  ResidueType GetResidues()
  {
    return m_Residues;
  }

  /** Get the global residue from last computation */
  itkGetMacro(GlobalResidue,PrecisionType);

protected:
  /** Constructor */
  LineOfSightOptimizer();

  /** Destructor */
  virtual ~LineOfSightOptimizer(){};


private:
  LineOfSightOptimizer(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

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
#include "otbLineOfSightOptimizer.txx"
#endif

#endif
