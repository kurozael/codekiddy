
/*
    Copyright 2011 Aevum Software aevum @ aevumlab.com

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

    @author Victor Vicente de Carvalho victor.carvalho@aevumlab.com
    @author Ozires Bortolon de Faria ozires@aevumlab.com
*/

#ifndef CBUFFER_H
#define CBUFFER_H

#include <vector>
#include <sstream>
#include <string.h>
#include <cassert>
#include <cstdlib>
#include <stdexcept>
#include <memory>

#define gdx_shared_ptr std::shared_ptr
#define gdx_unique_ptr std::unique_ptr
#define gdx_weak_ptr std::weak_ptr
#define gdx_enable_shared_from_this std::enable_shared_from_this
#define gdx_static_pointer_cast std::static_pointer_cast

struct shared_ptr_array_deleter {
    template <typename T>
    void operator ()(T* array) {
        delete [] array;
    }	
};

struct shared_ptr_free_deleter {
    template <typename T>
    void operator ()(T* array) {
        free(array);
    }
};

template <typename T>
struct ref_ptr_maker
{
public:
  typedef gdx_shared_ptr<T> shared_ptr_def;
  typedef gdx_weak_ptr<T> weak_ptr_def;
  typedef gdx_unique_ptr<T> unique_ptr_def;
};

struct null_shared_ptr {
public:
    template<typename T>
    operator gdx_shared_ptr<T>() {
        return gdx_shared_ptr<T>();
    }
};

struct buffer_base {
public:
    typedef ref_ptr_maker<char>::shared_ptr_def char_ptr;
    buffer_base(char_ptr bf, int capacity, int position, int mark, int limit)
            : bf(bf)
            , _capacity(capacity)
            , _position(position)
            , _mark(mark)
            , _limit(limit)
    {
    }

    buffer_base(const buffer_base& other)
            : bf(other.bf), _capacity(other._capacity), _position(other._position),
            _mark(other._mark), _limit(other._limit)
    {
    }

    buffer_base() : _capacity(0)
            , _position(0)
            , _mark(-1)
            , _limit(0) {
    }

    char_ptr bf;
    int _capacity;
    int _position;
    int _mark;
    int _limit;
};

template <typename T>
struct buffer : public buffer_base {
    buffer(int mark, int pos, int lim , int capacity)
        : buffer_base(buffer_base::char_ptr(new char[capacity], shared_ptr_array_deleter()), capacity, pos, mark, lim)
    {
		#ifdef DEBUG
				T* debug_buffer = (T*) bf.get();
		#endif

        limit(lim);
        position(pos);
        if (mark>= 0) {
            if (mark> pos) {
                std::stringstream ss;
                ss <<" mark> position: (" <<mark <<"> " <<pos + ")";
                throw std::runtime_error(ss.str());
            }
        }
    }

    buffer(buffer_base::char_ptr bf, int mark, int pos, int lim, int capacity)
            : buffer_base(bf , capacity, pos, mark, lim)
    {
		#ifdef DEBUG
				T* debug_buffer = (T*) bf.get();
		#endif

        limit(lim);
        position(pos);
        if (mark>= 0) {
            if (mark> pos) {
                std::stringstream ss;
                ss <<" mark> position: (" <<mark <<"> " <<pos + ")";
                throw std::runtime_error(ss.str());
            }
        }
    }

    buffer() : buffer_base() {
    }

    int capacity() {
        return _capacity;
    }

    int position() {
        return _position;
    }

    template <typename Other>
    buffer<Other> convert() {
        int new_cap = this->remaining()>> (sizeof(Other)> 4 ? 3 : sizeof(Other) <= 2 ? 1 : 2);
        return buffer<Other>(this->bf, -1, 0, new_cap, new_cap);
    }

    void put(const T& value) {
        ((T*)bf.get())[nextPutIndex()] = value;
    }

    T& get() {
        return ((T*)bf.get())[nextGetIndex()];
    }

    T& get(const int position) {
        return ((T*)bf.get())[checkIndex(position)];
    }

    buffer<T>& get(T* dst, int dstSize, int offset, int length) {
        checkBounds(offset, length, dstSize);
        if (length> remaining())
            throw std::runtime_error("buffer underflow");
        int end = offset + length;
        for (int i = offset; i <end; i++)
            dst[i] = get();
        return *this;
    }

    buffer<T>& get(std::vector<T>& dst, int offset, int length) {
        return get(&dst[0], dst.size(), offset, length);
    }

    T& operator[](int position) {
        return ((T*)bf.get())[checkIndex(position)];
    }

    void eraseBuffer() {
        memset(bf.get(), 0, sizeof(T) * _capacity);
    }
    
    buffer<T>& position(int newPosition) {
        if ((newPosition> _limit) || (newPosition <0))
            throw std::runtime_error("");
        _position = newPosition;
        if (_mark> _position) clearMark();
        return *this;
    }

