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
  typedef FillGapsFilter                                      Self;
  typedef itk::ProcessObject                            Superclass;
  typedef itk::SmartPointer<Self>                       Pointer;
  typedef itk::SmartPointer<const Self>                 ConstPointer;
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);  

  /** Run-time type information (and related methods). */
  itkTypeMacro(FillGapsFilter,itk::ProcessObject);

  /** Some convenient typedefs. */
  typedef otb::LineSpatialObjectList                           LineSpatialObjectListType;
  typedef LineSpatialObjectListType::LineType                  LineSpatialObjectType;

  
  /** Set the list of LineSpatialObject of this process object.  */
  void SetOutput(const LineSpatialObjectListType & pLineSpatialObjectList);

  /** Get the list of LineSpatialObject of this process object.  */
  LineSpatialObjectListType & GetOutput(void);
  
protected:
  FillGapsFilter();
  virtual ~FillGapsFilter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;
  
private:
  FillGapsFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented


        LineSpatialObjectListType m_InputLineSpatialObjectList;
        LineSpatialObjectListType m_OutputLineSpatialObjectList;

};
  

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbFillGapsFilter.txx"
#endif

#endif
