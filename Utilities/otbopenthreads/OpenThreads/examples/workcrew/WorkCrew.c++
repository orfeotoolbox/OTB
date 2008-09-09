//
// OpenThread library, Copyright (C) 2002 - 2003  The Open Thread Group
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#include <cstdio>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <cassert>
#include <cerrno>
#include "ThreadObserver.h"
#include "ThreadReporter.h"

//-----------------------------------------------------------------------------
// For the threads
//
#include <OpenThreads/Thread>
#include <OpenThreads/Mutex>
#include <OpenThreads/Condition>
#include <OpenThreads/Barrier>

extern int errno;

#ifdef DEBUG
# define DPRINTF(arg) printf arg
#else
# define DPRINTF(arg)
#endif


//-----------------------------------------------------------------------------
// Create a data structure for the worker's to work on
// 
struct WorkData {

    WorkData * next;               // Next work item.
    std::vector<float> datapts;         // Data points.

};

//-----------------------------------------------------------------------------
// Create a data structure for the crew.
// 
struct Crew {

    int crewSize;                  // Number of workers in the crew.
    long workCount;                // Number of work items.
    WorkData *first, *last;        // First & last work items.

    OpenThreads::Mutex mutex;                   
    OpenThreads::Condition go;
    OpenThreads::Condition done;
    OpenThreads::Barrier bar;

};

class WorkerThread : public OpenThreads::Thread {

public:
    
    WorkerThread(Crew *crew, float *workResult) : OpenThreads::Thread(), 
	_crew(crew), _result(workResult) {

	_bar   = &_crew->bar;
	_mutex = &_crew->mutex;
	_go    = &_crew->go;
	_done  = &_crew->done;

    };

    virtual ~WorkerThread() {};

protected:

    virtual void run() {
	
	int status;

	DPRINTF(("Created Thread %d\n", getProcessId()));

	//---------------------------------------------------------------------
	// Block until all of the crew members are ready.
	//
	_bar->block(_crew->crewSize+1);

	DPRINTF(("Reving Thread %d\n", getProcessId()));

	//---------------------------------------------------------------------
	// Lock the crew mutex
	//
	status = _mutex->lock();
	DPRINTF(("Thread %d got lock\n", getProcessId()));
	assert(status == 0); 

	//---------------------------------------------------------------------
	// There will be no work to do when the crew members are created,
	// so wait for the go signal.
	//
	while(_crew->workCount == 0) {

	    DPRINTF(("Thread %d waiting to go\n", getProcessId()));
	    status = _go->wait(_mutex);
	    assert(status == 0); 

	    DPRINTF(("Thread %d going\n", getProcessId()));
	}	

        //---------------------------------------------------------------------
	// unlock the crew mutex.
	//
	status = _mutex->unlock();
	assert(status == 0); 


	//---------------------------------------------------------------------
	// While there's work to do, do it !
	// 
	while(true) {

	    DPRINTF(("Thread %d entered while loop.\n", getProcessId()));

	    //-----------------------------------------------------------------
	    // Lock the crew mutex
	    //
	    status = _mutex->lock();
	    assert(status == 0); 
	    
	    //-----------------------------------------------------------------
	    // Wait while there is nothing to do. If _crew->first == 0, 
	    // there's no work.  But if _crew->workCount == 0, we're done.
	    //
	    while(_crew->first == 0) {

		DPRINTF(("Thread %d waiting for work\n", getProcessId()));
		
		status = _go->wait(_mutex);
		assert(status == 0); 

		DPRINTF(("Thread %d woke, %#lx, %d\n",
			 getProcessId(), _crew->first, _crew->workCount));
	    }
	    	   
	    //-----------------------------------------------------------------
	    // Remove and process a work item.
	    //
	    _work = _crew->first;
	    _crew->first = _work->next;
	    if(_crew->first == 0) {
		_crew->last = 0;
	    };

	    DPRINTF(("Thread %d took, %#lx, leaves first %#lx, last %#lx\n",
		   getProcessId(), _work, _crew->first, _crew->last));

	    //-----------------------------------------------------------------
	    // unlock the crew mutex.
	    //
	    status = _mutex->unlock();
	    assert(status == 0); 
	    
	    //-----------------------------------------------------------------
	    // Okay, we have a work item, process it.
	    //
	    float proc_result = pr_processWork();

	    //-----------------------------------------------------------------
	    // Lock the crew mutex to decrement the work count, and
	    // update the result.
	    //
	    status = _mutex->lock();
	    assert(status == 0); 
   
	    *(_result) += proc_result;

	    --_crew->workCount;

	    DPRINTF(("Thread %d decremented work count to %d\n",
		   getProcessId(), _crew->workCount));
	    
	    if(_crew->workCount <= 0) {
		DPRINTF(("Thread %d done.\n", getProcessId()));
	    
		//-------------------------------------------------------------
		// Signal that we have finished our job.
		//
		status = _done->broadcast();
		assert(status == 0); 
	    
		//-------------------------------------------------------------
		// unlock the crew mutex.
		//
		status = _mutex->unlock();	
		assert(status == 0);
		
		break;
	    }

	    //----------------------------------------------------------------
	    // unlock the crew mutex.
	    //
	    status = _mutex->unlock();    
	    assert(status == 0); 
	    
	}

    };

private:

