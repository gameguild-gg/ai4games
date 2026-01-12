#include "MemoryLeakDetector.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>

// Dynamic allocation tracking to avoid overflow issues
static allocation_tracker* allocations = nullptr;
static size_t total_allocated = 0;
static size_t current_usage = 0;
static size_t peak_usage = 0;
static bool initialized = false;

static void cleanup_and_report() {
  if (!initialized || !allocations)
    return;

  fprintf(stderr, "\n=== Memory Usage Statistics ===\n");
  fprintf(stderr, "Total memory allocated: %zu bytes\n", total_allocated);
  fprintf(stderr, "Peak memory usage: %zu bytes\n", peak_usage);
  fprintf(stderr, "Current memory usage: %zu bytes\n", current_usage);

  if (allocations->count > 0) {
    fprintf(stderr, "\n=== Memory Leaks Detected ===\n");
    for (std::size_t i = 0; i < allocations->count; ++i) {
      fprintf(stderr, "LEAK: %zu bytes at address %p\n", allocations->sizes[i], allocations->ptrs[i]);
    }
    fprintf(stderr, "Total leaked: %zu bytes in %zu allocation(s)\n",
            current_usage, allocations->count);
  } else {
    fprintf(stderr, "\nNo memory leaks detected!\n");
  }
  fprintf(stderr, "===============================\n");
  
  // Free the tracking arrays and structure
  if (allocations->ptrs) std::free(allocations->ptrs);
  if (allocations->sizes) std::free(allocations->sizes);
  std::free(allocations);
  allocations = nullptr;
}

static void ensure_initialized() {
  if (!initialized) {
    // Use malloc to avoid circular dependency
    allocations = (allocation_tracker*)std::malloc(sizeof(allocation_tracker));
    allocations->ptrs = nullptr;
    allocations->sizes = nullptr;
    allocations->count = 0;
    allocations->capacity = 0;
    total_allocated = 0;
    current_usage = 0;
    peak_usage = 0;
    initialized = true;
    atexit(cleanup_and_report);
  }
}

static void track_allocation(void *ptr, size_t size) {
  if (!ptr || !allocations)
    return;

  // Grow arrays if needed
  if (allocations->count >= allocations->capacity) {
    std::size_t new_capacity = (allocations->capacity == 0) ? 100 : allocations->capacity * 2;
    
    void** new_ptrs = (void**)std::realloc(allocations->ptrs, new_capacity * sizeof(void*));
    std::size_t* new_sizes = (std::size_t*)std::realloc(allocations->sizes, new_capacity * sizeof(std::size_t));
    
    if (!new_ptrs || !new_sizes) {
      // Allocation failed, can't track this allocation
      return;
    }
    
    allocations->ptrs = new_ptrs;
    allocations->sizes = new_sizes;
    allocations->capacity = new_capacity;
  }
  
  allocations->ptrs[allocations->count] = ptr;
  allocations->sizes[allocations->count] = size;
  allocations->count++;

  total_allocated += size;
  current_usage += size;
  if (current_usage > peak_usage) {
    peak_usage = current_usage;
  }
}

static void untrack_allocation(void *ptr) {
  if (!ptr || !allocations)
    return;

  for (std::size_t i = 0; i < allocations->count; ++i) {
    if (allocations->ptrs[i] == ptr) {
      std::size_t size = allocations->sizes[i];
      
      // Move last element to this position
      allocations->ptrs[i] = allocations->ptrs[allocations->count - 1];
      allocations->sizes[i] = allocations->sizes[allocations->count - 1];
      allocations->count--;
      
      if (current_usage >= size) {
        current_usage -= size;
      } else {
        current_usage = 0;
      }
      break;
    }
  }
}

// Dummy implementations for compatibility
track_type *get_map() {
  static track_type dummy;
  return &dummy;
}

memory_stats *get_stats() {
  static memory_stats dummy;
  return &dummy;
}

void *operator new(std::size_t size) noexcept(false) {
  ensure_initialized();

  std::size_t actual_size = (size == 0) ? 1 : size;
  void *mem = std::malloc(actual_size);
  if (!mem)
    throw std::bad_alloc();

  if (initialized) {
    track_allocation(mem, actual_size);
  }
  return mem;
}

void operator delete(void *mem) noexcept {
  if (!mem)
    return;

  if (initialized) {
    untrack_allocation(mem);
  }
  std::free(mem);
}

void *operator new[](std::size_t size) noexcept(false) {
  return operator new(size);
}

void operator delete[](void *mem) noexcept { operator delete(mem); }

void print_memory_report() { cleanup_and_report(); }