#ifndef __otbSEMClassifier_txx
#define __otbSEMClassifier_txx

#include <cstdlib>

#include <iostream>
#include "itkNumericTraits.h"
#include "itkImageIterator.h"
#include "itkImageRegionIterator.h"

#include "otbMacro.h"
// default mixture model
#include "otbGaussianModelComponent.h"

#include "otbSEMClassifier.h"

namespace otb
{

template<class TInputImage, class TOutputImage>
SEMClassifier<TInputImage, TOutputImage>
::SEMClassifier()
{
  m_TerminationCode = NOT_CONVERGED;
  m_ExternalLabels = 0;
  m_ComponentDeclared = 0;
  m_Sample = 0;
  m_NbSamples = 0;
  m_SampleList = 0;
  m_NbChange = 0;
  m_NbClasses = 0;
  m_TerminationThreshold = 1E-5;
  m_Neighborhood = 1;

  m_OutputImage = 0;
  m_Output = 0;

}

template<class TInputImage, class TOutputImage>
void
SEMClassifier<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  for (int componentIndex = 0; componentIndex < m_NbClasses; ++componentIndex)
    {
    os << indent << "Component num " << componentIndex;
    os << " (prop " << m_Proportions[componentIndex] << ") ";
    m_ComponentVector[componentIndex]->ShowParameters(os, indent);
    }
}

template<class TInputImage, class TOutputImage>
void
SEMClassifier<TInputImage, TOutputImage>
::SetNeighborhood(int neighborhood)
{
  m_Neighborhood = 2 * (neighborhood / 2) + 1;
  if (m_Neighborhood < 1) m_Neighborhood = 1;
}

template<class TInputImage, class TOutputImage>
int
SEMClassifier<TInputImage, TOutputImage>
::GetNeighborhood()
{
  return m_Neighborhood;
}

template<class TInputImage, class TOutputImage>
void
SEMClassifier<TInputImage, TOutputImage>
::SetInitialProportions(ProportionVectorType& proportions)
{
  m_InitialProportions = proportions;
  m_ExternalLabels = 0;
}

template<class TInputImage, class TOutputImage>
typename SEMClassifier<TInputImage, TOutputImage>::ProportionVectorType*
SEMClassifier<TInputImage, TOutputImage>
::GetInitialProportions()
{
  return m_InitialProportions;
}

template<class TInputImage, class TOutputImage>
void
SEMClassifier<TInputImage, TOutputImage>
::SetClassLabels(OutputType * labels)
{
  if (m_Sample == NULL)
    {
    m_ClassLabels.resize(labels->Size());

    ClassLabelVectorType::iterator          iterClassLabel = m_ClassLabels.begin();
    typename OutputType::ConstIterator      iterLabels = labels->Begin();
    typename OutputType::InstanceIdentifier id = 0;

    do
      {
      *iterClassLabel = iterLabels->GetClassLabel(id);
      ++iterLabels;
      ++iterClassLabel;
      id++;
      }
    while (iterLabels != labels->End());
    m_ExternalLabels = 1;
    }
  else if (m_ClassLabels.size() == m_NbSamples) //FIXME check if this
                                                //is really the right condition
    {
    m_ClassLabels.resize(labels->Size());

    ClassLabelVectorType::iterator          iterClassLabel = m_ClassLabels.begin();
    typename OutputType::iterator           iterLabels = labels->Begin();
    typename OutputType::InstanceIdentifier id = 0;

    do
      {
      *iterClassLabel = iterLabels->GetClassLabel(id);
      ++iterLabels;
      ++iterClassLabel;
      id++;
      }
    while (iterLabels != labels->End());
    m_ExternalLabels = 1;
    }
  else
    {
    otbMsgDebugMacro(<< "m_ClassLabels size = " << GetClassLabels().size() << " / m_Sample size = " << m_NbSamples);
    throw itk::ExceptionObject(__FILE__, __LINE__, "Vector size missmatch", ITK_LOCATION);
    }
}