    float pr_processWork() {

	//---------------------------------------------------------------------
	// Just take the numbers and add them
	// 
	float result = 0;
	register unsigned int i, j;
	float total;
	for(i=0; i<_work->datapts.size();++i) {
	    for(total=0, j=0; j<10000000; ++j) {
#if defined(WIN32) && !defined(__CYGWIN__)
		total += double(rand()) / double(RAND_MAX);
		total -= double(rand()) / double(RAND_MAX);
#else
		total += drand48();
		total -= drand48();
#endif
	    }
	    result += _work->datapts[i] + total;
	    result -= total;
	}

	_work->next = 0;
	delete _work;
	_work = 0;

	return result;

    }

private:

    WorkData *_work;

    Crew *_crew;

    OpenThreads::Mutex *_mutex;

    OpenThreads::Condition *_go;

    OpenThreads::Condition *_done;

    OpenThreads::Barrier *_bar;

    float *_result;

};

int crew_create(Crew *crew,  
		float & result, std::vector<WorkerThread *> &workers) {

    int crew_index;
    int status;

    //-------------------------------------------------------------------------
    // Create the worker threads.
    //
    for(crew_index = 0; crew_index < crew->crewSize; ++crew_index) {

	WorkerThread *thread = new WorkerThread(crew, &result);
	workers.push_back(thread);
	status = thread->start();
	assert(status == 0);

    }

    crew->bar.block(crew->crewSize+1);

    return 0;
}

int crew_start(Crew *crew) {


    int status = 0;
    
    //-------------------------------------------------------------------------
    // Lock the crew mutex
    //
    status = crew->mutex.lock();
    if(status != 0) {
	DPRINTF(("Lock Error: %s\n", strerror(errno)));
    }
    assert(status == 0); 

    DPRINTF(("Checking busy signal.\n"));

    //-------------------------------------------------------------------------
    // If the crew is busy, wait 'till they're done.
    // 
    while(crew->workCount > 0) {

	status = crew->done.wait(&(crew->mutex));
	if(status != 0) {

	    status = crew->mutex.lock();
	    return status;
	}
    }

    DPRINTF(("Crew not busy, creating some work data.\n"));
    
    errno = 0;

    //-------------------------------------------------------------------------
    //  Create some work data.
    //
    register int i;
    for(i=0; i<4; ++i) {
	WorkData *work = new WorkData;
	work->datapts.push_back(10.0);
	work->next = 0;
	if(crew->first == 0) {
	    crew->first = work;
	    crew->last = work;
	} else {
	    crew->last->next = work;
	    crew->last = work;
	}
	++crew->workCount;
    }

    DPRINTF(("Giving the go-ahead.\n"));

    status = crew->go.signal();
    if(status != 0) {
	delete crew->first;
	crew->first = NULL;
	crew->workCount = 0;
	status = crew->mutex.unlock();
	return status;
    }
    
    DPRINTF(("Waiting on the crew to finish....\n"));

    while(crew->workCount > 0) {

	DPRINTF(("Work Count: %d\n", crew->workCount));
	status = crew->done.wait(&(crew->mutex));
	assert(status == 0);
    }

    DPRINTF(("Crew finished.\n"));
  
    status = crew->mutex.unlock();		        
    assert(status == 0);

    return 0;
}

int main (int argc, char **argv) {

    std::vector<WorkerThread *> workers;

    if(argc != 2) {
        std::cout << "Usage: workCrew [NUM_THREADS] " << std::endl;
        return 0;
    };

    const int NUM_THREADS = atoi(argv[1]);

    DPRINTF(("ROOT PID = %d\n", getpid()));

    Crew *crew = new Crew;

    crew->crewSize = NUM_THREADS;
    crew->workCount = 0;
    crew->first = 0;
    crew->last  = 0;
    
    float result = 0;
    unsigned int status = crew_create(crew, result, workers);
    assert(status == 0);

    status = crew_start(crew);
    assert(status == 0);

    printf("FINAL RESULT: %f\n", result);

    for(status = 0; status < workers.size(); ++status) {

	WorkerThread *thread = workers[status];
	delete thread;

    }
    workers.clear();

    delete crew;
}










