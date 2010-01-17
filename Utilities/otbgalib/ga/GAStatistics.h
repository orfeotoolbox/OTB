// $Header$
/* ----------------------------------------------------------------------------
  statistics.h
  mbwall 14jul95
  Copyright (c) 1995 Massachusetts Institute of Technology 
                   - all rights reserved

 DESCRIPTION:
  Header for the statistics object used by the GA objects.
---------------------------------------------------------------------------- */
#ifndef _ga_statistics_h_
#define _ga_statistics_h_

#include <ga/gatypes.h>
#include <ga/gaconfig.h>
#include <ga/GAGenome.h>
#include <ga/GAPopulation.h>



// Default settings and their names.
extern int  gaDefNumBestGenomes;
extern int  gaDefScoreFrequency1;
extern int  gaDefScoreFrequency2;
extern int  gaDefFlushFrequency;
extern char gaDefScoreFilename[];




/* ----------------------------------------------------------------------------
Statistics class
  We define this class as a storage object for the current state of the GA.
Whereas the parameters object keeps track of the user-definable settings for 
the GA, the statistics object keeps track of the data that the GA generates 
along the way.
---------------------------------------------------------------------------- */
class GAStatistics {
public:
  enum {
    NoScores=0x00,
    Mean=0x01,
    Maximum=0x02,
    Minimum=0x04,
    Deviation=0x08,
    Diversity=0x10,
    AllScores=0xff
    };
  
  GAStatistics();
  GAStatistics(const GAStatistics&);
  GAStatistics& operator=(const GAStatistics& orig){copy(orig); return *this;}
  virtual ~GAStatistics();
  void copy(const GAStatistics &);

  float online() const {return on;}
  float offlineMax() const {return offmax;}
  float offlineMin() const {return offmin;}
  float initial(int w=Maximum) const;
  float current(int w=Maximum) const;
  float maxEver() const {return maxever;}
  float minEver() const {return minever;}

  int generation() const {return curgen;}
  unsigned long int selections() const {return numsel;}
  unsigned long int crossovers() const {return numcro;}
  unsigned long int mutations() const {return nummut;}
  unsigned long int replacements() const {return numrep;}
  unsigned long int indEvals() const {return numeval;}
  unsigned long int popEvals() const {return numpeval;}
  float convergence() const;

  int nConvergence() const { return Nconv; }
  int nConvergence(unsigned int);
  int nBestGenomes(const GAGenome&, unsigned int);
  int nBestGenomes() const { return(boa ? boa->size() : 0); }
  int scoreFrequency(unsigned int x) { return(scoreFreq = x); }
  int scoreFrequency() const { return scoreFreq; }
  int flushFrequency(unsigned int x);
  int flushFrequency() const { return Nscrs; }
  const char* scoreFilename(const char *filename);
  const char* scoreFilename() const { return scorefile; }
  int selectScores(int w){ return which = w; }
  int selectScores() const { return which; }
  GABoolean recordDiversity(GABoolean flag){ return dodiv=flag; }
  GABoolean recordDiversity() const { return dodiv; }
  void flushScores();

  void update(const GAPopulation & pop);
  void reset(const GAPopulation & pop);
  const GAPopulation & bestPopulation() const { return *boa; }
  const GAGenome & bestIndividual(unsigned int n=0) const;

#ifdef GALIB_USE_STREAMS
  int scores(const char* filename, int which=NoScores);
  int scores(STD_OSTREAM & os, int which=NoScores);
  int write(const char* filename) const;
  int write(STD_OSTREAM & os) const;
#endif

// These should be protected (accessible only to the GA class) but for now they
// are publicly accessible.  Do not try to set these unless you know what you
// are doing!!
  unsigned long int numsel;     // number of selections since reset
  unsigned long int numcro;	// number of crossovers since reset
  unsigned long int nummut;	// number of mutations since reset
  unsigned long int numrep;	// number of replacements since reset
  unsigned long int numeval;	// number of individual evaluations since reset
  unsigned long int numpeval;	// number of population evals since reset

protected:
  unsigned int curgen;		// current generation number
  unsigned int scoreFreq;	// how often (in generations) to record scores
  GABoolean dodiv;		// should we record diversity?

  float maxever;		// maximum score since initialization
  float minever;		// minimum score since initialization
  float on;			// "on-line" performance (ave of all scores)
  float offmax;			// "off-line" performance (ave of maximum)
  float offmin;			// "off-line" performance (ave of minimum)

  float aveInit;		// stats from the initial population
  float maxInit;
  float minInit;
  float devInit;
  float divInit;

  float aveCur;			// stats from the current population
  float maxCur;
  float minCur;
  float devCur;
  float divCur;

  unsigned int nconv, Nconv;	// how many scores we're recording (flushFreq)
  float * cscore;		// best score of last n generations

  unsigned int nscrs, Nscrs;	// how many scores do we have?
  int * gen;			// generation number corresponding to scores
  float * aveScore;		// average scores of each generation
  float * maxScore;		// best scores of each generation
  float * minScore;		// worst scores of each generation
  float * devScore;		// stddev of each generation
  float * divScore;		// diversity of each generation
  char * scorefile;		// name of file to which scores get written
  int which;			// which data to write to file
  GAPopulation * boa;		// keep a copy of the best genomes

  void setConvergence(float);
  void setScore(const GAPopulation&);
  void updateBestIndividual(const GAPopulation&, GABoolean flag=gaFalse);
  void writeScores();
  void resizeScores(unsigned int);

  friend class GA;
};



inline const char* GAStatistics::scoreFilename(const char* filename){
  delete [] scorefile;
  scorefile = 0;
  if(filename){
    scorefile = new char[strlen(filename)+1];
    strcpy(scorefile, filename);
  }
  return scorefile;
}

inline float GAStatistics::convergence() const {
  double cnv = 0.0;
  if(nconv >= Nconv-1 && cscore[nconv%Nconv] != 0)
    cnv = (double)(cscore[(nconv+1)%Nconv]) / (double)(cscore[nconv%Nconv]);
  return (float)cnv;
}

inline float GAStatistics::initial(int w) const {
  float val = 0.0;
  switch(w){
  case Mean:      val = aveInit; break;
  case Maximum:   val = maxInit; break;
  case Minimum:   val = minInit; break;
  case Deviation: val = devInit; break;
  case Diversity: val = divInit; break;
  default: break;
  }
  return val;
}

inline float GAStatistics::current(int w) const {
  float val = 0.0;
  switch(w){
  case Mean:      val = aveCur; break;
  case Maximum:   val = maxCur; break;
  case Minimum:   val = minCur; break;
  case Deviation: val = devCur; break;
  case Diversity: val = divCur; break;
  default: break;
  }
  return val;
}



#ifdef GALIB_USE_STREAMS
inline STD_OSTREAM & operator<< (STD_OSTREAM & os, const GAStatistics& s)
{ s.write(os); return os; }
#endif

#endif
