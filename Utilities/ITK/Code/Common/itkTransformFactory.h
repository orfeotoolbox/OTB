/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkTransformFactory.h,v $
  Language:  C++
  Date:      $Date: 2009-03-03 15:10:26 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkTransformFactory_h
#define __itkTransformFactory_h

#include "itkTransformFactoryBase.h"

namespace itk
{
/** \class TransformFactory
 * \brief Create instances of Transforms
 */

template <class T>
class TransformFactory : public TransformFactoryBase
{
public:  
  static void RegisterTransform ()
    {
    typename T::Pointer t = T::New();

    TransformFactoryBase::Pointer f = TransformFactoryBase::GetFactory();

    f->RegisterTransform ( t->GetTransformTypeAsString().c_str(),
                           t->GetTransformTypeAsString().c_str(),
                           t->GetTransformTypeAsString().c_str(),
                           1,
                           CreateObjectFunction<T>::New() );
    }
};
} // end namespace itk

#endif
