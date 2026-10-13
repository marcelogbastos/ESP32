const fs=require('node:fs');
WebAssembly.instantiate(fs.readFileSync(process.argv[2])).then(({instance})=>{
 const result=instance.exports.runTests();
 if(result)throw Error('Teste falhou: '+result);
 console.log('PASS: sequencia, modo manual, rollover, piscadas e botoes');
}).catch(e=>{console.error(e);process.exitCode=1;});
