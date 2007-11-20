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
#ifndef __otbSensorModelBase_h
#define __otbSensorModelBase_h

#include <iostream>

#include "otbMacro.h"
#include "otbImageKeywordlist.h"
#include "otbDEMHandler.h"

#include "projection/ossimProjection.h"

#include "itkTransform.h"
#include "itkSmartPointer.h"
#include "itkObject.h"

namespace otb
{
  /** \class SensorModelBase
   *  \brief Base class for the sensor model projection classes.
   *  This class allows to transform a geographic point in (lat,long) to a point in the sensor geometry.
   *  (lat,lon) -> (i,j) ou (lat,lon,h) -> (i,j)
   */
template <class TScalarType,
          unsigned int NInputDimensions=3,
          unsigned int NOutputDimensions=2,
          unsigned int NParametersDimensions=3>
class ITK_EXPORT SensorModelBase : public itk::Transform<TScalarType,          
										                                     NInputDimensions,  
			            							                         NOutputDimensions> 
{

		public :

      /** Standard class typedefs. */
      typedef SensorModelBase                             Self;
      typedef itk::Transform< TScalarType,
                              NInputDimensions,
                              NOutputDimensions >         Superclass;
      typedef itk::SmartPointer<Self>                     Pointer;
      typedef itk::SmartPointer<const Self>               ConstPointer;
      
      typedef itk::Point<TScalarType, NInputDimensions >  InputPointType;
      typedef itk::Point<TScalarType, NOutputDimensions > OutputPointType;      
      
      typedef DEMHandler				 DEMHandlerType;
      typedef typename DEMHandlerType::Pointer		 DEMHandlerPointerType;     
			
      /** Method for creation through the object factory. */
      itkNewMacro( Self );
      
      /** Run-time type information (and related methods). */
      itkTypeMacro( SensorModelBase, Transform );
      
      itkStaticConstMacro(InputSpaceDimension,  unsigned int, NInputDimensions);
      itkStaticConstMacro(OutputSpaceDimension, unsigned int, NOutputDimensions);
      itkStaticConstMacro(ParametersDimension,  unsigned int, NParametersDimensions); //A voir!!
      
      /* Get the ImageKeywordlist */
      ImageKeywordlist & GetImageGeometryKeywordlist(void) const;
      /* Get an ossimKeywordlist */
      ossimKeywordlist GetOssimKeywordlist(void);
      
      /* Set the Imagekeywordlist and affect the ossim projection ( m_Model) */
      virtual void SetImageGeometry(ImageKeywordlist image_kwl);
      
      /* Set the Imagekeywordlist and affect the ossim projection ( m_Model) */
      virtual void SetImageGeometry(ossimKeywordlist &geom_kwl);
      
     
//      itkGetObjectMacro(DEMHandler, DEMHandlerType);
      
/*      virtual void SetDEMHandler(DEMHandlerType* _arg) 
      { 
				if (this->m_DEMHandler != _arg) 
	  		{ 
	    		this->m_DEMHandler = _arg; 
	    		this->Modified(); 
	    		this->UseDEM(true);
	  		} 
      }*/
      
      virtual bool SetDEMDirectory(const std::string& directory)
      {
				bool b = m_DEMHandler->OpenDEMDirectory(directory.c_str());
				m_DEMIsLoaded=true;
				this->EnableDEM();	
	
				return b;	
      }
			
			virtual void DisableDEM()
			{
				m_UseDEM = false; 
				this->Modified();
			}
      
      virtual void EnableDEM() 
			{ 
				if (m_DEMIsLoaded) 
					m_UseDEM = true; 
				this->Modified();
			} 

   protected:
      SensorModelBase(); 
      virtual ~SensorModelBase();
      
      /** Create the projection ( m_Model). Called by the SetImageGeometry methods */
      void CreateProjection(const ImageKeywordlist & image_kwl);
      
      /** PrintSelf method */
      void PrintSelf(std::ostream& os, itk::Indent indent) const;
      
      /** ImageKeywordlist */
      ImageKeywordlist m_ImageKeywordlist;
      /** Pointer on an ossim projection (created with the keywordlist) */
      ossimProjection * m_Model;

      /** Specify if DEM is used in Point Transformation */
      bool m_UseDEM ;
			
      /** Object that read and use DEM */
      DEMHandlerPointerType m_DEMHandler;
			
   private :
      SensorModelBase(const Self&); //purposely not implemented
      void operator=(const Self&); //purposely not implemented
			
			/** Specify if DEM is loaded */
			bool m_DEMIsLoaded ;
     
    };

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSensorModelBase.txx"
#endif


#endif
