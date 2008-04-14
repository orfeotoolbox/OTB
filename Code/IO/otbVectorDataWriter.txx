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

#ifndef __otbVectorDataWriter_txx
#define __otbVectorDataWriter_txx


#include "otbMacro.h"
#include "otbSystem.h"
#include "otbVectorDataWriter.h"
#include "otbFileName.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputShapeFile>
VectorDataWriter<TInputShapeFile>
::VectorDataWriter()
{
  m_FileName = "";
}
/**
 * Destructor
 */
template <class TInputShapeFile>
VectorDataWriter<TInputShapeFile>
::~VectorDataWriter()
{
}


//---------------------------------------------------------
template <class TInputShapeFile>
void 
VectorDataWriter<TInputShapeFile>
::SetInput(const InputShapeFileType *input)
{
	// ProcessObject is not const_correct so this cast is required here.
  this->ProcessObject::SetNthInput(0, 
                                   const_cast<TInputShapeFile *>(input ) );
}


//---------------------------------------------------------
template <class TInputShapeFile>
const typename VectorDataWriter<TInputShapeFile>::InputShapeFileType *
VectorDataWriter<TInputShapeFile>
::GetInput(void)
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }
  
  return static_cast<TInputShapeFile*>
    (this->ProcessObject::GetInput(0));
}
  
//---------------------------------------------------------
template <class TInputShapeFile>
const typename VectorDataWriter<TInputShapeFile>::InputShapeFileType *
VectorDataWriter<TInputShapeFile>
::GetInput(unsigned int idx)
{
  return static_cast<TInputShapeFile*> (this->ProcessObject::GetInput(idx));
}

template <class TInputShapeFile>
void
VectorDataWriter<TInputShapeFile>
::Write()
{
  const InputShapeFileType * input = this->GetInput();
	
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
  InputShapeFileType * nonConstImage = const_cast<InputShapeFileType *>(input);

   // Make sure the data is up-to-date.
  if( nonConstImage->GetSource() )
  {
    nonConstImage->GetSource()->Update();
  }

  // Actually do something
  this->GenerateData();
  
  // Release upstream data if requested
  if ( input->ShouldIReleaseData() )
  {
    nonConstImage->ReleaseData();
  }
}

/**
 * Main computation method
 */
template <class TInputShapeFile>
void
VectorDataWriter<TInputShapeFile>
::GenerateData()
{
	const InputShapeFileType* shapeFile = this->GetInput();
	
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

template <class TInputShapeFile>
void
VectorDataWriter<TInputShapeFile>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
	Superclass::PrintSelf(os, indent);
	os << indent << "ShapeFileWriter" << std::endl;
}



} //namespace otb

#endif
