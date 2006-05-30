/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkConditionalConstIterator.txx,v $
  Language:  C++
  Date:      $Date: 2003/09/10 14:29:04 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkConditionalConstIterator_txx
#define _itkConditionalConstIterator_txx

#include "itkConditionalConstIterator.h"

namespace itk
{

template <typename TImageType>
ConditionalConstIterator<TImageType>
::ConditionalConstIterator()
{
}


template <typename TImageType>
ConditionalConstIterator<TImageType>
::~ConditionalConstIterator()
{
}




} // end namespace itk

#endif
