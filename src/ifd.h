#pragma once
#include <vector>
#include "tag.h"
class IFD
{
    public:
        IFD()
        {
            this.tags = []
            NextIFDOffset = 0
        };

        void setBuffer(buf, offset)
        {
            this.buf = buf
            this.offset = offset
            let currentDataOffset = offset + 2 + this.tags.length*12 + 4
            let currentTagOffset = offset + 2
            this.tags.forEach(tag=>{
                tag.setBuffer(buf, currentTagOffset, currentDataOffset);
                currentTagOffset += 12;
                currentDataOffset += tag.dataLen();
                currentDataOffset = (currentDataOffset + 3) & 0xFFFFFFFC;
            })
        }
        int dataLen()
        {
            let totalLength = 2 + this.tags.length*12 + 4;
            this.tags.sort((a,b)=>{
                return a.TagId-b.TagId;
            })
            this.tags.forEach(tag=>{
                totalLength += tag.dataLen()
            });
            return (totalLength + 3) & 0xFFFFFFFC
        };

        void write()
        {
            this.buf.writeUInt16LE(this.tags.length,this.offset);
            this.tags.forEach(tag=>{
                tag.write();
            });
            this.buf.writeUInt32LE(this.NextIFDOffset,this.offset + 2 + (this.tags.length)*12);
        };
    private:
        vector<Tag> tags;
        unsigned char* buf;
        unsigned int offset;
        unsigned int NextIFDOffset;
};
