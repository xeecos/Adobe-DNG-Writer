#include "ifd.h"
namespace ifd
{
    IFD::IFD()
    {
        NextIFDOffset = 0;
    };

    void IFD::setBuffer(VBuf*buf, int offset)
    {
        mBuf = buf;
        mOffset = offset;
        int currentDataOffset = offset + 2 + tags.size()*12 + 4;
        int currentTagOffset = offset + 2;
        for(int i=0;i<tags.size();i++)
        {   
            Tag* tag = tags.at(i);
            tag->setBuffer(mBuf, currentTagOffset, currentDataOffset);
            currentTagOffset += 12;
            currentDataOffset += tag->dataLen();
            currentDataOffset = (currentDataOffset + 3) & 0xFFFFFFFC;
        }
    }
    int IFD::dataLen()
    {
        int totalLength = 2 + tags.size()*12 + 4;
        Tag* t;
        for(int i=1; i<tags.size(); i++) {
            for(int j=0; j<tags.size()-i; j++) {
                if(tags[j]->TagId>=tags[j+1]->TagId) {
                    t=tags[j];
                    tags[j]=tags[j+1];
                    tags[j+1]=t;
                }
            }
        }
        for(int i=0;i<tags.size();i++)
        {  
            Tag* tag = tags.at(i);
            totalLength += tag->dataLen();
        };
        return (totalLength + 3) & 0xFFFFFFFC;
    };

    void IFD::write()
    {
        mBuf->writeUInt16LE(tags.size(),mOffset);
        for(int i=0;i<tags.size();i++)
        {
            Tag* tag = tags.at(i);  
            tag->write();
        };
        mBuf->writeUInt32LE(NextIFDOffset,mOffset + 2 + (tags.size())*12);
    };
}