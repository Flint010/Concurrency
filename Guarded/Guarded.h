#pragma once
#include <mutex>
#include <memory>
#include <string>
#include <atomic>

template<typename T>
class RefCounter {
public:
	RefCounter(T* obj, std::mutex* mt) 
		: obj_(obj), cnt_(new std::atomic<size_t>(1)),  mutex_ref_(mt) {};

	RefCounter(const RefCounter& other) {
		cnt_ = other.cnt_;
		++(*cnt_);
		mutex_ref_ = other.mutex_ref_;
		obj_ = other.obj_;
	};

	~RefCounter() {
		if (cnt_->load() > 1) {
			mutex_ref_->unlock();
			--(*cnt_);
		} else {
			delete cnt_;
		}
	}

	void operator=(const RefCounter& other) = delete;
	void operator=(RefCounter&&) = delete;
	RefCounter(RefCounter&&) = delete;

	T* operator->() {
		return obj_;
	}

private:
	T* obj_;
	std::atomic<size_t>* cnt_;
	std::mutex* mutex_ref_;
};


template<typename T>
class Guarded
{
public:
	Guarded();
	RefCounter<T> operator->();
	
private:
	T object_;
	std::mutex mutex_;
	RefCounter<T> ref_;
};

template<typename T>
inline Guarded<T>::Guarded()
	: ref_(&object_, &mutex_)
{
}

template<typename T>
inline RefCounter<T> Guarded<T>::operator->()
{
	mutex_.lock();
	return ref_;
}
