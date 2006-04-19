/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - T.Feuvrier
  Language  :   C++
  Date      :   6 avril 2006
  Version   :   
  Role      :   Fill Gaps source Class
  $Id$

=========================================================================*/
#ifndef _otbFillGapsFilter_h
#define _otbFillGapsFilter_h

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif
#include "itkProcessObject.h"
#include "itkDataObject.h"
#include "otbLineSpatialObjectList.h"

namespace otb
{
/** \class FillGapsFilter
 */
 
//template <class TLineSpatialObject>
class ITK_EXPORT FillGapsFilter : public itk::ProcessObject
{
public:
  /** Standard class typedefs. */
  typedef FillGapsFilter                                Self;
  typedef itk::ProcessObject                            Superclass;
  typedef itk::SmartPointer<Self>                       Pointer;
  typedef itk::SmartPointer<const Self>                 ConstPointer;
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);  

  /** Run-time type information (and related methods). */
  itkTypeMacro(FillGapsFilter,itk::ProcessObject);

  /** Some convenient typedefs. */
  typedef LineSpatialObjectList                                 LineSpatialObjectListType;
  typedef LineSpatialObjectListType::Pointer                    LineSpatialObjectListPointer;
  typedef LineSpatialObjectListType::LineType                   LineSpatialObjectType;
  typedef LineSpatialObjectType::PointListType                  PointListType;
  typedef LineSpatialObjectType::LinePointType                  PointType;

  virtual void SetInput(const LineSpatialObjectListType * input);
  const LineSpatialObjectListType * GetInput();

  LineSpatialObjectListType * GetOutput();


  itkSetMacro(AngularBeam, double);
  itkGetConstReferenceMacro(AngularBeam, double);  
  itkSetMacro(Radius, double);
  itkGetConstReferenceMacro(Radius, double);  

 
protected:
  FillGapsFilter();
  virtual ~FillGapsFilter(){};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;
  
  virtual void GenerateData();

  
private:
  FillGapsFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  double m_AngularBeam;
  double m_Radius;
  
};
  

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbFillGapsFilter.txx"
#endif

#endif

