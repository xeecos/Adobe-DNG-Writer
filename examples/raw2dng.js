const fs = require('fs');
const {Tag} = require('../src/tag');
const {TagType} = require('../src/types')
const {DNG} = require('../src/dng')
let width = 1920;
let height = 1080;
let bpp = 12;

let rawFile = './assets/hello.dng';
console.log(fs.readFileSync(rawFile));
let rawData = Buffer.alloc(width*height*2);
for(let i=0,len=width*height;i<len;i++)
{
    rawData.writeUInt16LE(0xffff,i);
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
// tags.push(new Tag(TagType.ColorMatrix1, ccm1));
tags.CalibrationIlluminant1 = (new Tag(TagType.CalibrationIlluminant1, 21));
// tags.push(new Tag(TagType.AsShotNeutral, [[1,1],[1,1],[1,1]]));
tags.DNGVersion = (new Tag(TagType.DNGVersion, [1, 4, 0, 0]));
tags.DNGBackwardVersion = (new Tag(TagType.DNGBackwardVersion, [1, 2, 0, 0]));
tags.Make = (new Tag(TagType.Make, "Camera Brand"));
tags.Model = (new Tag(TagType.Model, "Camera Model"));
tags.PreviewColorSpace = (new Tag(TagType.PreviewColorSpace, 2));
let buffer = DNG.convert(rawData, tags, "custom", "")
fs.writeFileSync('out.dng',buffer);