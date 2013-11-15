#ifndef __otbSEMClassifier_h
#define __otbSEMClassifier_h

#include "itkListSample.h"
#include "itkSampleClassifierFilter.h"
#include "itkMembershipFunctionBase.h"

#include "otbModelComponentBase.h"

#include "otbGaussianModelComponent.h"

namespace otb
{

/** \class SEMClassifier
   *  \brief This class implements the Stochastic Expectation
   *  Maximization algorithm to perform an estimation of a mixture model.
   *
   * The first template argument is the type of the target sample
   * data. This estimator expects one or more model component objects
   * of the classes derived from the ModelComponentBase. The actual
   * component (or module) parameters are updated by each component.
   * Users can think this class as a strategy or a integration point
   * for the SEM procedure.
   *
   * The number of classes (SetNumberOfClasses), the initial
   * proportion (SetInitialProportions), the input sample (SetSample),
   * the model components (AddComponent), and the maximum iteration
   * (SetMaximumIteration) are required. The SEM procedure terminates
   * when the current iteration reaches the maximum iteration or the model
   * parameters converge.
   *
   * The difference from ExpectationMaximizationMixtureModelEstimator is
   * that SEMClassifier include the maximum a posteriori decision on each
   * sample. The class is to be seen as a classification and not an estimator.
   *
   * <b>Recent API changes:</b>
   * N/A
   *
   * \sa ModelComponentBase, GaussianModelComponent
 */
template<class TInputImage, class TOutputImage>
class ITK_EXPORT SEMClassifier
  : public itk::Statistics::SampleClassifierFilter<
      typename itk::Statistics::ListSample<typename TInputImage::PixelType> >
{
public:
  /** Standard class typedef*/
  typedef SEMClassifier Self;
  typedef itk::Statistics::SampleClassifierFilter<
      typename itk::Statistics::ListSample<typename TInputImage::PixelType> >
  Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macros */
  itkTypeMacro(SEMClassifier, itk::Object);
  itkNewMacro(Self);

  /** TSample template argument related typedefs */
  typedef typename itk::Statistics::ListSample<typename TInputImage::PixelType> SampleType;
  typedef typename itk::Statistics::Subsample<SampleType>                       ClassSampleType;

  typedef typename SampleType::MeasurementType       MeasurementType;
  typedef typename SampleType::MeasurementVectorType MeasurementVectorType;

  /** Type of the segmentation vector */
  typedef unsigned int                ClassLabelType;
  typedef std::vector<ClassLabelType> ClassLabelVectorType;

  /** Output type for GetClassSample method */
  typedef itk::Statistics::MembershipSample<SampleType> OutputType;

  /** Type of the mixture model component base class.
   * Due to the stochastic purpose, the inital list of sample (from TInputImage)
   * is duplicated as many times as the number of classes (into SampleLists). */
  typedef otb::Statistics::ModelComponentBase<ClassSampleType> ComponentType;
  typedef typename ComponentType::Pointer                      ComponentPointerType;

  /** Type of the component pointer storage, one component per class */
  typedef std::vector<ComponentPointerType> ComponentVectorType;

  /** type of the parameters needed for the component vectors */
  typedef itk::Array<double> ParameterType;

  /** Type of the parameter pointer storage, one parameter set per class */
  typedef std::vector<ParameterType> ParameterVectorType;

  /** Type of the vector of the probability values for each sample */
  typedef std::vector<double> ProportionVectorType;
  typedef std::vector<double> ProbaVectorType;

  /** Type of the probability values of each sample per class */
  typedef std::vector<ProbaVectorType> ProbaByClassVectorType;

  /** Sets the target data that will be classified by this */
  void SetSample(const TInputImage * sample);

  /** Returns the target data */
  const TInputImage* GetSample() const;
  SampleType * GetSampleList() const;

  /** Set/Gets the initial proportion values. The size of proportion
   * vector should be same as the number of component (or classes).
   * Choose between SetInitialProportions, SetClassLabels or
   * SetNumberOfComponents */
  void SetInitialProportions(ProportionVectorType& propotion);
  ProportionVectorType* GetInitialProportions();

  /** Gets the result proportion values */
  ProportionVectorType* GetProportions();

  /** Set/Gets the initial segmentation. the size of the vector should be
   * the same as the number of samples (length of MeasurementVector) */
  void SetClassLabels(OutputType * labels);
  void SetClassLabels(TOutputImage * imgLabels);
  ClassLabelVectorType& GetClassLabels();

  /** Sets/Gets the total number of classes currently plugged in. */
  void SetNumberOfClasses(int theSize);
  unsigned int GetNumberOfClasses();

  /** Set/Gets the maximum number of iterations. When the optimization
   * process reaches the maximum number of interations, even if the
   * class parameters aren't converged, the optimization process
   * stops. */
  itkSetMacro(MaximumIteration, int);
  itkGetMacro(MaximumIteration, int);

  /** Set/Gets the neighborhood to take into consideration
   * in the contextual decision rule. Should be odd (def. 3). */
  void SetNeighborhood(int neighborhood);
  int GetNeighborhood();

  /** Gets the current iteration. */
  int GetCurrentIteration();

  /** Adds a new component (or class). Has to be called after SetNumberOfClasses */
  int AddComponent(int id, ComponentType* component);

  /** Runs the optimization process. */
  void Update();

  /** Termination status after running optimization */
  typedef enum { CONVERGED = 0, NOT_CONVERGED = 1 } TerminationCodeType;

  /** Set/Get the termination threshold (ratio of the number of sample that
   * change affected class during interation over the total number of
   * samples (def is 1E-5) */
  itkSetMacro(TerminationThreshold, double);
  itkGetMacro(TerminationThreshold, double);

  /** Gets the termination status */
  itkGetMacro(TerminationCode, TerminationCodeType);

  /* Return the classification result (as a standard classification result) */
  OutputType * GetOutput();

  /* Return the classification result (as an image) */
  TOutputImage * GetOutputImage();

protected:
  SEMClassifier();
  virtual ~SEMClassifier() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Initialize the first segmentation, either randomly or by using
   *  a ClassLabelVectorType given in SetClassLabels. */
  void InitParameters();
  /** Stochastic part of the SEM */
  void PerformStochasticProcess();
  /** Estimation part of the SEM */
  void PerformExpectationProcess();
  /** Maximization part of the SEM.
   * This method should be upgraded this a contextual point of view...
   * It required to a Neighborhood knowledge into the TSample type... */
  void PerformMaximizationProcess();
  /** Make Decision through a Maximum a posteriori */
  void GetMaximumAposterioriLabels();

private:
  /** Target data sample pointer*/
  const TInputImage* m_Sample;
  int                m_NbSamples;
  typename SampleType::Pointer m_SampleList;

  int    m_NbClasses;
  int    m_MaximumIteration;
  int    m_CurrentIteration;
  int    m_NbChange;
  double m_TerminationThreshold;
  int    m_Neighborhood;

  TerminationCodeType    m_TerminationCode;
  ComponentVectorType    m_ComponentVector;
  ProportionVectorType   m_InitialProportions;
  ProportionVectorType   m_Proportions;
  ProbaByClassVectorType m_Proba;
  ClassLabelVectorType   m_ClassLabels;

  int m_ExternalLabels;
  int m_ComponentDeclared;

  typename TOutputImage::Pointer m_OutputImage;
  typename OutputType::Pointer m_Output;
}; // end of class

} // end of namespace

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSEMClassifier.txx"
#endif

#endif
