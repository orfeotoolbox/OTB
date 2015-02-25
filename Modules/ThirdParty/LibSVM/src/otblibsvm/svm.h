#ifndef _LIBSVM_H
#define _LIBSVM_H

#define LIBSVM_VERSION 300

/*** Begin OTB modification ***/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <map>
#include <string>
#include <utility>
#include <vector>


struct svm_node;
struct svm_parameter;

/** \class GenericKernelFunctorBase
 * \brief Undocumented
 */
class GenericKernelFunctorBase
{
public:
  typedef GenericKernelFunctorBase Self;

  typedef std::map<std::string, std::string> MapType;
  typedef MapType::iterator MapIterator;
  typedef MapType::const_iterator MapConstIterator;

  GenericKernelFunctorBase();

  virtual ~GenericKernelFunctorBase();

  virtual GenericKernelFunctorBase* Clone() const;

  virtual double operator()(const svm_node * /*x*/, const svm_node * /*y*/, const svm_parameter& /*param*/) const;


  virtual double derivative(const svm_node * /*x*/, const svm_node * /*y*/, const svm_parameter& /*param*/,
                            int /*degree*/, int /*index*/, bool /*isAtEnd*/, double /*constValue*/) const;

  virtual int load_parameters(FILE ** pfile);

  virtual int save_parameters(FILE ** pfile, const char * generic_kernel_parameters_keyword) const;

  virtual void print_parameters(void) const;

  virtual double dot(const svm_node *px, const svm_node *py) const;

  virtual svm_node * sub(const svm_node *px, const svm_node *py) const;

  virtual svm_node * add(const svm_node *px, const svm_node *py) const;

  virtual void SetName(std::string name);
  virtual std::string GetName(void) const;

  virtual void SetMapParameters(const MapType & map);
  virtual const MapType & GetMapParameters() const;
  virtual MapType GetMapParameters();

  template<class T>
  T GetValue(const char *option) const
  {
    std::string Value = m_MapParameters.find(std::string(option))->second;
    T lValeur;
    std::stringstream flux;
    flux << Value;
    flux >> lValeur;
    return lValeur;
  }

  template<class T>
  void SetValue(const char *option, const T & value)
  {
    std::string lValeur;
    std::stringstream flux;
    flux << value;
    flux >> lValeur;
    m_MapParameters[std::string(option)] = lValeur;
  }

  // Override this method to to split m_MapParameters into specific variables to speed up kernel evaluations
  virtual void Update(void);

protected:
  GenericKernelFunctorBase(const Self& copy);

  Self& operator=(const Self& copy);

private:
  /** Kernel functor parameters */
  MapType m_MapParameters;

  /** Functor label name (without space) */
  std::string m_Name;
};

/** \class ComposedKernelFunctor
 * \brief Undocumented
 */
class ComposedKernelFunctor: public GenericKernelFunctorBase
{
public:
  typedef ComposedKernelFunctor    Self;
  typedef GenericKernelFunctorBase Superclass;

  ComposedKernelFunctor();

  virtual ~ComposedKernelFunctor();

  virtual ComposedKernelFunctor* Clone() const;

  typedef std::vector<GenericKernelFunctorBase *> KernelListType;

  virtual double operator()(const svm_node *x, const svm_node *y, const svm_parameter& param) const;

  /** Used for Taylor classification*/
  // degree is the developement degree
  // index is the current value
  // isAtEnd to indicate that it's the last possible derivation
  // baseValue is the constant of the formula
  virtual double derivative(const svm_node *x, const svm_node *y, const svm_parameter& param, int degree, int index, bool isAtEnd, double constValue) const;

  virtual int load_parameters(FILE ** pfile);

  virtual int save_parameters(FILE ** pfile, const char * composed_kernel_parameters_keyword) const;

  virtual void print_parameters(void)const;

  /** Get a reference to the internal kernel list */
  KernelListType& GetKernelFunctorList();

  /** Set internal kernel list. A clone is made of each kernel when copying */
  void SetKernelFunctorList(const KernelListType& kernelFunctorList);

  // Add 1 element to the end of the list. A clone is made of the kernel
  void AddKernelFunctorModelToKernelList(const GenericKernelFunctorBase * kernelfunctor);

  /** Set/Get the ponderation list to apply to each svm_model of the composed kernel */
  std::vector<double> GetPonderationList();
  void SetPonderationModelList(const std::vector<double> & list);
  // Add 1 element to the end of the list
  void AddPonderationToPonderationList(const double & pond);

  /** Set/Get the boolean to know which operation has to be done with the kernel functors. */
  void SetMultiplyKernelFunctor( bool val );
  bool GetMultiplyKernelFunctor();

protected:
  /** Copy constructor */
  ComposedKernelFunctor(const Self& copy);

  /* Assignment operator */
  Self& operator=(const Self& copy);

private:

  void ClearFunctorList();

  typedef GenericKernelFunctorBase::MapType MapType;
  typedef GenericKernelFunctorBase::MapIterator MapIterator;
  typedef GenericKernelFunctorBase::MapConstIterator MapConstIterator;

  /** Generic kernel functors that composed kernel */
  KernelListType m_KernelFunctorList;
  /** Ponderation list to apply to each svm_model of the composed kernel*/
  std::vector<double> m_PonderationList;
};

