/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpoetess <bpoetess@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/09 17:06:15 by bpoetess          #+#    #+#             */
/*   Updated: 2023/01/09 19:32:23 by bpoetess         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>

#ifndef VECTOR_CPP
#define VECTOR_CPP

namespace ft {
template<
    class T,
    class Allocator = std::allocator<T>
> class vector{
private:
  size_t allocatedElements;
  size_t constructedElements;
  T *array;

public:
  typedef T value_type;
  typedef Allocator allocator_type;
  typedef value_type &reference;
  typedef const T& const_reference;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;
  typedef typename Allocator::pointer pointer;
  typedef typename Allocator::const_pointer const_pointer;

  typedef T* iterator;

  size_type capacity() const { return (this->allocatedElements); }

  size_type size() const { return (this->constructedElements); }
  size_type max_size() const {
    return (std::numeric_limits<difference_type>::max());
  }
  allocator_type get_allocator() const {Allocator alloc; return (alloc); }
  bool empty() const {
    if (this->constructedElements > 0)
      return (true);
    return (false);
  }

  void reserve(size_type new_cap) {
    T *ptrTmp;
    Allocator alloc;

    if (new_cap <= this->constructedElements)
      return;
    if (new_cap > max_size())
      throw(std::length_error("length_error"));
    ptrTmp = alloc.allocate(new_cap);
    for (size_t i = 0; i < this->constructedElements; i++) {
      try {
        alloc.construct(ptrTmp + i, this->array[i]);
      } catch (const std::exception &e) {
        for (size_t j = 0; j < i; j++)
          alloc.destroy(ptrTmp + j);
        alloc.deallocate(ptrTmp, new_cap);
        throw(e);
      }
    }
    for (size_t i = 0; i < this->constructedElements; i++)
      alloc.destroy(this->array + i);
    alloc.deallocate(this->array, this->capacity());
    this->array = ptrTmp;
    this->allocatedElements = new_cap;
  }

  void resize(size_type count, T value = T()) {
    Allocator alloc;

    if (count == this->constructedElements)
      return;

    if (count > this->allocatedElements)
      reserve(count);
    else if (count < this->constructedElements) {
      for (size_t i = this->constructedElements; i < count; i++)
        alloc.destroy(this->array + i);
      this->constructedElements = count;
      return ;
    }

    for (size_t i = this->constructedElements; i < count; i++)
      this->array[i] = T(value);
    this->constructedElements = count;
  }
  void resize(size_type count) { this->resize(count, T()); }

  void clear() {
    Allocator alloc;

    for (size_t i = 0; i < this->constructedElements; i++)
      alloc.destroy(this->array + i);
    this->constructedElements = 0;
  }

  reference at(size_type pos) {
    if (!(pos < this->allocatedElements))
      throw(std::out_of_range("out of range"));
    return (&this->array[pos]);
  }
  const_reference at( size_type pos ) const {
    if (!(pos < this->allocatedElements))
      throw(std::out_of_range("out of range"));
    return (&this->array[pos]);
  }

  reference back() {
    if (!this->constructedElements)
      return(T());
    return(&this->array[this->constructedElements]);
  }
  const_reference back() const {
    if (!this->constructedElements)
      return(T());
    return(&this->array[this->constructedElements]);
  }

  reference front() {
    if (!this->constructedElements)
      return(T());
    return(&this->array[0]);
  }
  const_reference front() const {
    if (!this->constructedElements)
      return(T());
    return(&this->array[0]);
  }

  T* data() {
    if (!this->capacity)
      return (NULL);
    return (this->array);
  }
  const T* data() const {
    if (!this->capacity)
      return (NULL);
    return (this->array);
  }

  vector<T> &operator=(vector<T> const &obj) {
    Allocator alloc;

    this->clear();
    alloc.deallocate(this->array, this->capacity());
    this->allocatedElements = 0;
    this->constructedElements = 0;
    this->array = NULL;
    this->reserve(obj.allocatedElements);
    for (size_t i = 0; i < obj.constructedElements; i++) {
      try {
        alloc.construct(this->array + i, obj.array[i]);
      } catch (const std::exception &e) {
        for (size_t j = 0; j < i; j++)
          alloc.destroy(this->array + j);
        alloc.deallocate(this->array, obj.allocatedElements);
        this->array = NULL;
        this->allocatedElements = 0;
        this->constructedElements = 0;
        throw(e);
      }
    }
    return (*this);
  }

  reference operator[](size_type pos) {
    if (this->allocatedElements - 1 < pos)
      return (&array[pos]);
    return (T());
  }
  const_reference operator[](size_type pos) const {
    if (this->allocatedElements - 1 < pos)
      return (&array[pos]);
    return (T());
  }
  
  vector() {
    this->allocatedElements = 0;
    this->constructedElements = 0;
    this->array = NULL;
  }

  ~vector() {
    Allocator alloc;

    this->clear();
    if (this->array)
      alloc.deallocate(this->array, this->capacity());
  }

};
} // namespace ft

#endif