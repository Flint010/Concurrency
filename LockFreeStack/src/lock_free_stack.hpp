#pragma once

#include "atomic_stamped_ptr.hpp"
#include <atomic>
#include <memory>

#include <optional>

// Treiber unbounded lock-free stack
// https://en.wikipedia.org/wiki/Treiber_stack

template <typename T>
class LockFreeStack {
  struct Node {
    std::shared_ptr<T> value;
    std::atomic<int> internal_cnt;
    
    StampedPtr<Node> next;
    Node(const T& val)
      : value(std::make_shared<T>(val)), internal_cnt(0) {};
      
    Node(T&& val)
      : value(std::make_shared<T>(std::move(val))), internal_cnt(0) {};

  };

 public:
  LockFreeStack(): head_(nullptr) {};
	
  ~LockFreeStack() {
    while (TryPop());
  }
  
  void Push(const T& val) {
    StampedPtr<Node> new_node = {new Node(val), 1};

    new_node->next = head_.Load();

    while(!head_.CompareExchangeWeak(new_node->next, new_node));
  }

  void Push(T&& val) {
    StampedPtr<Node> new_node = {new Node(std::move(val)), 1};

    new_node->next = head_.Load();

    while(!head_.CompareExchangeWeak(new_node->next, new_node));
  }

  std::shared_ptr<T> TryPop() {
    StampedPtr<Node> old_head = head_.Load();
  
    while (true) {
      increment_cnt(old_head);
    
      if (!old_head.raw_ptr) {
        return {};
      }

      Node* ptr = old_head.raw_ptr;

      if (head_.CompareExchangeStrong(old_head, old_head->next)) {
        std::shared_ptr<T> data;
        data.swap(old_head->value); 
      
        int external_cnt = old_head.stamp - 2;
    
        if (old_head->internal_cnt.fetch_add(external_cnt) == -external_cnt) {
          delete old_head.raw_ptr;
        }
        return data;

      } else if (ptr->internal_cnt.fetch_sub(1) == 1) {
        delete ptr;
      }
    }
  }


 private:
  void increment_cnt(StampedPtr<Node>& old_ptr) {
    StampedPtr<Node> inc_ptr;
    do {
      inc_ptr = old_ptr.IncrementStamp();
    } while (!head_.CompareExchangeStrong(old_ptr, inc_ptr));
    
    old_ptr.stamp = inc_ptr.stamp;
  }

 private:
  AtomicStampedPtr<Node> head_;
};
