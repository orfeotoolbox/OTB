#ifndef _LIBSVM_H
#define _LIBSVM_H

//OTB's modifications
//namespace otb
//{
class GenericKernelFunctorBase;
class ComposedKernelFunctor;
//}

#include <map>
#include <vector>
#include "otbMacro.h"

#ifdef __cplusplus
extern "C" {
#endif

struct svm_node
{
	int index;
	double value;
};

struct svm_problem
{
	int l;
	double *y;
	struct svm_node **x;
};

enum { C_SVC, NU_SVC, ONE_CLASS, EPSILON_SVR, NU_SVR };	/* svm_type */
//OTB's modifications
enum { LINEAR, POLY, RBF, SIGMOID, PRECOMPUTED, GENERIC, COMPOSED }; /* kernel_type */

struct svm_parameter
{
  int svm_type;
  int kernel_type;
  int degree;	   /* for poly */
  double gamma;	   /* for poly/rbf/sigmoid */
  double coef0;	   /* for poly/sigmoid */
  double const_coef; /* for otbSVMKernels.h. */
  double lin_coef;   /* for otbSVMKernels.h. */
  char custom[500];  /* for user supplied kernel */
  //OTB's modifications : Use by the generic kernel
  /*otb::*/GenericKernelFunctorBase * kernel_generic;
  // Composed kernel
  ComposedKernelFunctor * kernel_composed;

  /* these are for training only */
  double cache_size; /* in MB */
  double eps;	/* stopping criteria */
  double C;	/* for C_SVC, EPSILON_SVR and NU_SVR */
  int nr_weight;		/* for C_SVC */
  int *weight_label;	/* for C_SVC */
  double* weight;		/* for C_SVC */
  double nu;	/* for NU_SVC, ONE_CLASS, and NU_SVR */
  double p;	/* for EPSILON_SVR */
  int shrinking;	/* use the shrinking heuristics */
  int probability; /* do probability estimates */
};

//
// svm_model
//
struct svm_model
{
  svm_parameter param;	// parameter
  int nr_class;		// number of classes, = 2 in regression/one class svm
  int l;		// total #SV
  svm_node **SV;	// SVs (SV[l])
  double **sv_coef;	// coefficients for SVs in decision functions (sv_coef[k-1][l])
  double *rho;		// constants in decision functions (rho[k*(k-1)/2])
  double *probA;        // pariwise probability information
  double *probB;

  // for classification only

  int *label;		// label of each class (label[k])
  int *nSV;		// number of SVs for each class (nSV[k])
  // nSV[0] + nSV[1] + ... + nSV[k-1] = l
  // XXX
  int free_sv;		// 1 if svm_model is created by svm_load_model
  bool delete_composed; // to know if the composed functor was set using load method
  // 0 if svm_model is created by svm_train
};

struct svm_model *svm_train(const struct svm_problem *prob, const struct svm_parameter *param);
void svm_cross_validation(const struct svm_problem *prob, const struct svm_parameter *param, int nr_fold, double *target);

int svm_save_model(const char *model_file_name, const struct svm_model *model);

int svm_get_svm_type(const struct svm_model *model);
int svm_get_nr_class(const struct svm_model *model);
void svm_get_labels(const struct svm_model *model, int *label);
double svm_get_svr_probability(const struct svm_model *model);

void svm_predict_values(const struct svm_model *model, const struct svm_node *x, double* dec_values);
double svm_predict(const struct svm_model *model, const struct svm_node *x);
double svm_predict_probability(const struct svm_model *model, const struct svm_node *x, double* prob_estimates);

void svm_destroy_model(struct svm_model *model);
void svm_destroy_param(struct svm_parameter *param);

const char *svm_check_parameter(const struct svm_problem *prob, const struct svm_parameter *param);
int svm_check_probability_model(const struct svm_model *model);

//OTB's modifications
struct svm_model *svm_load_model(const char *model_file_name, /*otb::*/GenericKernelFunctorBase * generic_kernel_functor = NULL);
struct svm_model *svm_copy_model( const svm_model *model );

#ifdef __cplusplus
}
#endif


/*
namespace otb
{
*/
//OTB's modifications
/** \class GenericKernelFunctorBase
 * \brief Undocumented
 */
class GenericKernelFunctorBase
{
public:
  GenericKernelFunctorBase() : m_Name("FunctorName") {};

  /** Recopy constructor */
  GenericKernelFunctorBase( const GenericKernelFunctorBase& copy);
  virtual GenericKernelFunctorBase& operator=(const GenericKernelFunctorBase& copy);

  virtual ~GenericKernelFunctorBase() {};

  typedef GenericKernelFunctorBase          Superclass;
  typedef std::map<std::string,std::string> MapType;
  typedef MapType::iterator                 MapIterator;
  typedef MapType::const_iterator           MapConstIterator;

