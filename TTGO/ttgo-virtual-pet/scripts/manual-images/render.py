"""Illustrations from real firmware drawing calls; host font rasterization is approximate."""
from pathlib import Path
import json
from PIL import Image,ImageDraw,ImageFont
ROOT=Path(__file__).resolve().parents[2]
OUT=ROOT/'docs/imagens';OUT.mkdir(parents=True,exist_ok=True)
fonts=Path('C:/Windows/Fonts')
small=ImageFont.truetype(str(fonts/'consola.ttf'),9)
large=ImageFont.truetype(str(fonts/'consola.ttf'),14)
caption=ImageFont.truetype(str(fonts/'segoeui.ttf'),24)
title=ImageFont.truetype(str(fonts/'segoeuib.ttf'),28)
def ink(v):return ((v>>11&31)*255//31,(v>>5&63)*255//63,(v&31)*255//31)
def scene(ops):
 image=Image.new('RGB',(240,135));clip=(0,0,240,135)
 for item in ops:
  op=item['op'];x,y,a,b,c,d=(item[k] for k in ('x','y','a','b','c','d'));color=ink(item['color'])
  if op==20:clip=(x,y,x+a,y+b);continue
  if op==21:clip=(0,0,240,135);continue
  layer=Image.new('RGBA',image.size);draw=ImageDraw.Draw(layer)
  if op in (0,1):draw.rectangle((x,y,x+a-1,y+b-1),fill=color)
  elif op in (2,9):draw.ellipse((x-a,y-a,x+a,y+a),fill=color if op==2 else None,outline=color)
  elif op==3:draw.ellipse((x-a,y-b,x+a,y+b),fill=color)
  elif op in (4,10):draw.rounded_rectangle((x,y,x+a-1,y+b-1),radius=c,fill=color if op==4 else None,outline=color)
  elif op==5:draw.polygon([(x,y),(a,b),(c,d)],fill=color)
  elif op==6:draw.line((x,y,a,b),fill=color)
  elif op==7:
   font=small if a==1 else large
   if b:x-=draw.textlength(item['text'],font=font)/2
   draw.text((x,y-2),item['text'],font=font,fill=color)
  elif op==8:draw.point((x,y),fill=color)
  visible=layer.crop(clip);image.paste(visible,clip[:2],visible)
 return image
scenes=[scene(ops) for ops in json.loads((ROOT/'dist/manual-scenes.json').read_text())]
def save(index,name):scenes[index].resize((720,405),Image.Resampling.NEAREST).save(OUT/name)
for i,name in [(0,'tela-principal.png'),(1,'menu-cuidados.png'),(2,'estatisticas.png'),(3,'objetivos.png'),(4,'personalizar.png'),(5,'lojinha.png')]:save(i,name)
def montage(items,name,columns=2,scale=2):
 w,h=240*scale,135*scale;rows=(len(items)+columns-1)//columns
 result=Image.new('RGB',(columns*(w+24)+24,rows*(h+62)+24),'#f1f5f9');draw=ImageDraw.Draw(result)
 for n,(index,label) in enumerate(items):
  x=24+(n%columns)*(w+24);y=24+(n//columns)*(h+62)
  draw.text((x,y),label,font=caption,fill='#17212f')
  result.paste(scenes[index].resize((w,h),Image.Resampling.NEAREST),(x,y+38))
 result.save(OUT/name)
montage([(7,'Comendo'),(8,'Brincando'),(9,'Tomando banho'),(10,'Tomando remédio')],'cuidados-animados.png')
montage([(6,'Dormindo'),(11,'Com fome'),(12,'Sujo'),(13,'Doente'),(14,'Triste'),(15,'Cansado')],'expressoes.png',3)
montage([(16,'Bebê'),(17,'Jovem'),(18,'Adulto carinhoso')],'evolucao.png',3)
controls=Image.new('RGB',(1000,350),'#f1f5f9');draw=ImageDraw.Draw(controls)
for x,color,heading,line1,line2,line3 in [(25,'#147d92','Navegação · GPIO35','Toque: abrir menu / próxima opção','Na inicial, segure: dormir/acordar','Em outras telas, segure: voltar'),(515,'#7542a4','Ação · GPIO0','Toque: carinho / confirmar opção','Na inicial, segure: estatísticas','Em outras telas, segure: voltar')]:
 draw.rounded_rectangle((x,20,x+460,325),radius=18,fill='white',outline='#cbd5e1',width=2)
 draw.ellipse((x+25,45,x+100,120),fill=color);draw.text((x+120,67),heading,font=title,fill='#17212f')
 for i,line in enumerate((line1,line2,line3)):draw.text((x+20,154+i*44),line,font=ImageFont.truetype(str(fonts/'segoeui.ttf'),20),fill='#334155')
controls.save(OUT/'controles.png')
print('10 imagens PNG geradas em docs/imagens')
