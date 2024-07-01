# Treiber Stack

[Lock-Free Treiber Stack](https://en.wikipedia.org/wiki/Treiber_stack).

## Memory management

Reference counting is used for memory menagment.

The reference count of the top of the stack is divided into positive and negative parts:

- The negative component is stored inside the node
- Positive - right at the pointer to the top of the stack

### `AtomicStampedPtr`

Class [`AtomicStampedPtr<T>`](atomic_stamped_ptr.hpp), stores a pointer + 16-bit counter in the form of one machine word 
and can perform standard atomic operations on this pair:

```cpp
AtomicStampedPtr<T> asp({nullptr, 0});

asp.Store({raw_ptr, 7});
auto stamped_ptr = asp.Load();
// Method `IncrementCount` returnы new `StampedPtr`, 
// in which the counter is incremented by one
bool succeeded = asp.CompareExchangeWeak(stamped_ptr, stamped_ptr.IncrementCount());
```
The counter in `AtomicStampedPtr` has no independent meaning.
