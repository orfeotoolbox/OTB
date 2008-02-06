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
#include "otbJPEG2000ImageIO.h"

#include "itkExceptionObject.h"
#include "otbMacro.h"

#include "otbSystem.h"

namespace otb
{

JPEG2000ImageIO::JPEG2000ImageIO()
{
  // By default set number of dimensions to two.
  this->SetNumberOfDimensions(2);
  m_PixelType = SCALAR;
  m_ComponentType = UCHAR;

  // Set default spacing to one
  m_Spacing[0] = 1.0;
  m_Spacing[1] = 1.0;
  // Set default origin to zero
  m_Origin[0] = 0.0;
  m_Origin[1] = 0.0;
}

JPEG2000ImageIO::~JPEG2000ImageIO()
{
}

bool JPEG2000ImageIO::CanReadFile( const char* filename )
{
        return false;
}


// Used to print information about this object
void JPEG2000ImageIO::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

// Read a 3D image (or event more bands)... not implemented yet
void JPEG2000ImageIO::ReadVolume(void*)
{
}

// Read image 
void JPEG2000ImageIO::Read(void* buffer)
{
        unsigned long step = this->GetNumberOfComponents();
        char * p = static_cast<char *>(buffer);
   
        int lNbLignes   = this->GetIORegion().GetSize()[1];
        int lNbColonnes = this->GetIORegion().GetSize()[0];
        int lPremiereLigne   = this->GetIORegion().GetIndex()[1] ; // [1... ]
        int lPremiereColonne = this->GetIORegion().GetIndex()[0] ; // [1... ]

otbMsgDevMacro( <<" JPEG2000ImageIO::Read()  ");
otbMsgDevMacro( <<" Dimensions de l'image  : "<<m_Dimensions[0]<<","<<m_Dimensions[1]);
otbMsgDevMacro( <<" Region lue (IORegion)  : "<<this->GetIORegion());
otbMsgDevMacro( <<" Nb Of Components       : "<<this->GetNumberOfComponents());

        std::streamoff  headerLength(0);
        std::streamoff  numberOfBytesPerLines = static_cast<std::streamoff>(this->GetComponentSize() * m_Dimensions[0]);
        std::streamoff  offset;
        std::streamsize numberOfBytesToBeRead = this->GetComponentSize() * lNbColonnes;
        std::streamsize numberOfBytesRead;        
        unsigned long cpt = 0;

        // Update the step variable
        step = step * (unsigned long)(this->GetComponentSize());
        
        char * value = new char[numberOfBytesToBeRead];
        if(value==NULL)
        {
                itkExceptionMacro(<<"Memory bad alloc");
                return;
        }





        delete [] value;

}


void JPEG2000ImageIO::ReadImageInformation()
{


otbMsgDebugMacro( <<"Driver to read: JPEG2000");
otbMsgDebugMacro( <<"         Read  file         : "<< m_FileName);
otbMsgDebugMacro( <<"         Size               : "<<m_Dimensions[0]<<","<<m_Dimensions[1]);
otbMsgDebugMacro( <<"         ComponentType      : "<<this->GetComponentType() );
otbMsgDebugMacro( <<"         NumberOfComponents : "<<this->GetNumberOfComponents());
otbMsgDebugMacro( <<"         ComponentSize      : "<<this->GetComponentSize());
otbMsgDebugMacro( <<"         GetPixelSize       : "<<this->GetPixelSize());

}


bool JPEG2000ImageIO::CanWriteFile( const char* filename )
{
return false;
#if 0
        std::string lFileName(filename);
        if( System::IsADirName(lFileName) == true )
        {
                return false;
        }
        const std::string Extension = System::GetExtension(filename);
        if( (Extension == "j2k") || (Extension == "J2K") || (Extension == "jp2") || (Extension == "JP2"))
        {
                return true;
        }
        else
        {
                return false;
        }
#endif
}

void JPEG2000ImageIO::Write(const void* buffer)
{
}


void JPEG2000ImageIO::WriteImageInformation()
{
        if ( m_FileName == "" )
        {
                itkExceptionMacro(<<"A FileName must be specified.");
        }
        if( CanWriteFile(m_FileName.c_str()) == false)
        {
                itkExceptionMacro(<< "The file "<<m_FileName.c_str()<<" is not defined as a JPEG2000 file");
        }

otbMsgDebugMacro( <<"Driver to write: JPEG2000");
otbMsgDebugMacro( <<"         Write file         : "<< m_FileName);
otbMsgDebugMacro( <<"         Size               : "<<m_Dimensions[0]<<","<<m_Dimensions[1]);
otbMsgDebugMacro( <<"         ComponentType      : "<<this->GetComponentType() );
otbMsgDebugMacro( <<"         NumberOfComponents : "<<this->GetNumberOfComponents());
otbMsgDebugMacro( <<"         ComponentSize      : "<<this->GetComponentSize());
otbMsgDebugMacro( <<"         GetPixelSize       : "<<this->GetPixelSize());

}

  
} // end namespace otb

