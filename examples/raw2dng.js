const fs = require('fs');
const {Tag} = require('../src/tag');
const {TagType} = require('../src/types')
// const {DNG} = require('../src/dng')
let width = 64;
let height = 64;
let bpp = 12;

// let rawFile = 'assets/tesdng.raw16';
// let rawData = fs.readFileSync(rawFile);

let ccm1 = [[19549, 10000], [-7877, 10000], [-2582, 10000],	
        [-5724, 10000], [10121, 10000], [1917, 10000],
        [-1267, 10000], [ -110, 10000], [ 6621, 10000]]
let tags = {};
tags.ImageWidth = (new Tag(TagType.ImageWidth, width));
tags.push(new Tag(TagType.ImageLength, height));
tags.push(new Tag(TagType.TileWidth, width));
tags.push(new Tag(TagType.TileLength, height));
tags.push(new Tag(TagType.Orientation, 1));
tags.push(new Tag(TagType.PhotometricInterpretation, 32803));
tags.push(new Tag(TagType.SamplesPerPixel, 1));
tags.push(new Tag(TagType.BitsPerSample, bpp));
tags.push(new Tag(TagType.CFARepeatPatternDim, [2,2]));
tags.push(new Tag(TagType.CFAPattern, [1, 2, 0, 1]));
tags.push(new Tag(TagType.BlackLevel, (4096 >> (16 - bpp))));
tags.push(new Tag(TagType.WhiteLevel, ((1 << bpp) -1) ));
// tags.push(new Tag(TagType.ColorMatrix1, ccm1));
tags.push(new Tag(TagType.CalibrationIlluminant1, 21));
// tags.push(new Tag(TagType.AsShotNeutral, [[1,1],[1,1],[1,1]]));
tags.push(new Tag(TagType.DNGVersion, [1, 4, 0, 0]));
tags.push(new Tag(TagType.DNGBackwardVersion, [1, 2, 0, 0]));
tags.push(new Tag(TagType.Make, "Camera Brand"));
tags.push(new Tag(TagType.Model, "Camera Model"));
tags.push(new Tag(TagType.PreviewColorSpace, 2));
console.log(tags);
// let buffer = DNG.convert(rawData, tags, "custom", "")