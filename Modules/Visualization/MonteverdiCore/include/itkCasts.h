/*=========================================================================
 * TODO: Add licence notice.
 *=======================================================================*/

// Submitted by Stéphane ALBERT (otb.salbert@gmail.com).

#ifndef __itkCasts_h
#define __itkCasts_h

#include "itkSmartPointer.h"

namespace itk
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

} // end namespace itk.

#endif // __itkCasts_h
