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
#ifndef _otbStreamingImageFileWriter_txx
#define _otbStreamingImageFileWriter_txx
#include "otbStreamingImageFileWriter.h"

#include "itkCommand.h"
#include "itkImageRegionIterator.h"
#include "itkObjectFactoryBase.h"
#include "itkImageFileWriter.h"
#include "otbMacro.h"
#include "otbConfigure.h"

namespace otb
{

/**
 *
 */
template <class TInputImage>
StreamingImageFileWriter<TInputImage>
::StreamingImageFileWriter()
{
  m_BufferMemorySize = 0;
  m_BufferNumberOfLinesDivisions = 0;
  // default to 10 divisions
  m_NumberOfStreamDivisions = 0;
  // default to AUTOMATIC_NUMBER_OF_DIVISIONS
  m_CalculationDivision = SET_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS;
  
  // create default region splitter
  m_RegionSplitter = itk::ImageRegionSplitter<InputImageDimension>::New();

  m_UserSpecifiedIORegion = true;
  m_FactorySpecifiedImageIO = false;

}

/**
 *
 */
template <class TInputImage>
StreamingImageFileWriter<TInputImage>
::~StreamingImageFileWriter()
{
}

/**
 *
 */
template <class TInputImage>
void 
StreamingImageFileWriter<TInputImage>
::SetBufferMemorySize(unsigned long memory_size_divisions)
{
	m_BufferMemorySize = memory_size_divisions;
	m_CalculationDivision = SET_BUFFER_MEMORY_SIZE;
}

/**
 *
 */
template <class TInputImage>
void 
StreamingImageFileWriter<TInputImage>
::SetBufferNumberOfLinesDivisions(unsigned long nb_lines_divisions)
{
	m_BufferNumberOfLinesDivisions = nb_lines_divisions;
	m_CalculationDivision = SET_BUFFER_NUMBER_OF_LINES;
}

/**
 *
 */
template <class TInputImage>
void 
StreamingImageFileWriter<TInputImage>
::SetNumberOfStreamDivisions(unsigned long nb_divisions)
{
	m_NumberOfStreamDivisions = nb_divisions;
	m_CalculationDivision = SET_NUMBER_OF_STREAM_DIVISIONS;
}

/**
 *
 */
template <class TInputImage>
void 
StreamingImageFileWriter<TInputImage>
::SetAutomaticNumberOfStreamDivisions(void)
{
	m_CalculationDivision = SET_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS;
}

/**
 *
 */
template <class TInputImage>
unsigned long 
StreamingImageFileWriter<TInputImage>
::GetNumberOfStreamDivisions(void)
{
  		return(CalculateNumberOfStreamDivisions());
}

/**
 *
 */
template <class TInputImage>
std::string 
StreamingImageFileWriter<TInputImage>
::GetMethodUseToCalculateNumberOfStreamDivisions(void)
{
  switch(m_CalculationDivision)
    {
    case SET_NUMBER_OF_STREAM_DIVISIONS:
      return "SET_NUMBER_OF_STREAM_DIVISIONS";
      break;
    case SET_BUFFER_MEMORY_SIZE:
      return "SET_BUFFER_MEMORY_SIZE";
      break;
    case SET_BUFFER_NUMBER_OF_LINES:
      return "SET_BUFFER_NUMBER_OF_LINES";
      break;
    case SET_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS:
      return "SET_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS";
      break;
    default:
      return "unknown";
      break;
    }
}

/**
 *
 */
template <class TInputImage>
void 
StreamingImageFileWriter<TInputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "File Name: " 
     << (m_FileName.data() ? m_FileName.data() : "(none)") << std::endl;

  os << indent << "Image IO: ";
  if ( m_ImageIO.IsNull() )
    {
    os << "(none)\n";
    }
  else
    {
    os << m_ImageIO << "\n";
    }

  os << indent << "IO Region: " << m_IORegion << "\n";


  if (m_UseCompression)
    {
    os << indent << "Compression: On\n";
    }
  else
    {
    os << indent << "Compression: Off\n";
    }

  if (m_UseInputMetaDataDictionary)
    {
    os << indent << "UseInputMetaDataDictionary: On\n";
    }
  else
    {
    os << indent << "UseInputMetaDataDictionary: Off\n";
    }

  if (m_FactorySpecifiedImageIO)
    {
    os << indent << "FactorySpecifiedmageIO: On\n";
    }
  else
    {
    os << indent << "FactorySpecifiedmageIO: Off\n";
    }


