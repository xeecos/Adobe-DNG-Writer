#pragma once

#include <vector>
#include "types.h"
#include "utils.h"
#include <string.h>
#include <string>
#include <stdlib.h>
#include <any>
#include "vbuf.h"
#include "ifd.h"

using namespace std;
using namespace types;
using namespace tag;
using namespace data;
namespace ifd
{
    class IFD;
};

class Tag
{
    public:
        TagType Type;
        int TagId;
        DataType dataType;
        int DataCount;
        int DataOffset;
        bool selfContained;
        vector<any> rawValue;
        int DataLength;
        ifd::IFD* subIFD;
        VBuf* DataValue;
        int TagOffset;
        Tag(TagType tagType, vector<any> value);
        void setValue(vector<any> value);
        void setBuffer(VBuf* buf, int tagOffset, int dataOffset);
        int dataLen();
        void write();
    private:
        VBuf *mBuf;
};
