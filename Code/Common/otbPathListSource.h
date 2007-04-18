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
#ifndef _otbPathListSource_h
#define _otbPathListSource_h

#include "itkProcessObject.h"
#include "otbObjectList.h"

namespace otb
{
  
/** \class PathListSource
 * \brief Base class for filters with a PathList output type.  
 *
 * PathListSource is the base class for all filters producing a PathList.
 * This class defines the GetOutput().
 *
 * \ingroup DataSources
 * \ingroup Paths
 * \ingroup Lists
 */

template <class TOutputPath >
  class ITK_EXPORT PathListSource : public itk::ProcessObject
{
public:
  /** Standard class typedefs. */
  typedef PathListSource                 Self;
  typedef itk::ProcessObject             Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;
    
  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(PathListSource,itk::ProcessObject);
  
  /** Some convenient typedefs. */
  
  typedef TOutputPath                               OutputPathType;
  typedef typename OutputPathType::Pointer          OutputPathPointerType;
  typedef otb::ObjectList<OutputPathType>           OutputPathListType;
  typedef typename OutputPathListType::Pointer      OutputPathListPointerType;
  typedef typename OutputPathListType::ConstPointer OutputPathListConstPointerType;
  
  /** Overiding of the GetOutput() method */
  OutputPathListType * GetOutput(void);

protected:
  PathListSource();
  virtual ~PathListSource() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;
    
private:
  PathListSource(const Self&); //purposely not implemented
  void operator=(const Self&);   //purposely not implemented
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPathListSource.txx"
#endif

#endif
