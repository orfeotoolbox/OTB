#ifndef _otbImageFileReader_txx
#define _otbImageFileReader_txx
#include "otbImageFileReader.h"

#include "itkObjectFactory.h"
#include "itkConvertPixelBuffer.h"
#include "itkImageRegion.h"
#include "itkPixelTraits.h"
#include "itkVectorImage.h"

#include "otbMacro.h"
#include "otbImageIOFactory.h"

#include <itksys/SystemTools.hxx>
#include <fstream>

namespace otb
{

template <class TOutputImage>
ImageFileReader<TOutputImage>
::ImageFileReader() : itk::ImageFileReader<TOutputImage>()
{
}


template <class TOutputImage>
ImageFileReader<TOutputImage>
::~ImageFileReader()
{
}

template <class TOutputImage>
void ImageFileReader<TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  if (this->m_ImageIO)
    {
    os << indent << "ImageIO: \n";
    this->m_ImageIO->Print(os, indent.GetNextIndent());
    }
  else
    {
    os << indent << "ImageIO: (null)" << "\n";
    }

  os << indent << "UserSpecifiedImageIO flag: " << this->m_UserSpecifiedImageIO << "\n";
  os << indent << "m_FileName: " << this->m_FileName << "\n";
}

template <class TOutputImage>
void
ImageFileReader<TOutputImage>
::GenerateData()
{

  typename TOutputImage::Pointer output = this->GetOutput();

  // allocate the output buffer
  output->SetBufferedRegion( output->GetRequestedRegion() );
  output->Allocate();

//otbMsgDebugMacro( <<"ImageFileReader<TOutputImage>::GenerateData : ");
//otbMsgDebugMacro( <<" output->GetRequestedRegion() : "<<output->GetRequestedRegion());

  // Test if the file exist and if it can be open.
  // and exception will be thrown otherwise.
  this->TestFileExistanceAndReadability();

  // Tell the ImageIO to read the file
  //
  OutputImagePixelType *buffer = 
    output->GetPixelContainer()->GetBufferPointer();
  this->m_ImageIO->SetFileName(this->m_FileName.c_str());

  itk::ImageIORegion ioRegion(TOutputImage::ImageDimension);
//otbMsgDebugMacro( <<" Avant ioRegion : "<<ioRegion);
  
//  itk::ImageIORegion ioRegionStreaming = output->GetRequestedRegion();

  itk::ImageIORegion::SizeType ioSize = ioRegion.GetSize();
  itk::ImageIORegion::IndexType ioStart = ioRegion.GetIndex();

/* Init IORegion with size or streaming size */
  SizeType dimSize;
  for(unsigned int i=0; i<TOutputImage::ImageDimension; i++)
    {
    if (i < this->m_ImageIO->GetNumberOfDimensions())
      {
      if( !this->m_ImageIO->CanStreamRead() )
      dimSize[i] = this->m_ImageIO->GetDimensions(i);
      else
      dimSize[i] = output->GetRequestedRegion().GetSize()[i];
      }
    else
      {
      // Number of dimensions in the output is more than number of dimensions
      // in the ImageIO object (the file).  Use default values for the size,
      // spacing, and origin for the final (degenerate) dimensions.
      dimSize[i] = 1;
      }
    }

  for(unsigned int i = 0; i < dimSize.GetSizeDimension(); ++i)
    {
    ioSize[i] = dimSize[i];
    }

  typedef typename TOutputImage::IndexType   IndexType;
  IndexType start;
  if( !this->m_ImageIO->CanStreamRead() )  start.Fill(0);
  else start = output->GetRequestedRegion().GetIndex();
  for(unsigned int i = 0; i < start.GetIndexDimension(); ++i)
    {
    ioStart[i] = start[i];
    }

  ioRegion.SetSize(ioSize);
  ioRegion.SetIndex(ioStart);

//otbMsgDebugMacro( <<" Apres ioRegion : "<<ioRegion);

  itkDebugMacro (<< "ioRegion: " << ioRegion);
 
  this->m_ImageIO->SetIORegion(ioRegion);

  typedef itk::DefaultConvertPixelTraits< typename TOutputImage::IOPixelType >  ConvertPixelTraits;
  
  
  
  if ( this->m_ImageIO->GetComponentTypeInfo()
       == typeid(ITK_TYPENAME ConvertPixelTraits::ComponentType)
       && (this->m_ImageIO->GetNumberOfComponents()
           == ConvertPixelTraits::GetNumberOfComponents()))
    {
    itkDebugMacro(<< "No buffer conversion required.");
    // allocate a buffer and have the ImageIO read directly into it
    this->m_ImageIO->Read(buffer);
    return;
    }
  else // a type conversion is necessary
    {
    itkDebugMacro(<< "Buffer conversion required.");
    // note: char is used here because the buffer is read in bytes
    // regardles of the actual type of the pixels.
    ImageRegionType region = output->GetBufferedRegion();
//THOMAS : PB : le GetImageSizeInBytes s'appuie sur m_Dimension lue, donc dimensions physique,de 
// l'image (pas celle bufferiséd ans la cas du streaming )
    char * loadBuffer = 
      new char[this->m_ImageIO->GetImageSizeInBytes()];

    this->m_ImageIO->Read(loadBuffer);
    
    itkDebugMacro(<< "Buffer conversion required from: "
                  << this->m_ImageIO->GetComponentTypeInfo().name()
                  << " to: "
                  << typeid(ITK_TYPENAME ConvertPixelTraits::ComponentType).name());

    this->DoConvertBuffer(loadBuffer, region.GetNumberOfPixels());
    delete [] loadBuffer;
    }
}


