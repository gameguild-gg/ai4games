#ifndef MemoryLeakTracker_h
#define MemoryLeakTracker_h

#include <exception>
#include <iostream>
#include <map>
#include <memory>

template <typename T> struct track_alloc : std::allocator<T> {
  template <typename U> struct rebind {
    typedef track_alloc<U> other;
  };

  track_alloc() {}

  template <typename U>
  track_alloc(track_alloc<U> const &u) : std::allocator<T>(u) {}

  T *allocate(size_t size, const T * = 0) {
    void *p = std::malloc(size * sizeof(T));
    if (p == nullptr) {
      throw std::bad_alloc();
    }
    return static_cast<T *>(p);
  }

  void deallocate(T *p, size_t) { std::free(p); }
};

typedef std::map<void *, std::size_t, std::less<void *>,
                 track_alloc<std::pair<void *const, std::size_t>>>
    track_type;

// Dynamic allocation tracking using C-style arrays to avoid STL circular dependencies
struct allocation_tracker {
    void** ptrs;
    std::size_t* sizes;
    std::size_t count;
    std::size_t capacity;
    
    allocation_tracker() : ptrs(nullptr), sizes(nullptr), count(0), capacity(0) {}
};

struct memory_stats {
  std::size_t total_allocated =
      0; // Total memory allocated during program lifetime
  std::size_t current_usage = 0; // Current memory usage
  std::size_t peak_usage = 0;    // Peak memory usage

  void add_allocation(std::size_t size) {
    total_allocated += size;
    current_usage += size;
    if (current_usage > peak_usage) {
      peak_usage = current_usage;
    }
  }

  void remove_allocation(std::size_t size) { current_usage -= size; }
};

// Memory leak reporting is now handled via atexit() to avoid destructor order
// issues

track_type *get_map();
memory_stats *get_stats();
void print_memory_report();

#endif