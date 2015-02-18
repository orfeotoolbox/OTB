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
#ifndef __otbRCC8Edge_h
#define __otbRCC8Edge_h

#include "itkDataObject.h"
#include "itkObjectFactory.h"
#include "otbRCC8Value.h"

namespace otb
{
/** \class RCC8Edge
 *  \brief Base class to represent an edge in a RCC8 Graph.
 *
 * \sa RCC8Graph, RCC8VertexBase
 */
class ITK_EXPORT RCC8Edge : public itk::DataObject
{
public:
  /** Standard class typedefs */
  typedef RCC8Edge                      Self;
  typedef itk::DataObject               Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  /** Run-time type information (and related methods). */
  itkTypeMacro(RCC8Edge, DataObject);
  /** RCC8 values typedef */
  typedef RCC8Value RCC8ValueType;
  /** RCC8 relation value accessors */
  itkGetMacro(Value, RCC8ValueType);
  itkSetMacro(Value, RCC8ValueType);

protected:
  /** Constructor */
  RCC8Edge();
  /** Desctructor */
  ~RCC8Edge() {}
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  /** The RCC8 value */
  RCC8ValueType m_Value;
};
} // end namespace otb
#endif
