#ifndef __PACKET_H__
#define __PACKET_H__

#include <boost/shared_ptr.hpp>
#include <string>

namespace medianet
{
    /**
     * A class for reading and writing byte stream.
     * It doesn't care whether the structure is little endian or big endian.
     * @author leejm
     */
    class packet
    {
        public:
            /// 1440 is maximum limit not to be segmented.
            static const int buffer_length = 1440;
            /// The header contains total body length.
            static const int header_length = sizeof(int16_t);
            
            /**
             * Create new empty packet instance and wrap it in shared_ptr.
             * It is recommended to use this static method rather than constructors.
             */
            static boost::shared_ptr<packet> create();
            
        public:
            packet();
            packet(char *buffer);
            packet(const packet &orig);
            ~packet();

            /**
             * Record final stream length on the header.
             */
            void record_body_length();
            /**
             * Read buffer and update m_body_length.
             */
            void decode_body_length();

            // Getters and Setters
            char* get_buffer() const;
            char* get_body() const;
            int get_position() const;
            /**
             * Returns the value of header length + body length.
             */
            int get_total_length() const;
            /**
             * Returns the body length.
             */
            int get_body_length() const;

            // Pop methods
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
            int m_body_length;
    };
}

#endif