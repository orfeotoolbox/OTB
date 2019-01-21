/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


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
