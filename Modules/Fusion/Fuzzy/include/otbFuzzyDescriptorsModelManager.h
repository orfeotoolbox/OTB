/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbFuzzyDescriptorsModelManager_h
#define otbFuzzyDescriptorsModelManager_h

#include <vector>
#include <utility>
#include <string>


namespace otb {

/** \class FuzzyDescriptorsModelManager
 *  \brief   Read a xml file where are stored several statistics
 *
 * To get a specific statistic Measurement vector, use the method
 * GetStatisticVectorByName(name) which allow you to get the
 * Measurement vector for the statistic set as parameter.
 *
 *
 *
 * \ingroup OTBFuzzy
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
