#pragma once
#include <pgmspace.h>
static const char INDEX_HTML[] PROGMEM = R"HTML(<!doctype html><html lang="pt-BR"><head><meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1"><title>Robo Face</title>
<style>
:root{--bg:#10141a;--fg:#e8edf2;--card:#1b222c;--ac:#34d1e0;--mut:#8b97a5}
*{box-sizing:border-box}body{margin:0;background:var(--bg);color:var(--fg);font:16px system-ui,sans-serif;display:flex;flex-direction:column;height:100dvh}
header{display:flex;gap:8px;align-items:center;padding:10px 14px;background:var(--card)}
header b{flex:1}button,select,input{font:inherit;color:inherit;background:#26303d;border:0;border-radius:10px;padding:9px 12px}
button:active{background:var(--ac);color:#000}
#faces{display:flex;gap:6px;overflow-x:auto;padding:8px 14px;white-space:nowrap}
#faces button{flex:none;padding:6px 10px;font-size:14px}
#log{flex:1;overflow-y:auto;padding:10px 14px;display:flex;flex-direction:column;gap:8px}
.m{max-width:85%;padding:9px 12px;border-radius:14px;background:var(--card);white-space:pre-wrap}
.u{align-self:flex-end;background:#1f4d57}.s{align-self:center;color:var(--mut);font-size:13px;background:none}
#f{display:flex;gap:8px;padding:10px 14px;background:var(--card)}#f input{flex:1;min-width:0}
html{color-scheme:dark}*{scrollbar-width:thin;scrollbar-color:#3a4656 transparent}
dialog::backdrop{background:#000a}dialog button{width:100%;background:var(--ac);color:#000;font-weight:600}
dialog input[type=checkbox]{width:auto;vertical-align:middle;margin-right:6px}
dialog{background:var(--card);color:var(--fg);border:0;border-radius:14px;width:min(92vw,420px)}
dialog label{display:block;margin:10px 0 4px;color:var(--mut);font-size:14px}dialog input{width:100%}
</style></head><body>
<header><b>Robo Face</b><button id="cfg">Config</button></header>
<div id="faces"></div><div id="log"></div>
<form id="f"><input id="t" placeholder="Fale com o robo..." autocomplete="off"><button>Enviar</button></form>
<dialog id="d"><form method="dialog"><label>Chave da API Anthropic (fica so neste celular)</label>
<input id="key" type="password" placeholder="sk-ant-..."><label>Modelo</label><input id="model">
<label>Personalidade</label><input id="persona"><label style="color:var(--fg)"><input type="checkbox" id="voice">Falar em voz alta</label>
<p style="margin-top:14px"><button value="ok">Salvar</button></p></form></dialog>
<script>
const N=["Neutro","Feliz","Muito feliz","Rindo","Bravo","Furioso","Triste","Chorando","Surpreso","Assustado","Sonolento","Dormindo","Piscando","Curioso","Pensando","Desconfiado","Confuso","Timido","Carinhoso","Animado","Entediado","Concentrado","Preocupado","Travesso"];
const $=i=>document.getElementById(i),log=$("log");
const L=(k,d)=>{try{return localStorage.getItem(k)||d}catch(e){return d}},S=(k,v)=>{try{localStorage.setItem(k,v)}catch(e){}};
$("key").value=L("key","");$("model").value=L("model","claude-haiku-5-5");
$("persona").value=L("persona","um robozinho simpatico e curioso");$("voice").checked=L("voice","")=="1";
$("cfg").onclick=()=>$("d").showModal();
$("d").onclose=()=>{S("key",$("key").value.trim());S("model",$("model").value.trim());S("persona",$("persona").value);S("voice",$("voice").checked?"1":"")};
const setFace=i=>fetch("/set?face="+i).catch(()=>{});
N.forEach((n,i)=>{const b=document.createElement("button");b.textContent=n;b.onclick=()=>setFace(i);$("faces").append(b)});
function add(c,t){const d=document.createElement("div");d.className="m "+c;d.textContent=t;log.append(d);log.scrollTop=log.scrollHeight;return d}
let hist=[];
$("f").onsubmit=async e=>{
 e.preventDefault();const t=$("t").value.trim();if(!t)return;
 if(!L("key","")){add("s","Configure a chave da API em Config.");return}
 $("t").value="";add("u",t);hist.push({role:"user",content:t});hist=hist.slice(-20);setFace(14);
 const sys="Voce e "+$("persona").value+", com um rosto animado numa telinha. Responda em portugues, em no maximo 2 frases curtas. "+
 "Responda SOMENTE com JSON: {\"face\":<indice 0-23>,\"text\":\"fala\"}. Faces: "+N.map((n,i)=>i+"="+n).join(", ")+". Escolha a face que combina com a emocao da resposta.";
 try{
  const r=await fetch("https://api.anthropic.com/v1/messages",{method:"POST",headers:{"content-type":"application/json","x-api-key":L("key",""),
   "anthropic-version":"2023-06-01","anthropic-dangerous-direct-browser-access":"true"},
   body:JSON.stringify({model:L("model","claude-haiku-5-5"),max_tokens:300,system:sys,messages:hist})});
  const j=await r.json();if(!r.ok)throw new Error(j.error&&j.error.message||r.status);
  const raw=j.content.map(c=>c.text||"").join("");let o;
  try{o=JSON.parse(raw.slice(raw.indexOf("{"),raw.lastIndexOf("}")+1))}catch(e){o={face:0,text:raw}}
  hist.push({role:"assistant",content:raw});
  add("",o.text);setFace(Math.max(0,Math.min(23,parseInt(o.face)||0)));
  if(L("voice","")&&window.speechSynthesis){const u=new SpeechSynthesisUtterance(o.text);u.lang="pt-BR";speechSynthesis.speak(u)}
 }catch(err){hist.pop();add("s","Erro: "+err.message);setFace(22)}
};
</script></body></html>)HTML";
