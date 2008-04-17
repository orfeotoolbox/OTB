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

#ifndef __otbVectorDataFileWriter_txx
#define __otbVectorDataFileWriter_txx


#include "otbMacro.h"
#include "otbVectorDataFileWriter.h"
#include "otbFileName.h"
#include "otbVectorDataIOFactory.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputVectorData>
VectorDataFileWriter<TInputVectorData>
::VectorDataFileWriter() :      m_FileName(""),
                                m_VectorDataIO(0), 
                                m_UserSpecifiedVectorDataIO(false),
                                m_UseInputMetaDataDictionary(true)

 
{
}
/**
 * Destructor
 */
template <class TInputVectorData>
VectorDataFileWriter<TInputVectorData>
::~VectorDataFileWriter()
{
}


//---------------------------------------------------------
template <class TInputVectorData>
void 
VectorDataFileWriter<TInputVectorData>
::SetInput(const InputVectorDataType *input)
{
	// ProcessObject is not const_correct so this cast is required here.
  this->ProcessObject::SetNthInput(0, 
                                   const_cast<TInputVectorData *>(input ) );
}


//---------------------------------------------------------
template <class TInputVectorData>
const typename VectorDataFileWriter<TInputVectorData>::InputVectorDataType *
VectorDataFileWriter<TInputVectorData>
::GetInput(void)
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }
  
  return static_cast<TInputVectorData*>
    (this->ProcessObject::GetInput(0));
}
  
//---------------------------------------------------------
template <class TInputVectorData>
const typename VectorDataFileWriter<TInputVectorData>::InputVectorDataType *
VectorDataFileWriter<TInputVectorData>
::GetInput(unsigned int idx)
{
  return static_cast<TInputVectorData*> (this->ProcessObject::GetInput(idx));
}

/*
template <class TInputVectorData>
void
VectorDataFileWriter<TInputVectorData>
::Write()
{
  const InputVectorDataType * input = this->GetInput();
	
	if ( input == 0 )
  {
   	itkExceptionMacro(<< "No input to writer!");
  }
	
	if ( m_FileName == "" )
  {
    itkExceptionMacro(<<"No filename was specified");
  }

  // NOTE: this const_cast<> is due to the lack of const-correctness
  // of the ProcessObject.
  InputVectorDataType * nonConstVectorData = const_cast<InputVectorDataType *>(input);

   // Make sure the data is up-to-date.
  if( nonConstVectorData->GetSource() )
  {
    nonConstVectorData->GetSource()->Update();
  }

  // Actually do something
  this->GenerateData();
  
  // Release upstream data if requested
  if ( input->ShouldIReleaseData() )
  {
    nonConstVectorData->ReleaseData();
  }
}

template <class TInputVectorData>
void
VectorDataFileWriter<TInputVectorData>
::GenerateData()
{
	const InputVectorDataType* shapeFile = this->GetInput();
	
	// If output file already exists, this file is erased
	// to avoid ogr write problems
	otb::FileName fileName(m_FileName.c_str());

	OGRRegisterAll();

  OGRDataSource *poDS = shapeFile->GetOGRDataSource();

	poDS->GetLayerCount();
	otbGenericMsgTestingMacro(<< "WRITE");

  const char *pszDriverName = "ESRI Shapefile";
  OGRSFDriver *poDriver;
   	
	poDriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(
                pszDriverName );
  
	if( poDriver == NULL )
  {
        itkExceptionMacro(<< pszDriverName << " driver not available");
  }

  OGRDataSource *poDSW;

  poDSW = poDriver->CreateDataSource( m_FileName.c_str(), NULL );
  
	if( poDSW == NULL )
  {
      itkExceptionMacro(<< "Creation of output file failed");
  }

  unsigned int nbOfLayers = poDS->GetLayerCount();
	
	for (unsigned int i=0; i<nbOfLayers; i++)
	{
			otbGenericMsgTestingMacro(<< "Layer number " << i+1);
			OGRLayer  *poLayer2 = poDS->GetLayer(i);		
			poLayer2->ResetReading();
	
	std::cout << "Filename ::: " << fileName.ObtainFileNameWithNoExtension().c_str() << std::endl;
			OGRLayer* poWLayer = poDSW->CreateLayer(fileName.ObtainFileNameWithNoExtension().c_str());
				
   		if( poLayer2 == NULL )
   		{
       	itkExceptionMacro(<< "Layer creation failed." );
   		}
				
			OGRFeature* poFeature2;			
			while( (poFeature2 = poLayer2->GetNextFeature()) != NULL )
   		{
				otbGenericMsgTestingMacro(<< "Feature! ");
				OGRFeature* poWFeature;
					
				unsigned int nbFields = poFeature2->GetFieldCount();
				for (unsigned int i=0; i<nbFields; i++)
				{
					OGRFieldDefn* oField = poFeature2->GetFieldDefnRef(i);
						
					otbGenericMsgTestingMacro(<< "Field : " << poFeature2->GetRawFieldRef(i)->String);
    			if( poWLayer->CreateField( oField ) != OGRERR_NONE )
   				{
        			itkExceptionMacro(<<"Creating Name field failed." );
    			}
	
	      	poWFeature = OGRFeature::CreateFeature( poLayer2->GetLayerDefn() );

					poWFeature->SetField(i,poFeature2->GetRawFieldRef(i));
					otbGenericMsgTestingMacro(<< "FieldW : " << poWFeature->GetRawFieldRef(i)->String);
				}

				poWFeature->SetGeometry(poFeature2->GetGeometryRef());
	
    		if( poWLayer->CreateFeature( poWFeature ) != OGRERR_NONE )
       	{
          itkExceptionMacro(<<"Failed to create feature in shapefile." );
       	}
					
				OGRFeature::DestroyFeature(poWFeature);

			}		
	}
		
	OGRDataSource::DestroyDataSource( poDSW );
}
*/


