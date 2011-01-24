/*
 * GrsirInverseFilter.h
 * Date:     $Date$
 * Version:  $Revision$
 *
 * Vahine Framework
 * Copyright (C) 2008 to 2010 Ludovic Léau-Mercier and Laboratoire de Planétologie de Grenoble
 * See LICENCE and COPYING for details.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * CeCILLl License for more details and http://www.cecill.info
 *
 */

#ifndef __GRSIRINVERSEFILTER_H_
#define __GRSIRINVERSEFILTER_H_

#include <bitset>
#include <itkImageToImageFilter.h>
#include <itkImageLinearIteratorWithIndex.h>
#include <otb/BasicFilters/otbImageListToVectorImageFilter.h>
#include <otb/Common/otbMultiToMonoChannelExtractROI.h>
#include <otb/Common/otbImageList.h>
#include <otb/IO/otbImage.h>

#include "VahineMacro.h"
#include "VahineInterpol.h"
#include "VahineException.h"
#include "InverseFilter.h"

namespace otb {

template <class TImage>
class ITK_EXPORT VahineGrsirInverseFilter
  : public itk::ImageToImageFilter<TImage, TImage> {
public:

  typedef VahineGrsirInverseFilter                Self;
  typedef itk::ImageToImageFilter<TImage, TImage> Superclass;
  typedef itk::SmartPointer<Self>                 Pointer;
  typedef itk::SmartPointer<const Self>           ConstPointer;

  // creation of SmartPointer
  itkNewMacro(Self);
  // runtime type information
  itkTypeMacro(VahineGrsirInverseFilter, itk::ImageToImageFilter);

  // Types to used for computation
  const static unsigned int Dimension = 2;
  typedef TImage                                                   ImageType;
  typedef typename ImageType::Pointer                              ImagePointer;
  typedef typename Superclass::InputImageConstPointer              ImageConstPointer;
  typedef typename ImageType::InternalPixelType                    InternalPixelType;
  typedef typename itk::NumericTraits<InternalPixelType>::RealType RealType;
  typedef vnl_vector<InternalPixelType>                            VnlVectorType;

  // type for managing learning parameter
  typedef Image<InternalPixelType, Dimension>                               BandType;
  typedef typename BandType::Pointer                                        BandPointer;
  typedef MultiToMonoChannelExtractROI<InternalPixelType,InternalPixelType> ExtractROImonoFilterType;
  typedef typename ExtractROImonoFilterType::Pointer                        ExtractROImonoFilterPointer;
  typedef VahineInverseFilter<ImageType, BandType>                          VahineInverseType;
  typedef typename VahineInverseType::Pointer                               VahineInversePointer;

  typedef itk::ImageLinearIteratorWithIndex<BandType> BandLinearIterator;
  // type for output
  typedef ImageList<BandType>                                            InverseResultListType;
  typedef typename InverseResultListType::Pointer                        InverseResultListPointer;
  typedef ImageListToVectorImageFilter<InverseResultListType, ImageType> ImageListToVectorImageFilterType;
  typedef typename ImageListToVectorImageFilterType::Pointer             ImageListToVectorImageFilterPointer;

  /**
   * 1 : 1 == inverse this parameter
   * 2-3 : regularization parameter
   *      00 : not use
   *      01 : optimal regularization parameter
   *      10 : custom regularization parameter
   *      11 : inverse all regularization parameter
   * 4-7 : inverse one value : index between 0 and 2^4 (16)
   * 8 : not use
   * exemple :
   * 0000 0011 inverse with optimal regularization value
   * 0001 0101 inverse with the value with index 1 (second value)
   * 0000 0111 inverse for all regularization values
   */
  typedef std::bitset<8> BandParameterType;
  const BandParameterType INVERSE_MASK;
  const BandParameterType OPTREGUL_MASK;
  const BandParameterType ALLREGUL_MASK;
  const BandParameterType CUSREGUL_MASK;
  const BandParameterType REGULVALUE_MASK;

  const unsigned int NbEigenVector;

  void PushBackParameterBand(BandParameterType);

  itkSetMacro( LearningParam, ImagePointer );

  bool GetDebug(){
    return true;
  };

  void PrintSelf(std::ostream& os, itk::Indent indent) const {
    Superclass::PrintSelf(os, indent);
  };

  void GenerateOutputInformation();

  void GenerateInputRequestedRegion();

protected:
  VahineGrsirInverseFilter();
  void GenerateData();

private:

  struct EigenStruct {
    VnlVectorType eigenVector;
    vahine::VahineInterpol<InternalPixelType> * interpolation;
    };

  // learning parameters : one band by parameter
  ImagePointer m_LearningParam;
  // result list
  InverseResultListPointer m_ResultList;

  typedef std::list<BandParameterType> ParameterListType;
  typedef ParameterListType::iterator  ItParameterList;
  ParameterListType m_parameter;

  VahineGrsirInverseFilter(const Self&); //not implemented
  void operator=(const Self&);           //not implemented

  inline bool checkParameter(BandParameterType b, BandParameterType mask){
    return ( (b &= mask) == mask);
  }

  unsigned int getNumberOfRegulParameters(BandPointer learnedParameters);

  BandPointer getLearningParameters(unsigned int bandNumber);

  unsigned int getIndexForBestRegul(BandPointer learnedParameters);

  unsigned int getIndexForCustRegul(BandParameterType param);

  unsigned int getNbRegularization(BandPointer learnedParameters);

  VnlVectorType getDataFromLine(BandPointer band, unsigned int, bool);

  void UpdateEigenStruct(BandPointer band, EigenStruct & eigenStruct, unsigned int index);

  void InverseImage(EigenStruct eigenStruct);

};

} // end namespace otb

#include "GrsirInverseFilter.txx"

#endif /* GRSIRINVERSEFILTER_H_ */