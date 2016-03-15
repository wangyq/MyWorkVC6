// ThreadPool.h: interface for the CThreadPool class.
//
//////////////////////////////////////////////////////////////////////

#include "BitArray.h"

#if !defined(AFX_THREADPOOL_H__51CA8967_7888_4D38_A831_26EDFC5C73E0__INCLUDED_)
#define AFX_THREADPOOL_H__51CA8967_7888_4D38_A831_26EDFC5C73E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

template <class Worker>
class CThreadPool
{
#define THREAD_POOL_SHUTDOWN ((OVERLAPPED*)-1L)
#define THREAD_POOL_STARTUP ((OVERLAPPED*)-2L)

protected:
    enum {
        ENUM_TIMEOUT_MILIISECOND = 600,
        ENUM_DEFAULT_MAX_THREAD_NUM = 256,  //ONE core's thread num
        ENUM_THREADNUM_PER_CORE = 128,
        ENUM_SYSTEM_MAX_THREAD_NUM = 8 * (1 << 10), //
        ENUM_END
    };
    bool m_bForcedStop;

    bool m_bStart;
    //int m_bThreadMark;
    //CBitArray m_bThreadMark;

    unsigned int m_nThreadNumPerCore;
    unsigned int m_nMaxThreadNum;
    unsigned int m_nThreadNum ;
    unsigned int m_nCurThreadIndex ;
    unsigned int m_dwMaxWaitMilliseconds ;
    void *m_pvWorkerParam;

    //==
    BOOL StartWorkerThreadProc() {

        BOOL bOK = FALSE;

        DWORD dwThreadID = 0;
        HANDLE hThread = NULL;

        m_bStart = true; // start threadpool!

        m_nCurThreadIndex = 0; //it's value will be changed by worker's thread process.
        for (unsigned int i = 0; i < m_nThreadNum; i++) {
            //m_nCurThreadIndex = i;

            // start each thread one by one!
            ResetEvent(m_hThreadEvent);
            hThread = ::CreateThread(NULL, 0, WorkerThreadProc, (LPVOID)this, 0, &dwThreadID);
            if (!hThread) {
                //return FALSE;
                goto FAIL_RET;
            }
            CloseHandle(hThread);
            hThread = NULL;

            DWORD dwRet = ::WaitForSingleObject(m_hThreadEvent, m_dwMaxWaitMilliseconds);
            if (dwRet != WAIT_OBJECT_0) {
                //goto FAIL_RET;
                if (dwRet == WAIT_TIMEOUT) { // timeout, so do not add thread now!
                    //return HRESULT_FROM_WIN32(WAIT_TIMEOUT);
					m_nCurThreadIndex ++;  //start thread ok!
					break;
                } else { //nothing to do!
                    //return FALSE;
					Stop();  //unknown error! must stop it.
                }

				goto FAIL_RET;
            }
			m_nCurThreadIndex ++;  //start thread ok!

        }//end of for!
        bOK = TRUE;
        goto OK_RET;

FAIL_RET:
		//Stop();
		;

OK_RET:
        m_bStart = false; //end of start()
        return bOK;
    }

#if 0
    //==
    BOOL StartWorkerThreadProc() {//chain start up
        for (unsigned int i = 1; i < m_nThreadNum; i++) {
            hThread = ::CreateThread(NULL, 0, WorkerThreadProc, (LPVOID)this, 0, &dwThreadID);
            if (!hThread) {
                return FALSE;
            }
            CloseHandle(hThread);
            hThread = NULL;
        }
    }
#endif

