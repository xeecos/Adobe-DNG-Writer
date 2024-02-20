#pragma once
#include <vector>
#include <stdint.h>
#include "utils.h"
using namespace std;

class VBuf
{
    public:
        VBuf(int size)
        {
            data.resize(size);
        };
        unsigned char* raw()
        {
            return data.data();
        }
        void copy(unsigned char*buf, int offset, int src_offset, int src_length)
        {

        }
        void writeUInt8(unsigned char v, int offset)
        {

        }
        void writeInt16LE(int16_t v, int offset)
        {

        }
        void writeUInt16LE(uint16_t v, int offset)
        {

        }
        void writeUInt32LE(uint32_t v, int offset)
        {

        }
        void writeInt32LE(int32_t v, int offset)
        {
            
        }
        void writeFloatLE(float v, int offset)
        {
            
        }
        void writeDoubleLE(double v, int offset)
        {
            
        }
    private:
        vector<unsigned char> data;
};