  template<class T>
  T GetValue(const char *option) const
  {
        std::string Value = m_MapParameters.find(std::string(option))->second;
        T lValeur;
        ::otb::StringStream flux;
        flux << Value;
        flux >> lValeur;
        return lValeur;
  }
  template<class T>
  void SetValue(const char *option, const T & value)
  {
        std::string lValeur;
        ::otb::StringStream flux;
        flux << value;
        flux >> lValeur;
        m_MapParameters[std::string(option)] = lValeur;
  }

  /* FOR VISUAL 6 COMPILATION (visual 6 doesn't consider template)******************
#define otbGetValueMacro(name,type)                                                 \
  virtual type GetValue##name (const char *option) const                            \
  {                                                                                 \
        std::string Value = m_MapParameters.find(std::string(option))->second;      \
        type lValeur;                                                               \
        ::otb::StringStream flux;                                                   \
        flux << Value;                                                              \
        flux >> lValeur;                                                            \
        return lValeur;                                                             \
  }

  otbGetValueMacro(String,std::string);
  otbGetValueMacro(Short,short);
  otbGetValueMacro(UShort,unsigned short);
  otbGetValueMacro(Int,int);
  otbGetValueMacro(UInt,unsigned int);
  otbGetValueMacro(Long,long);
  otbGetValueMacro(ULong,unsigned long);
  otbGetValueMacro(Float,float);
  otbGetValueMacro(Double,double);

#define otbSetValueMacro(name,type)                                                 \
  virtual void SetValue##name (const char *option, const type & value)              \
  {                                                                                 \
        std::string lValeur;                                                        \
        ::otb::StringStream flux;                                                   \
        flux << value;                                                              \
        flux >> lValeur;                                                            \
        m_MapParameters[std::string(option)] = lValeur;\
  }

  otbSetValueMacro(String,std::string);
  otbSetValueMacro(Short,short);
  otbSetValueMacro(UShort,unsigned short);
  otbSetValueMacro(Int,int);
  otbSetValueMacro(UInt,unsigned int);
  otbSetValueMacro(Long,long);
  otbSetValueMacro(ULong,unsigned long);
  otbSetValueMacro(Float,float);
  otbSetValueMacro(Double,double);
*/

  virtual double operator()(const svm_node *x, const svm_node *y, const svm_parameter& param)const
    {
      itkGenericExceptionMacro(<<"Kernel functor not definied (Null)");
      return static_cast<double>(0.);
    }

  /** Used for Taylor classification*/
  // degree is the developement degree
  // index is the current value
  // isAtEnd to indicate that it's the last possible derivation
  // baseValue is the constant of the formula
  virtual double derivative(const svm_node *x, const svm_node *y, const svm_parameter& param, int degree, int index, bool isAtEnd, double constValue)const
    {
      itkGenericExceptionMacro(<<"derivative method not definied (Null)");
      return 0.;
    }

  virtual int load_parameters(FILE ** pfile);

  virtual int save_parameters(FILE ** pfile, const char * generic_kernel_parameters_keyword)const;

  virtual void print_parameters(void)const;

  virtual void Update(void){ }

  virtual double dot(const svm_node *px, const svm_node *py)const;

  virtual svm_node * sub(const svm_node *px, const svm_node *py)const;

  virtual svm_node * add(const svm_node *px, const svm_node *py)const;

  virtual void SetName(std::string name ) { m_Name = name;}
  virtual std::string GetName(void) { return m_Name;}
  virtual const std::string GetName(void) const { return m_Name;}

  virtual void SetMapParameters(const MapType & map){ m_MapParameters = map; };
  virtual const MapType & GetMapParameters()const { return m_MapParameters; };
  virtual MapType GetMapParameters(){ return m_MapParameters; };

private:

    /** Kernel functor parameters */
    MapType m_MapParameters;

    /** Functor label name (without space) */
    std::string m_Name;
};


/** \class ComposedKernelFunctor
 * \brief Undocumented
 */
class ComposedKernelFunctor : public GenericKernelFunctorBase
{
public:
  ComposedKernelFunctor()
    {
      this->SetName("ComposedFunctorName");
      this->SetValue<bool>("MultiplyKernelFunctor", false);
    };
  virtual ~ComposedKernelFunctor()
    {
      for(unsigned int i=0; i<m_HaveToBeDeletedList.size(); i++)
	{
	  for(unsigned int j=0; j<m_KernelFunctorList.size(); j++)
	    {
	      if(m_KernelFunctorList[j] == m_HaveToBeDeletedList[i])
		{
		  delete m_KernelFunctorList[j];
		  m_HaveToBeDeletedList[i] = NULL;
		}
	    }
	}
    };
 /** Recopy constructor */
  ComposedKernelFunctor( const ComposedKernelFunctor& copy );
  /* ComposedKernelFunctor( const ComposedKernelFunctor& c ) : GenericKernelFunctorBase(c),  */
/*                                                            m_KernelFunctorList(c.m_KernelFunctorList) */
/*                                                            m_HaveToBeDeletedList(c.m_HaveToBeDeletedList) */
/*                                                            m_PonderationList(c.m_PonderationList)         {}; */
  ComposedKernelFunctor& operator=(const ComposedKernelFunctor& copy);

