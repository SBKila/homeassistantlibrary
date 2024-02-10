#pragma once
#include <Arduino.h>
#include "debug.h"

template <typename T>
class BufferFIFO
{
protected:
    boolean isFull = false;
    int m_size = 0;
    int m_index = -1;
    T *m_buffer;
    T m_overflow;
    int nextIndex(int index);

public:
    BufferFIFO(int size);
    ~BufferFIFO();
    const T *push(T data);
    void clear();
};

// Initialize LinkedList with false values
template <typename T>
BufferFIFO<T>::BufferFIFO(int size)
{
    m_size = size;
    m_buffer = (T *)calloc(size, sizeof(T));
};
template <typename T>
BufferFIFO<T>::~BufferFIFO() { free(m_buffer); };

template <typename T>
int BufferFIFO<T>::nextIndex(int index)
{
    int nextIndex = index + 1;
    if (nextIndex == m_size)
    {
        nextIndex = 0;
        isFull = true;
    };
    return nextIndex;
}

template <typename T>
const T *BufferFIFO<T>::push(T data)
{
    m_index = nextIndex(m_index);
    m_overflow = m_buffer[m_index];
    m_buffer[m_index] = data;
    return (isFull) ? &m_overflow : NULL;
}

template <typename T>
void BufferFIFO<T>::clear()
{
    m_size = 0;
    m_index = -1;
    isFull = false;
}