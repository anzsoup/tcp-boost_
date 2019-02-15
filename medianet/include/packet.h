#ifndef __PACKET_H__
#define __PACKET_H__

#include <string>
#include "session.h"

namespace medianet
{
    /**
     * A class for reading and writing byte stream.
     * It doesn't care whether current structure is little endian or big endian, 
     * but just copying bytes written in the memory.
     */
    class packet
    {
        public:
            static const int BUFFER_SIZE = 1440;                // 1440 is maximum limit not to be segmented
            static const int HEADER_SIZE = sizeof(int16_t);
            static packet* create(int16_t protocol_id);
            
        public:
            packet();
            packet(char *buffer, session *owner);
            packet(const packet &orig);
            ~packet();

            /**
             * Record final body size on the header.
             */
            void record_size();

            // Getters and Setters
            session* get_owner() const;
            char* get_buffer() const;
            int get_position() const;
            int get_size() const;
            int16_t get_protocol_id() const;

            // Pop methods
            int16_t pop_protocol_id();
            char pop_byte();
            bool pop_bool();
            int16_t pop_int16();
            int32_t pop_int32();
            int64_t pop_int64();
            float pop_single();
            double pop_double();
            char* pop_byte_array();
            std::string pop_string();
            
            // Push methods
            // I didn't overload methods because I wasn't sure which type is overloadable or not.
            void set_porotocol_id(const int16_t protocol_id);
            void push_byte(char data);
            void push_bool(bool data);
            void push_int16(int16_t data);
            void push_int32(int32_t data);
            void push_int64(int64_t data);
            void push_single(float data);
            void push_double(double data);
            void push_byte_array(char *data, int16_t length);
            void push_string(std::string data);

        private:
            /**
             * Copy m_buffer into dest array by the given length starts from the position that m_position indicates.
             * Modify this method if you need to handle endian structure.
             */
            char* read_buffer(char *dest, size_t length);

            /**
             * Copy dest array into m_buffer by the given length starts from the position that m_position indicates.
             * Modify this method if you need to handle endian structure.
             */
            void write_buffer(char *src, size_t length);

        private:
            char *m_buffer;
            int m_position;
            int m_size;
            int16_t m_protocol_id;
            session *m_owner;
    };
}

#endif