  os << indent << "Number of stream divisions: " << m_NumberOfStreamDivisions
     << std::endl;
  if (m_RegionSplitter)
    {
    os << indent << "Region splitter:" << m_RegionSplitter << std::endl;
    }
  else
    {
    os << indent << "Region splitter: (none)" << std::endl;
    }
}

//---------------------------------------------------------
template<class TInputImage>
void 
StreamingImageFileWriter<TInputImage>
::SetIORegion (const itk::ImageIORegion& region) 
{
  itkDebugMacro("setting IORegion to " << region );
  if ( m_IORegion != region)
    {
    m_IORegion = region;
    this->Modified();
    m_UserSpecifiedIORegion = true;
    }
} 

/**
 *
 */
template<class TInputImage>
unsigned long 
StreamingImageFileWriter<TInputImage>
::CalculateNumberOfStreamDivisions(void)
{
	unsigned long numDivisions(0);
  	OutputImagePointer outputPtr = this->GetOutput(0);
  	OutputImageRegionType outputRegion = outputPtr->GetRequestedRegion();
	
	switch(m_CalculationDivision)
	{
		case SET_NUMBER_OF_STREAM_DIVISIONS :
		{
  			numDivisions = m_NumberOfStreamDivisions;
		}
		break;
		case SET_BUFFER_MEMORY_SIZE :
		{
      		const unsigned long bufferMemorySize = m_BufferMemorySize/8;
			unsigned long numberColumnsOfRegion = outputRegion.GetSize()[0]; // X dimension
      		const unsigned long sizeLine = numberColumnsOfRegion * \
      											outputPtr->GetNumberOfComponentsPerPixel() * \
      											sizeof(OutputImagePixelType);
      		unsigned long regionSize = outputRegion.GetSize()[1] * sizeLine;
      		otbMsgDebugMacro(<<"outputPtr->GetNumberOfComponentsPerPixel()   = "<<outputPtr->GetNumberOfComponentsPerPixel());
      		otbMsgDebugMacro(<<"sizeof(OutputImagePixelType)                 = "<<sizeof(OutputImagePixelType));
      		otbMsgDebugMacro(<<"numberColumnsOfRegion                        = "<<numberColumnsOfRegion);
      		otbMsgDebugMacro(<<"sizeLine                                     = "<<sizeLine);
      		otbMsgDebugMacro(<<"regionSize                                   = "<<regionSize);
      		otbMsgDebugMacro(<<"m_BufferMemorySize                           = "<<m_BufferMemorySize);
      		otbMsgDebugMacro(<<"bufferMemorySize                             = "<<bufferMemorySize);

      		//Active streaming 
	      	if( regionSize > bufferMemorySize )
      		{
      			//The regionSize must be at list equal to the sizeLine 
      			if( regionSize < sizeLine )
      			{
       				otbMsgDebugMacro(<<"Force buffer size.");
      				regionSize = sizeLine;
      			}
      			//Calculate NumberOfStreamDivisions
				numDivisions = static_cast<unsigned long>(vcl_ceil(static_cast<double>(regionSize)/static_cast<double>(bufferMemorySize)));
      		}
      		else
      		{
      			//Non streaming
      			numDivisions = 1;
      		}
		}
		break;
		case SET_BUFFER_NUMBER_OF_LINES :
		{
			if( m_BufferNumberOfLinesDivisions < 1 )
			{
				itkExceptionMacro(<<"Buffer number of lines division must be greater than 0 !");
			}
			/* Calculate number of split */
			unsigned long numberLinesOfRegion = outputRegion.GetSize()[1]; // Y dimension
			if( numberLinesOfRegion > m_BufferNumberOfLinesDivisions )
			{
				numDivisions = static_cast<unsigned long>(vcl_ceil(static_cast<double>(numberLinesOfRegion)/static_cast<double>(m_BufferNumberOfLinesDivisions)));
			}
			else
			{
      			//Non streaming
				numDivisions = 1;
			}
		}
		break;
		case SET_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS :
		{
      		const unsigned long streamMaxSizeBufferForStreamingBytes = OTB_STREAM_MAX_SIZE_BUFFER_FOR_STREAMING;
      	    const unsigned long streamImageSizeToActivateStreamingBytes = OTB_STREAM_IMAGE_SIZE_TO_ACTIVATE_STREAMING;
      		//Convert in octet unit
      		unsigned long streamMaxSizeBufferForStreaming = streamMaxSizeBufferForStreamingBytes/8;
      	    const unsigned long streamImageSizeToActivateStreaming = streamImageSizeToActivateStreamingBytes/8;
      	    
			unsigned long numberColumnsOfRegion = outputRegion.GetSize()[0]; // X dimension
      		const unsigned long sizeLine = numberColumnsOfRegion * \
      											outputPtr->GetNumberOfComponentsPerPixel() * \
      											sizeof(OutputImagePixelType);
      		const unsigned long regionSize = outputRegion.GetSize()[1] * sizeLine;
      		otbMsgDebugMacro(<<"streamImageSizeToActivateStreaming in Bytes  = "<<streamImageSizeToActivateStreamingBytes);
      		otbMsgDebugMacro(<<"streamMaxSizeBufferForStreaming in Bytes     = "<<streamMaxSizeBufferForStreamingBytes);
      		otbMsgDebugMacro(<<"streamImageSizeToActivateStreaming           = "<<streamImageSizeToActivateStreaming);
      		otbMsgDebugMacro(<<"streamMaxSizeBufferForStreaming              = "<<streamMaxSizeBufferForStreaming);
      		otbMsgDebugMacro(<<"outputPtr->GetNumberOfComponentsPerPixel()   = "<<outputPtr->GetNumberOfComponentsPerPixel());
      		otbMsgDebugMacro(<<"sizeof(OutputImagePixelType)                 = "<<sizeof(OutputImagePixelType));
      		otbMsgDebugMacro(<<"numberColumnsOfRegion                        = "<<numberColumnsOfRegion);
      		otbMsgDebugMacro(<<"sizeLine                                     = "<<sizeLine);
      		otbMsgDebugMacro(<<"regionSize                                   = "<<regionSize);

      		//Active streaming 
	      	if( regionSize > streamImageSizeToActivateStreaming )
      		{
      			//On s'assure que la taille du bandeau fait au moins une ligne de l'image
      			if( streamMaxSizeBufferForStreaming < sizeLine )
      			{
       				otbMsgDebugMacro(<<"Force buffer size.");
      				streamMaxSizeBufferForStreaming = sizeLine;
      			}
   				otbMsgDebugMacro(<<"Buffer size : "<<streamMaxSizeBufferForStreaming);
      			//Calculate NumberOfStreamDivisions
				numDivisions = static_cast<unsigned long>(vcl_ceil(static_cast<double>(regionSize)/static_cast<double>(streamMaxSizeBufferForStreaming)));
      		}
      		else
      		{
      			//Non streaming
      			numDivisions = 1;
      		}
		}
		break;
		default :
			itkExceptionMacro(<<"Method use to calculate number of stream divisions is not set !");
		break;
	}
    if( numDivisions == 0) numDivisions = 1;
	otbMsgDebugMacro(<<" -> Resume : method : "<<GetMethodUseToCalculateNumberOfStreamDivisions()<<"\n -> Number of divisions = "<<numDivisions);
	return(numDivisions);
}


