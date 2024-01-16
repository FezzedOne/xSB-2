#include "StarMemory.hpp"

#if defined STAR_USE_MIMALLOC
#include "mimalloc/mimalloc.h"
// #elifdef apparently isn't compatible with MSVC.
#elif defined STAR_USE_JEMALLOC
#include "jemalloc/jemalloc.h"
#endif

namespace Star {

#if defined STAR_USE_MIMALLOC
  void* malloc(size_t size) {
    return mi_malloc(size);
  }

  void* realloc(void* ptr, size_t size) {
    return mi_realloc(ptr, size);
  }

  void free(void* ptr) {
    mi_free(ptr);
  }

  void free(void* ptr, size_t size) {
    mi_free(ptr);
  }
#elif defined STAR_USE_JEMALLOC
  #if defined STAR_CROSS_COMPILE
  // FezzedOne: `jemalloc`'s stuff isn't renamed by default in the MinGW version off the AUR.
  void* malloc(size_t size) {
    return ::malloc(size);
  }

  void* realloc(void* ptr, size_t size) {
    return ::realloc(ptr, size);
  }

  void free(void* ptr) {
    ::free(ptr);
  }

  void free(void* ptr, size_t size) {
    if (ptr)
      ::sdallocx(ptr, size, 0);
  }
  #else
  void* malloc(size_t size) {
    return je_malloc(size);
  }

  void* realloc(void* ptr, size_t size) {
    return je_realloc(ptr, size);
  }

  void free(void* ptr) {
    je_free(ptr);
  }

  void free(void* ptr, size_t size) {
    if (ptr)
      je_sdallocx(ptr, size, 0);
  }
  #endif
#else
  void* malloc(size_t size) {
    return ::malloc(size);
  }

  void* realloc(void* ptr, size_t size) {
    return ::realloc(ptr, size);
  }

  void free(void* ptr) {
    return ::free(ptr);
  }

  void free(void* ptr, size_t) {
    return ::free(ptr);
  }
#endif

}

void* operator new(std::size_t size) {
  auto ptr = Star::malloc(size);
  if (!ptr)
    throw std::bad_alloc();
  return ptr;
}

void* operator new[](std::size_t size) {
  auto ptr = Star::malloc(size);
  if (!ptr)
    throw std::bad_alloc();
  return ptr;
}

// Globally override new and delete.  As the per standard, new and delete must
// be defined in global scope, and must not be inline.

void* operator new(std::size_t size, std::nothrow_t const&) noexcept {
  return Star::malloc(size);
}

void* operator new[](std::size_t size, std::nothrow_t const&) noexcept {
  return Star::malloc(size);
}

void operator delete(void* ptr) noexcept {
  Star::free(ptr);
}

void operator delete[](void* ptr) noexcept {
  Star::free(ptr);
}

void operator delete(void* ptr, std::nothrow_t const&) noexcept {
  Star::free(ptr);
}

void operator delete[](void* ptr, std::nothrow_t const&) noexcept {
  Star::free(ptr);
}

void operator delete(void* ptr, std::size_t size) noexcept {
  Star::free(ptr, size);
}

void operator delete[](void* ptr, std::size_t size) noexcept {
  Star::free(ptr, size);
}
