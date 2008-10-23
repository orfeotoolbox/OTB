/*=========================================================================

  Program:   MetaIO
  Module:    $RCSfile: metaImageUtils.h,v $
  Language:  C++
  Date:      $Date: 2008-04-09 01:44:28 $
  Version:   $Revision: 1.11 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "metaTypes.h"

#ifndef ITKMetaIO_METAIMAGEUTILS_H
#define ITKMetaIO_METAIMAGEUTILS_H

#include "metaImageTypes.h"

#if (METAIO_USE_NAMESPACE)
namespace METAIO_NAMESPACE {
#endif

METAIO_EXPORT bool MET_StringToImageModality(const char * _str,
                                      MET_ImageModalityEnumType * _type);

METAIO_EXPORT bool MET_ImageModalityToString(MET_ImageModalityEnumType _type,
                                      char * _str);

#if (METAIO_USE_NAMESPACE)
};
#endif

#endif
