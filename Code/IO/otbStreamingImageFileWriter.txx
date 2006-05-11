#ifndef _otbStreamingImageFileWriter_txx
#define _otbStreamingImageFileWriter_txx

#include "otbStreamingImageFileWriter.h"
#include "otbImageIOFactory.h"

#include "itkObjectFactoryBase.h"
#include "itkCommand.h"
#include "vnl/vnl_vector.h"
#include "itkVectorImage.h"

namespace otb
{

//---------------------------------------------------------
template <class TInputImage>
StreamingImageFileWriter<TInputImage>
::StreamingImageFileWriter() : itk::ImageFileWriter<TInputImage>(),
                                m_UserSpecifiedIORegion(false),
                                m_FactorySpecifiedImageIO(false)

{
        m_UseStreaming = false;
}


//---------------------------------------------------------
template <class TInputImage>
StreamingImageFileWriter<TInputImage>
::~StreamingImageFileWriter()
{
}


  
//---------------------------------------------------------
template <class TInputImage>
void 
StreamingImageFileWriter<TInputImage>
::Write()
{
  const InputImageType * input = this->GetInput();

std::cout << "Image Writer : "<<std::endl;
std::cout << " Requested : "<<input->GetRequestedRegion()<<std::endl;
std::cout << " Largest Possible : "<<input->GetLargestPossibleRegion()<<std::endl;

  itkDebugMacro( <<"Writing an image file" );

  // Make sure input is available
  if ( input == 0 )
    {
    itkExceptionMacro(<< "No input to writer!");
    }

  // Make sure that we can write the file given the name
  //
  if ( this->GetFileName() == "" )
    {
    itkExceptionMacro(<<"No filename was specified");
    }

//  if ( this->GetImageIO()->IsNull() ) //try creating via factory
  if ( this->GetImageIO() == 0 ) //try creating via factory
    {
    itkDebugMacro(<<"Attempting factory creation of ImageIO for file: " 
                  << this->GetFileName());
    this->SetImageIO( ImageIOFactory::CreateImageIO( this->GetFileName(), 
                                               itk::ImageIOFactory::WriteMode ) );
    m_FactorySpecifiedImageIO = true;
    }
  else
    {
    if( m_FactorySpecifiedImageIO && !this->GetImageIO()->CanWriteFile( this->GetFileName() ) )
      {
      itkDebugMacro(<<"ImageIO exists but doesn't know how to write file:" 
                    << this->GetFileName() );
      itkDebugMacro(<<"Attempting creation of ImageIO with a factory for file:"
                    << this->GetFileName());
      this->SetImageIO( ImageIOFactory::CreateImageIO( this->GetFileName(), 
                                                 itk::ImageIOFactory::WriteMode ) );
      m_FactorySpecifiedImageIO = true;
      }
    }

//  if ( this->GetImageIO()->IsNull() )
  if ( this->GetImageIO() == 0 )
    {
    itk::ImageFileWriterException e(__FILE__, __LINE__);
    itk::OStringStream msg;
    msg << " Could not create IO object for file "
        << this->GetFileName() << std::endl;
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
    throw e;
    return;
    }

  // NOTE: this const_cast<> is due to the lack of const-correctness
  // of the ProcessObject.
  InputImageType * nonConstImage = const_cast<InputImageType *>(input);
std::cout << "Image Writer : nonConstImage "<<std::endl;
std::cout << " Requested : "<<nonConstImage->GetRequestedRegion()<<std::endl;
std::cout << " Largest Possible : "<<nonConstImage->GetLargestPossibleRegion()<<std::endl;


  typedef typename TInputImage::RegionType   RegionType;

//m_UserSpecifiedIORegion = true;

  if ( ! m_UserSpecifiedIORegion )
    {
std::cout << " m_UserSpecifiedIORegion = false "<<std::endl;
    // Make sure the data is up-to-date.
    if( nonConstImage->GetSource() )
      {
      nonConstImage->GetSource()->UpdateLargestPossibleRegion();
      }
    // Write the whole image
    itk::ImageIORegion ioRegion(TInputImage::ImageDimension);
    RegionType region = input->GetLargestPossibleRegion();

    for(unsigned int i=0; i<TInputImage::ImageDimension; i++)
      {
      ioRegion.SetSize(i,region.GetSize(i));
      ioRegion.SetIndex(i,region.GetIndex(i));
      }
    this->SetIORegion( ioRegion ); //used by GenerateData
    }
  else
    {
std::cout << " m_UserSpecifiedIORegion = true "<<std::endl;
    nonConstImage->Update();
    }

  // Setup the ImageIO
  //
  this->GetImageIO()->SetNumberOfDimensions(TInputImage::ImageDimension);
  RegionType region = input->GetLargestPossibleRegion();
  const typename TInputImage::SpacingType& spacing = input->GetSpacing();
  const typename TInputImage::PointType& origin = input->GetOrigin();
  const typename TInputImage::DirectionType& direction = input->GetDirection();

std::cout << " input Requested : "<<input->GetRequestedRegion()<<std::endl;
std::cout << " input Largest Possible : "<<input->GetLargestPossibleRegion()<<std::endl;

  for(unsigned int i=0; i<TInputImage::ImageDimension; i++)
    {
    this->GetImageIO()->SetDimensions(i,region.GetSize(i));
    this->GetImageIO()->SetSpacing(i,spacing[i]);
    this->GetImageIO()->SetOrigin(i,origin[i]);
    vnl_vector< double > axisDirection(TInputImage::ImageDimension);
// Please note: direction cosines are stored as columns of the
// direction matrix
    for(unsigned int j=0; j<TInputImage::ImageDimension; j++)
      {
      axisDirection[j] = direction[j][i];
      }
    this->GetImageIO()->SetDirection( i, axisDirection );
    }

std::cout << " this->GetIORegion() : "<<this->GetIORegion()<<std::endl;

  this->GetImageIO()->SetUseCompression(this->GetUseCompression());
  if( this->GetUseInputMetaDataDictionary() )
    {
    this->GetImageIO()->SetMetaDataDictionary(input->GetMetaDataDictionary());
    }

  //Not use streaming
  if( m_UseStreaming == false )
  {

        // Notify start event observers
        this->InvokeEvent( itk::StartEvent() );

        this->GetImageIO()->SetIORegion(this->GetIORegion());


        // Actually do something
        this->GenerateData();
  
        // Notify end event observers
        this->InvokeEvent( itk::EndEvent() );

        // Release upstream data if requested
        if ( input->ShouldIReleaseData() )
        {
                nonConstImage->ReleaseData();
        }
  }
  else
  {
  
  
  }
}


//---------------------------------------------------------
template <class TInputImage>
void 
StreamingImageFileWriter<TInputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

} // end namespace otb

#endif
