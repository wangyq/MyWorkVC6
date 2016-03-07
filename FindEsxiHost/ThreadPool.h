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
        ENUM_MAX_THREAD_NUM = 64,
		ENUM_THREADNUM_PER_CORE = 64,
        ENUM_END
    };
	int m_bForcedStop;

    //int m_bThreadMark;
	CBitArray m_bThreadMark;
	
	unsigned int m_nMaxThreadNum;
    unsigned int m_nThreadNum ;
    unsigned int m_nCurThreadIndex ;
    unsigned int m_dwMaxWaitMilliseconds ;
    void *m_pvWorkerParam;

    //==
    BOOL StartWorkerThreadProc() {
        DWORD dwThreadID = 0;
        HANDLE hThread = NULL;

        for (unsigned int i = 0; i < m_nThreadNum; i++) {
            m_nCurThreadIndex = i;

            // start each thread one by one!
            ResetEvent(m_hThreadEvent);
            hThread = ::CreateThread(NULL, 0, WorkerThreadProc, (LPVOID)this, 0, &dwThreadID);
            if (!hThread) {
                return FALSE;
            }
            CloseHandle(hThread);
            hThread = NULL;

            DWORD dwRet = ::WaitForSingleObject(m_hThreadEvent, m_dwMaxWaitMilliseconds);
            if (dwRet != WAIT_OBJECT_0) {
                if (dwRet == WAIT_TIMEOUT) {
                    return HRESULT_FROM_WIN32(WAIT_TIMEOUT);
                } else {
                    return FALSE;
                }
            }
        }//end of for!

        return TRUE;
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

	void ShutDown(){
		if( isRunning() ){
			Stop();
		}
		if( isStopped() )
		{
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
	
	BOOL isFirsThread(int index){
		//if( (index <0) || (index >=ENUM_MAX_THREAD_NUM) ) return FALSE;
		//int mask = ~(1<<index);
		//return (m_bThreadMark & mask) == 0 ;
		return m_bThreadMark.isOtherClear(index);
	}
	//
	BOOL isLastThread(int index){
		//if( (index <0) || (index >=ENUM_MAX_THREAD_NUM) ) return FALSE;
		//int mask = ~(1<<index);
		//return (m_bThreadMark & mask) == 0 ;

		return m_bThreadMark.isOtherClear(index);
	}
    //==
    int ThreadProc() {
        int nThreadIndex = m_nCurThreadIndex;
		bool bLastTerminate = false;
		bool bFirstRun = (nThreadIndex==0)?true:false;

        DWORD dwBytesTransfered;
        ULONG_PTR dwCompletionKey;

        OVERLAPPED* pOverlapped;

        Worker theWorker;

        if (theWorker.Initialize(m_pvWorkerParam, bFirstRun) == FALSE) {
            return 1;
        }

        //== mark thread running
        //m_bThreadMark |= (1 << nThreadIndex);
		m_bThreadMark.Set(nThreadIndex);

        SetEvent(m_hThreadEvent); //tell that is running now!

        while (GetQueuedCompletionStatus(m_hRequestQueue, &dwBytesTransfered, &dwCompletionKey, &pOverlapped, INFINITE)) {
			if( m_bForcedStop ){//forced stop now!
				if (pOverlapped != THREAD_POOL_SHUTDOWN){
					continue;  //fetch next until reach THREAD_POOL_SHUTDOWN
				}
				
				//mark thread end here!
				//m_bThreadMark &= (~(1 << nThreadIndex));
				m_bThreadMark.Clear(nThreadIndex);

				if( isLastThread(nThreadIndex) ){
					bLastTerminate = true;
					//theWorker.ExecuteLast(m_pvWorkerParam,m_bForcedStop);
				}else{//finished one by one!
					PostQueuedCompletionStatus(m_hRequestQueue, 0, 0, THREAD_POOL_SHUTDOWN);
				}
				break;
			}else if (pOverlapped == THREAD_POOL_SHUTDOWN ){
				m_bForcedStop = TRUE;
				PostQueuedCompletionStatus(m_hRequestQueue, 0, 0, THREAD_POOL_SHUTDOWN);
				continue;
				//break;
            }/*else if (pOverlapped == THREAD_POOL_STARTUP) { // STARTUP
				bFirstRun = true;

                break; //end
            } */else { //do worker!
                Worker::RequestType request = (Worker::RequestType) dwCompletionKey;
				if( Worker::isAutoExitThread()){
					if( request == Worker::GetAutoExitRequest() ) {//can we make sure this is just running only once?
						
						//mark thread end here!
						//m_bThreadMark &= (~(1 << nThreadIndex));
						m_bThreadMark.Clear(nThreadIndex);

						if( isLastThread(nThreadIndex) ){
							bLastTerminate = true;
							//theWorker.Execute(request, m_pvWorkerParam, pOverlapped);
						} else{
							
							AddWorkerRequest(request); //chain react!
						}
						break;
					}
				}
				
                // Process the request.  Notice the following:
                // (1) It is the worker's responsibility to free any memory associated
                // with the request if the request is complete
                // (2) If the request still requires some more processing
                // the worker should queue the request again for dispatching

                theWorker.Execute(request, m_pvWorkerParam, pOverlapped);
                //BOOL bFinished = theWorker.Execute(request, m_pvWorkerParam, pOverlapped);
                //if (bFinished) break;   //Finished thread now.
            }
        }//end while

        theWorker.Terminate(m_pvWorkerParam, bLastTerminate);

        //== clear mark thread
        //m_bThreadMark &= (~(1 << nThreadIndex));
        //SetEvent(m_hThreadEvent); not need it now!

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
        m_bThreadMark = 0;
        m_dwMaxWaitMilliseconds = 600;
        m_nCurThreadIndex = 0;
        m_pvWorkerParam = NULL;
		m_nMaxThreadNum = ENUM_MAX_THREAD_NUM;
		m_bThreadMark.SetSize(m_nMaxThreadNum);
        m_nThreadNum = 0;
		m_bForcedStop = 0;
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
		return m_bThreadMark.isEmpty();
    }
    BOOL isRunning() {
        //return m_bThreadMark != 0;
		return !(m_bThreadMark.isEmpty());
    }
    //==
    BOOL Start(unsigned int nThreadNum = 0) {
		if( isRunning() ) return TRUE;

		if( nThreadNum>m_nMaxThreadNum ){
			m_nThreadNum = m_nMaxThreadNum;
		} else if( nThreadNum>0 ){
			m_nThreadNum = nThreadNum;
		}
		else{
			;//nothing to do!
		}
		m_bForcedStop = 0;  //no set force stop!

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
    BOOL Stop() {//force stop
        if (isRunning()) {
			m_bForcedStop = 1; //force stop now!
			//chain one by one to stop!
			PostQueuedCompletionStatus(m_hRequestQueue, 0, 0, THREAD_POOL_SHUTDOWN);
        } 
		
        return TRUE;
    }

    //==
    BOOL Initialize(void *pvWorkerParam = NULL, int nNumCores=0) {
		//==
		if( nNumCores>0 )
		{
			m_nMaxThreadNum = nNumCores * ENUM_THREADNUM_PER_CORE;
			m_bThreadMark.SetSize(m_nMaxThreadNum);
			m_bThreadMark.ClearAll();
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
template<class T, bool bAutoExit=false, T EndRequest = 0>
class CWorkerBase{
public:
	typedef T RequestType;
	//typedef T ThreadParam;
	//typedef T* PThreadParam;

public:
	static bool isAutoExitThread(){
		return bAutoExit;
	}
	static RequestType GetAutoExitRequest(){
		return EndRequest;
	}

public:
	BOOL Initialize( void * /*pvParam*/, bool bFirstStartup = false) 
	{
		return TRUE;
	}

	void Execute(
		 RequestType request,
		 void *pvWorkerParam,
		 OVERLAPPED *pOverlapped)
	{

	}

	void Terminate( void* /*pvParam*/, bool bLastExecute = false) 
	{
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
