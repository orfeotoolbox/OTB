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

 *=======================================================================*/

#ifndef __otbCast_h
#define __otbCast_h

#include "itkSmartPointer.h"

namespace otb
{
/**
 * \brief dynamic_cast<>() like operator to work with itk::SmartPointer<>.
 *
 * Works like C++ dynamic_cast<>() operator:
 *
 *   DerivedClass::Pointer pointer(
 *     itk::DynamicCast< DerivedClass >( smartPointerToBaseClass )
 *   );
 */
template< typename T2, typename T1 >
inline
itk::SmartPointer< T2 >
DynamicCast( const itk::SmartPointer< T1 > & p1 )
{
  return
    typename itk::SmartPointer< T2 >(
      dynamic_cast< typename itk::SmartPointer< T2 >::ObjectType * >(
	p1.GetPointer()
      )
    );
}

/**
 * \brief const_cast<>() like operator to work with itk::SmartPointer<>.
 *
 * Works like C++ const_cast<>() operator:
 *
 *   MyClass::Pointer pointer(
 *     itk::ConstCast< DerivedClass >( smartPointerToConstInstance )
 *   );
 */
template< typename T2, typename T1 >
inline
itk::SmartPointer< const T2 >
ConstCast( const itk::SmartPointer< T1 > & p1 )
{
  return
    typename itk::SmartPointer< const T2 >(
      const_cast< typename itk::SmartPointer< const T2 >::ObjectType * >(
	p1.GetPointer()
      )
    );
}

/**
 * \brief const_cast<>() like operator to work with itk::SmartPointer<>.
 *
 * Works like C++ const_cast<>() operator:
 *
 *   MyClass::ConstPointer constPointer(
 *     itk::ConstCast< const DerivedClass >(
 *       smartPointerToNonConstInstance
 *     )
 *   );
 */
template< typename T2, typename T1 >
inline
itk::SmartPointer< T2 >
ConstCast( const itk::SmartPointer< const T1 > & p1 )
{
  return
    typename itk::SmartPointer< T2 >(
      const_cast< typename itk::SmartPointer< T2 >::ObjectType * >(
	p1.GetPointer()
      )
    );
}

} // end namespace otb.

#endif // __otbCast_h