//---------------------------------------------------------
template <class TInputVectorData>
void
VectorDataFileWriter<TInputVectorData>
::Write()
{
  const InputVectorDataType * input = this->GetInput();

  itkDebugMacro( <<"Writing a vector data file" );

  // Make sure input is available
  if ( input == 0 )
    {
    itkExceptionMacro(<< "No input to writer!");
    }

  // Make sure that we can write the file given the name
  //
  if ( m_FileName == "" )
    {
    itkExceptionMacro(<<"No filename was specified");
    }

  if ( m_VectorDataIO.IsNull() ) //try creating via factory
    {
    itkDebugMacro(<<"Attempting factory creation of VectorDataIO for file: " 
                  << m_FileName);
    m_VectorDataIO = VectorDataIOFactory<TInputVectorData>::CreateVectorDataIO( m_FileName.c_str(), 
                                               VectorDataIOFactory<TInputVectorData>::WriteMode );
    m_FactorySpecifiedVectorDataIO = true;
    }
  else
    {
    if( m_FactorySpecifiedVectorDataIO && !m_VectorDataIO->CanWriteFile( m_FileName.c_str() ) )
      {
      itkDebugMacro(<<"VectorDataIO exists but doesn't know how to write file:" 
                    << m_FileName );
      itkDebugMacro(<<"Attempting creation of VectorDataIO with a factory for file:"
                    << m_FileName);
      m_VectorDataIO = VectorDataIOFactory<TInputVectorData>::CreateVectorDataIO( m_FileName.c_str(), 
                                                 VectorDataIOFactory<TInputVectorData>::WriteMode );
      m_FactorySpecifiedVectorDataIO = true;
      }
    }

  if ( m_VectorDataIO.IsNull() )
    {
    VectorDataFileWriterException e(__FILE__, __LINE__);
    itk::OStringStream msg;
    msg << " Could not create IO object for file "
        << m_FileName.c_str() << std::endl;
    msg << "  Tried to create one of the following:" << std::endl;
    std::list<itk::LightObject::Pointer> allobjects = 
      itk::ObjectFactoryBase::CreateAllInstance("otbVectorDataIOBase");
    for(std::list<LightObject::Pointer>::iterator i = allobjects.begin();
        i != allobjects.end(); ++i)
      {
      VectorDataIOBase<TInputVectorData>* io = dynamic_cast<VectorDataIOBase<TInputVectorData>*>(i->GetPointer());
      msg << "    " << io->GetNameOfClass() << std::endl; 
      }
    msg << "  You probably failed to set a file suffix, or" << std::endl;
    msg << "    set the suffix to an unsupported type." << std::endl;
    e.SetDescription(msg.str().c_str());
    e.SetLocation(ITK_LOCATION);
    throw e;
    }

  // NOTE: this const_cast<> is due to the lack of const-correctness
  // of the ProcessObject.
  InputVectorDataType * nonConstVectorData = const_cast<InputVectorDataType *>(input);


  // Make sure the data is up-to-date.
  if( ! nonConstVectorData->GetSource() )
  {
        nonConstVectorData->GetSource()->Update();
  }

  if( m_UseInputMetaDataDictionary )
    {
    m_VectorDataIO->SetMetaDataDictionary(input->GetMetaDataDictionary());
    }


  // Notify start event observers
  this->InvokeEvent( itk::StartEvent() );

  // Actually do something
  this->GenerateData();
  
  // Notify end event observers
  this->InvokeEvent( itk::EndEvent() );

  // Release upstream data if requested
  if ( input->ShouldIReleaseData() )
    {
    nonConstVectorData->ReleaseData();
    }
}


//---------------------------------------------------------
template <class TInputVectorData>
void
VectorDataFileWriter<TInputVectorData>
::GenerateData(void)
{
  const InputVectorDataType * input = this->GetInput();

  itkDebugMacro(<<"Writing file: " << m_FileName);
  
  // Setup the vector data IO for writing.
  //
  m_VectorDataIO->SetFileName(m_FileName.c_str());

  m_VectorDataIO->Write(input);

}

template <class TInputVectorData>
void
VectorDataFileWriter<TInputVectorData>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
	Superclass::PrintSelf(os, indent);
	os << indent << "VectorDataFileWriter" << std::endl;
}



} //namespace otb

#endif
