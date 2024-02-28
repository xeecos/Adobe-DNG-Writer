#pragma once
#include <vector>
#include "types.h"
#include "utils.h"
#include <string.h>
#include <string>
#include <stdlib.h>
#include <any>
#include "tag.h"
#include "vbuf.h"
using namespace std;
using namespace types;
using namespace tag;
using namespace data;

using namespace std;

class Tag;
namespace ifd
{
    class IFD
    {
        public:
            vector<Tag*> tags;
            IFD();
            void setBuffer(VBuf*buf, int offset);
            int dataLen();
            void write();
        private:
            VBuf* mBuf;
            unsigned int mOffset;
            unsigned int NextIFDOffset;
    };
};
