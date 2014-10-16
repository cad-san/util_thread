#ifndef D_MESSAGE_QUEUE_H
#define D_MESSAGE_QUEUE_H

#include "Errorable.h"

#include <queue>
#include <pthread.h>

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

    void queue_lock() const
    {
        pthread_mutex_lock(&guard_);
    }

    void queue_unlock() const
    {
        pthread_mutex_unlock(&guard_);
    }

public:
    MessageQueue()
        : queue_max_(DEFAULT_QUEUE_MAX)
    {
        pthread_mutex_init(&guard_, NULL);
        pthread_cond_init(&send_event_, NULL);
        pthread_cond_init(&recv_event_, NULL);
    }

    explicit
    MessageQueue(size_type max_size)
        : queue_max_(max_size)
    {
        pthread_mutex_init(&guard_, NULL);
        pthread_cond_init(&send_event_, NULL);
        pthread_cond_init(&recv_event_, NULL);
    }

    ~MessageQueue() {}

    bool send( const MsgType& message )
    {
        if(this->full())
            return false;

        /* メッセージキューに登録 */
        queue_lock();
        queue_.push(message);
        pthread_cond_signal(&send_event_);
        queue_unlock();

        return true;
    }

    bool send_wait( const MsgType& message )
    {
        queue_lock();

        /* キューに空きができるまで待つ */
        while(!(queue_.size() < this->max()))
            pthread_cond_wait(&recv_event_, &guard_);

        /* メッセージキューに登録 */
        queue_.push(message);
        pthread_cond_signal(&send_event_);

        queue_unlock();
        return true;
    }

    Errorable<MsgType> recv()
    {
        if(this->empty())
            return Error<std::string>("no message");

        /* メッセージキューから取り出して削除 */
        queue_lock();
        MsgType message = queue_.front();
        queue_.pop();
        pthread_cond_signal(&recv_event_);
        queue_unlock();

        return message;
    }

    Errorable<MsgType> recv_wait()
    {
        queue_lock();

        /* 受診するまで待つ */
        while(queue_.empty())
            pthread_cond_wait(&send_event_, &guard_);

        /* メッセージキューから取り出して削除 */
        MsgType message = queue_.front();
        queue_.pop();
        pthread_cond_signal(&recv_event_);

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