    void ShutDown() {
        if (isRunning()) {
            Stop();
        }
        if (isStopped()) {
            if (m_hRequestQueue) {
                // Close the request queue handle
                CloseHandle(m_hRequestQueue);

                // Clear the queue handle
                m_hRequestQueue = NULL;

            }
            if (m_hThreadEvent) {
                // Close the request queue handle
                CloseHandle(m_hThreadEvent);

                // Clear the queue handle
                m_hThreadEvent = NULL;
            }
        }
    }
#if 0
    //==
    BOOL StopWorkerThreadProc() {
        unsigned int i = 0;
        BOOL bOK = TRUE;
        while (isRunning() && (i < m_nCurThreadIndex)) {
            i++;
            //
            ResetEvent(m_hThreadEvent);
            PostQueuedCompletionStatus(m_hRequestQueue, 0, 0, THREAD_POOL_SHUTDOWN);
            DWORD dwRet = ::WaitForSingleObject(m_hThreadEvent, m_dwMaxWaitMilliseconds);
            if (dwRet != WAIT_OBJECT_0) {
                bOK = FALSE;
            }
        }
        return bOK;
    }
#endif
    /*
    bool isFirsThread( ){
        //if( (index <0) || (index >=ENUM_MAX_THREAD_NUM) ) return FALSE;
        //int mask = ~(1<<index);
        //return (m_bThreadMark & mask) == 0 ;
        //return m_bThreadMark.isOtherClear(index);
        return m_nCurThreadIndex == 0;
    }
    //
    bool isLastThread( ){
        //if( (index <0) || (index >=ENUM_MAX_THREAD_NUM) ) return FALSE;
        //int mask = ~(1<<index);
        //return (m_bThreadMark & mask) == 0 ;

        //return m_bThreadMark.isOtherClear(index);
        return m_nCurThreadIndex == 0;
    }
    */

    //==
    int ThreadProc() {
        //int nThreadIndex = m_nCurThreadIndex; //local variable, this thread's thread index!
        bool bLastTerminate = false;
        bool bFirstRun = (m_nCurThreadIndex == 0);//isFirsThread(  ); //(nThreadIndex==0)?true:false;

        DWORD dwBytesTransfered;
        ULONG_PTR dwCompletionKey;

        OVERLAPPED* pOverlapped;

        Worker theWorker;

        if (theWorker.Initialize(m_pvWorkerParam, bFirstRun) == FALSE) {
            return 1;
        }

        //== mark thread running
        //m_bThreadMark |= (1 << nThreadIndex);
        //m_bThreadMark.Set(nThreadIndex);
        //m_nCurThreadIndex ++;  //do it in call start() thread.

        SetEvent(m_hThreadEvent); //tell that this thread is running now!

        while (GetQueuedCompletionStatus(m_hRequestQueue, &dwBytesTransfered, &dwCompletionKey, &pOverlapped, INFINITE)) {
            if (m_bForcedStop) {//forced stop now!
                if (pOverlapped != THREAD_POOL_SHUTDOWN) {
                    continue;  //fetch next until reach THREAD_POOL_SHUTDOWN
                }

                if (m_nCurThreadIndex > 1) {
                    //mark thread end here!
                    //m_bThreadMark &= (~(1 << nThreadIndex));
                    //m_bThreadMark.Clear(nThreadIndex);
                    
					m_nCurThreadIndex -- ;   //delay decrease!
                    PostQueuedCompletionStatus(m_hRequestQueue, 0, 0, THREAD_POOL_SHUTDOWN);
                    break; //end execute!
                } else if (m_bStart) { //m_nCurThreadIndex <= 1
                    PostQueuedCompletionStatus(m_hRequestQueue, 0, 0, THREAD_POOL_SHUTDOWN);
                    continue;
                } else { //m_bStart = false and m_nCurThreadIndex <= 1
                    m_nCurThreadIndex --;//= 0; //delay
                    bLastTerminate = true;

                    /*
                    //mark thread end here!
                    //m_bThreadMark &= (~(1 << nThreadIndex));
                    //m_bThreadMark.Clear(nThreadIndex);
                    m_nCurThreadIndex -- ;

                    //if( isLastThread() ){
                    if (m_nCurThreadIndex == 0) {
                        bLastTerminate = true;
                        //theWorker.ExecuteLast(m_pvWorkerParam,m_bForcedStop);
                    } else { //finished one by one!
                        PostQueuedCompletionStatus(m_hRequestQueue, 0, 0, THREAD_POOL_SHUTDOWN);
                    }*/
                    break;
                }

            } else if (pOverlapped == THREAD_POOL_SHUTDOWN) { //not set m_bForceStop!
                m_bForcedStop = TRUE;
                PostQueuedCompletionStatus(m_hRequestQueue, 0, 0, THREAD_POOL_SHUTDOWN);
                continue;
                //break;
            }/*else if (pOverlapped == THREAD_POOL_STARTUP) { // STARTUP
                bFirstRun = true;

                break; //end
            } */else { //do worker!
                Worker::RequestType request = (Worker::RequestType) dwCompletionKey;
                if (Worker::isAutoExitThread()) {
                    if (request == Worker::GetAutoExitRequest()) { //can we make sure this is just running only once?

                        if (m_nCurThreadIndex > 1) {
                            //mark thread end here!
                            //m_bThreadMark &= (~(1 << nThreadIndex));
                            //m_bThreadMark.Clear(nThreadIndex);
                            m_nCurThreadIndex -- ; //delay
                            AddWorkerRequest(request); //chain react!
                            break; //end execute!
                        } else if (m_bStart) { //m_nCurThreadIndex <= 1
                            AddWorkerRequest(request); //chain react! and waiting!
                            continue;
                        } else { //m_bStart = false and m_nCurThreadIndex <= 1
                            m_nCurThreadIndex --;//= 0; //delay
                            bLastTerminate = true;

                            /*
                                //mark thread end here!
                                //m_bThreadMark &= (~(1 << nThreadIndex));
                                //m_bThreadMark.Clear(nThreadIndex);
                                m_nCurThreadIndex -- ;

                                //if( isLastThread() ){
                                if (m_nCurThreadIndex == 0) {
                                    bLastTerminate = true;
                                    //theWorker.Execute(request, m_pvWorkerParam, pOverlapped);
                                } else {
                                    AddWorkerRequest(request); //chain react!
                                }*/
                            break;
                        }
                    }//end of auto exit request!
                }//end of isAutoExitThread

                // Process the request.  Notice the following:
                // (1) It is the worker's responsibility to free any memory associated
                // with the request if the request is complete
                // (2) If the request still requires some more processing
                // the worker should queue the request again for dispatching

                theWorker.Execute(request, m_pvWorkerParam, pOverlapped);
                //BOOL bFinished = theWorker.Execute(request, m_pvWorkerParam, pOverlapped);
                //if (bFinished) break;   //Finished thread now.
            } //do worker!
        }//end while

        theWorker.Terminate(m_pvWorkerParam, bLastTerminate);

        //== clear mark thread
        //m_bThreadMark &= (~(1 << nThreadIndex));
        //SetEvent(m_hThreadEvent); not need it now!
		//m_nCurThreadIndex -- ; 

        return 0;
    }

