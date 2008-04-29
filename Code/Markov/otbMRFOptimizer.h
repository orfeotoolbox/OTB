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
#ifndef _MRFOptimizer_h
#define _MRFOptimizer_h

#include "itkObject.h"
#include "itkObjectFactory.h"

namespace otb
{
/**
 * \class MRFOptimizer
 * \brief This is the base class for optimizer used in the MRF framework.
 */

class ITK_EXPORT MRFOptimizer : public itk::Object
  {       
  public:
    typedef MRFOptimizer                  Self;
    typedef itk::Object                   Superclass;
    typedef itk::SmartPointer<Self>       Pointer;
    typedef itk::SmartPointer<const Self> ConstPointer;
    typedef itk::Array< double >          ParametersType;
    
    itkNewMacro(Self);
    
    itkTypeMacro(MRFOptimizer, itk::Object);
    
    unsigned int GetNumberOfParameters(void) const 
    { return m_NumberOfParameters; }
    
    // Get the parameters
    const ParametersType& GetParameters( void ) const
    {
      return this->m_Parameters;
    }
    
    void SetParameters( const ParametersType & parameters )
    {
      bool modified = false;
      for( unsigned int i=0; i<m_NumberOfParameters; i++ )
      {
        if (m_Parameters[i] != parameters[i])
        {
          m_Parameters[i] = parameters[i];
          modified = true;
        }
      }
      if (modified)
      {
        this->Modified();
      }
    }
    
    virtual bool Compute(double deltaEnergy)
      {
	return false;
      }   
    
  protected:
    MRFOptimizer() {}
    virtual ~MRFOptimizer() {}
    unsigned int m_NumberOfParameters;
    ParametersType m_Parameters;
    
  };      
}

#endif