  typedef std::vector<GenericKernelFunctorBase *> KernelListType;

  virtual double operator()(const svm_node *x, const svm_node *y, const svm_parameter& param)const // = 0
    {
      double out = 0.;
      if (m_KernelFunctorList.size() != 0 && m_PonderationList.size() != 0 && m_KernelFunctorList.size() == m_PonderationList.size())
	{
	  for (unsigned int i = 0; i<m_KernelFunctorList.size(); i++)
	    {
	      if ((this->GetValue<bool>("MultiplyKernelFunctor")) == false)
		{
		  out += m_PonderationList[i]*(*m_KernelFunctorList[i])(x, y, param);
		}
	      else
		{
		  out *= (*m_KernelFunctorList[i])(x, y, param);
		}
	    }
	}
      else
	{
	  itkGenericExceptionMacro(<<"ComposedKernelFunctor::operator() : lists dimensions mismatch");
	}
      return out;
    }

  /** Used for Taylor classification*/
  // degree is the developement degree
  // index is the current value
  // isAtEnd to indicate that it's the last possible derivation
  // baseValue is the constant of the formula
  virtual double derivative(const svm_node *x, const svm_node *y, const svm_parameter& param, int degree, int index, bool isAtEnd, double constValue)const
    {
     double out = 0.;
      if (m_KernelFunctorList.size() != 0 && m_PonderationList.size() != 0 && m_KernelFunctorList.size() == m_PonderationList.size())
	{
	  for (unsigned int i = 0; i<m_KernelFunctorList.size(); i++)
	    {
	      if ((this->GetValue<bool>("MultiplyKernelFunctor")) == false)
		{
		  out += m_PonderationList[i]*(m_KernelFunctorList[i]->derivative(x, y, param, degree, index, isAtEnd, constValue));
		}
	      else
		{
		  itkGenericExceptionMacro(<<"derivative method not definied (Null)");
		}
	    }
	}
      else
	{
	  itkGenericExceptionMacro(<<"ComposedKernelFunctor::operator() : lists dimensions mismatch");
	}
      return out;
    }

  virtual int load_parameters(FILE ** pfile);

  virtual int save_parameters(FILE ** pfile, const char * composed_kernel_parameters_keyword) const;

  virtual void print_parameters(void)const;

  //virtual void Update(void){};


  /** Set/Get the SVM Model vector for the composed kernel */
  KernelListType GetKernelFunctorList(){ return m_KernelFunctorList; };
  void SetKernelFunctorList(KernelListType kernelFunctorList){ m_KernelFunctorList = kernelFunctorList; };
  // Add 1 element to the end of the list
  void AddKernelFunctorModelToKernelList(GenericKernelFunctorBase * kernelfunctor){ m_KernelFunctorList.push_back(kernelfunctor); };

  /** Generic kernel functors that have to be deleted. */
  KernelListType GetHaveToBeDeletedList(){ return m_HaveToBeDeletedList; };
  void SetHaveToBeDeletedList(KernelListType kernelFunctorList){ m_HaveToBeDeletedList = kernelFunctorList; };
  // Add 1 element to the end of the list
  void AddKernelFunctorModelToDeleteKernelList(GenericKernelFunctorBase * kernelfunctor){ m_HaveToBeDeletedList.push_back(kernelfunctor); };

  /** Set/Get the ponderation list to apply to each svm_model of the composed kernel */
  std::vector<double> GetPonderationList(){ return m_PonderationList; };
  void SetPonderationModelList(const std::vector<double> & list){ m_PonderationList = list; };
  // Add 1 element to the end of the list
  void AddPonderationToPonderationList(const double & pond){ m_PonderationList.push_back(pond); };

   /** Set/Get the boolean to know which operation has to be done with the kernel functors. */
   void SetMultiplyKernelFunctor( bool val ){ this->SetValue<bool>("MultiplyKernelFunctor", val); };
   bool GetMultiplyKernelFunctor(){ return (this->GetValue<bool>("MultiplyKernelFunctor")); };

private:
   typedef GenericKernelFunctorBase::MapType MapType;
 typedef GenericKernelFunctorBase::MapIterator MapIterator;
 typedef GenericKernelFunctorBase::MapConstIterator MapConstIterator;


   /** Generic kernel functors that composed kernel */
   KernelListType m_KernelFunctorList;
  /** Generic kernel functors that have to be deleted.
    * This list was made for the load_parameters methods where you set new functors using new.
    * But, in other cases, functor can be added with reference. Thus, we need to know which ones have to be deleted.  */
   KernelListType m_HaveToBeDeletedList;
   /** Ponderation list to apply to each svm_model of the composed kernel*/
   std::vector<double> m_PonderationList;

};



//} // namespace otb

#endif /* _LIBSVM_H */
