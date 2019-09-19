/*
 * otbMosaicFromDirectoryHandler.hxx
 *
 *  Created on: 24 mars 2016
 *      Author: cresson
 */

#ifndef MODULES_REMOTE_OTB_MosaicFromDirectoryHandler_INCLUDE_OTBMosaicFromDirectoryHandler_HXX_
#define MODULES_REMOTE_OTB_MosaicFromDirectoryHandler_INCLUDE_OTBMosaicFromDirectoryHandler_HXX_

#include "otbMosaicFromDirectoryHandler.h"
#include "otbImageFileWriter.h"

namespace otb
{

template <class TOutputImage, class TReferenceImage>
MosaicFromDirectoryHandler<TOutputImage, TReferenceImage>
::MosaicFromDirectoryHandler()
 {
  mosaicFilter = MosaicFilterType::New();
  castFilter = CastFilterType::New();
  m_UseReferenceImage = false;
  m_RefImagePtr = 0;
 }

template <class TOutputImage, class TReferenceImage>
MosaicFromDirectoryHandler<TOutputImage, TReferenceImage>
::~MosaicFromDirectoryHandler()
{
}

template <class TOutputImage, class TReferenceImage>
void
MosaicFromDirectoryHandler<TOutputImage, TReferenceImage>
::GenerateOutputInformation()
 {
  if (m_Directory[m_Directory.size()-1] != '/')
    {
    // If not, we add the separator
    m_Directory.append("/");
    }

  // Get the list of files in the directory
  itk::Directory::Pointer dir = itk::Directory::New();
  if (!dir->Load(m_Directory.c_str()))
    {
    itkExceptionMacro(<< "Unable to browse directory " << m_Directory);
    }

  // Instanciate a new mosaic filter
  mosaicFilter = MosaicFilterType::New();
  mosaicFilter->SetGlobalWarningDisplay(false);
  readers.clear();
  resamplers.clear();

  // Browse the directory
  for (unsigned int i = 0; i < dir->GetNumberOfFiles(); i++)
    {
    const char *filename = dir->GetFile(i);
    std::string sfilename(filename);
    sfilename = m_Directory + sfilename;

    // Try to read the file
    otb::ImageIOBase::Pointer imageIO =
        otb::ImageIOFactory::CreateImageIO(sfilename.c_str(),otb::ImageIOFactory::ReadMode);
    if( imageIO.IsNotNull() )
      {
        // create reader
        ReaderPointerType reader = ReaderType::New();
        reader->SetFileName(sfilename);
        reader->UpdateOutputInformation();

        readers.push_back(reader);

        if (m_UseReferenceImage)
          {
            ResamplerPointerType resampler = ResamplerType::New();
            resampler->SetInput(reader->GetOutput());

            // Setup transform through projRef and Keywordlist
            SpacingType defSpacing = m_RefImagePtr->GetSignedSpacing();
            defSpacing[0] *= 10;
            defSpacing[1] *= 10;
            resampler->SetDisplacementFieldSpacing(defSpacing);
            resampler->SetInputKeywordList(reader->GetOutput()->GetImageKeywordlist());
            resampler->SetInputProjectionRef(reader->GetOutput()->GetProjectionRef());
            resampler->SetOutputKeywordList(m_RefImagePtr->GetImageKeywordlist());
            resampler->SetOutputProjectionRef(m_RefImagePtr->GetProjectionRef());
            resampler->SetOutputOrigin(m_RefImagePtr->GetOrigin());
            resampler->SetOutputSpacing(m_RefImagePtr->GetSignedSpacing());
            resampler->SetOutputSize(m_RefImagePtr->GetLargestPossibleRegion().GetSize());
            resampler->SetOutputStartIndex(m_RefImagePtr->GetLargestPossibleRegion().GetIndex());

            typename NNInterpolatorType::Pointer interpolator = NNInterpolatorType::New();
            resampler->SetInterpolator(interpolator);

            resamplers.push_back(resampler);

            mosaicFilter->PushBackInput(resampler->GetOutput());
          }
        else
          {
            mosaicFilter->PushBackInput(reader->GetOutput());
          }
      }
    else
      {
      //      itkWarningMacro(<<"Unable to read file " << sfilename);
      }

    }

  if (m_UseReferenceImage)
    {
      mosaicFilter->SetOutputOrigin(m_RefImagePtr->GetOrigin());
      mosaicFilter->SetOutputSpacing(m_RefImagePtr->GetSignedSpacing());
      mosaicFilter->SetOutputSize(m_RefImagePtr->GetLargestPossibleRegion().GetSize());
    }
  else
    {
      mosaicFilter->SetOutputOrigin(m_OutputOrigin);
      mosaicFilter->SetOutputSpacing(m_OutputSpacing);
      mosaicFilter->SetOutputSize(m_OutputSize);
    }
  mosaicFilter->SetAutomaticOutputParametersComputation(false);

  castFilter->SetInput(mosaicFilter->GetOutput());

  castFilter->GraftOutput( this->GetOutput() );
  castFilter->UpdateOutputInformation();
  this->GraftOutput( castFilter->GetOutput() );
 }

template <class TOutputImage, class TReferenceImage>
void
MosaicFromDirectoryHandler<TOutputImage, TReferenceImage>
::GenerateData()
 {
  castFilter->GraftOutput( this->GetOutput() );
  castFilter->Update();
  this->GraftOutput( castFilter->GetOutput() );
 }

} // end namespace otb

#endif /* MODULES_REMOTE_OTB_MosaicFromDirectoryHandler_INCLUDE_OTBMosaicFromDirectoryHandler_HXX_ */
