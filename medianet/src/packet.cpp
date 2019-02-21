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
        : m_position(header_length),
          m_size(0),
          m_protocol_id(0)
    {
        m_buffer = new char[buffer_length]();
    }

    packet::packet(char *buffer)
        : m_position(0)
    {
        // deep copy
        m_buffer = (char*)std::memcpy(new char[buffer_length], buffer, buffer_length);
        char temp[header_length];
        std::memcpy(temp, m_buffer, header_length);
        m_size = *((int*)temp);
        m_protocol_id = pop_int16();
        // HEADER - PROTOCOL_ID - DATA1 - DATA2 - ...
        // We are here! --------^
    }

    packet::packet(const packet &orig)
        : m_position(orig.get_position()),
          m_size(orig.get_size()),
          m_protocol_id(orig.get_protocol_id())
    {
        // deep copy
        m_buffer = (char*)std::memcpy(new char[buffer_length], orig.get_buffer(), buffer_length);
    }

    packet::~packet()
    {
        delete[] m_buffer;
    }

    void
    packet::record_size()
    {
        int16_t body_size = (int16_t)(m_position - header_length);
        char *header = (char*)&body_size;
        std::memcpy(m_buffer, header, header_length);
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
    packet::get_size() const
    {
        return m_size;
    }

    int
    packet::get_body_length() const
    {
        return buffer_length - header_length;
    }

    int16_t
    packet::get_protocol_id() const
    {
        return m_protocol_id;
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
        m_position = header_length;
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
            std::cout << "Packet reading warning : You are trying reading buffer over the m_size.\n";
        }
        else if (m_position + length > buffer_length)
        {
            std::cout << "Packet reading failed : Tryed reading buffer over the BUFFER_SIZE.\n";
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
            std::cout << "Packet writing failed : Tryed writing buffer over the BUFFER_SIZE.\n";
            return;
        }

        std::memcpy(m_buffer + m_position, src, length);
        m_position += length;
        m_size += length;
    }
}