    HANDLE m_hThreadEvent;
    HANDLE m_hRequestQueue;
public:
    //==
    static DWORD WINAPI StartPoolThreadProc(LPVOID pv) {
        CThreadPool* pThis =
            reinterpret_cast< CThreadPool* >(pv);

        return pThis->StartWorkerThreadProc();
    }

#if 0
    static DWORD WINAPI StopPoolThreadProc(LPVOID pv) {
        CThreadPool* pThis =
            reinterpret_cast< CThreadPool* >(pv);

        return pThis->StopWorkerThreadProc();
    }
#endif

    static DWORD WINAPI WorkerThreadProc(LPVOID pv) {
        CThreadPool* pThis =
            reinterpret_cast< CThreadPool* >(pv);

        return pThis->ThreadProc();
    }
    // ==
    CThreadPool(): m_hRequestQueue(NULL), m_hThreadEvent(NULL) {
        m_bStart = false;

        m_dwMaxWaitMilliseconds = ENUM_TIMEOUT_MILIISECOND; //600
        m_nCurThreadIndex = 0;
        m_pvWorkerParam = NULL;
        m_nMaxThreadNum = ENUM_DEFAULT_MAX_THREAD_NUM;
        m_nThreadNumPerCore = ENUM_THREADNUM_PER_CORE;
        //m_bThreadMark = 0;
        //m_bThreadMark.SetSize(m_nMaxThreadNum);
        m_nThreadNum = 0;
        m_bForcedStop = false;
    }
    virtual ~CThreadPool() {
        ShutDown();
    }
    /**************************
    *
    *
    **************************/
public:
    //==
    BOOL isStopped() {
        //return m_bThreadMark == 0;
        //return m_bThreadMark.isEmpty();
        return m_nCurThreadIndex <= 0;
    }
    BOOL isRunning() {
        //return m_bThreadMark != 0;
        //return !(m_bThreadMark.isEmpty());
        return m_nCurThreadIndex > 0;
    }
    //==
    BOOL Start(unsigned int nThreadNum = 0) {
        if (isRunning()) return FALSE;   //here return TRUE or FALSE ?

        if (nThreadNum > m_nMaxThreadNum) {
            m_nThreadNum = m_nMaxThreadNum;
        } else if (nThreadNum > 0) {
            m_nThreadNum = nThreadNum;
        } else {
            ;//nothing to do!
        }
        m_bForcedStop = 0;  //no set force stop!

        //too long time for main thread to wait.
        //return StartPoolThreadProc(this);


        DWORD dwThreadID = 0;
        HANDLE hThread = NULL;
        hThread = ::CreateThread(NULL, 0, StartPoolThreadProc, (LPVOID)this, 0, &dwThreadID);
        if (!hThread) {
            return FALSE;
        }
        CloseHandle(hThread);
        hThread = NULL;
        return TRUE;

    }
    /****************************************
    * This function should be called only once!
    * Because it stop threadpool's thread one by one using chain technology.
    *
    */
    BOOL Stop() {//force stop
        if (isRunning() && (!m_bForcedStop)) {
            m_bForcedStop = true; //force stop now!
            //chain one by one to stop!
            PostQueuedCompletionStatus(m_hRequestQueue, 0, 0, THREAD_POOL_SHUTDOWN);
        }

        return TRUE;
    }

