#ifndef D_MESSAGE_QUEUE_H
#define D_MESSAGE_QUEUE_H

#include <queue>
#include <pthread.h>

template<typename MsgType>
class MessageQueue {
private:
    std::queue<MsgType> queue_;
    mutable pthread_mutex_t guard_;

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
    {
        pthread_mutex_init(&guard_, NULL);
    }

    ~MessageQueue() {}

    bool send( const MsgType& message )
    {
        queue_lock();

        /* メッセージキューに登録 */
        queue_.push(message);

        queue_unlock();
        return true;
    }

    MsgType recv()
    {
        queue_lock();

        /* メッセージキューから取り出して削除 */
        MsgType message = queue_.front();
        queue_.pop();

        queue_unlock();
        return message;
    }

    unsigned int size() const
    {
        queue_lock();
        unsigned int ret = queue_.size();
        queue_unlock();
        return ret;
    }

};

#endif
