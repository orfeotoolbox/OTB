/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - T.Feuvrier
  Language  :   C++
  Date      :   6 avril 2006
  Version   :   
  Role      :   Base class for a list of itk LineSpatialObject
  $Id$

=========================================================================*/
#ifndef _otbLineSpatialObjectList_h
#define _otbLineSpatialObjectList_h

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkObject.h"
#include "itkLineSpatialObject.h"

#include <list>

namespace otb
{
/** \class LineSpatialObjectList
 */
 
class ITK_EXPORT LineSpatialObjectList :        public std::list< itk::LineSpatialObject<2>::Pointer >
                                                //public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef LineSpatialObjectList                         Self;
//  typedef itk::Object                                   Superclass;
  typedef std::list< itk::LineSpatialObject<2>::Pointer >  Superclass;

#if 0
  typedef itk::SmartPointer<Self>                       Pointer;
  typedef itk::SmartPointer<const Self>                 ConstPointer;
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);  

  /** Run-time type information (and related methods). */
  itkTypeMacro(LineSpatialObjectList,itk::Object);
#endif

  /** Some convenient typedefs. */
  typedef itk::LineSpatialObject<2>                     LineType;

};
  

} // end namespace otb

#endif
