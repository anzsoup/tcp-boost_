#include <cstring>
#include <iostream>
#include "packet.h"

namespace medianet
{
    boost::shared_ptr<packet>
    packet::create()
    {
        boost::shared_ptr<packet> newone(new packet());
        return newone;
    }

    packet::packet()
        : m_position(header_length),
          m_body_length(0)
    {
        m_buffer = new char[buffer_length]();
    }

    packet::packet(char *buffer)
        : m_position(header_length),
          m_body_length(0)
    {
        // deep copy
        m_buffer = (char*)std::memcpy(new char[buffer_length], buffer, buffer_length);
        decode_body_length();
    }

    packet::packet(const packet &orig)
        : m_position(orig.get_position()),
          m_body_length(orig.get_body_length())
    {
        // deep copy
        m_buffer = (char*)std::memcpy(new char[buffer_length], orig.get_buffer(), buffer_length);
    }

    packet::~packet()
    {
        delete[] m_buffer;
    }

    void
    packet::record_body_length()
    {
        char *header = (char*)&m_body_length;
        std::memcpy(m_buffer, header, header_length);
    }

    void
    packet::decode_body_length()
    {
        char header[header_length];
        std::memcpy(header, m_buffer, header_length);
        m_body_length =  *((int16_t*)header);
    }

    char*
    packet::get_buffer() const
    {
        return m_buffer;
    }

    char*
    packet::get_body() const
    {
        return m_buffer + header_length;
    }

    int
    packet::get_position() const
    {
        return m_position;
    }

    int
    packet::get_total_length() const
    {
        return m_body_length + header_length;
    }

    int
    packet::get_body_length() const
    {
        return m_body_length;
    }

    char
    packet::pop_byte()
    {
        size_t size = 1;
        char *data = new char[size];
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
        char *data = new char[size];
        read_buffer(data, size);
        return *((int16_t*)data);
    }

    int32_t
    packet::pop_int32()
    {
        size_t size = sizeof(int32_t);
        char *data = new char[size];
        read_buffer(data, size);
        return *((int32_t*)data);
    }

    int64_t
    packet::pop_int64()
    {
        size_t size = sizeof(int64_t);
        char *data = new char[size];
        read_buffer(data, size);
        return *((int64_t*)data);
    }

    float
    packet::pop_single()
    {
        size_t size = sizeof(float);
        char *data = new char[size];
        read_buffer(data, size);
        return *((float*)data);
    }

    double
    packet::pop_double()
    {
        size_t size = sizeof(double);
        char *data = new char[size];
        read_buffer(data, size);
        return *((double*)data);
    }

    char*
    packet::pop_byte_array()
    {
        size_t size = pop_int64();
        char *data = new char[size];
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
    packet::push_byte_array(char *data, size_t length)
    {
        push_int64(length);
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
        if (m_position + length > get_total_length())
        {
            std::cout << "Packet reading warning : You are trying to read buffer over the total length.\n";
        }
        else if (m_position + length > buffer_length)
        {
            std::cout << "Packet reading failed : Tryed to read buffer over the buffer length.\n";
            return nullptr;
        }

        std::memcpy(dest, m_buffer + m_position, length);
        m_position += length;

        return dest;
    }

    void
    packet::write_buffer(char *src, size_t length)
    {
        if (m_position + length > buffer_length)
        {
            std::cout << "Packet writing failed : Tryed to write buffer over the buffer length.\n";
            return;
        }

        std::memcpy(m_buffer + m_position, src, length);
        m_position += length;
        m_body_length += length;
    }
}