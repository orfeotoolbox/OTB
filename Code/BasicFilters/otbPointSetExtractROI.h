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
#ifndef __otbPointSetExtractROI_h
#define __otbPointSetExtractROI_h

#include "otbPointSetToPointSetFilter.h"
#include "itkTransform.h"

namespace otb
{

/** \class PointSetExtractROI
 * \brief 
 *
 * PointSetExtractROI applies a transform to all the points
 * of a PointSet.
 *
 * The additional content of the PointSet is passed untouched.
 * 
 * 
 * \ingroup PointSetFilters
 */
template <class TInputPointSet, class TOutputPointSet>
class ITK_EXPORT PointSetExtractROI :
    public PointSetToPointSetFilter<TInputPointSet,TOutputPointSet>
{
public:
  /** Standard class typedefs. */
  typedef PointSetExtractROI                                       Self;
  typedef PointSetToPointSetFilter<TInputPointSet,TOutputPointSet> Superclass;
  typedef itk::SmartPointer<Self>                                  Pointer;
  typedef itk::SmartPointer<const Self>                            ConstPointer;
  
  typedef TInputPointSet                       InputPointSetType;
  typedef TOutputPointSet                      OutputPointSetType;
  typedef typename InputPointSetType::Pointer  InputPointSetPointer;
  typedef typename OutputPointSetType::Pointer OutputPointSetPointer;
  
  /** Type for representing coordinates. */
  typedef typename TInputPointSet::CoordRepType  CoordRepType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(PointSetExtractROI,PointSetToPointSetFilter);

  /** Set/Get Start methods */
  itkSetMacro(StartX,unsigned long);
  itkSetMacro(StartY,unsigned long);
  /** Set/Get Size methods */
  itkSetMacro(SizeX,unsigned long);
  itkSetMacro(SizeY,unsigned long);

protected:
  PointSetExtractROI();
  ~PointSetExtractROI() {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;
  
  /** Generate Requested Data */
  virtual void GenerateData( void );


private:
  PointSetExtractROI(const PointSetExtractROI&); //purposely not implemented
  void operator=(const PointSetExtractROI&); //purposely not implemented
  
  /** X/Y coordinates of the first point of the region to extract. */
  unsigned long m_StartX;
  unsigned long m_StartY;
  /** Size in X/Y of the region to extract. */
  unsigned long m_SizeX;
  unsigned long m_SizeY;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPointSetExtractROI.txx"
#endif

#endif