//#ifdef __cplusplus
//extern "C" {
//#endif
/*** End OTB modification ***/

extern int libsvm_version;

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
enum { LINEAR, POLY, RBF, SIGMOID, PRECOMPUTED, GENERIC, COMPOSED }; /* kernel_type */

struct svm_parameter
{
  /*** Begin OTB modification ***/
  svm_parameter()
  : kernel_generic(NULL),
    kernel_composed(NULL),
    nr_weight(0),
    weight_label(NULL),
    weight(NULL)
  {
  }

  svm_parameter(const svm_parameter& copy)
  : kernel_generic(NULL),
    kernel_composed(NULL),
    nr_weight(0),
    weight_label(NULL),
    weight(NULL)
  {
    *this = copy;
  }

  ~svm_parameter()
  {
    delete kernel_generic;
    delete kernel_composed;
    free(weight_label);
    free(weight);
  }

  svm_parameter& operator=(const svm_parameter& copy)
  {
    svm_type = copy.svm_type;
    kernel_type = copy.kernel_type;
    degree = copy.degree;
    gamma = copy.gamma;
    coef0 = copy.coef0;
    const_coef = copy.const_coef;
    lin_coef = copy.lin_coef;
    memcpy(custom, copy.custom, 500);
    delete kernel_generic;
    delete kernel_composed;
    kernel_generic = copy.kernel_generic != NULL ? copy.kernel_generic->Clone() : NULL;
    kernel_composed = copy.kernel_composed != NULL ? copy.kernel_composed->Clone() : NULL;
    cache_size = copy.cache_size;
    eps = copy.eps;
    C = copy.C;

    if (nr_weight > 0)
      {
      free(weight_label);
      free(weight);
      }

    nr_weight = copy.nr_weight;
    if (nr_weight > 0)
      {
      weight_label = (int *)malloc(nr_weight*sizeof(int));
      weight = (double *)malloc(nr_weight*sizeof(double));
      }
    else
      {
      weight_label = NULL;
      weight = NULL;
      }
    memcpy(weight_label, copy.weight_label, nr_weight);
    memcpy(weight, copy.weight, nr_weight);
    nu = copy.nu;
    p = copy.p;
    shrinking = copy.shrinking;
    probability = copy.probability;
    return *this;
  }
  /*** End OTB modification ***/

  int svm_type;
	int kernel_type;
	int degree;	/* for poly */
	double gamma;	/* for poly/rbf/sigmoid */
	double coef0;	/* for poly/sigmoid */

	/*** Begin OTB modification ***/
  double const_coef;
  double lin_coef;
  char custom[500];
  GenericKernelFunctorBase * kernel_generic;
  ComposedKernelFunctor * kernel_composed;
  /*** End OTB modification ***/

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
	struct svm_parameter param;	/* parameter */
	int nr_class;		/* number of classes, = 2 in regression/one class svm */
	int l;			/* total #SV */
	struct svm_node **SV;		/* SVs (SV[l]) */
	double **sv_coef;	/* coefficients for SVs in decision functions (sv_coef[k-1][l]) */
	double *rho;		/* constants in decision functions (rho[k*(k-1)/2]) */
	double *probA;		/* pariwise probability information */
	double *probB;

	/* for classification only */

	int *label;		/* label of each class (label[k]) */
	int *nSV;		/* number of SVs for each class (nSV[k]) */
				/* nSV[0] + nSV[1] + ... + nSV[k-1] = l */
	/* XXX */
	int free_sv;		/* 1 if svm_model is created by svm_load_model*/
				/* 0 if svm_model is created by svm_train */
};

struct svm_model *svm_train(const struct svm_problem *prob, const struct svm_parameter *param);
void svm_cross_validation(const struct svm_problem *prob, const struct svm_parameter *param, int nr_fold, double *target);

int svm_save_model(const char *model_file_name, const struct svm_model *model);
/*** Begin OTB modification ***/
struct svm_model *svm_load_model(const char *model_file_name, GenericKernelFunctorBase* generic_kernel_functor = NULL);
struct svm_model *svm_copy_model( const svm_model *model );
/*** End OTB modification ***/

int svm_get_svm_type(const struct svm_model *model);
int svm_get_nr_class(const struct svm_model *model);
void svm_get_labels(const struct svm_model *model, int *label);
double svm_get_svr_probability(const struct svm_model *model);

double svm_predict_values(const struct svm_model *model, const struct svm_node *x, double* dec_values);
double svm_predict(const struct svm_model *model, const struct svm_node *x);
double svm_predict_probability(const struct svm_model *model, const struct svm_node *x, double* prob_estimates);

void svm_free_model_content(struct svm_model *model_ptr);
void svm_free_and_destroy_model(struct svm_model **model_ptr_ptr);
void svm_destroy_param(struct svm_parameter *param);

const char *svm_check_parameter(const struct svm_problem *prob, const struct svm_parameter *param);
int svm_check_probability_model(const struct svm_model *model);

void svm_set_print_string_function(void (*print_func)(const char *));

// deprecated
// this function will be removed in future release
void svm_destroy_model(struct svm_model *model_ptr);

/*** Begin OTB modification ***/
//#ifdef __cplusplus
//}
//#endif
/*** End OTB modification ***/

#endif /* _LIBSVM_H */
