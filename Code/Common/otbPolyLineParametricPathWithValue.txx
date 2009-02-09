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
#ifndef __otbPolyLineParametricPathWithValue_txx
#define __otbPolyLineParametricPathWithValue_txx

#include "otbPolyLineParametricPathWithValue.h"

namespace otb
{
/**
 * Constructor
 */
template < class TValue,unsigned int VDimension>
PolyLineParametricPathWithValue<TValue,VDimension>
::PolyLineParametricPathWithValue()
{
  itk::MetaDataDictionary & dict = this->GetMetaDataDictionary();
  m_Key = "Value";
  itk::EncapsulateMetaData<ValueType>(dict,m_Key,0);
  m_LengthIsValid = false;
  m_Length = -1.0;
}

template < class TValue,unsigned int VDimension>
void PolyLineParametricPathWithValue<TValue,VDimension>
::AddVertex(const ContinuousIndexType &vertex)
{
  Superclass::AddVertex(vertex);
  m_LengthIsValid=false;
}

template < class TValue,unsigned int VDimension>
double PolyLineParametricPathWithValue<TValue,VDimension>
::GetLength() const
{
  if (!m_LengthIsValid)
  {
    ComputeLength();
  }
  return m_Length;
}

template < class TValue,unsigned int VDimension>
void 
PolyLineParametricPathWithValue<TValue,VDimension>
::ComputeLength() const
{
  double length = 0.0;
  VertexListConstIteratorType it =  this->GetVertexList()->Begin();

  if (this->GetVertexList()->Size()>1)
  {


    VertexType pt1 = it.Value();//just init, won't be used like that
    VertexType pt2 = it.Value();

    it++;
    while (it != this->GetVertexList()->End())
    {
      pt1=pt2;
      pt2 = it.Value();
      double accum=0.0;
      for (unsigned int i=0; i<VDimension; i++)
      {
        accum += (pt1[i]-pt2[i])*(pt1[i]-pt2[i]);
      }
      length += vcl_sqrt(accum);
      it++;
    }

  }
  else //if there is strictly less than 2 points, length is 0
  {
    length = 0.0;
  }

  m_Length = length;
  m_LengthIsValid = true;
}


/**
 * PrintSelf Method
 */
template < class TValue,unsigned int VDimension>
void
PolyLineParametricPathWithValue<TValue,VDimension>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  VertexListConstIteratorType it =  this->GetVertexList()->Begin();
  while (it != this->GetVertexList()->End())
  {
    os << it.Value() << " - ";
    it++;
  }
  os << std::endl;
}

} // end namespace otb
#endif
