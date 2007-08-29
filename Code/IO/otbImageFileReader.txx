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
#ifndef _otbImageFileReader_txx
#define _otbImageFileReader_txx
#include "otbImageFileReader.h"

#include "itkObjectFactory.h"
#include "itkConvertPixelBuffer.h"
#include "itkImageRegion.h"
#include "itkPixelTraits.h"
#include "itkVectorImage.h"
#include "itkMetaDataObject.h"

#include "otbMacro.h"
#include "otbSystem.h"
#include "otbImageIOFactory.h"
#include "otbMetaDataKey.h"
#include "otbImageKeywordlist.h"

#include "imaging/ossimImageHandlerRegistry.h"
#include "imaging/ossimImageHandler.h"
#include "init/ossimInit.h"
#include "base/ossimKeywordlist.h"

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

  typedef itk::DefaultConvertPixelTraits< ITK_TYPENAME TOutputImage::IOPixelType >  ConvertPixelTraits;
  
  
  
  if ( this->m_ImageIO->GetComponentTypeInfo()
       == typeid(ITK_TYPENAME ConvertPixelTraits::ComponentType)
       && (this->m_ImageIO->GetNumberOfComponents()
           == ConvertPixelTraits::GetNumberOfComponents()))
    {
    otbMsgDevMacro(<< "No buffer conversion required.");
    // allocate a buffer and have the ImageIO read directly into it
    this->m_ImageIO->Read(buffer);
    return;
    }
  else // a type conversion is necessary
    {
    // note: char is used here because the buffer is read in bytes
    // regardles of the actual type of the pixels.
    ImageRegionType region = output->GetBufferedRegion();
//THOMAS : PB : le GetImageSizeInBytes s'appuie sur m_Dimension lue, donc dimensions physique,de 
// l'image (pas celle bufferis�d ans la cas du streaming )
// JULIEN: fix for the problem mentionned : the componentsize is computed here since ad hoc methods in 
// itkImageIOBase are protected.
//THOMAS : PB with big file : "unsigned int" is not adapted. We use streamoff type.
//    unsigned int componentSize = this->m_ImageIO->GetImageSizeInBytes();
/*    componentSize = static_cast<std::streamoff>(this->m_ImageIO->GetImageSizeInPixels());
    componentSize = componentSize * static_cast<std::streamoff>(this->m_ImageIO->GetNumberOfComponents()); */
// PB : the GetComponentSize() method of the itkImageIOBase  class is protected. 
// duplicated this method in this class (very BAD)
	
    std::streamoff nbBytes = static_cast<std::streamoff>(this->GetComponentSize())*static_cast<std::streamoff>(region.GetNumberOfPixels())*static_cast<std::streamoff>(this->m_ImageIO->GetNumberOfComponents());
    otbMsgDevMacro(<<"NbBytes "<<nbBytes);

    otbMsgDevMacro(<< "Buffer conversion required from: "
                     << this->m_ImageIO->GetComponentTypeInfo().name()
                     << " to: "
                     << typeid(ITK_TYPENAME ConvertPixelTraits::ComponentType).name() << "  NbBytes"<<nbBytes<<"  region.GetNumberOfPixels()"<<region.GetNumberOfPixels());
    char * loadBuffer = new char[nbBytes];
    
    this->m_ImageIO->Read(loadBuffer);

    this->DoConvertBuffer(loadBuffer, region.GetNumberOfPixels());

    delete [] loadBuffer;
    }
}