template<class TInputImage, class TOutputImage>
void
SEMClassifier<TInputImage, TOutputImage>
::SetClassLabels(TOutputImage * imgLabels)
{
  otbMsgDebugMacro(<< "Initalizing segmentation from an external image");

  typename TInputImage::SizeType size = imgLabels->GetBufferedRegion().GetSize();
  int                            theSize = (int) size[0] * size[1];

  if (m_Sample == NULL)
    {
    m_ClassLabels.resize(theSize);

    typename itk::ImageRegionIterator<TOutputImage> imgLabelIter(imgLabels,
                                                                 imgLabels->GetBufferedRegion());
    imgLabelIter.GoToBegin();
    typename itk::ImageRegionIterator<TOutputImage> imgLabelIterEnd(imgLabels,
                                                                    imgLabels->GetBufferedRegion());
    imgLabelIterEnd.GoToEnd();

    ClassLabelVectorType::iterator iterClassLabel = m_ClassLabels.begin();

    do
      {
      *iterClassLabel = imgLabelIter.Get();
      ++imgLabelIter;
      ++iterClassLabel;
      }
    while (imgLabelIter != imgLabelIterEnd);
    m_ExternalLabels = 1;
    }
  else if (theSize == m_NbSamples)
    {
    m_ClassLabels.resize(theSize);

    typename itk::ImageRegionIterator<TOutputImage> imgLabelIter(imgLabels,
                                                                 imgLabels->GetBufferedRegion());
    imgLabelIter.GoToBegin();
    typename itk::ImageRegionIterator<TOutputImage> imgLabelIterEnd(imgLabels,
                                                                    imgLabels->GetBufferedRegion());
    imgLabelIterEnd.GoToEnd();

    ClassLabelVectorType::iterator iterClassLabel = m_ClassLabels.begin();

    do
      {
      *iterClassLabel = imgLabelIter.Get();
      ++imgLabelIter;
      ++iterClassLabel;
      }
    while (imgLabelIter != imgLabelIterEnd);
    m_ExternalLabels = 1;

    }
  else
    {
    otbMsgDebugMacro(
      << "m_ClassLabels size = " << GetClassLabels().size() << " size of the image = " << theSize <<
      " / m_Sample size = " << m_NbSamples);
    throw itk::ExceptionObject(__FILE__, __LINE__, "Vector size missmatch", ITK_LOCATION);
    }
}

template <class TInputImage, class TOutputImage>
typename SEMClassifier<TInputImage, TOutputImage>::ClassLabelVectorType&
SEMClassifier<TInputImage, TOutputImage>
::GetClassLabels()
{
  return m_ClassLabels;
}

template<class TInputImage, class TOutputImage>
typename SEMClassifier<TInputImage, TOutputImage>::ProportionVectorType*
SEMClassifier<TInputImage, TOutputImage>
::GetProportions()
{
  return &m_Proportions;
}

template<class TInputImage, class TOutputImage>
void
SEMClassifier<TInputImage, TOutputImage>
::SetSample(const TInputImage* sample)
{
  m_Sample = sample;
  m_NbSamples = 0;
  m_SampleList = SampleType::New();
  m_SampleList->SetMeasurementVectorSize(m_Sample->GetVectorLength());

  itk::ImageRegionIterator<TInputImage> imgIter((TInputImage *) m_Sample,
                                                m_Sample->GetBufferedRegion());
  imgIter.GoToBegin();
  itk::ImageRegionIterator<TInputImage> imgIterEnd((TInputImage *) m_Sample,
                                                   m_Sample->GetBufferedRegion());
  imgIterEnd.GoToEnd();

  do
    {
    m_SampleList->PushBack(imgIter.Get());
    ++m_NbSamples;
    ++imgIter;
    }
  while (imgIter != imgIterEnd);

  if (m_ExternalLabels)
    {
    typename TInputImage::SizeType size = m_Sample->GetBufferedRegion().GetSize();
    if ((size[0] * size[1]) != m_ClassLabels.size())
      throw itk::ExceptionObject(__FILE__, __LINE__,
                                 "Vector size missmatch", ITK_LOCATION);
    }
}

template<class TInputImage, class TOutputImage>
const TInputImage *
SEMClassifier<TInputImage, TOutputImage>
::GetSample() const
{
  return m_Sample;
}

template<class TInputImage, class TOutputImage>
typename SEMClassifier<TInputImage, TOutputImage>::SampleType *
SEMClassifier<TInputImage, TOutputImage>
::GetSampleList() const
{
  return m_SampleList;
}

template<class TInputImage, class TOutputImage>
int
SEMClassifier<TInputImage, TOutputImage>
::GetCurrentIteration()
{
  return m_CurrentIteration;
}

