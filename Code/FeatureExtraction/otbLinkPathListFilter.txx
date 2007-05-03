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
#ifndef _otbLinkPathListFilter_txx
#define _otbLinkPathListFilter_txx

#include "otbLinkPathListFilter.h"

namespace otb
{
/**
 * Constructor
 */
template <class TPath>
LinkPathListFilter<TPath>
::LinkPathListFilter()
{
}


template <class TPath>
void
LinkPathListFilter<TPath>
::GenerateData()
{
        const PathListType *  inputPathList  = this->GetInput();
        PathListType *  outputPathList = this->GetOutput();

        typename PathListType::ConstIterator listIt = inputPathList->Begin();
        outputPathList->Clear();
        
        PathListPointerType newTempPathList = PathListType::New(); 
        while( listIt != inputPathList->End())
        {



                ++listIt;
        }
}

/**
 * PrintSelf Method
 */
template <class TPath>
void
LinkPathListFilter<TPath>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // End namespace otb
#endif
