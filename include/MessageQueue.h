#ifndef D_MESSAGE_QUEUE_H
#define D_MESSAGE_QUEUE_H

#include "Errorable.h"
#include "UtilTime.h"

#include <queue>
#include <pthread.h>

#ifndef PTHREAD_MUTEX_RECURSIVE_NP
#define PTHREAD_MUTEX_RECURSIVE_NP PTHREAD_MUTEX_RECURSIVE
#endif

template<typename MsgType>
class MessageQueue {
public:
    typedef typename std::queue<MsgType>::size_type size_type;

    static const size_type DEFAULT_QUEUE_MAX = 20;

private:
    std::queue<MsgType> queue_;
    mutable pthread_mutex_t guard_;

    pthread_cond_t send_event_;
    pthread_cond_t recv_event_;

    const size_type queue_max_;

    /* コピー・ムーブは削除 */
    MessageQueue(MessageQueue const&) = delete;
    MessageQueue(MessageQueue &&) = delete;
    MessageQueue& operator=(MessageQueue const&) = delete;
    MessageQueue& operator=(MessageQueue &&) = delete;

    void queue_lock_init()
    {
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);

        pthread_mutex_init(&guard_, &attr);

        pthread_mutexattr_destroy(&attr);
    }

    void queue_lock() const
    {
        pthread_mutex_lock(&guard_);
    }

    void queue_unlock() const
    {
        pthread_mutex_unlock(&guard_);
    }

    bool send_critical( const MsgType& message )
    {
        /* メッセージキューに登録 */
        queue_.push(message);
        pthread_cond_signal(&send_event_);
        return true;
    }

    Errorable<MsgType> recv_critical()
    {
        /* メッセージキューから取り出して削除 */
        MsgType message = queue_.front();
        queue_.pop();
        pthread_cond_signal(&recv_event_);
        return message;
    }

public:
    MessageQueue()
        : queue_max_(DEFAULT_QUEUE_MAX)
    {
        queue_lock_init();
        pthread_cond_init(&send_event_, NULL);
        pthread_cond_init(&recv_event_, NULL);
    }

    explicit
    MessageQueue(size_type max_size)
        : queue_max_(max_size)
    {
        queue_lock_init();
        pthread_cond_init(&send_event_, NULL);
        pthread_cond_init(&recv_event_, NULL);
    }

    virtual ~MessageQueue()
    {
        pthread_cond_destroy(&send_event_);
        pthread_cond_destroy(&recv_event_);
        pthread_mutex_destroy(&guard_);
    }

    bool send( const MsgType& message )
    {
        if(this->full())
            return false;

        /* 送信 */
        queue_lock();
        auto result = send_critical(message);
        queue_unlock();

        return result;
    }

    bool send_wait( const MsgType& message, const UtilTime& time )
    {
        queue_lock();

        /* キューに空きができるまで待つ */
        if(this->full()) {
            UtilTime expect = UtilTime::now() + time;

            /* 指定時刻まで待つ */
            int err = pthread_cond_timedwait(&recv_event_, &guard_, &expect);

            /* タイムアウト */
            if(err == ETIMEDOUT) {
                queue_unlock();
                return false;
            }

        }

        /* 送信 */
        auto result = send_critical(message);

        queue_unlock();
        return result;
    }

    bool send_wait( const MsgType& message )
    {
        queue_lock();

        /* キューに空きができるまで待つ */
        while(this->full())
            pthread_cond_wait(&recv_event_, &guard_);

        /* 送信 */
        auto result = send_critical(message);

        queue_unlock();
        return result;
    }

    Errorable<MsgType> recv()
    {
        if(this->empty())
            return Error<std::string>("no message");

        /* 受信 */
        queue_lock();
        auto message = recv_critical();
        queue_unlock();

        return message;
    }

    Errorable<MsgType> recv_wait()
    {
        queue_lock();

        /* 受診するまで待つ */
        while(this->empty())
            pthread_cond_wait(&send_event_, &guard_);

        /* 受信 */
        auto message = recv_critical();

        queue_unlock();
        return message;
    }

    Errorable<MsgType> recv_wait(const UtilTime& time)
    {
        queue_lock();

        if(this->empty()) {
            UtilTime expect = UtilTime::now() + time;

            /* 指定時刻まで待つ */
            int err = pthread_cond_timedwait(&send_event_, &guard_, &expect);

            /* タイムアウト */
            if(err == ETIMEDOUT) {
                queue_unlock();
                return Error<std::string>("time out");
            }
        }

        /* 受信 */
        auto message = recv_critical();

        queue_unlock();
        return message;
    }

    size_type size() const
    {
        queue_lock();
        auto ret = queue_.size();
        queue_unlock();
        return ret;
    }

    bool empty() const
    {
        queue_lock();
        auto ret = queue_.empty();
        queue_unlock();
        return ret;
    }

    size_type max() const
    {
        return queue_max_;
    }

    bool full() const
    {
        return !(this->size() < this->max());
    }
};

#endif
