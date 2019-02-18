#include <cstring>
#include <iostream>
#include "packet.h"

namespace medianet
{
    packet*
    packet::create(int16_t protocol_id)
    {
        auto newone = new packet();
        newone->set_porotocol_id(protocol_id);
        return newone;
    }

    packet::packet()
        : m_owner(nullptr),
          m_position(HEADER_SIZE),
          m_size(0),
          m_protocol_id(0)
    {
        m_buffer = new char[BUFFER_SIZE]();
    }

    packet::packet(char *buffer, session *owner)
        : m_owner(owner),
          m_position(0)
    {
        char *temp = read_buffer(new char[HEADER_SIZE], HEADER_SIZE);
        m_size = *((int*)temp);
        // deep copy
        m_buffer = (char*)std::memcpy(new char[BUFFER_SIZE], buffer, m_size);
        m_protocol_id = pop_protocol_id();
        // return to initial position
        m_position = HEADER_SIZE;
    }

    packet::packet(const packet &orig)
        : m_owner(orig.get_owner()),
          m_position(orig.get_position()),
          m_size(orig.get_size()),
          m_protocol_id(orig.get_protocol_id())
    {
        m_buffer = (char*)std::memcpy(new char[BUFFER_SIZE], orig.get_buffer(), m_size);
    }

    packet::~packet()
    {
        delete[] m_buffer;
    }

    void
    packet::record_size()
    {
        int16_t body_size = (int16_t)(m_position - HEADER_SIZE);
        char *header = (char*)&body_size;
        std::memcpy(m_buffer, header, HEADER_SIZE);
    }

    session*
    packet::get_owner() const
    {
        return m_owner;
    }

    char*
    packet::get_buffer() const
    {
        return m_buffer;
    }

    int
    packet::get_position() const
    {
        return m_position;
    }

    int
    packet::get_size() const
    {
        return m_size;
    }

    int16_t
    packet::get_protocol_id() const
    {
        return m_protocol_id;
    }

    int16_t
    packet::pop_protocol_id()
    {
        return pop_int16();
    }

    char
    packet::pop_byte()
    {
        size_t size = 1;
        char data[size];
        read_buffer(data, size);
        return *data;
    }

    bool
    packet::pop_bool()
    {
        char data = pop_byte();
        return data != 0;
    }

    int16_t
    packet::pop_int16()
    {
        size_t size = sizeof(int16_t);
        char data[size];
        read_buffer(data, size);
        return *((int16_t*)data);
    }

    int32_t
    packet::pop_int32()
    {
        size_t size = sizeof(int32_t);
        char data[size];
        read_buffer(data, size);
        return *((int32_t*)data);
    }

    int64_t
    packet::pop_int64()
    {
        size_t size = sizeof(int64_t);
        char data[size];
        read_buffer(data, size);
        return *((int64_t*)data);
    }

    float
    packet::pop_single()
    {
        size_t size = sizeof(float);
        char data[size];
        read_buffer(data, size);
        return *((float*)data);
    }

    double
    packet::pop_double()
    {
        size_t size = sizeof(double);
        char data[size];
        read_buffer(data, size);
        return *((double*)data);
    }

    char*
    packet::pop_byte_array()
    {
        int16_t size = pop_int16();
        char data[size];
        read_buffer(data, size);
        return data;
    }

    std::string
    packet::pop_string()
    {
        char *data = pop_byte_array();
        return std::string(data);
    }

    void
    packet::set_porotocol_id(int16_t protocol_id)
    {
        m_protocol_id = protocol_id;
        m_position = HEADER_SIZE;
        push_int16(protocol_id);
    }

    void
    packet::push_byte(char data)
    {
        size_t size = 1;
        char *temp = &data;
        write_buffer(temp, size);
    }

    void
    packet::push_bool(bool data)
    {
        push_byte((char)data);
    }

    void
    packet::push_int16(int16_t data)
    {
        size_t size = sizeof(int16_t);
        char *temp = (char*)&data;
        write_buffer(temp, size);
    }

    void
    packet::push_int32(int32_t data)
    {
        size_t size = sizeof(int32_t);
        char *temp = (char*)&data;
        write_buffer(temp, size);
    }

    void
    packet::push_int64(int64_t data)
    {
        size_t size = sizeof(int64_t);
        char *temp = (char*)&data;
        write_buffer(temp, size);
    }

    void
    packet::push_single(float data)
    {
        size_t size = sizeof(float);
        char *temp = (char*)&data;
        write_buffer(temp, size);
    }

    void
    packet::push_double(double data)
    {
        size_t size = sizeof(double);
        char *temp = (char*)&data;
        write_buffer(temp, size);
    }

    void
    packet::push_byte_array(char *data, int16_t length)
    {
        push_int16(length);
        write_buffer(data, length);
    }

    void
    packet::push_string(std::string data)
    {
        size_t size = data.length() + 1;
        char *temp = strcpy(new char[size], data.c_str());
        push_byte_array(temp, size);
    }

    char*
    packet::read_buffer(char *dest, size_t length)
    {
        // overflow
        if (m_position + length > m_size)
        {
            std::cout << "Packet reading warning : " << "You are trying reading buffer over the m_size." << std::endl;
        }
        else if (m_position + length > BUFFER_SIZE)
        {
            std::cout << "Packet reading failed : " << "Tryed reading buffer over the BUFFER_SIZE." << std::endl;
            return nullptr;
        }

        std::memcpy(dest, m_buffer + m_position, length);
        m_position += length;

        return dest;
    }

    void
    packet::write_buffer(char *src, size_t length)
    {
        if (m_position + length > BUFFER_SIZE)
        {
            std::cout << "Packet writing failed : " << "Tryed writing buffer over the BUFFER_SIZE." << std::endl;
            return;
        }

        std::memcpy(m_buffer + m_position, src, length);
        m_position += length;
        m_size += length;
    }
}