    //==
    BOOL Initialize(void *pvWorkerParam = NULL, int nNumCores = 0, int nThreadNumPerCore = 0) {
        //==
        if (nThreadNumPerCore > 0) {
            m_nThreadNumPerCore = nThreadNumPerCore;
        }
        if (nNumCores > 0) {
            m_nMaxThreadNum = nNumCores * m_nThreadNumPerCore;
            m_nMaxThreadNum = m_nMaxThreadNum < ENUM_SYSTEM_MAX_THREAD_NUM ? m_nMaxThreadNum : ENUM_SYSTEM_MAX_THREAD_NUM; //

            //m_bThreadMark.SetSize(m_nMaxThreadNum);
            //m_bThreadMark.ClearAll();
        }

        // Automatic reset Event!
        m_hThreadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
        if (!m_hThreadEvent) {
            return FALSE;
        }
        // Create IO completion port to queue the requests
        m_hRequestQueue = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

        if (m_hRequestQueue == NULL) {
            // failed creating the Io completion port
            CloseHandle(m_hThreadEvent);
            return FALSE;
        }

        m_pvWorkerParam = pvWorkerParam;

        return TRUE;

    }
    //==
    BOOL AddWorkerRequest(typename Worker::RequestType request) {
        if (!PostQueuedCompletionStatus(m_hRequestQueue, 0, (ULONG_PTR) request, NULL))
            return FALSE;
        return TRUE;
    }
};

//==
template<class T, bool bAutoExit = false, T EndRequest = 0>
class CWorkerBase
{
public:
    typedef T RequestType;
    //typedef T ThreadParam;
    //typedef T* PThreadParam;

public:
    static bool isAutoExitThread() {
        return bAutoExit;
    }
    static RequestType GetAutoExitRequest() {
        return EndRequest;
    }

public:
    BOOL Initialize(void * /*pvParam*/, bool bFirstStartup = false) {
        return TRUE;
    }

    void Execute(
        RequestType request,
        void *pvWorkerParam,
        OVERLAPPED *pOverlapped) {

    }

    void Terminate(void* /*pvParam*/, bool bLastExecute = false) {
    }
};

// This class is a simple wrapper for use with CThreadPool.
//  It instantiates one instance of Worker per request
//  this allows Worker to hold state for each request
//  and depend on the destructor being called
//  Worker:
//      is a class that is responsible for handling requests
//      queued on the thread pool (See CThreadPool)
template <class Worker>
class CStatelessWorker
{
public:
    typedef typename Worker::RequestType RequestType;

    BOOL Initialize(void * /*pvParam*/, bool bFirstStartup = false) {
        return TRUE;
    }

    void Execute(
        typename Worker::RequestType request,
        void *pvWorkerParam,
        OVERLAPPED *pOverlapped) {
        Worker worker;
        if (worker.Initialize(pvWorkerParam)) {
            worker.Execute(request, pvWorkerParam, pOverlapped);
            worker.Terminate(pvWorkerParam);
        }
    }

    void Terminate(void* /*pvParam*/, bool bLastExecute = false)  {
    }
}; // class CNonStatelessWorker

#endif // !defined(AFX_THREADPOOL_H__51CA8967_7888_4D38_A831_26EDFC5C73E0__INCLUDED_)
