const fs = require("fs");
let buf = fs.readFileSync("../cpp/out.dng");
let buf2 = fs.readFileSync("./out.dng");
for(let i = 0; i < 220; i++)
{
    if(buf[i].toString(16)!=buf2[i].toString(16)){
        console.log(i,buf[i].toString(16),buf2[i].toString(16))
    }
}