#pragma once
#include <Arduino.h>
#include "debug.h"

template <typename T>
class BufferFIFO
{
protected:
    boolean m_isFull = false;
    int m_capacity = 0;
    int m_headIndex = -1; // Position of the latest element added
    T *m_buffer = nullptr;
    T m_overflowItem; // Stores the item that gets overwritten/pushed out

    // internal helper to calculate circular index
    int nextIndex(int index);

public:
    BufferFIFO() = default;
    ~BufferFIFO();

    // Allocate memory for the buffer
    void initialize(int size);

    // Adds data. Returns pointer to the overwritten item if buffer was full (for moving average subtraction), else NULL.
    const T *push(T data);

    // Resets the buffer state without freeing memory
    void clear();
};

// -------------------------------------------------------------------------
// Implementation
// -------------------------------------------------------------------------

template <typename T>
void BufferFIFO<T>::initialize(int size)
{
    // Safety: prevent memory leaks if initialized twice
    if (m_buffer != nullptr)
    {
        delete[] m_buffer;
    }

    m_capacity = size;
    // Use 'new' to ensure constructors are called for complex types
    m_buffer = new T[size];

    clear();
};

template <typename T>
BufferFIFO<T>::~BufferFIFO()
{
    if (m_buffer != nullptr)
    {
        delete[] m_buffer; // Matching delete[] for new[]
        m_buffer = nullptr;
    }
};

template <typename T>
int BufferFIFO<T>::nextIndex(int index)
{
    int next = index + 1;
    if (next >= m_capacity)
    {
        next = 0;
        m_isFull = true; // We wrapped around, so buffer is definitely full
    };
    return next;
}

template <typename T>
const T *BufferFIFO<T>::push(T data)
{
    if (m_capacity == 0)
        return NULL; // Safety check

    m_headIndex = nextIndex(m_headIndex);

    // Save the data that is about to be overwritten (for moving average logic)
    m_overflowItem = m_buffer[m_headIndex];

    // Write new data
    m_buffer[m_headIndex] = data;

    // Return the overwritten item only if we wrapped around (buffer full)
    return (m_isFull) ? &m_overflowItem : NULL;
}

template <typename T>
void BufferFIFO<T>::clear()
{
    // Do NOT reset m_capacity (m_size) here, or you lose the memory reference
    m_headIndex = -1;
    m_isFull = false;
}