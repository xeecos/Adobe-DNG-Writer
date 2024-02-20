#include "tag.h"
#include "vbuf.h"
Tag::Tag(TagType tagType, vector<any> value)
{
    // DataValue = new VBuf(255);
    subIFD = NULL;
    Type = tagType;
    TagId = tagType.id;
    dataType = DataTypeList[tagType.dataType];
    DataCount = value.size();
    DataOffset = 0;
    setValue(value);
    rawValue = value;
    DataLength = DataValue->size();
    selfContained = DataLength <= 4;
};
void Tag::setValue(vector<any> value)
{

    int len = value.size();
    if (dataType.type == DataTypeEnum::Byte)
    {
        DataValue = new VBuf(len);
        for (int i = 0; i < len; i++)
        {
            DataValue->writeUInt8(any_cast<int>(value.at(i)), i);
        }
    }
    else if (dataType.type == DataTypeEnum::Short)
    {
        DataValue = new VBuf(len * 2);
        for (int i = 0; i < len; i++)
        {
            unsigned short val = any_cast<int>(value[i]);
            DataValue->writeUInt16LE(val, i * 2);
        }
    }
    else if (dataType.type == DataTypeEnum::Long)
    {
        DataValue = new VBuf(len * 4);
        for (int i = 0; i < len; i++)
        {
            long val = any_cast<int>(value[i]);
            DataValue->writeInt32LE(val, i * 4);
        }
    }
    else if (dataType.type == DataTypeEnum::Sbyte)
    {
        DataValue = new VBuf(len);
        for (int i = 0; i < len; i++)
        {
            char val = any_cast<char>(value[i]);
            DataValue->writeInt8(val, i);
        }
    }
    else if (dataType.type == DataTypeEnum::Undefined)
    {
        DataValue = new VBuf(len);
        for (int i = 0; i < len; i++)
        {
            DataValue->writeUInt8(any_cast<int>(value[i]), i);
        }
    }
    else if (dataType.type == DataTypeEnum::Sshort)
    {
        DataValue = new VBuf(len * 2);
        for (int i = 0; i < len; i++)
        {
            short val = any_cast<short>(value[i]);
            DataValue->writeInt16LE(val, i * 2);
        }
    }
    else if (dataType.type == DataTypeEnum::Slong)
    {
        DataValue = new VBuf(len * 4);
        for (int i = 0; i < len; i++)
        {
            long val = any_cast<long>(value[i]);
            DataValue->writeInt32LE(val, i * 4);
        }
    }
    else if (dataType.type == DataTypeEnum::Float)
    {
        DataValue = new VBuf(len * 4);
        for (int i = 0; i < len; i++)
        {
            float val = any_cast<float>(value[i]);
            DataValue->writeFloatLE(val, i*4);
        }
    }
    else if (dataType.type == DataTypeEnum::Double)
    {
        DataValue = new VBuf(len * 8);
        for (int i = 0; i < len; i++)
        {
            double val = any_cast<double>(value[i]);
            DataValue->writeDoubleLE(val, i * 8);
        }
    }
    else if (dataType.type == DataTypeEnum::Rational)
    {
        DataValue = new VBuf(len * 4);
        for (int i = 0; i < len; i++)
        {
            DataValue->writeUInt32LE(any_cast<int>(value[i]), i * 4);
        }
    }
    else if (dataType.type == DataTypeEnum::Srational)
    {
        DataValue = new VBuf(len * 4);
        for (int i = 0; i < len; i++)
        {
            DataValue->writeInt32LE(any_cast<int>(value[i]), i * 4);
        }
    }
    else if (dataType.type == DataTypeEnum::Ascii)
    {
        const char* val = any_cast<const char*>(value[0]);
        int len = strlen(val);
        DataValue = new VBuf(len+1);
        for(int i=0;i<len;i++)
        {
            DataValue->writeUInt8(val[i], i);
        }
        DataCount += 1;
    }
    else if (dataType.type == DataTypeEnum::IFD)
    {
        DataValue = new VBuf(4);
        subIFD = any_cast<ifd::IFD *>(value[0]);
    }
    else
    {
        printf("unknow data type:%d", dataType.type);
    }
    int count = ((((DataValue->size() + 3) & 0xFFFFFFFC) - (DataValue->size())));
    int offset = DataValue->size();
    if (count > 0)
    {
        unsigned char *tmp = (unsigned char *)malloc(offset + count);
        int byteLength = offset + count;
        for (int i = 0; i < byteLength; i++)
        {
            tmp[i] = 0;
        }
        unsigned char *raw = DataValue->raw();
        for (int i = 0; i < offset; i++)
        {
            tmp[i] = raw[i];
        }
        DataValue->resize(byteLength);
        DataValue->copy(tmp, 0, 0, offset);
    }
}

void Tag::setBuffer(VBuf *buf, int tagOffset, int dataOffset)
{
    mBuf = buf;
    TagOffset = tagOffset;
    DataOffset = dataOffset;
    if (subIFD)
    {
        subIFD->setBuffer(mBuf, DataOffset);
    }
}
int Tag::dataLen()
{
    if (subIFD)
    {
        return subIFD->dataLen();
    }

    if (selfContained)
    {
        return 0;
    }

    return (DataValue->size() + 3) & 0xFFFFFFFC;
}
void Tag::write()
{
    if (subIFD)
    {
        subIFD->write();
        VBuf *buf = new VBuf(12);
        buf->writeUInt16LE(TagId, 0);
        buf->writeUInt16LE(DataTypeList[DataTypeEnum::Long].type, 2);
        buf->writeUInt32LE(DataCount, 4);
        buf->writeUInt32LE(DataOffset, 8);
        mBuf->copy(buf->raw(), TagOffset, 0, 12);
        free(buf);
    }
    else
    {
        if (selfContained)
        {
            VBuf *buf = new VBuf(12);
            buf->writeUInt16LE(TagId, 0);
            buf->writeUInt16LE(dataType.type, 2);
            buf->writeUInt32LE(DataCount, 4);
            buf->copy(DataValue->raw(), 8, 0, 4);
            mBuf->copy(buf->raw(), TagOffset, 0, 12);
            free(buf);
        }
        else
        {
            VBuf *buf = new VBuf(12);
            buf->writeUInt16LE(TagId, 0);
            buf->writeUInt16LE(dataType.type, 2);
            buf->writeUInt32LE(DataCount, 4);
            buf->writeUInt32LE(DataOffset, 8);
            mBuf->copy(buf->raw(), TagOffset, 0, 12);
            mBuf->copy(DataValue->raw(), DataOffset, 0, DataLength);
            free(buf);
        }
    }
};