template <class TOutputImage>
void
ImageFileReader<TOutputImage>
::EnlargeOutputRequestedRegion(itk::DataObject *output)
{
  typename TOutputImage::Pointer out = dynamic_cast<TOutputImage*>(output);

  // the ImageIO object cannot stream, then set the RequestedRegion to the
  // LargestPossibleRegion
  if (!this->m_ImageIO->CanStreamRead())
    {
    if (out)
      {
      out->SetRequestedRegion( out->GetLargestPossibleRegion() );
      }
    else
      {
      throw itk::ImageFileReaderException(__FILE__, __LINE__,
                                     "Invalid output object type");
      }
    }
    else
    {
//otbMsgDebugMacro( << " Streaming Image Read ");
    }
}


template <class TOutputImage>
void 
ImageFileReader<TOutputImage>
::GenerateOutputInformation(void)
{

  typename TOutputImage::Pointer output = this->GetOutput();

  itkDebugMacro(<<"Reading file for GenerateOutputInformation()" << this->m_FileName);
  
  // Check to see if we can read the file given the name or prefix
  //
  if ( this->m_FileName == "" )
    {
    throw itk::ImageFileReaderException(__FILE__, __LINE__, "FileName must be specified");
    }

  // Test if the file exist and if it can be open.
  // and exception will be thrown otherwise.
  //
  this->TestFileExistanceAndReadability();
  
  if ( this->m_UserSpecifiedImageIO == false ) //try creating via factory
    {
    this->m_ImageIO = ImageIOFactory::CreateImageIO( this->m_FileName.c_str(), itk::ImageIOFactory::ReadMode );
    }
  
  if ( this->m_ImageIO.IsNull() )
    {
    itk::ImageFileReaderException e(__FILE__, __LINE__);
    itk::OStringStream msg;
    msg << " Could not create IO object for file "
        << this->m_FileName.c_str() << std::endl;
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
  
  // Got to allocate space for the image. Determine the characteristics of
  // the image.
  //
  this->m_ImageIO->SetFileName(this->m_FileName.c_str());
  this->m_ImageIO->ReadImageInformation();
  // Initialisation du nombre de Composante par pixel
// THOMAS ceci n'est pas dans ITK !!
//  output->SetNumberOfComponentsPerPixel(this->m_ImageIO->GetNumberOfComponents()); 
  
  SizeType dimSize;
  double spacing[ TOutputImage::ImageDimension ];
  double origin[ TOutputImage::ImageDimension ];
  typename TOutputImage::DirectionType direction;
  std::vector<double> axis;

  for(unsigned int i=0; i<TOutputImage::ImageDimension; i++)
    {
    if ( i < this->m_ImageIO->GetNumberOfDimensions() )
      {
      dimSize[i] = this->m_ImageIO->GetDimensions(i);
      spacing[i] = this->m_ImageIO->GetSpacing(i);
      origin[i]  = this->m_ImageIO->GetOrigin(i);
// Please note: direction cosines are stored as columns of the
// direction matrix
      axis = this->m_ImageIO->GetDirection(i);
      for (unsigned j=0; j<TOutputImage::ImageDimension; j++)
        {
        if (j < this->m_ImageIO->GetNumberOfDimensions())
          {
          direction[j][i] = axis[j];
          }
        else
          {
          direction[j][i] = 0.0;
          }
        }
      }
    else
      {
      // Number of dimensions in the output is more than number of dimensions
      // in the ImageIO object (the file).  Use default values for the size,
      // spacing, origin and direction for the final (degenerate) dimensions.
      dimSize[i] = 1;  
      spacing[i] = 1.0;
      origin[i] = 0.0;
      for (unsigned j = 0; j < TOutputImage::ImageDimension; j++)
        {
        if (i == j)
          {
          direction[j][i] = 1.0;
          }
        else
          {
          direction[j][i] = 0.0;
          }
        }
      }
    }

  output->SetSpacing( spacing );     // Set the image spacing
  output->SetOrigin( origin );       // Set the image origin
  output->SetDirection( direction ); // Set the image direction cosines

  //Copy MetaDataDictionary from instantiated reader to output image.
  output->SetMetaDataDictionary(this->m_ImageIO->GetMetaDataDictionary());
  this->SetMetaDataDictionary(this->m_ImageIO->GetMetaDataDictionary());

  typedef typename TOutputImage::IndexType   IndexType;

  IndexType start;
  start.Fill(0);

  ImageRegionType region;
  region.SetSize(dimSize);
  region.SetIndex(start);

// THOMAS : ajout 
  // If a VectorImage, this requires us to set the 
  // VectorLength before allocate
  if( strcmp( output->GetNameOfClass(), "VectorImage" ) == 0 ) 
    {
    typedef typename TOutputImage::AccessorFunctorType AccessorFunctorType;
    AccessorFunctorType::SetVectorLength( output, this->m_ImageIO->GetNumberOfComponents() );
    }
 
  output->SetLargestPossibleRegion(region);

}

} //namespace otb

#endif
