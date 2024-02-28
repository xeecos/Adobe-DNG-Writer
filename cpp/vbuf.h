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
        ~VBuf()
        {
            data.clear();
            data.resize(0);
        }
        void resize(int size)
        {
            data.resize(size);
        }
        uint32_t size()
        {
            return data.size();
        }
        unsigned char* raw()
        {
            return data.data();
        }
        void copy(unsigned char*buf, int offset, int src_offset, int src_length)
        {
            for(int i=0; i<src_length; i++)
            {
                data[offset+i] = buf[src_offset+i];
            }
        }
        void writeInt8(char v, int offset)
        {
            sbyte2byte s2b;
            s2b.val = v;
            data[offset] = s2b.b;
        }
        void writeUInt8(unsigned char v, int offset)
        {
            data[offset] = v;
        }
        void writeInt16LE(int16_t v, int offset)
        {
            short2byte s2b;
            s2b.val = v;
            copy(s2b.b, offset, 0, 2);
        }
        void writeUInt16LE(uint16_t v, int offset)
        {
            ushort2byte s2b;
            s2b.val = v;
            copy(s2b.b, offset, 0, 2);
        }
        void writeUInt32LE(uint32_t v, int offset)
        {
            uint2byte l2b;
            l2b.val = v;
            copy(l2b.b, offset, 0, 4);
        }
        void writeInt32LE(int32_t v, int offset)
        {
            int2byte l2b;
            l2b.val = v;
            copy(l2b.b, offset, 0, 4);
        }
        void writeFloatLE(float v, int offset)
        {
            float2byte f2b;
            f2b.val = v;
            copy(f2b.b, offset, 0, 4);
        }
        void writeDoubleLE(double v, int offset)
        {
            double2byte d2b;
            d2b.val = v;
            copy(d2b.b, offset, 0, 8);
        }
    private:
        vector<unsigned char> data;
};