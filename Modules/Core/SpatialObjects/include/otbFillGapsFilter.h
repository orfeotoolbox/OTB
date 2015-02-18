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
#ifndef __otbFillGapsFilter_h
#define __otbFillGapsFilter_h


#include "itkProcessObject.h"
#include "otbLineSpatialObjectList.h"

namespace otb
{
/** \class FillGapsFilter
 * \brief To be documented
 *
 * \ingroup OTBSpatialObjects
 */
class ITK_EXPORT FillGapsFilter : public itk::ProcessObject
{

public:
  /** Standard class typedefs. */
  typedef FillGapsFilter                Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(FillGapsFilter, itk::ProcessObject);

  /** Some convenient typedefs. */
  typedef LineSpatialObjectList                LineSpatialObjectListType;
  typedef LineSpatialObjectListType::Pointer   LineSpatialObjectListPointer;
  typedef LineSpatialObjectListType::LineType  LineSpatialObjectType;
  typedef LineSpatialObjectType::PointListType PointListType;
  typedef LineSpatialObjectType::LinePointType PointType;

  typedef itk::ProcessObject ProcessObjectType;

  void SetInput(const LineSpatialObjectListType * input);
  const LineSpatialObjectListType * GetInput();

  LineSpatialObjectListType * GetOutput();

  itkSetMacro(AngularBeam, double);
  itkGetConstReferenceMacro(AngularBeam, double);
  itkSetMacro(Radius, double);
  itkGetConstReferenceMacro(Radius, double);

protected:
  FillGapsFilter();

  virtual ~FillGapsFilter() {}

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  virtual void GenerateData();

private:
  FillGapsFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  double m_AngularBeam;
  double m_Radius;

};

} // end namespace otb

#endif
