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
#ifndef __otbDEMReader_h
#define __otbDEMReader_h

#include "itkIndent.h"
#include "itkImageSource.h"
#include "otbImage.h"
#include <iostream>
#include <stdio.h>
#include "elevation/ossimElevManager.h"
#include "base/ossimFilename.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkIndent.h"

namespace otb
{
/** \class DEMReader
 *
 * \brief Class for Reading a DEM data
 *
 * This class is based on ossimElevManager. It takes in input the UL and LR geographic coordinates and the spacing.
 * Handle DTED and SRTM formats.
 * \ingroup Images
 *
 */

class ITK_EXPORT DEMReader: public itk::Object
{
public :
/** Standard class typedefs. */
  typedef itk::Indent														Indent;
  typedef DEMReader                             Self;
  typedef itk::Object														Superclass;
  typedef itk::SmartPointer<Self>               Pointer;
  typedef itk::SmartPointer<const Self>         ConstPointer;
	
	typedef itk::Point<double, 2> 								PointType;
	
	/** Method for creation through the object factory. */
  itkNewMacro(Self);
  
	/** Run-time type information (and related methods). */
  itkTypeMacro(DEMReader,Object);
  	                                      
	/** Try to open the DEM directory. */
	bool OpenDEMDirectory(const char* DEMDirectory);         

	/** Compute the height above MSL(Mean Sea Level) of a geographic point. */
	virtual double GetHeightAboveMSL(const PointType& geoPoint);     

protected:
	DEMReader();
	~DEMReader();

	void PrintSelf(std::ostream& os, Indent indent) const;

	ossimElevManager* m_ElevManager;

};

} // namespace otb



#endif
