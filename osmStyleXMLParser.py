import xml.parsers.expat

fout= open("osm-style-in-c.c", 'w')
writeNextCharData = 0
currentStyleName = ""
isStroke=0
isStrokeWidth=0
isStrokeOpacity=0
strokeWidth=""
strokeOpacity=""
strokeColor=""

# 3 handler functions
def start_element(name, attrs):
    print 'Start element:', name, attrs
    global writeNextCharData
    global currentStyleName
    if (name == 'Style'):
      currentStyleName = attrs['name']
      fout.write("{\nmapnik::feature_type_style style;\n")
    if (name == 'Rule'):
      fout.write("{\nmapnik::rule_type rule;\n")
    if (name == 'MaxScaleDenominator'):
      fout.write("rule.set_max_scale(")
      writeNextCharData=1
    if (name == 'MinScaleDenominator'):
      fout.write("rule.set_min_scale(")
      writeNextCharData=1
    if (name == 'LineSymbolizer'):
      fout.write("{\nmapnik::line_symbolizer geom = mapnik::line_symbolizer();\n")
    if (name == 'CssParameter'):
      global isStroke
      global isStrokeWidth
      global isStrokeOpacity
      if (attrs['name'] == 'stroke'):
        isStroke=1
      if (attrs['name'] == 'stroke-width'):
        isStrokeWidth=1
      if (attrs['name'] == 'stroke-opacity'):
        isStrokeOpacity=1
    if (name == 'TextSymbolizer'):
      fout.write("mapnik::text_symbolizer textSymb(\""+attrs['name']+"\", \""+attrs['face_name']+"\", "+attrs['size']+", mapnik::color(\""+attrs['fill']+"\"));\n")
      #continue here
def end_element(name):
    print 'End element:', name
    global writeNextCharData
    global strokeColor
    global strokeWidth
    global strokeOpacity
    global currentStyleName
    if (name == 'Style'):
      fout.write("mapnikMap.insert_style(\""+currentStyleName+"\",style);\n}\n")
      currentStyleName = ""
    if (name == 'Rule'):
      fout.write("style.add_rule(rule);\n}\n")
    if (name == 'MaxScaleDenominator'):
      fout.write("LLU);\n");
      writeNextCharData=0
    if (name == 'MinScaleDenominator'):
      fout.write("LLU);\n");
      writeNextCharData=0
    if (name == 'LineSymbolizer'):
      if (strokeWidth != ""):
        fout.write("mapnik::stroke stroke = mapnik::stroke(mapnik::color(\""+strokeColor+"\"), "+strokeWidth+");\n")
      else:
        fout.write("mapnik::stroke stroke = mapnik::stroke(mapnik::color(\""+strokeColor+"\"));\n")
      if (strokeOpacity != ""):
        fout.write("stroke.set_opacity ("+strokeOpacity+");\n")
      fout.write("geom.set_stroke(stroke);\n")
      fout.write("rule.append(geom);\n}\n")
      strokeWidth, strokeOpacity, strokeColor = "", "", ""
    if (name == 'CssParameter'):
      global isStroke
      global isStrokeWidth
      global isStrokeOpacity
      if (isStroke):
        isStroke=0
      if (isStrokeWidth):
        isStrokeWidth=0
      if (isStrokeOpacity):
        isStrokeOpacity=0


def char_data(data):
    global strokeColor
    global strokeWidth
    global strokeOpacity
    print 'Character data:', repr(data)
    if(writeNextCharData):
      fout.write(data)
    if (isStroke):
      strokeColor=data
    if (isStrokeWidth):
      strokeWidth=data
    if (isStrokeOpacity):
      strokeOpacity=data

p = xml.parsers.expat.ParserCreate()

p.StartElementHandler = start_element
p.EndElementHandler = end_element
p.CharacterDataHandler = char_data

#p.Parse("""<?xml version="1.0"?>
#<parent id="top"><child1 name="paul">Text goes here</child1>
#<child2 name="fred">More text</child2>
#</parent>""", 1)
fin = open("osm-styles.xml", 'r')

p.ParseFile(fin)

fin.close()
fout.close()

