const fs = require('fs');
const {Tag} = require('../src/tag');
const {TagType} = require('../src/types')
const {DNG} = require('../src/dng')
let width = 1280;
let height = 960;
let bpp = 16;

let rawData = Buffer.alloc(width*height*2);
for(let i=0,len=width*height;i<len;i++)
{
    rawData.writeUInt16LE((0xffff*Math.random())>>0,i*2);
}
let ccm1 = [[19549, 10000], [-7877, 10000], [-2582, 10000],	
        [-5724, 10000], [10121, 10000], [1917, 10000],
        [-1267, 10000], [ -110, 10000], [ 6621, 10000]]
let tags = {};
tags.ImageWidth = (new Tag(TagType.ImageWidth, width));
tags.ImageLength = (new Tag(TagType.ImageLength, height));
tags.TileWidth = (new Tag(TagType.TileWidth, width));
tags.TileLength = (new Tag(TagType.TileLength, height));
tags.Orientation = (new Tag(TagType.Orientation, 1));
tags.PhotometricInterpretation = (new Tag(TagType.PhotometricInterpretation, 32803));
tags.SamplesPerPixel = (new Tag(TagType.SamplesPerPixel, 1));
tags.BitsPerSample = (new Tag(TagType.BitsPerSample, bpp));
tags.CFARepeatPatternDim = (new Tag(TagType.CFARepeatPatternDim, [2,2]));
tags.CFAPattern = (new Tag(TagType.CFAPattern, [1, 2, 0, 1]));
tags.BlackLevel = (new Tag(TagType.BlackLevel, (4096 >> (16 - bpp))));
tags.WhiteLevel = (new Tag(TagType.WhiteLevel, ((1 << bpp) -1) ));
tags.ColorMatrix1 = (new Tag(TagType.ColorMatrix1, ccm1));
tags.CalibrationIlluminant1 = (new Tag(TagType.CalibrationIlluminant1, 21));
tags.AsShotNeutral = (new Tag(TagType.AsShotNeutral, [[1,1],[1,1],[1,1]]));
tags.DNGVersion = (new Tag(TagType.DNGVersion, [1, 4, 0, 0]));
tags.DNGBackwardVersion = (new Tag(TagType.DNGBackwardVersion, [1, 2, 0, 0]));
tags.Make = (new Tag(TagType.Make, "Camera Brand"));
tags.Model = (new Tag(TagType.Model, "Camera Model"));
tags.PreviewColorSpace = (new Tag(TagType.PreviewColorSpace, [2]));
let buffer = DNG.convert(rawData, tags, "custom", "")
let rawFile = './assets/white.dng';
let buf = (fs.readFileSync(rawFile));
let arr = []
for(let i=0;i<buf.length;i++)
{
    arr.push(buf[i])
}
fs.writeFileSync('out.txt',arr.join("\n"));
arr = []
for(let i=0;i<buffer.length;i++)
{
    arr.push(buffer[i])
}
fs.writeFileSync('out2.txt',arr.join("\n"));
// console.log(buf)
// console.log(buffer)
fs.writeFileSync('out.dng',buffer);