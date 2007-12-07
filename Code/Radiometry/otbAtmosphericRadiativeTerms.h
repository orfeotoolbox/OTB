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
#ifndef _otbAtmosphericRadiativeTerms_h
#define _otbAtmosphericRadiativeTerms_h

#include "itkDataObject.h"
#include "itkObjectFactory.h"
#include "itkMacro.h"
#include <vector>


namespace otb 
{

/** \class AtmosphericRadiativeTermsSingleChannel
 *  \brief This class contains all atmospheric radiative terms for one channel.
 *
 * Each value can be deducted from the atmospheric correction parameters (using 6S) or directly set by the user.
 */

class ITK_EXPORT AtmosphericRadiativeTermsSingleChannel : public itk::DataObject
{
public:
  /** Standard typedefs */
  typedef AtmosphericRadiativeTermsSingleChannel     Self;
  typedef itk::DataObject                            Superclass;
  typedef itk::SmartPointer<Self>                    Pointer;
  typedef itk::SmartPointer<const Self>              ConstPointer;

  /** Type macro */
  itkTypeMacro(AtmosphericRadiativeTermsSingleChannel,DataObject);

  /** Creation through object factory macro */
  itkNewMacro(Self);


  /**
   * Set/Get the intrinsic atmospheric reflectance.
   */
  itkSetMacro(IntrinsicAtmosphericReflectance,double); 
  itkGetMacro(IntrinsicAtmosphericReflectance,double); 


  /**
   * Set/Get the spherical albedo of the atmosphere.
   */
  itkSetMacro(SphericalAlbedo,double); 
  itkGetMacro(SphericalAlbedo,double); 
  /**
   * Set/Get the total gaseous transmission.
   */
  itkSetMacro(TotalGaseousTransmission,double); 
  itkGetMacro(TotalGaseousTransmission,double); 

   /**
   * Set/Get the downward transmittance of the atmosphere.
   */
  itkSetMacro(DownwardTransmittance,double); 
  itkGetMacro(DownwardTransmittance,double); 

  /**
   * Set/Get the upward transmittance of the atmosphere.
   */
  itkSetMacro(UpwardTransmittance,double); 
  itkGetMacro(UpwardTransmittance,double); 

protected:
  /** Constructor */
  AtmosphericRadiativeTermsSingleChannel(){};
  /** Destructor */
  ~AtmosphericRadiativeTermsSingleChannel(){};
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  AtmosphericRadiativeTermsSingleChannel(const Self&) ; //purposely not implemented
  void operator=(const Self&) ; //purposely not implemented


  /** The intrinsic atmospheric reflectance. */
  double m_IntrinsicAtmosphericReflectance; 
  
  /** The spherical albedo of the atmosphere.*/
  double m_SphericalAlbedo; 

  /**The total gaseous transmission (for all species). */
  double m_TotalGaseousTransmission; 

  /** The downward transmittance. */
  double m_DownwardTransmittance; 

 /** The upward transmittance. */
  double m_UpwardTransmittance; 

};


/** \class AtmosphericRadiativeTerms
 *  \brief This class is a vector of AtmosphericRadiativeTermsSingleChannel, 
 *         it contains all atmospheric radiative terms for each studied channel.
 *
 * \ingroup AtmosphericRadiativeTermSingleChannel
 */


class ITK_EXPORT AtmosphericRadiativeTerms : public itk::DataObject
{
public:
  /** Standard typedefs */
  typedef AtmosphericRadiativeTerms        Self;
  typedef itk::DataObject                  Superclass;
  typedef itk::SmartPointer<Self>          Pointer;
  typedef itk::SmartPointer<const Self>    ConstPointer;

  /** Type macro */
  itkTypeMacro(AtmosphericRadiativeTerms,DataObject);

  /** Creation through object factory macro */
  itkNewMacro(Self);
  
  typedef AtmosphericRadiativeTermsSingleChannel::Pointer   ValueType;
  typedef std::vector<ValueType>                            VectorValueType;
  typedef std::vector<double>                               DataVectorType;  
  /**
   * Set/Get the values.
   */
  void SetValues( const VectorValueType & val) 
    { 
      m_Values = val; 
      this->Modified();
    }; 
  VectorValueType & GetValues() { return m_Values; }; 
  const VectorValueType & GetValues() const { return m_Values; }; 
  
  /** Set/Get the data classified by channel. */
  /** Set methods with vectors. */
  void SetIntrinsicAtmosphericReflectances(const DataVectorType & vect); 
  void SetSphericalAlbedos(const DataVectorType & vect); 
  void SetTotalGaseousTransmissions(const DataVectorType & vect); 
  void SetDownwardTransmittances(const DataVectorType & vect); 
  void SetUpwardTransmittances(const DataVectorType & vect); 
  /** Set methods with index. */
  void SetValueByIndex(unsigned int id, const ValueType & val);
  void SetIntrinsicAtmosphericReflectances(unsigned int id, const double & val); 
  void SetSphericalAlbedos(unsigned int id, const double & val); 
  void SetTotalGaseousTransmissions(unsigned int id, const double & val); 
  void SetDownwardTransmittances(unsigned int id, const double & val ); 
  void SetUpwardTransmittances(unsigned int id, const  double & val ); 

  /** Get methods with vectors. */
  DataVectorType GetIntrinsicAtmosphericReflectances(); 
  DataVectorType GetSphericalAlbedos(); 
  DataVectorType GetTotalGaseousTransmissions(); 
  DataVectorType GetDownwardTransmittances(); 
  DataVectorType GetUpwardTransmittances(); 
 /** Get methods with index. */
  double GetIntrinsicAtmosphericReflectances(unsigned int id); 
  double GetSphericalAlbedos(unsigned int id); 
  double GetTotalGaseousTransmissions(unsigned int id); 
  double GetDownwardTransmittances(unsigned int id); 
  double GetUpwardTransmittances(unsigned int id);

  const ValueType GetValueByIndex(unsigned int id) const;
 
  /** Initialization method.*/
  void ValuesInitialization(unsigned int nbChannel);

protected:
  /** Constructor */
  AtmosphericRadiativeTerms();
  /** Destructor */
  ~AtmosphericRadiativeTerms(){};
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  AtmosphericRadiativeTerms(const Self&) ; //purposely not implemented
  void operator=(const Self&) ; //purposely not implemented

  /** The vector containing each channel information. */
  VectorValueType m_Values; 
  /** Boolean to know if m_Values has been initialized. */
  bool m_IsInitialized; 
};


} // end namespace otb

#endif


