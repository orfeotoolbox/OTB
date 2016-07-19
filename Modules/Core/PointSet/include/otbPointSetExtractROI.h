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
#ifndef otbPointSetExtractROI_h
#define otbPointSetExtractROI_h

#include "otbPointSetToPointSetFilter.h"
#include "itkTransform.h"

namespace otb
{

/** \class PointSetExtractROI
 * \brief Extract a part of the points specified by a region
 *
 * PointSetExtractROI extract all the points of a PointSet that are
 * in a given region.
 *
 * The additional content of the PointSet is passed untouched.
 *
 *
 * \ingroup PointSetFilters
 *
 * \ingroup OTBPointSet
 */
template <class TInputPointSet, class TOutputPointSet>
class ITK_EXPORT PointSetExtractROI :
  public PointSetToPointSetFilter<TInputPointSet, TOutputPointSet>
{
public:
  /** Standard class typedefs. */
  typedef PointSetExtractROI                                        Self;
  typedef PointSetToPointSetFilter<TInputPointSet, TOutputPointSet> Superclass;
  typedef itk::SmartPointer<Self>                                   Pointer;
  typedef itk::SmartPointer<const Self>                             ConstPointer;

  typedef TInputPointSet                       InputPointSetType;
  typedef TOutputPointSet                      OutputPointSetType;
  typedef typename InputPointSetType::Pointer  InputPointSetPointer;
  typedef typename OutputPointSetType::Pointer OutputPointSetPointer;

  /** Type for representing coordinates. */
  typedef typename TInputPointSet::CoordRepType CoordRepType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(PointSetExtractROI, PointSetToPointSetFilter);

  /** Set/Get Start methods */
  itkGetMacro(StartX, unsigned long);
  itkGetMacro(StartY, unsigned long);
  itkSetMacro(StartX, unsigned long);
  itkSetMacro(StartY, unsigned long);
  /** Set/Get Size methods */
  itkGetMacro(SizeX, unsigned long);
  itkGetMacro(SizeY, unsigned long);
  itkSetMacro(SizeX, unsigned long);
  itkSetMacro(SizeY, unsigned long);

protected:
  PointSetExtractROI();
  ~PointSetExtractROI() ITK_OVERRIDE {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

  /** Generate Requested Data */
  void GenerateData(void) ITK_OVERRIDE;

private:
  PointSetExtractROI(const PointSetExtractROI &); //purposely not implemented
  void operator =(const PointSetExtractROI&); //purposely not implemented

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