/**
 *
 */
template<class TInputImage>
void 
StreamingImageFileWriter<TInputImage>
::UpdateOutputData(itk::DataObject *itkNotUsed(output))
{
  unsigned int idx;

  /**
   * prevent chasing our tail
   */
  if (this->m_Updating)
    {
    return;
    }


  /**
   * Prepare all the outputs. This may deallocate previous bulk data.
   */
  this->PrepareOutputs();

  /**
   * Make sure we have the necessary inputs
   */
  unsigned int ninputs = this->GetNumberOfValidRequiredInputs();
  if (ninputs < this->GetNumberOfRequiredInputs())
    {
    itkExceptionMacro(<< "At least " << static_cast<unsigned int>( this->GetNumberOfRequiredInputs() ) << " inputs are required but only " << ninputs << " are specified.");
    return;
    }
  this->SetAbortGenerateData(0);
  this->SetProgress(0.0);
  this->m_Updating = true;
    

  /**
   * Tell all Observers that the filter is starting
   */
  this->InvokeEvent( itk::StartEvent() );

  /**
   * Allocate the output buffer. 
   */
  OutputImagePointer outputPtr = this->GetOutput(0);
  OutputImageRegionType outputRegion = outputPtr->GetRequestedRegion();

  /** Prepare ImageIO  : create ImageFactory */
  
  // Make sure that we can write the file given the name
  //
  if ( m_FileName == "" )
    {
    itkExceptionMacro(<<"No filename was specified");
    }

  if ( m_ImageIO.IsNull() ) //try creating via factory
    {
    itkDebugMacro(<<"Attempting factory creation of ImageIO for file: " 
                  << m_FileName);
    this->SetImageIO( ImageIOFactory::CreateImageIO( m_FileName.c_str(), 
                                               itk::ImageIOFactory::WriteMode ) );

/*    m_ImageIO = ImageIOFactory::CreateImageIO( m_FileName.c_str(), 
                                               itk::ImageIOFactory::WriteMode );*/
    m_FactorySpecifiedImageIO = true;
    }
  else
    {
    if( m_FactorySpecifiedImageIO && !m_ImageIO->CanWriteFile( m_FileName.c_str() ) )
      {
      itkDebugMacro(<<"ImageIO exists but doesn't know how to write file:" 
                    << m_FileName );
      itkDebugMacro(<<"Attempting creation of ImageIO with a factory for file:"
                    << m_FileName);
      m_ImageIO = ImageIOFactory::CreateImageIO( m_FileName.c_str(), 
                                                 itk::ImageIOFactory::WriteMode );
      m_FactorySpecifiedImageIO = true;
      }
    }

  if ( m_ImageIO.IsNull() )
    {
    itk::ImageFileWriterException e(__FILE__, __LINE__);
    itk::OStringStream msg;
    msg << " Could not create IO object for file "
        << m_FileName.c_str() << std::endl;
    msg << "  Tried to create one of the following:" << std::endl;
    std::list<itk::LightObject::Pointer> allobjects = 
      itk::ObjectFactoryBase::CreateAllInstance("itkImageIOBase");
    for(std::list<itk::LightObject::Pointer>::iterator i = allobjects.begin();
        i != allobjects.end(); ++i)
      {
      itk::ImageIOBase* io = dynamic_cast<itk::ImageIOBase*>(i->GetPointer());
      msg << "    " << io->GetNameOfClass() << std::endl; 
      }
    msg << "  You probably failed to set a file suffix, or" << std::endl;
    msg << "    set the suffix to an unsupported type." << std::endl;
    e.SetDescription(msg.str().c_str());
    e.SetLocation(ITK_LOCATION);
    throw e;
    }

  /** End of Prepare ImageIO  : create ImageFactory */


  /** Control if the ImageIO is CanStreamWrite */
    if( m_ImageIO->CanStreamWrite() == false )
    {
        itk::OStringStream msg;
        msg << "The ImageFactory selected for the this image file <"<<m_FileName.c_str()<<"> is not StreamWrite "<< std::endl;
        msg << m_ImageIO<< std::endl;
        itkExceptionMacro(<<msg.str().c_str());
    }


  /**
   * Grab the input
   */
  InputImagePointer inputPtr = 
    const_cast< InputImageType * >( this->GetInput(0) );

  /**
   * Determine of number of pieces to divide the input.  This will be the
   * minimum of what the user specified via SetNumberOfStreamDivisions()
   * and what the Splitter thinks is a reasonable value.
   */
  unsigned int numDivisions, numDivisionsFromSplitter;

  numDivisions = static_cast<unsigned int>(CalculateNumberOfStreamDivisions());
  numDivisionsFromSplitter = m_RegionSplitter->GetNumberOfSplits(outputRegion, numDivisions);
  if (numDivisionsFromSplitter < numDivisions)
    {
    numDivisions = numDivisionsFromSplitter;
    }
  
  /**
   * Loop over the number of pieces, execute the upstream pipeline on each
   * piece, and copy the results into the output image.
   */
  InputImageRegionType streamRegion;
  streamRegion = m_RegionSplitter->GetSplit(0, numDivisions,
                                              outputRegion);

  // Allocation du bandeau
//  outputPtr->SetBufferedRegion( streamRegion );
//  outputPtr->Allocate();



  // On s'appuie sur 'outputPtr' pour d�terminer les initialiser le 'm_ImageIO'
  // Setup the ImageIO
  //
  m_ImageIO->SetNumberOfDimensions(TInputImage::ImageDimension);
  const typename TInputImage::SpacingType& spacing = outputPtr->GetSpacing();
  const typename TInputImage::PointType& origin = outputPtr->GetOrigin();
  const typename TInputImage::DirectionType& direction = outputPtr->GetDirection();

  for(unsigned int i=0; i<TInputImage::ImageDimension; i++)
    {
// Final image size
    m_ImageIO->SetDimensions(i,outputRegion.GetSize(i));
    m_ImageIO->SetSpacing(i,spacing[i]);
    m_ImageIO->SetOrigin(i,origin[i]);
    vnl_vector< double > axisDirection(TInputImage::ImageDimension);
// Please note: direction cosines are stored as columns of the
// direction matrix
    for(unsigned int j=0; j<TInputImage::ImageDimension; j++)
      {
      axisDirection[j] = direction[j][i];
      }
    m_ImageIO->SetDirection( i, axisDirection );
    }

  m_ImageIO->SetUseCompression(m_UseCompression);
  m_ImageIO->SetMetaDataDictionary(inputPtr->GetMetaDataDictionary());


   /** Create Image file */
   // Setup the image IO for writing.
   //
   m_ImageIO->SetFileName(m_FileName.c_str());
   m_ImageIO->WriteImageInformation();
  

  /**
   * Loop over the number of pieces, execute the upstream pipeline on each
   * piece, and copy the results into the output image.
   */
  unsigned int piece;
  for (piece = 0;
       piece < numDivisions && !this->GetAbortGenerateData();
       piece++)
    {
                streamRegion = m_RegionSplitter->GetSplit(piece, numDivisions,
                                              outputRegion);
      
                inputPtr->SetRequestedRegion(streamRegion);
                inputPtr->PropagateRequestedRegion();
                inputPtr->UpdateOutputData();

                // Write the whole image
                itk::ImageIORegion ioRegion(TInputImage::ImageDimension);
                for(unsigned int i=0; i<TInputImage::ImageDimension; i++)
                {
                        ioRegion.SetSize(i,streamRegion.GetSize(i));
                        ioRegion.SetIndex(i,streamRegion.GetIndex(i));
                }
                this->SetIORegion( ioRegion );
                m_ImageIO->SetIORegion(m_IORegion);


                // Start writing streamregion in the image file
                this->GenerateData();

                this->UpdateProgress((float) piece / numDivisions );
    }

  /**
   * If we ended due to aborting, push the progress up to 1.0 (since
   * it probably didn't end there)
   */
  if ( !this->GetAbortGenerateData() )
    {
    this->UpdateProgress(1.0);
    }

  // Notify end event observers
  this->InvokeEvent( itk::EndEvent() );

  /**
   * Now we have to mark the data as up to data.
   */
  for (idx = 0; idx < this->GetNumberOfOutputs(); ++idx)
    {
    if (this->GetOutput(idx))
      {
      this->GetOutput(idx)->DataHasBeenGenerated();
      }
    }
  
  /**
   * Release any inputs if marked for release
   */
  this->ReleaseInputs();
  
  // Mark that we are no longer updating the data in this filter
  this->m_Updating = false;
}