template<class TInputImage, class TOutputImage>
int
SEMClassifier<TInputImage, TOutputImage>
::AddComponent(int id, ComponentType * component)
{
  m_ComponentVector[id] = component;
  m_ComponentDeclared = 1;

  return static_cast<int>(m_ComponentVector.size());
}

template<class TInputImage, class TOutputImage>
unsigned int
SEMClassifier<TInputImage, TOutputImage>
::GetNumberOfClasses()
{
  return m_NbClasses;
}

template <class TInputImage, class TOutputImage>
void
SEMClassifier<TInputImage, TOutputImage>
::SetNumberOfClasses(int theNumberOfClasses)
{
  m_NbClasses = theNumberOfClasses;
  m_ComponentVector.resize(theNumberOfClasses);

  if (m_ComponentDeclared == 1)
    otbMsgDebugMacro(
      << "Previous component declarations will be lost since called before SetNumberOfClasses");

  m_ComponentDeclared = 0;
}

template <class TInputImage, class TOutputImage>
TOutputImage *
SEMClassifier<TInputImage, TOutputImage>
::GetOutputImage()
{
  return m_OutputImage;
}

template <class TInputImage, class TOutputImage>
typename SEMClassifier<TInputImage, TOutputImage>::OutputType *
SEMClassifier<TInputImage, TOutputImage>
::GetOutput()
{
  return m_Output;
}

template <class TInputImage, class TOutputImage>
void
SEMClassifier<TInputImage, TOutputImage>
::InitParameters()
{
  if (!m_ExternalLabels)
    {
    m_ClassLabels.resize(m_NbSamples);
    if (static_cast<int>(m_InitialProportions.size()) != m_NbClasses)
      {
      int label;
      for (typename ClassLabelVectorType::iterator labelIter = m_ClassLabels.begin();
           labelIter != m_ClassLabels.end();
           ++labelIter)
        {
        //label = (int) floor( 0.5 + nbClassesDbl * ran / double(RAND_MAX+1) );
        label = rand() % m_NbClasses;
        if (label < 0) label = 0;
        else if (label >= m_NbClasses) label = m_NbClasses - 1;
        *labelIter = label;
        }
      }
    else
      {
      // Be sure, the sum of intial proportion remains to 1
      double                                  sumProportion = 0.0;
      typename ProportionVectorType::iterator iterProportion = m_InitialProportions.begin();
      do
        {
        sumProportion += *iterProportion;
        }
      while (++iterProportion != m_InitialProportions.end());

      if (sumProportion != 1.0)
        {
        for (iterProportion = m_InitialProportions.begin();
             iterProportion != m_InitialProportions.end();
             ++iterProportion)
          *iterProportion /= sumProportion;
        }

      // non uniform random sampling according to m_InitialProportions
      double sample;
      double cumulativeProportion;
      for (typename ClassLabelVectorType::iterator labelIter = m_ClassLabels.begin();
           labelIter != m_ClassLabels.end();
           ++labelIter)
        {
        cumulativeProportion = 0.0;
        sample = double(rand()) / (double(RAND_MAX) + 1.0);

        *labelIter = m_NbClasses - 1;
        for (int componentIndex = 0; componentIndex < m_NbClasses; ++componentIndex)
          {
          if (cumulativeProportion <= sample
              && sample < cumulativeProportion + m_InitialProportions[componentIndex])
            {
            *labelIter = componentIndex;
            break;
            }
          else cumulativeProportion += m_InitialProportions[componentIndex];
          }
        }
      }
    }

  m_Proportions.resize(m_NbClasses);
  m_Proba.resize(m_NbClasses);
  for (int i = 0; i < m_NbClasses; ++i)
    m_Proba[i].resize(m_NbSamples);

  if (!m_ComponentDeclared)
    {
    otbMsgDebugMacro(<< "default mixture initialization with " << m_NbClasses
                     << " Gaussian components");
    typedef otb::Statistics::GaussianModelComponent<ClassSampleType> GaussianType;

    for (int componentIndex = 0; componentIndex < m_NbClasses; ++componentIndex)
      {
      typename GaussianType::Pointer comp = GaussianType::New();
      AddComponent(componentIndex, comp);
      }
    }

  otbMsgDevMacro(<< "num class   : " << m_NbClasses);
  otbMsgDevMacro(<< "num sample : " << GetSampleList()->Size());
  otbMsgDevMacro(<< "num labels : " << GetClassLabels().size());
  otbMsgDevMacro(<< "contextual neighborhood : " << m_Neighborhood);
  otbMsgDevMacro(<< "terminationThreshold    : " << m_TerminationThreshold);

}

