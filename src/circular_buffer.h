/*
 * igrill-2-cloud - Load temperature data from Weber iGrill2 products and beam
 * it to the cloud for worldwide monitoring.
 *
 * circular_buffer.h - Templatized circular buffer to store data between input
 *                     and output. Assumes single-threaded access.
 */

#ifndef IG2C_CIRCULAR_BUFFER_H
#define IG2C_CIRCULAR_BUFFER_H

#include "igrill-config.h"

template<typename T>
class CircularBuffer {
public:
    CircularBuffer(size_t elements)
        : m_buffer((T*)malloc(elements * sizeof(T))),
          m_bufferEnd(m_buffer + (elements * sizeof(T))),
          m_readPtr(nullptr),
          m_writePtr(m_buffer),
          m_count(elements)
    {
        if (m_buffer == nullptr) {
            // Things will go downhill from here.
            DEBUG("Could not allocate enough memory for circular buffer");
        }
    }

    ~CircularBuffer() {
        free(m_buffer);
    }

    size_t WriteElement(const T& element) {
        return WriteElement(&element);
    }

    size_t WriteElement(const T* element) {
        if (m_writePtr >= m_bufferEnd) {
            m_writePtr = m_buffer;
        }

        if (m_writePtr == m_readPtr) {
            DEBUG("Buffer overflow, discarding data");
            return 0;
        }

        memcpy((void*)m_writePtr, (void*)element, sizeof(T));

        if (m_readPtr == nullptr) {
            m_readPtr = m_writePtr;
        }

        m_writePtr++;

        return 1;
    }

    T* ReadElement() {
        // If readPtr was advanced to/past the end of the buffer, move to head
        if (m_readPtr >= m_bufferEnd) {
            m_readPtr = m_buffer;
        }

        // If readPtr matches writePtr, or if writePtr is past end of buffer,
        // all data has been read. Reset to null.
        if (m_readPtr == m_writePtr || m_writePtr >= m_bufferEnd) {
            m_readPtr = nullptr;
        }

        // m_readPtr is null if there is no valid/unread data in the buffer
        if (m_readPtr == nullptr) {
            return nullptr;
        }

        return m_readPtr++;
    }
private:
    T* m_buffer;
    T* m_bufferEnd;
    T* m_readPtr;
    T* m_writePtr;

    size_t m_count;
};

#endif // IG2C_CIRCULAR_BUFFER_H