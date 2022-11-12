// Common/DynamicBuffer.h

#ifndef __COMMON_DYNAMICBUFFER_H
#define __COMMON_DYNAMICBUFFER_H

#include "Buffer.h"

template <class T> class CDynamicBuffer: public CBuffer<T>
{
  void GrowLength(size_t size)
  {
    size_t delta;
    if (this->_capacity > 64)
      delta = this->_capacity / 4;
    else if (this->_capacity > 8)
      delta = 16;
    else
      delta = 4;
    delta = MyMax(delta, size);
    SetCapacity(this->_capacity + delta);
  }
protected:
    size_t _capacity;
    T* _items;
public:
  CDynamicBuffer(): CBuffer<T>() {};
  CDynamicBuffer(const CDynamicBuffer &buffer): CBuffer<T>(buffer) {};
  CDynamicBuffer(size_t size): CBuffer<T>(size) {};
  CDynamicBuffer& operator=(const CDynamicBuffer &buffer)
  {
    this->Free();
    if (buffer._capacity > 0)
    {
      SetCapacity(buffer._capacity);
      memmove(this->_items, buffer._items, buffer._capacity * sizeof(T));
    }
    return *this;
  }
  void SetCapacity(size_t newCapacity)
  {
      if (newCapacity == _capacity)
          return;
      T* newBuffer;
      if (newCapacity > 0)
      {
          newBuffer = new T[newCapacity];
          if (_capacity > 0)
              memmove(newBuffer, _items, MyMin(_capacity, newCapacity) * sizeof(T));
      }
      else
          newBuffer = 0;
      delete[]_items;
      _items = newBuffer;
      _capacity = newCapacity;
  }
  void EnsureCapacity(size_t capacity)
  {
    if (this->_capacity < capacity)
      GrowLength(capacity - this->_capacity);
  }
};

typedef CDynamicBuffer<char> CCharDynamicBuffer;
typedef CDynamicBuffer<unsigned short> CWCharDynamicBuffer;
typedef CDynamicBuffer<unsigned char> CByteDynamicBuffer;

#endif
