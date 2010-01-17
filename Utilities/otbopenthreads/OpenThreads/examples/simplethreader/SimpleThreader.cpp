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

#include <sys/types.h>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <vector>
#include <OpenThreads/Thread>
#include <OpenThreads/Mutex>
#include <OpenThreads/Barrier>
#include "ThreadObserver.h"
#include "ThreadReporter.h"

#ifdef _WIN32
#include <process.h>
#define getpid() _getpid()
#else
#include <unistd.h>
#endif 

OpenThreads::Barrier bar;

int GLOBAL_NUM_THREADS;

//-----------------------------------------------------------------------------
// Custom Thread Observer (w/finished count)
//
class MyThreadObserver : public ThreadObserver {

public:

    MyThreadObserver() : ThreadObserver(), _finishedCount(0) {};

    virtual ~MyThreadObserver() {};

    void threadFinished(const int threadId) {

        ThreadObserver::threadFinished(threadId);

        ++_finishedCount;
    }

    int getFinishedCount() {return _finishedCount;};


private:

    volatile int _finishedCount;

};

// check the working of OpenThreads::Thread::CurrentThread()
static OpenThreads::Thread* CurrentChecker(){
	return OpenThreads::Thread::CurrentThread();
};

//-----------------------------------------------------------------------------
// Custom thread class - with reporting
//
class MyThread : public OpenThreads::Thread, public ThreadReporter {

public:

    MyThread(int *dataPtr, int numElts) : OpenThreads::Thread(), 
	ThreadReporter(),
        _dataPtr(dataPtr), _numElts(numElts), _quitflag(false) {};

    virtual ~MyThread() {};

    virtual void run() {

	if( CurrentChecker()!=this)
		notifyObserversMessage(getThreadId(), "Thread::CurrentThread is NOT working");
	else
		notifyObserversMessage(getThreadId(), "Thread::CurrentThread is working");
		
	
	bar.block(GLOBAL_NUM_THREADS);  // Sync the threads.

	char tmp[80];
	sprintf(tmp, "StackSize: %d\n", static_cast<int>(getStackSize()));

        notifyObserversStarted(getThreadId());
        notifyObserversMessage(getThreadId(), "This is a thread message.");
		notifyObserversMessage(getThreadId(), tmp);
	
        register int i;
        for (i=0; i<_numElts; ++i) {
            _dataPtr[i] = getThreadId();
        }

	notifyObserversMessage(getThreadId(), "Finished my work");
        
	bar.block(GLOBAL_NUM_THREADS);  // Sync the threads.

        //---------------------------------------------------------------------
        // Now that we've done our work, wait for a sign that we should quit.
        //
	while (true) {
	    
	    _quitmutex.lock();
	    if(_quitflag == true) break;
	    _quitmutex.unlock();

	    OpenThreads::Thread::YieldCurrentThread();
	}


	notifyObserversFinished(getThreadId());

    }

    void quit() {

        _quitmutex.lock();
        _quitflag = true;
        _quitmutex.unlock();

    }

private:

    int *_dataPtr;

    int _numElts;

    volatile bool _quitflag;

    OpenThreads::Mutex _quitmutex;
};


int main(int argc, char **argv) {

    if(argc != 3) {
        std::cout << "Usage: simpleThreader [NUM_ELTS] [NUM_THREADS] " 
		  << std::endl;
        return 0;
    };

    std::cout << "Root Thread ID: " << getpid() << std::endl;

    int NUM_ELTS = atoi(argv[1]);
    int NUM_THREADS = atoi(argv[2]);
    
    GLOBAL_NUM_THREADS = NUM_THREADS + 1;
    
    MyThreadObserver observer;
    register int i;

    int *dataArray = new int[NUM_ELTS];
    std::vector<MyThread *> threads;

    for(i=0; i<NUM_ELTS; ++i) {
	
        dataArray[i] = -1;
    }

    OpenThreads::Thread::SetConcurrency(NUM_THREADS);

    OpenThreads::Thread::Init();
    for(i=0; i<NUM_THREADS; ++i) {
        int status;
        MyThread *thread = new MyThread(dataArray + (i*(NUM_ELTS/NUM_THREADS)),
                                        NUM_ELTS/NUM_THREADS);
        threads.push_back(thread);
        thread->addObserver(&observer);
		thread->setStackSize(1024*256);
        status = thread->start();
        assert(status == 0);
    }

    bar.block(GLOBAL_NUM_THREADS);  // Block 'till ready
    bar.block(GLOBAL_NUM_THREADS);  // Block 'till finished

    char val;
    std::cout << "Press any key + return to quit." << std::endl;
    std::cin >> val;

    // Notify the threads to quit, wait for this to happen.
    for(i=0;i<static_cast<int>(threads.size());++i) {
        MyThread *thread = threads[i];
        thread->quit();
    }

    while(observer.getFinishedCount() != NUM_THREADS) {
        // Spin our wheels.
    }

    std::cout << "Data Array: " << std::endl;
    for(i=0; i < NUM_THREADS; ++i) {
        register int j;
        for (j=0; j<NUM_ELTS/NUM_THREADS; ++j) {
            std::cout << dataArray[(i*NUM_ELTS/NUM_THREADS)+j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // Delete all the threads.
    for(i=0;i<static_cast<int>(threads.size());++i) {
        MyThread *thread = threads[i];
        delete thread;
    }

    threads.clear();

    delete [] dataArray;

	return 0;
}