template <class TOutputImage>
unsigned int 
ImageFileReader<TOutputImage>
::GetComponentSize() const
{
  switch(this->m_ImageIO->GetComponentType())
    {
    case itk::ImageIOBase::UCHAR:
      return sizeof(unsigned char);
    case itk::ImageIOBase::CHAR:
      return sizeof(char);
    case itk::ImageIOBase::USHORT:
      return sizeof(unsigned short);
    case itk::ImageIOBase::SHORT:
      return sizeof(short);
    case itk::ImageIOBase::UINT:
      return sizeof(unsigned int);
    case itk::ImageIOBase::INT:
      return sizeof(int);
    case itk::ImageIOBase::ULONG:
      return sizeof(unsigned long);
    case itk::ImageIOBase::LONG:
      return sizeof(long);
    case itk::ImageIOBase::FLOAT:
      return sizeof(float);
    case itk::ImageIOBase::DOUBLE:
      return sizeof(double);
    case itk::ImageIOBase::UNKNOWNCOMPONENTTYPE:
    default:
      itkExceptionMacro ("Unknown component type: " << this->m_ImageIO->GetComponentType());
    }

  return 0;
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
otbMsgDevMacro( << " Streaming Image Read ");
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

  // Find real image file name
  // !!!!  Update FileName
  std::string lFileName;
  bool found = GetGdalReadImageFileName(this->m_FileName,lFileName);
  if( found == false )
  {
  	  otbMsgDebugMacro( <<"Filename was NOT unknowed. May be reconize by a Image factory ! ");
  }
  // Update FileName
  this->m_FileName = lFileName;
  
  std::string lFileNameOssimKeywordlist = this->m_FileName ;
  

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

  // Trying to read ossim MetaData
  
  // Itinialize ossim environment
  ossimImageHandler* handler = ossimImageHandlerRegistry::instance()->open(ossimFilename(lFileNameOssimKeywordlist.c_str()));
  

  if (!handler)
  {
  	  otbMsgDebugMacro( <<"OSSIM Open Image FAILED ! ");
  }

  else
  {
	  ossimKeywordlist geom_kwl, tmp_kwl, tmp_kwl2;// = new ossimKeywordlist();
  
	  // Read OSSIM Keyword List
	  bool hasMetaData = handler->getImageGeometry(geom_kwl);
	  otbMsgDebugMacro( << " AVANT *geom_kwl : "<<geom_kwl<<std::endl);

	  if (!hasMetaData)
	  {
	  	  otbMsgDebugMacro( <<"OSSIM MetaData not present ! ");
	  }
	  else
	  {
	  	  
		  otbMsgDebugMacro( <<"Image keyword lists are :" << std::endl << geom_kwl);
		  
	      // Update otb Keywordlist
		  ImageKeywordlist otb_kwl;
		  otb_kwl.SetKeywordlist( geom_kwl );
		  
	  	  // Update itk MetaData Dictionnary
//		  otbMsgDebugMacro( <<"Start update ITK Dictionnary ? ");
		  
		  otb_kwl.convertToOSSIMKeywordlist(tmp_kwl);

		  itk::MetaDataDictionary& dico = this->m_ImageIO->GetMetaDataDictionary();
  
//		  otbMsgDebugMacro( <<"Before write ITK Dictionnary ? ");
	  	  itk::EncapsulateMetaData< ImageKeywordlist >(dico,
	  											 MetaDataKey::m_OSSIMKeywordlistKey,
												 otb_kwl);
												 
//		  otbMsgDebugMacro( <<"After write ITK Dictionnary ? ");
//		  itk::ExposeMetaData< ImageKeywordlist >(dico,
//	  											 MetaDataKey::m_OSSIMKeywordlistKey,
//												 otb_tmp);
//		  otbMsgDebugMacro( <<"After read ITK Dictionnary ? ");
		  
//		  otb_tmp.convertToOSSIMKeywordlist(tmp_kwl2);
//		  otbMsgDebugMacro( << " DEBUT THOMAS : Ossim key word list copy : "<<tmp_kwl2<<std::endl);

		 // otbMsgDebugMacro( <<"Image keyword lists in dictionnary are :" << std::endl << geom_tmp);
		  
	  }
	  // Free memory
	  otbMsgDebugMacro( <<"OSSIM Free Memory ? ");
	  delete handler;  		
	  otbMsgDebugMacro( <<"OSSIM Free Memory OK ! ");
  }
  
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

template <class TOutputImage>
void 
ImageFileReader<TOutputImage>
::TestFileExistanceAndReadability()
{
    // Test if the file exists.
    if( ! itksys::SystemTools::FileExists( this->m_FileName.c_str() ) )
      {
      itk::ImageFileReaderException e(__FILE__, __LINE__);
      itk::OStringStream msg;
      msg <<"The file doesn't exists. "
          << std::endl << "Filename = " << this->m_FileName
          << std::endl;
      e.SetDescription(msg.str().c_str());
      throw e;
      return;
      }

    // Test if the file can be open for reading access.
    //Only if m_FileName speciy a filname (not a dirname)
    if( System::IsAFileName( this->m_FileName ) == true )
    {
        std::ifstream readTester;
        readTester.open( this->m_FileName.c_str() );
        if( readTester.fail() )
        {
                readTester.close();
                itk::OStringStream msg;
                msg <<"The file couldn't be opened for reading. "
                        << std::endl << "Filename: " << this->m_FileName
                        << std::endl;
                itk::ImageFileReaderException e(__FILE__, __LINE__,msg.str().c_str(),ITK_LOCATION);
                throw e;
                return;

        }
        readTester.close();
    }
}

template <class TOutputImage>
bool
ImageFileReader<TOutputImage>
::GetGdalReadImageFileName( const std::string & filename, std::string & GdalFileName )
{
        std::vector<std::string> listFileSearch;
        listFileSearch.push_back("DAT_01.001");// RADARSAT ou SAR_ERS2
        listFileSearch.push_back("IMAGERY.TIF");//For format SPOT5TIF
        listFileSearch.push_back("IMAG_01.DAT");//For format SPOT4
        std::string str_FileName;
        bool fic_trouve(false);

        // Si c'est un repertoire, on regarde le contenu pour voir si c'est pas du RADARSAT, ERS
        std::vector<std::string> listFileFind;
        listFileFind = System::Readdir(filename);
        if( listFileFind.empty() == false )
        {
                unsigned int cpt(0);
		while ( (cpt < listFileFind.size()) && (fic_trouve==false) )
                {
			str_FileName = std::string(listFileFind[cpt]);
                        for(unsigned int i = 0 ; i < listFileSearch.size() ; i++)
                        {
         			if(str_FileName.compare(listFileSearch[i]) == 0)
	        		{
                                         GdalFileName = std::string(filename)+listFileSearch[i];
                                         fic_trouve=true;
        			}
                        }
                        cpt++;
                }
	}
        else 
        {
                std::string strFileName(filename);
                
                std::string extension = System::GetExtension(strFileName);
                if( (extension=="HDR") || (extension=="hdr") )
                {
                        //Supprime l'extension
                        GdalFileName = System::GetRootName(strFileName);
                }

                else
                {
                        // Sinon le filename est le nom du fichier a ouvrir
                        GdalFileName = std::string(filename);
                }
		fic_trouve=true;
        }
	otbMsgDevMacro(<<"lFileNameGdal : "<<GdalFileName.c_str());
	otbMsgDevMacro(<<"fic_trouve : "<<fic_trouve);
	return( fic_trouve );
}


} //namespace otb

#endif
