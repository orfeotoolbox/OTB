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

#ifndef __otbVectorDataToImageFilter_txx
#define __otbVectorDataToImageFilter_txx

#include "otbVectorDataToImageFilter.h"

namespace otb
{

/**
   * Constructor
 */
  template <class TVectorData, class TImage>
      VectorDataToImageFilter<TVectorData, TImage>
  ::VectorDataToImageFilter()
  {
    this->SetNumberOfRequiredInputs( 1 );
    m_Spacing.Fill(1.0);
    m_Origin.Fill(0.0);
    m_Direction.SetIdentity();
    m_Size.Fill( 0 );
    m_StartIndex.Fill( 0 );
  }


  template <class TVectorData, class TImage>
      void
          VectorDataToImageFilter<TVectorData, TImage>
  ::SetInput(const VectorDataType *input)
  {
// Process object is not const-correct so the const_cast is required here
    this->itk::ProcessObject::SetNthInput(0,
                                          const_cast< VectorDataType * >( input ) );
  }

  template <class TVectorData, class TImage>
      const typename VectorDataToImageFilter<TVectorData, TImage>::VectorDataType *
          VectorDataToImageFilter<TVectorData, TImage>
  ::GetInput(void)
  {
    if (this->GetNumberOfInputs() < 1)
    {
      return 0;
    }

    return static_cast<const TVectorData * >
        (this->itk::ProcessObject::GetInput(0) );
  }



//----------------------------------------------------------------------------
  template <class TVectorData, class TImage>
      void
          VectorDataToImageFilter<TVectorData, TImage>
  ::SetSpacing(const SpacingType & spacing )
  {
    itkDebugMacro("setting Spacing to " << spacing);
    if ( this->m_Spacing != spacing )
    {
      this->m_Spacing = spacing;
      this->Modified();
    }
  }


//----------------------------------------------------------------------------
  template <class TVectorData, class TImage>
      void
          VectorDataToImageFilter<TVectorData, TImage>
  ::SetSpacing(const double spacing[2] )
  {
    SpacingType s(spacing);
    this->SetSpacing(s);
  }


//----------------------------------------------------------------------------
  template <class TVectorData, class TImage>
      void
          VectorDataToImageFilter<TVectorData, TImage>
  ::SetSpacing(const float spacing[2] )
  {
    itk::Vector<float, 2> sf(spacing);
    SpacingType s;
    s.CastFrom( sf );
    this->SetSpacing(s);
  }

//----------------------------------------------------------------------------
  template <class TVectorData, class TImage>
      void
          VectorDataToImageFilter<TVectorData, TImage>
  ::SetOrigin(const double origin[2] )
  {
    OriginType p(origin);
    this->SetOrigin( p );
  }


//----------------------------------------------------------------------------
  template <class TVectorData, class TImage>
      void
          VectorDataToImageFilter<TVectorData, TImage>
  ::SetOrigin(const float origin[2] )
  {
    itk::Point<float, 2> of(origin);
    OriginType p;
    p.CastFrom( of );
    this->SetOrigin( p );
  }

/**
   * Inform pipeline of required output region
 */
  template <class TVectorData, class TImage>
      void
          VectorDataToImageFilter<TVectorData, TImage>
  ::GenerateOutputInformation()
  {

    // we can't call the superclass method here.

    // get pointers to the input and output
    ImagePointer outputPtr = this->GetOutput();
    if ( !outputPtr )
    {
      return;
    }

    // Set the size of the output region
    typename TImage::RegionType outputLargestPossibleRegion;
    outputLargestPossibleRegion.SetSize( m_Size );
    outputLargestPossibleRegion.SetIndex( m_StartIndex );
    outputPtr->SetLargestPossibleRegion( outputLargestPossibleRegion );

    // Set spacing and origin
    outputPtr->SetSpacing( m_Spacing );
    outputPtr->SetOrigin( m_Origin );
    outputPtr->SetDirection( m_Direction );


    //TODO update or check the projection information

    return;
  }

  /**
   * Generate Data
   */
  template <class TVectorData, class TImage>
      void
          VectorDataToImageFilter<TVectorData, TImage>
  ::GenerateData(void)
  {
    this->AllocateOutputs();

    this->BeforeThreadedGenerateData();



    this->AfterThreadedGenerateData();
  }

  template <class TVectorData, class TImage>
      void
          VectorDataToImageFilter<TVectorData, TImage>
  ::BeforeThreadedGenerateData(void)
  {
    Superclass::BeforeThreadedGenerateData();

  }


  /**
   * PrintSelf Method
   */
  template <class TVectorData, class TImage>
      void
          VectorDataToImageFilter<TVectorData, TImage>
  ::PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
  }
}

#endif