/* Copyright (C) 2000-2020 by Massimiliano Ghilardi
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _TWIN_STL_ARRAY_H
#define _TWIN_STL_ARRAY_H

#include "stl/span.h"
#include "stl/alloc.h"

#include <string.h> // memcpy(), memset()

template <class T> class Array : private Span<T> {
private:
  typedef Span<T> Base;

protected:
  STL_USING Base::data_;
  STL_USING Base::size_;
  size_t cap_;

  bool init(size_t n) {
    data_ = reinterpret_cast<T *>(AllocMem0(sizeof(T) * n));
    if (n && !data_) {
      // AllocMem0() failed
      data_ = (T *)(size_t)-1;
      cap_ = size_ = 0;
      return false;
    }
    cap_ = size_ = n;
    return true;
  }

  void destroy() {
    if (data_ != (T *)(size_t)-1) {
      FreeMem(data());
    }
  }

  bool reallocate(size_t n) {
    if (cap_ < n && !reserve(n >= cap_ * 2 ? n : cap_ * 2)) {
      return false;
    }
    size_ = n;
    return true;
  }

public:
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef T value_type;
  typedef T &reference;
  typedef T *pointer;
  typedef T *iterator;
  typedef const T &const_reference;
  typedef const T *const_pointer;
  typedef const T *const_iterator;

  Array() : Base(), cap_(0) {
  }
  explicit Array(size_t n) : Base() {
    init(n);
  }
  Array(const T *mem, size_t n) : Base() {
    copy(mem, n);
  }
  explicit Array(const View<T> &other) : Base() {
    copy(other.data(), other.size());
  }
  explicit Array(const Span<T> &other) : Base() {
    copy(other.data(), other.size());
  }
  Array(const Array<T> &other) : Base() {
    copy(other.data(), other.size());
  }
  ~Array() {
    destroy();
  }

  Array<T> &operator=(const Array<T> &other) {
    copy(other.data(), other.size());
  }

  bool fail() const {
    return data_ == (T *)(size_t)-1;
  }
  size_t capacity() const {
    return cap_;
  }
  STL_USING Base::size;
  STL_USING Base::data;
  STL_USING Base::operator[];
  STL_USING Base::begin;
  STL_USING Base::end;

  bool copy(const T *mem, size_t n) {
    if (!reallocate(n)) {
      return false;
    }
    memcpy(data(), mem, n * sizeof(T));
    return true;
  }
  bool copy(const View<T> &other) {
    return copy(other.data(), other.size());
  }
  bool copy(const Span<T> &other) {
    return copy(other.data(), other.size());
  }
  bool copy(const Array<T> &other) {
    return copy(other.data(), other.size());
  }

  void clear() {
    (void)resize(0);
  }

  bool resize(size_t n) {
    if (!reallocate(n)) {
      return false;
    }
    if (n > size_) {
      memset(data() + size_, '\0', (n - size_) * sizeof(T));
    }
    size_ = n;
    return true;
  }

  bool reserve(size_t newcap) {
    if (newcap > cap_) {
      T *olddata = fail() ? (T *)0 : data();
      T *newdata = reinterpret_cast<T *>(ReAllocMem(olddata, newcap * sizeof(T)));
      if (!newdata) {
        if (cap_ == 0) {
          data_ = (T *)(size_t)-1;
        }
        return false;
      }
      data_ = newdata;
      cap_ = newcap;
    }
    return true;
  }
};

template <class T> void View<T>::ref(const Array<T> &other) {
  data_ = other.data();
  size_ = other.size();
}
template <class T> void Span<T>::ref(Array<T> &other) {
  data_ = other.data();
  size_ = other.size();
}

#endif /* _TWIN_STL_ARRAY_H */