template <class TInputImage, class TOutputImage>
void
SEMClassifier<TInputImage, TOutputImage>
::PerformStochasticProcess()
{
  double x, y, z;
  m_NbChange = 0;

  int posSample = 0;
  for (typename ClassLabelVectorType::iterator iter = m_ClassLabels.begin();
       iter != m_ClassLabels.end();
       ++iter)
    {
    x = double(rand()) / (double(RAND_MAX) + 1.0);
    z = 0.0;

    for (int componentIndex = 0; componentIndex < m_NbClasses; ++componentIndex)
      {
      y = z;
      z += m_Proba[componentIndex][posSample];

      if ((y < x) && (x <= z))
        {
        if (static_cast<unsigned int>(componentIndex) != *iter) m_NbChange++;

        *iter = componentIndex;
        break;
        }
      }
    posSample++;
    }

  switch (GetCurrentIteration())
    {
    case 0:
    case 1:
      otbMsgDebugMacro(<< "Doing iteration " << GetCurrentIteration());
      break;
    default:
      otbMsgDebugMacro(<< m_NbChange << " sample change at iteration "
                       << GetCurrentIteration());
    }
}

template<class TInputImage, class TOutputImage>
void
SEMClassifier<TInputImage, TOutputImage>
::PerformExpectationProcess()
{
  int componentIndex;
  for (componentIndex = 0; componentIndex < m_NbClasses; ++componentIndex)
    m_Proportions[componentIndex] = 0.0;

  std::vector<typename ClassSampleType::Pointer> coeffByClass;

  for (componentIndex = 0; componentIndex < m_NbClasses; ++componentIndex)
    {
    coeffByClass.push_back(ClassSampleType::New());
    coeffByClass[componentIndex]->SetMeasurementVectorSize(
      m_SampleList->GetMeasurementVectorSize());
    coeffByClass[componentIndex]->SetSample(m_SampleList);
    }

  typename SampleType::ConstIterator iterSample = m_SampleList->Begin();
  typename SampleType::ConstIterator lastSample = m_SampleList->End();

  ClassLabelVectorType::iterator iterLabel = m_ClassLabels.begin();
  ClassLabelVectorType::iterator lastLabel = m_ClassLabels.end();

  typename SampleType::InstanceIdentifier id = 0;

  do
    {
    coeffByClass[*iterLabel]->AddInstance(id);
    m_Proportions[*iterLabel] += 1.0;
    id++;
    }
  while (++iterSample != lastSample && ++iterLabel != lastLabel);

  for (componentIndex = 0; componentIndex < m_NbClasses; ++componentIndex)
    {
    if (m_Proportions[componentIndex] == 0.0)
      {
      std::cerr << "No sample on class " << componentIndex;
//      std::cerr << " in " << __PRETTY_FUNCTION__ << std::endl;
      std::cerr << " in " << ITK_LOCATION << std::endl;
      continue;
      }

    m_ComponentVector[componentIndex]->SetSample(coeffByClass[componentIndex]);
    m_ComponentVector[componentIndex]->Update();
    }

  for (componentIndex = 0; componentIndex < m_NbClasses; ++componentIndex)
    m_Proportions[componentIndex] /= static_cast<double> (m_NbSamples);

}

