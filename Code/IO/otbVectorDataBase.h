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

#ifndef __otbVectorDataBase_h
#define __otbVectorDataBase_h

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkDataObject.h"
#include "itkObjectFactory.h"
#include "itkMacro.h"

namespace otb
{
/** \class VectorDataBase
 * \brief Base class for templated image classes.
 *
 * ImageBase is the base class for the templated VectorDataBase
 * classes. VectorDataBase does not store any of the vector data values.
 *
 */

class ITK_EXPORT VectorDataBase : public itk::DataObject
{
public:

  /** Standard class typedefs. */
  typedef VectorDataBase                Self;
  typedef itk::DataObject               Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorDataBase, itk::DataObject);

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

protected:
  VectorDataBase();
  ~VectorDataBase();

private:
  VectorDataBase(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

} ;

} // end namespace otb

#endif // __otbVectorDataBase_h

