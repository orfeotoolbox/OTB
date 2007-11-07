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
#ifndef _otbPersistentFilterStreamingDecorator_txx
#define _otbPersistentFilterStreamingDecorator_txx

#include "otbPersistentFilterStreamingDecorator.h"

namespace otb
{
/**
 * Constructor
 */
template <class TFilter>
PersistentFilterStreamingDecorator<TFilter>
::PersistentFilterStreamingDecorator()
{
  m_Filter = FilterType::New();
  m_Streamer = StreamerType::New();
}
template <class TFilter>
void
PersistentFilterStreamingDecorator<TFilter>
::SetInput(const ImageType *input)
{
  // ProcessObject is not const_correct so this cast is required here.
  this->ProcessObject::SetNthInput(0,const_cast<ImageType *>(input ) );
}
template <class TFilter>
const typename PersistentFilterStreamingDecorator<TFilter>::ImageType *
PersistentFilterStreamingDecorator<TFilter>
::GetInput(void)
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }
  
  return static_cast<ImageType*>(this->ProcessObject::GetInput(0));
}
template <class TFilter>
const typename PersistentFilterStreamingDecorator<TFilter>::ImageType *
PersistentFilterStreamingDecorator<TFilter>
::GetInput(unsigned int idx)
{
  return static_cast<ImageType*>(this->ProcessObject::GetInput(idx));
}
template <class TFilter>
void
PersistentFilterStreamingDecorator<TFilter>
::GenerateData(void)
{
  // Reset the filter before the generation.
  this->GetFilter()->Reset();
  m_Streamer->SetInput(m_Filter->GetOutput());
  m_Streamer->Update();
  // Synthetize data afther the streaming of the whole image.
  this->GetFilter()->Synthetize();
}

template <class TFilter>
void
PersistentFilterStreamingDecorator<TFilter>
::Update(void)
{
  this->GenerateData();
}
/**
 * PrintSelf Method
 */
template <class TFilter>
void
PersistentFilterStreamingDecorator<TFilter>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
