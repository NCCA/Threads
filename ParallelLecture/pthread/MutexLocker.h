#ifndef MUTEXLOCKER_H__
#define MUTEXLOCKER_H__
#include <pthread.h>
    
// use the holder pattern to hold a mutex and lock on release

class MutexLocker 
{
  public:

    // constructor for a pointer: hold the mutex
    explicit MutexLocker (pthread_mutex_t *m) : m_mutex(m) 
    {
        pthread_mutex_lock(m_mutex);
    }

    // destructor: releases the object to which it refers (if any)
    ~MutexLocker() {pthread_mutex_unlock(m_mutex);}

    // assignment of new pointer
    MutexLocker& operator= (pthread_mutex_t* p) 
    {
      pthread_mutex_unlock(m_mutex);
      m_mutex = p;
      pthread_mutex_lock(m_mutex);
      return *this;
    }

    // release ownership of referenced object
    void release() 
    {
      pthread_mutex_unlock(m_mutex);
        m_mutex = 0;
    }

  private:
    pthread_mutex_t* m_mutex;    

    // no copying and copy assignment allowed
    MutexLocker (MutexLocker const&);
    MutexLocker operator= (MutexLocker const&);
};

#endif