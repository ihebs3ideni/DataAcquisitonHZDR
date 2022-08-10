//
// Created by iheb on 07.05.2022.
//

#ifndef HZDR_DATA_ACQUISITION_RINGBUFFER_H
#define HZDR_DATA_ACQUISITION_RINGBUFFER_H

#include <vector>
#include <mutex>
#include <memory>



template <class T>
class simple_RingBuffer{
    /* a rapper around the vector API making it effectively a ring buffer*/

public:
    simple_RingBuffer(int size = 1000) :
            buf_(std::shared_ptr<T[]>(new T[size])),
            max_size_(size)
    {
        std::cout << "Buffer_Capacity:" << this->capacity() << "; Buffer_size:" << this->size() << "\n";

    }
    simple_RingBuffer(simple_RingBuffer<T>&& rb) noexcept :
            buf_(std::move(rb.buf_)), max_size_(std::move(rb.max_size_))
    {
    }
    ~simple_RingBuffer(){
    }

//    bool put(const T& item)
//    {
//        std::lock_guard<std::mutex> lock(mutex_);
//        buf_[head_] = item;
//        if(full_)
//        {
//            tail_ = (tail_ + 1) % max_size_;
//        }
//
//        head_ = (head_ + 1) % max_size_;
//
//        full_ = head_ == tail_;
//        return true;
//    }

    ///returns the tail index
    unsigned push_back(const T& item){
        std::lock_guard<std::mutex> lock(mutex_);
        buf_[head_] = item;
        if(full_)
        {
            tail_ = (tail_ + 1) % max_size_;
        }

        head_ = (head_ + 1) % max_size_;

        full_ = head_ == tail_;
        return tail_;
    }


    T& get(int index) const
    {
        std::lock_guard<std::mutex> lock(mutex_);

        if(empty())
        {
            exit(0);
        }
        index = (index + tail_) % max_size_;
        //Read data and advance the tail (we now have a free space)
        auto& val = buf_[index];
        full_ = head_ == tail_;
        return val;
    }

    T& get() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if(empty())
        {
            exit(0);
        }
        //get index of last item
        auto i = head_ - 1 ;
        if (head_ == 0){
            i = this->size()-1;
        }
        //int & val = buf_[i];
        full_ = head_ == tail_;
        return buf_[i];
    }

//    T* get_ptr() const{
//        std::lock_guard<std::mutex> lock(mutex_);
//        if(empty())
//        {
//            exit(0);
//        }
//        //get index of last item
//        auto i = head_ - 1 ;
//        if (head_ == 0){
//            i = this->size()-1;
//        }
//        auto* val = &buf_[i];
//        full_ = head_ == tail_;
//        return val;
//    }

    std::vector<T> get_block(unsigned size){
        std::lock_guard<std::mutex> lock(mutex_);
        std::vector<T> block;
        if(this->size()< size) size = this->size();
//        block.reserve(size);
        for(int i=this->size()-size; i<this->size(); i++)
            block.push_back(buf_[(i + tail_) % max_size_]);

        return block;

    }
//
//    std::vector<T> to_vector(){
//        auto* ptr = buf_.get();
//        std::vector<T> block(ptr, std::end(ptr));
//        return block;
//    }

//    T get_copy(int index){
//        std::lock_guard<std::mutex> lock(mutex_);
//        if(empty())
//        {
//            exit(0);
//        }
//        index = (index + tail_) % max_size_;
//        //Read data and advance the tail (we now have a free space)
//        auto val = buf_[index];
//        full_ = head_ == tail_;
//        return val;
//    }

    void reset()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        head_ = tail_;
        full_ = false;
    }

    bool empty() const
    {
        //if head and tail are equal, we are empty
        return (!full_ && (head_ == tail_));
    }

    bool full() const
    {
        //If tail is ahead the head by 1, we are full
        return full_;
    }

    int capacity() const
    {
        return max_size_;
    }

    int size() const
    {
        int size = max_size_;
        if(!full_)
        {
            if(head_ >= tail_)
            {
                size = head_ - tail_;
            }
            else
            {
                size = max_size_ + head_ - tail_;
            }
        }
        size_ = size;
        return size;
    }

//    void lock_buffer(){
//        mutex_.lock();
//    }
//
//    void unlock_buffer(){
//        mutex_.unlock();
//    }


private:
    mutable std::mutex mutex_;
    std::shared_ptr<T[]> buf_;
    unsigned head_ = 0;
    unsigned tail_ = 0;
    mutable int size_ = 0;
    int max_size_;
    mutable bool full_ = false;
};


void test_rb(){
    simple_RingBuffer<int> rb(50);
    for (int i=0; i<100; i++){
        rb.push_back(i);
        std::cout << rb.get() << "; ";
    }
}

#endif //HZDR_DATA_ACQUISITION_RINGBUFFER_H