template <class TInputImage, class TOutputImage>
void
SEMClassifier<TInputImage, TOutputImage>
::PerformMaximizationProcess()
{
  int    i, j, a, b;
  int    voisinage = m_Neighborhood / 2;
  int    componentIndex;
  double sumPdf, aPdf;
  double neighborhoodWeight = (double) m_Neighborhood * m_Neighborhood;

  int                            line, cols;
  typename TInputImage::SizeType size = m_Sample->GetBufferedRegion().GetSize();
  cols = (int) size[0];
  line = (int) size[1];

  std::vector<double> pdf(m_NbClasses);
  std::vector<double> localWeight(m_NbClasses);
  std::vector<double> localCount(m_NbClasses);

  typename SampleType::ConstIterator iterSample = m_SampleList->Begin();
  typename SampleType::ConstIterator lastSample = m_SampleList->End();
  MeasurementVectorType              measurementVector;

  typename SampleType::InstanceIdentifier id = 0;

  do
    {
    id = iterSample.GetInstanceIdentifier();

    for (componentIndex = 0; componentIndex < m_NbClasses; ++componentIndex)
      localCount[componentIndex] = 0.0;

    i = id / cols;
    j = id % cols;

    for (a = (i - voisinage); a <= (i + voisinage); a++)
      for (b = (j - voisinage); b <= (j + voisinage); b++)
        {
        if (a < 0 || a >= line) continue;

        if (b < 0 || b >= cols) continue;

        localCount[m_ClassLabels[a * cols + b]] += 1.0;
        }

    for (componentIndex = 0; componentIndex < m_NbClasses; ++componentIndex)
      localWeight[componentIndex] = localCount[componentIndex]
                                    / neighborhoodWeight;

    sumPdf = 0.0;
    for (componentIndex = 0; componentIndex < m_NbClasses; ++componentIndex)
      {
      measurementVector = iterSample.GetMeasurementVector();
      aPdf = localWeight[componentIndex]
             * m_ComponentVector[componentIndex]->Pdf(measurementVector);
      sumPdf += aPdf;
      pdf[componentIndex] = aPdf;
      }

    for (componentIndex = 0; componentIndex < m_NbClasses; ++componentIndex)
      {
      if (sumPdf == 0.0) m_Proba[componentIndex][iterSample.GetInstanceIdentifier()] = 0.0;
      else
        m_Proba[componentIndex][iterSample.GetInstanceIdentifier()]
          = pdf[componentIndex] / sumPdf;
      }

    }
  while (++iterSample != lastSample);
}

template <class TInputImage, class TOutputImage>
void
SEMClassifier<TInputImage, TOutputImage>
::GetMaximumAposterioriLabels()
{
  // Class results initialization
  m_Output = OutputType::New();
  m_Output->SetSample(this->GetSampleList());
//  m_Output->Resize(this->GetSampleList()->Size()); //FIXME check if
//  still necessary
  m_Output->SetNumberOfClasses(m_NbClasses);

  // Image results classification
  m_OutputImage = TOutputImage::New();
  m_OutputImage->SetRegions(GetSample()->GetBufferedRegion());
  m_OutputImage->Allocate();

  int cluster, componentIndex;

  typename SampleType::ConstIterator sampleIter = this->GetSampleList()->Begin();
  typename SampleType::ConstIterator sampleIterEnd  = this->GetSampleList()->End();

  typename OutputType::ConstIterator outputIter = m_Output->Begin();
  typename OutputType::ConstIterator outputIterEnd  = m_Output->End();

  typename itk::ImageRegionIterator<TOutputImage> imgOutputIter(m_OutputImage,
                                                                m_OutputImage->GetBufferedRegion());
  imgOutputIter.GoToBegin();
  typename itk::ImageRegionIterator<TOutputImage> imgOutputIterEnd(m_OutputImage,
                                                                   m_OutputImage->GetBufferedRegion());
  imgOutputIterEnd.GoToEnd();

  do
    {
    cluster = 0;
    for (componentIndex = 1; componentIndex < m_NbClasses; ++componentIndex)
      {
      if (m_Proba[componentIndex][sampleIter.GetInstanceIdentifier()]
          > m_Proba[cluster][sampleIter.GetInstanceIdentifier()]) cluster = componentIndex;
      }

    m_Output->AddInstance(cluster, sampleIter.GetInstanceIdentifier());
    imgOutputIter.Set(cluster);

    }
  while (++sampleIter != sampleIterEnd
         && ++outputIter != outputIterEnd
         && ++imgOutputIter != imgOutputIterEnd);
}

template<class TInputImage, class TOutputImage>
void
SEMClassifier<TInputImage, TOutputImage>
::Update()
{

  InitParameters();

  m_CurrentIteration = 0;
  m_TerminationCode = NOT_CONVERGED;

  int    oldNbChange = 0;
  double step;

  do
    {
    oldNbChange = m_NbChange;

    PerformStochasticProcess();
    PerformExpectationProcess();
    PerformMaximizationProcess();

    step = static_cast<double>(oldNbChange - m_NbChange);
    if (step >= 0.0)
      {
      if ((step / static_cast<double>(m_NbSamples))
          < GetTerminationThreshold())
        {
        m_TerminationCode = CONVERGED;
        if (oldNbChange != 0) break;
        }
      }
    }
  while (++m_CurrentIteration < m_MaximumIteration);

  GetMaximumAposterioriLabels();
}

} // end of namesapce otb

#endif