    int limit() {
        return _limit;
    }

    buffer<T>& limit(int newLimit)
    {
        if ((newLimit> _capacity) || (newLimit <0))
            throw std::runtime_error("");

        _limit = newLimit;
        if (_position> _limit) _position = _limit;
        if (_mark> _position) clearMark();
        return *this;
    }

    template <typename U>
    void copy(const U* array, int count, int offset) {
        T* casted_array = (T*) bf.get();
        memcpy(casted_array, &array[offset], sizeof(U) * count);
        limit((count * sizeof(U)) / sizeof(T));
    }

    template <typename U>
    void copy(const std::vector<U>& array, int count, int offset) {
        T* casted_array = (T*) bf.get();
        memcpy(casted_array, &array[offset], sizeof(U) * count);
        limit((count * sizeof(U)) / sizeof(T));
    }

    buffer<T>& compact() {
        int pos = position();
        int lim = limit();
        assert (pos <= lim);
        int rem = (pos <= lim ? lim - pos : 0);
		
        memmove(bf.get(), ((T*) bf.get()) + position(), rem * sizeof(T));

        position(rem);
        limit(capacity());
        discardMark();
        
        return *this;
    }

    buffer<T>& reset() {
        int m = _mark;
        if (m <0)
            throw std::runtime_error("buffer underflow");
        _position = m;
        return *this;
    }
	
    template <typename U>
    operator U*() {
        return (U*) bf.get();
    }

    buffer<T>& mark() {
        _mark = _position;
        return *this;
    }

    buffer<T>& clear() {
        _position = 0;
        _limit = _capacity;
        clearMark();
        return *this;
    }

    buffer<T>& flip() {
        _limit = _position;
        _position = 0;
        clearMark();
        return *this;
    }

    buffer<T>& rewind() {
        _position = 0;
        clearMark();
        return *this;
    }

    int remaining() {
        return _limit - _position;
    }

    bool hasRemaining() {
        return _position <_limit;
    }

    buffer<T>& put(const T* src, int size, int offset, int length) {
        checkBounds(offset, length, size);
        if (length> remaining())
            throw std::runtime_error("buffer overflow");

        T* casted_array = (T*) bf.get();
        memcpy(&casted_array[_position], &src[offset], sizeof(T) * length);
        
        _position += length;
        
        return *this;
    }

    buffer<T>& put(const T* src, int size) {
        return put(src, size, 0, size);
    }

    buffer<T>& put(const std::vector<T>& src) {
        return put(src, 0, src.size());
    }

    buffer<T>& put(const std::vector<T>& src, int offset, int length) {
        checkBounds(offset, length, src.size());
        if (length> remaining())
            throw std::runtime_error("buffer overflow");

        T* casted_array = (T*) bf.get();
        memcpy(&casted_array[_position], &src[offset], sizeof(T) * length);
        
        _position += length;
        
        return *this;
    }

    buffer(const buffer_base& other)
            : buffer_base(other)
    {
    }

    int nextGetIndex() {
        if (_position>= _limit)
            throw std::runtime_error("buffer underflow");
        return _position++;
    }

    int nextGetIndex(int nb) {
        if (_limit - _position <nb)
            throw std::runtime_error("buffer underflow");
        int p = _position;
        _position += nb;
        return p;
    }

    int nextPutIndex() {
        if (_position>= _limit)
            throw std::runtime_error("buffer overflow");
        return _position++;
    }

    int nextPutIndex(int nb) {
        if (_limit - _position <nb)
            throw std::runtime_error("buffer overflow");
        int p = _position;
        _position += nb;
        return p;
    }

    int checkIndex(int i) {
        if ((i <0) || (i>= _limit))
            throw std::runtime_error("invalid index");
        return i;
    }

    int checkIndex(int i, int nb) {
        if ((i <0) || (nb> _limit - i))
            throw std::runtime_error("wrong index");
        return i;
    }

    void discardMark() {
        _mark = -1;
    }

    static void checkBounds(int off, int len, int size) {
        if ((off | len | (off + len) | (size - (off + len))) <0)
            throw std::runtime_error("index out of bounds");
    }

    int _markValue() {
        return _mark;
    }

    void clearMark() {
        _mark = -1;
    }
protected:
	#ifdef DEBUG
		T* debug_buffer;
	#endif
};

template <typename T>
struct default_buffer : public buffer<T> {
    default_buffer(int capacity) :
            buffer<T>(-1, 0, capacity, capacity)
    {
    }

    default_buffer() : buffer<T>() {
    }

    default_buffer(const buffer_base& other) :
            buffer<T>(other)
    {
    }
};

typedef default_buffer<char> byte_buffer;
typedef default_buffer<short> short_buffer;
typedef default_buffer<float> float_buffer;
typedef default_buffer<int> int_buffer;

#endif
