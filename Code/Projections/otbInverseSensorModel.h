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
#ifndef __otbInverseSensorModel_h
#define __otbInverseSensorModel_h

#include "otbSensorModelBase.h"
#include "otbDEMReader.h"

#include "itkMacro.h"
#include "itkSmartPointer.h"
#include "itkObject.h"


namespace otb
{

/** \class InverseSensorModel

 * \brief Class for inverse sensor models 
 *
 * Based to ossimProjectionFactoryRegistry and ossimProjection methods.
 * It takes in input an world point and compute the index position of the correspondant point in the input image.
 * (lat,lon,h) -> (i,j).
 * Notice that the elevation h is optional. 
 *
 * \ingroup Transform 
 */
template <class TScalarType,
          unsigned int NInputDimensions=2,
          unsigned int NOutputDimensions=2,
          unsigned int NParametersDimensions=3>
class ITK_EXPORT InverseSensorModel : public SensorModelBase<TScalarType,          
			                                    									 NInputDimensions,  
			                                    									 NOutputDimensions,
                                                             NParametersDimensions>
{

public :

	/** Standard class typedefs. */
  typedef InverseSensorModel                             Self;
  typedef SensorModelBase< TScalarType,
                   NInputDimensions,
                   NOutputDimensions,
                   NParametersDimensions >               Superclass;
  typedef itk::SmartPointer<Self>                    	   Pointer;
  typedef itk::SmartPointer<const Self>              	   ConstPointer;

  typedef typename Superclass::InputPointType            InputPointType;
//	typedef itk::Point<TScalarType, 3>										 InputPointType;
	typedef typename Superclass::OutputPointType           OutputPointType; 
	
	//typedef otb::Image<double, NInputDimensions>	 ImageType;
	typedef DEMReader													 						 DEMHandlerType;
	typedef typename DEMHandlerType::Pointer							 DEMHandlerPointerType;     

	/** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Run-time type information (and related methods). */
  itkTypeMacro( InverseSensorModel, SensorModelBase );
	       
  itkStaticConstMacro(InputSpaceDimension, unsigned int, NInputDimensions);
  itkStaticConstMacro(OutputSpaceDimension, unsigned int, NOutputDimensions);
  itkStaticConstMacro(ParametersDimension, unsigned int, NParametersDimensions); //A voir!!
	         
  // Transform of geographic point in image sensor index
	OutputPointType TransformPoint(const InputPointType &point) const;
  // Transform of geographic point in image sensor index -- Backward Compatibility
//  OutputPointType TransformPoint(const InputPointType &point, double height) const;

	itkGetMacro(UseDEM, bool);
	itkSetMacro(UseDEM, bool);
	
	itkGetObjectMacro(DEMHandler, DEMHandlerType);
	
	virtual void SetDEMHandler(DEMHandlerType* _arg) 
  { 
    if (this->m_DEMHandler != _arg) 
      { 
      this->m_DEMHandler = _arg; 
      this->Modified(); 
			m_UseDEM = true;
      } 
  }
	
	virtual bool SetDEMDirectory(const std::string& directory)
	{
		bool b = m_DEMHandler->OpenDEMDirectory(directory.c_str());
		this->UseDEM(true);	
		
		return b;	
	}
	
	virtual void UseDEM(bool b) { m_UseDEM = b; this->Modified(); } 

protected:
  InverseSensorModel(); 
  virtual ~InverseSensorModel();

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;
	
private :

  InverseSensorModel(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
	
	/** Object that read and use DEM */
	DEMHandlerPointerType m_DEMHandler;
	
	/** Specify if DEM is used in Point Transformation */
	bool m_UseDEM ;
	

};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbInverseSensorModel.txx"
#endif


#endif
