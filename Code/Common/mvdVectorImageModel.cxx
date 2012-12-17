/*=========================================================================

  Program:   Monteverdi2
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi2 is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)
#include "itkImageRegionConstIteratorWithIndex.h"
//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdVectorImageModel.h"

namespace mvd
{
/*
  TRANSLATOR mvd::VectorImageModel

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/

/*******************************************************************************/
VectorImageModel
::VectorImageModel( QObject* parent ) :
  AbstractImageModel( parent ),
  m_RasterizedBuffer( NULL )
{
}

/*******************************************************************************/
VectorImageModel
::~VectorImageModel()
{
  this->ClearBuffer();
}

/*******************************************************************************/
void
VectorImageModel
::loadFile( const QString& filename )
{
  DefaultImageFileReaderType::Pointer imageFileReader(
    DefaultImageFileReaderType::New()
  );

  imageFileReader->SetFileName( filename.toLatin1().data() );
  imageFileReader->UpdateOutputInformation();

  m_ImageFileReader = imageFileReader;

  // initialize the channel list for the rendering needs following the
  // input image
  // TODO : See if if needs to be moved somewhere else
  // TODO : use the default display
  if (m_ImageFileReader->GetOutput()->GetNumberOfComponentsPerPixel()  < 3)
    {
    m_Channels.resize(1);
    m_Channels[0]  = 0;
    }
  else
    {
    m_Channels.resize(3);
    m_Channels[0]  = 0;
    m_Channels[1]  = 1;
    m_Channels[2]  = 2;
    }
}

void
VectorImageModel
::ClearBuffer()
{
  // Delete previous buffer if needed
  if (m_RasterizedBuffer != NULL)
    {
    delete[] m_RasterizedBuffer;
    m_RasterizedBuffer = NULL;
    }
}

unsigned char *
VectorImageModel
::RasterizeRegion( const ImageRegionType& region)
{
  // Before doing anything, check if region is inside the buffered
  // region of image
  unsigned int currentIndex = 0;

  // TODO : add some checking
  const DefaultImageType* image =  this->GetOutput(currentIndex);

  // some checking
  if (!image->GetBufferedRegion().IsInside(region))
    {
    //itkExceptionMacro(<< "Region to read is oustside of the buffered region.");
    }

  // Extract the region of interest in the image
  m_ExtractFilter = ExtractFilterType::New();
  m_ExtractFilter->SetInput(image);
  m_ExtractFilter->SetExtractionRegion(region);
  
  // Use the rendering filter to get 
  m_RenderingFilter = RenderingFilterType::New();
  m_RenderingFilter->SetInput(m_ExtractFilter->GetOutput());
  m_RenderingFilter->GetRenderingFunction()->SetAutoMinMax(false);
  m_RenderingFilter->GetRenderingFunction()->SetChannelList(m_Channels);
  m_RenderingFilter->GetOutput()->SetRequestedRegion(region);
  m_RenderingFilter->Update();

  // Delete previous buffer if needed
  this->ClearBuffer();

  // Allocate new memory
  m_RasterizedBuffer = new unsigned char[3 * region.GetNumberOfPixels()];

  // Declare the iterator
  itk::ImageRegionConstIteratorWithIndex< RenderingFilterType::OutputImageType >
    it(m_RenderingFilter->GetOutput(), region);

  // Go to begin
  it.GoToBegin();

  while (!it.IsAtEnd())
    {
    // Fill the buffer
    unsigned int index = 0;
    index = ComputeBufferIndex(it.GetIndex(), region);

    // Fill the buffer
    m_RasterizedBuffer[index]  = it.Get()[0];
    m_RasterizedBuffer[index + 1] = it.Get()[1];
    m_RasterizedBuffer[index + 2] = it.Get()[2];
    ++it;
    }
  
  // if ok return the  buffer
  return m_RasterizedBuffer;
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

/*******************************************************************************/

} // end namespace 'mvd'
