#pragma once
#include <vector>
#include "tag.h"
class IFD
{
    public:
        vector<Tag*> tags;
        int NextIFDOffset;
        IFD()
        {
            NextIFDOffset = 0;
        };

        void setBuffer(unsigned char*buf, int offset)
        {
            mBuf = buf;
            mOffset = offset;
            int currentDataOffset = offset + 2 + tags.size()*12 + 4;
            int currentTagOffset = offset + 2;
            for(int i=0;i<tags.size();i++)
            {   
                Tag* tag = tags.at(i);
                tag->setBuffer(buf, currentTagOffset, currentDataOffset);
                currentTagOffset += 12;
                currentDataOffset += tag->dataLen();
                currentDataOffset = (currentDataOffset + 3) & 0xFFFFFFFC;
            }
        }
        int dataLen()
        {
            int totalLength = 2 + tags.size()*12 + 4;
            tags.sort((a,b)=>{
                return a.TagId-b.TagId;
            });
            for(int i=0;i<tags.size();i++)
            {  
                Tag* tag = tags.at(i);
                totalLength += tag->dataLen();
            };
            return (totalLength + 3) & 0xFFFFFFFC;
        };

        void write()
        {
            mBuf.writeUInt16LE(tags.size(),mOffset);
            for(int i=0;i<tags.size();i++)
            {
                Tag* tag = tags.at(i);  
                tag->write();
            };
            mBuf.writeUInt32LE(NextIFDOffset,mOffset + 2 + (tags.size())*12);
        };
    private:
        unsigned char* mBuf;
        unsigned int mOffset;
        unsigned int NextIFDOffset;
};
