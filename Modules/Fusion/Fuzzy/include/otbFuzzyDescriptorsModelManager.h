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
#ifndef __otbFuzzyDescriptorsModelManager_h
#define __otbFuzzyDescriptorsModelManager_h

#include <vector>
#include <utility>
#include <string>


namespace otb {

/** \class FuzzyDescriptorsModelManager
 *  \brief   Read a xml file where are stored several statistics
 *
 * To get a specific statistic Measurement vector, use the method
 * GetStatisticVectorByName(name) which allow you to get the
 * Measurement vector for the statistic set as paramater.
 *
 *
 */
  class  FuzzyDescriptorsModelManager
{
public:
  /** Standard class typedefs */
  typedef FuzzyDescriptorsModelManager     Self;

  /** Convenient typedef */
  typedef std::vector<double>                    ParameterType;
  typedef std::pair<std::string, ParameterType > PairType;
  typedef std::vector< PairType >                DescriptorsModelType;
  typedef std::vector< std::string >             DescriptorListType;

  /** Method to get descriptors */
  static PairType GetDescriptor(const char * model, const DescriptorsModelType & descModel);
  /** Method to get descriptor List */
  static DescriptorListType GetDescriptorList(const DescriptorsModelType & descModel);
  /** Method to set descriptors */
  static void AddDescriptor( PairType modelPair, DescriptorsModelType & descModel );
  static void AddDescriptor( std::string key, ParameterType model, DescriptorsModelType & descModel );
  /** Print a model */
  static void Print( const DescriptorsModelType & descModel );
  /** Read a model from a xml file */
  static DescriptorsModelType Read(const std::string & filename);
  /** Write a model in a xml file */
  static void Save(const std::string & filename, DescriptorsModelType & model );

  FuzzyDescriptorsModelManager();
  virtual ~FuzzyDescriptorsModelManager() {}


private:

}; // end of class FuzzyDescriptorsModelManager

} // end of namespace otb

#endif
