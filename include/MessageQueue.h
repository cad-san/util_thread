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
    }

    explicit
    MessageQueue(size_type max_size)
        : queue_max_(max_size)
    {
        pthread_mutex_init(&guard_, NULL);
    }

    ~MessageQueue() {}

    bool send( const MsgType& message )
    {
        if(this->full())
            return false;

        /* メッセージキューに登録 */
        queue_lock();
        queue_.push(message);
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
