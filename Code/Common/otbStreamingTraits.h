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
#ifndef _otbStreamingTraits_h
#define _otbStreamingTraits_h

#include "otbMacro.h"
#include "otbConfigure.h"

namespace otb
{

/**
 * this enum defines the different streaming mode available in OTB.
   */
  typedef enum
    { 
      SET_NUMBER_OF_STREAM_DIVISIONS = 1,
      SET_BUFFER_MEMORY_SIZE = 2,
      SET_BUFFER_NUMBER_OF_LINES = 3,
      SET_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS = 4
    } StreamingMode;
  
/** \class StreamingTraits
 *  \brief This class is a helper class for terminal streaming filter implementation.
 * \sa StreamingImageFileWriter
 * \sa StreamingStatisticsImageFilter
 */  
template <class TImage>
class ITK_EXPORT StreamingTraits
{
 public:
  typedef TImage ImageType;
  typedef typename ImageType::Pointer ImagePointerType;
  typedef typename ImageType::RegionType RegionType;
  typedef typename ImageType::InternalPixelType PixelType;
  
  typedef StreamingMode StreamingModeType;

  /**
   * This method computes the number of streaming divisions, based on
   * the streaming mode and the image attributes. The last three parameters are ignored if
   * the mode does not need them.
   * \param image The image to stream,
   * \param region the region to stream,
   * \param mode  the streaming mode,
   * \param numberOfStreamDivision the number of stream division if streaming mode is SET_NUMBER_OF_STREAM_DIVISIONS,
   * \param bufferMemorySize the buffer memory size in bytes if streaming mode is SET_BUFFER_MEMORY_SIZE,
   * \param bufferNumberOfLinesDivision the buffer number of lines division if the streaming mode is SET_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS.
   * \return The number of streaming divisions.
   */
  static unsigned long CalculateNumberOfStreamDivisions(const TImage * image,
							RegionType region,
							StreamingModeType mode, 
							unsigned long numberOfStreamDivision,
							unsigned long bufferMemorySize,
							unsigned long bufferNumberOfLinesDivisions)
    {      
      unsigned long numDivisions(0);
      
      switch(mode)
	{
	case SET_NUMBER_OF_STREAM_DIVISIONS :
	  {
	    numDivisions = numberOfStreamDivision;
	  }
		break;
		case SET_BUFFER_MEMORY_SIZE :
		{
      		const unsigned long bufferMemorySize = bufferMemorySize/8;
			unsigned long numberColumnsOfRegion = region.GetSize()[0]; // X dimension
      		const unsigned long sizeLine = numberColumnsOfRegion * \
		  image->GetNumberOfComponentsPerPixel() * \
		  sizeof(PixelType);
      		unsigned long regionSize = region.GetSize()[1] * sizeLine;
      		otbMsgDevMacro(<<"image->GetNumberOfComponentsPerPixel()   = "<<image->GetNumberOfComponentsPerPixel());
      		otbMsgDevMacro(<<"sizeof(PixelType)                 = "<<sizeof(PixelType));
      		otbMsgDevMacro(<<"numberColumnsOfRegion                        = "<<numberColumnsOfRegion);
      		otbMsgDevMacro(<<"sizeLine                                     = "<<sizeLine);
      		otbMsgDevMacro(<<"regionSize                                   = "<<regionSize);
      		otbMsgDevMacro(<<"m_BufferMemorySize                           = "<<bufferMemorySize);
      		otbMsgDevMacro(<<"bufferMemorySize                             = "<<bufferMemorySize);

      		//Active streaming 
	      	if( regionSize > bufferMemorySize )
      		{
      			//The regionSize must be at list equal to the sizeLine 
      			if( regionSize < sizeLine )
      			{
       				otbMsgDevMacro(<<"Force buffer size.");
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
			if( bufferNumberOfLinesDivisions < 1 )
			{
				itkGenericExceptionMacro(<<"Buffer number of lines division must be greater than 0 !");
			}
			/* Calculate number of split */
			unsigned long numberLinesOfRegion = region.GetSize()[1]; // Y dimension
			if( numberLinesOfRegion > bufferNumberOfLinesDivisions )
			{
				numDivisions = static_cast<unsigned long>(vcl_ceil(static_cast<double>(numberLinesOfRegion)/static_cast<double>(bufferNumberOfLinesDivisions)));
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
      	    
			unsigned long numberColumnsOfRegion = region.GetSize()[0]; // X dimension
      		const unsigned long sizeLine = numberColumnsOfRegion * \
      											image->GetNumberOfComponentsPerPixel() * \
      											sizeof(PixelType);
      		const unsigned long regionSize = region.GetSize()[1] * sizeLine;
      		otbMsgDevMacro(<<"streamImageSizeToActivateStreaming in Bytes  = "<<streamImageSizeToActivateStreamingBytes);
      		otbMsgDevMacro(<<"streamMaxSizeBufferForStreaming in Bytes     = "<<streamMaxSizeBufferForStreamingBytes);
      		otbMsgDevMacro(<<"streamImageSizeToActivateStreaming           = "<<streamImageSizeToActivateStreaming);
      		otbMsgDevMacro(<<"streamMaxSizeBufferForStreaming              = "<<streamMaxSizeBufferForStreaming);
      		otbMsgDevMacro(<<"image->GetNumberOfComponentsPerPixel()   = "<<image->GetNumberOfComponentsPerPixel());
      		otbMsgDevMacro(<<"sizeof(PixelType)                 = "<<sizeof(PixelType));
      		otbMsgDevMacro(<<"numberColumnsOfRegion                        = "<<numberColumnsOfRegion);
      		otbMsgDevMacro(<<"sizeLine                                     = "<<sizeLine);
      		otbMsgDevMacro(<<"regionSize                                   = "<<regionSize);

      		//Active streaming 
	      	if( regionSize > streamImageSizeToActivateStreaming )
      		{
      			//On s'assure que la taille du bandeau fait au moins une ligne de l'image
      			if( streamMaxSizeBufferForStreaming < sizeLine )
      			{
       				otbMsgDevMacro(<<"Force buffer size.");
      				streamMaxSizeBufferForStreaming = sizeLine;
      			}
   				otbMsgDevMacro(<<"Buffer size : "<<streamMaxSizeBufferForStreaming);
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
			itkGenericExceptionMacro(<<"Method use to calculate number of stream divisions is not set !");
		break;
	}
    if( numDivisions == 0) numDivisions = 1;
	otbMsgDevMacro(<<" -> Resume : method : "<<mode<<"\n -> Number of divisions = "<<numDivisions);
	return(numDivisions);



    }
};
}// End namespace otb

#endif
