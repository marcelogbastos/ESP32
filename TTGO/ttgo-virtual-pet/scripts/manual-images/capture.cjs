const fs=require('fs');
let instance;let operations=[];
function textAt(pointer){if(!pointer)return '';const data=new Uint8Array(instance.exports.memory.buffer);let end=pointer;while(data[end])++end;return new TextDecoder().decode(data.subarray(pointer,end));}
(async()=>{
 const loaded=await WebAssembly.instantiate(fs.readFileSync('dist/manual-render.wasm'),{env:{doc_emit:(op,x,y,a,b,c,d,color,pointer)=>operations.push({op,x,y,a,b,c,d,color,text:textAt(pointer)})}});
 instance=loaded.instance;const scenes=[];
 for(let scene=0;scene<=18;++scene){operations=[];if(instance.exports.render(scene))throw Error('Scene invalid '+scene);scenes.push(operations);}
 fs.writeFileSync('dist/manual-scenes.json',JSON.stringify(scenes));
 console.log('19 cenas capturadas do Screen.cpp real');
})().catch(error=>{console.error(error);process.exit(1)});
