/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBioCellularAggregateBase.cxx,v $
  Language:  C++
  Date:      $Date: 2004/10/20 11:31:48 $
  Version:   $Revision: 1.3 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "itkBioCellularAggregateBase.h"


namespace itk {


namespace bio {


CellularAggregateBase
::CellularAggregateBase()
{
}



CellularAggregateBase
::~CellularAggregateBase()
{
}



/** The actual implementation is provided in the derived classes where the Cell
 * dimension is known. */
CellularAggregateBase::SubstrateValueType 
CellularAggregateBase
::GetSubstrateValue( unsigned long int itkNotUsed(cellId), unsigned int itkNotUsed(substrateId) ) const
{
  return 0;
}



/** The actual implementation is provided in the derived classes where the Cell
 * dimension is known. */
void
CellularAggregateBase
::Add( CellBase *, CellBase *, double )
{
}


/** The actual implementation is provided in the derived classes where the Cell
 * dimension is known. */
void
CellularAggregateBase
::Remove( CellBase * )
{
}





void
CellularAggregateBase
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  
  os << "Cellular Aggregate Base " << std::endl;

}





} // end namespace bio

} // end namespace itk