//---------------------------------------------------------

/**
 *
 */
template<class TInputImage>
void 
StreamingImageFileWriter<TInputImage>
::GenerateData(void)
{
  const InputImageType * input = this->GetInput();

  // Make sure that the image is the right type and no more than 
  // four components.
  typedef typename InputImageType::PixelType ScalarType;

  if( strcmp( input->GetNameOfClass(), "VectorImage" ) == 0 ) 
    {
    typedef typename InputImageType::InternalPixelType VectorImageScalarType;
    m_ImageIO->SetPixelTypeInfo( typeid(VectorImageScalarType) );
    
    typedef typename InputImageType::AccessorFunctorType AccessorFunctorType;
    m_ImageIO->SetNumberOfComponents( AccessorFunctorType::GetVectorLength(input) );
    }
  else
    {
    // Set the pixel and component type; the number of components.
    m_ImageIO->SetPixelTypeInfo(typeid(ScalarType));  
    }


otbMsgDevMacro( <<" InputImage GetImageDimensions : "<<input->GetImageDimension());
otbMsgDevMacro( <<"     - GetRequestedRegion : "<<input->GetRequestedRegion());
otbMsgDevMacro( <<"     - GetLargestPossibleRegion : "<<input->GetLargestPossibleRegion());
otbMsgDevMacro( <<"     - GetBufferedRegion : "<<input->GetBufferedRegion());

  // Setup the image IO for writing.
  //
  //okay, now extract the data as a raw buffer pointer
  const void* dataPtr = (const void*) input->GetBufferPointer();
  m_ImageIO->Write(dataPtr);

}



} // end namespace otb

#endif
