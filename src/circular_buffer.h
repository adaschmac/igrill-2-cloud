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
#include "measurement.h"

template<typename T>
class CircularBuffer {
public:
    CircularBuffer(T* buffer, size_t elements)
        : m_buffer(buffer),
          m_bufferEnd(m_buffer + elements),
          m_readPtr(nullptr),
          m_writePtr(m_buffer),
          m_bufferSize(elements),
          m_itemCount(0),
          m_freeBuffer(false)
    {

    }

    CircularBuffer(size_t elements)
        : m_buffer((T*)malloc(elements * sizeof(T))),
          m_bufferEnd(m_buffer + elements),
          m_readPtr(nullptr),
          m_writePtr(m_buffer),
          m_bufferSize(elements),
          m_itemCount(0),
          m_freeBuffer(true)
    {
        if (m_buffer == nullptr) {
            // Things will go downhill from here.
            IG2C_DEBUG("Could not allocate enough memory for circular buffer");
        }
    }

    ~CircularBuffer() {
        if (m_freeBuffer)
            free(m_buffer);
    }

    size_t WriteElement(const T& element) {
        return WriteElement(&element);
    }

    size_t WriteElement(const T* element) {
        if (m_writePtr >= m_bufferEnd) {
            DumpState("Write: Wrapping around");
            m_writePtr = m_buffer;
        }

        if (m_writePtr == m_readPtr) {
            DumpState("Buffer overflow, discarding data");
            return 0;
        }

        memcpy((void*)m_writePtr, (void*)element, sizeof(T));
        ++m_itemCount;

        if (m_readPtr == nullptr) {
            m_readPtr = m_writePtr;
            DumpState("WriteElement OK, advance read pointer");
        }
        else
        {
            DumpState("WriteElement OK, no advance read ptr");
        }

        m_writePtr++;
        return 1;
    }

    T* ReadElement() {
        // If readPtr was advanced to/past the end of the buffer, move to head
        if (m_readPtr >= m_bufferEnd) {
            DumpState("ReadElement wrap buffer");
            m_readPtr = m_buffer;
        }

        // If item count is 0, there's no data left to be read.
        if (m_itemCount == 0) {
            m_readPtr = nullptr;
            return nullptr;
        }
        else
        {
            DumpState("ReadElement OK");
            --m_itemCount;
            return m_readPtr++;
        }
    }
private:
    T* m_buffer;
    T* m_bufferEnd;
    T* m_readPtr;
    T* m_writePtr;
    size_t m_bufferSize;
    size_t m_itemCount;
    bool m_freeBuffer;

    void DumpState(const char* message) {
#if IG2C_ENABLE_DEBUG_BUFFER
        IG2C_DEBUG_F("CB State: %s", message);
        IG2C_DEBUG_F("  m_buffer: %p -> %p, items: %u", m_buffer, m_bufferEnd, m_itemCount);
        IG2C_DEBUG_F("  read: %p [%d],  write: %p [%d]", m_readPtr, (m_readPtr - m_buffer) % sizeof(T), m_writePtr, (m_writePtr - m_buffer) % sizeof(T));
        IG2C_DEBUG("");
#endif
    }
};

template class CircularBuffer<Measurement>;

#endif // IG2C_CIRCULAR_BUFFER_H