import xml.parsers.expat

fout= open("osm-style-in-c.c", 'w')
writeNextCharData = 0
currentStyleName = ""
isStroke=0
isStrokeWidth=0
isStrokeOpacity=0
isStrokeLinejoin=0
isStrokeLinecap=0
isStrokeDasharray=0
strokeWidth=""
strokeOpacity=""
strokeColor=""
strokeLinejoin=""
strokeLinecap=""
strokeDasharray=""

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
    if (name == 'Filter'):
      fout.write("rule.set_filter(mapnik::create_filter(\"")
      writeNextCharData=1
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
      global isStrokeLinejoin
      global isStrokeLinecap
      global isStrokeDasharray
      if (attrs['name'] == 'stroke'):
        isStroke=1
      if (attrs['name'] == 'stroke-width'):
        isStrokeWidth=1
      if (attrs['name'] == 'stroke-opacity'):
        isStrokeOpacity=1
      if (attrs['name'] == 'stroke-linejoin'):
        isStrokeLinejoin=1
      if (attrs['name'] == 'stroke-linecap'):
        isStrokeLinecap=1
      if (attrs['name'] == 'stroke-dasharray'):
        isStrokeDasharray=1
    if (name == 'TextSymbolizer'):
      fout.write("mapnik::text_symbolizer textSymb(\""+attrs['name']+"\", \""+attrs['face_name']+"\", "+attrs['size']+", mapnik::color(\""+attrs['fill']+"\"));\n")
      if (('placement' in attrs) and (attrs['placement'] == "line")):
        fout.write("textSymb.set_label_placement(mapnik::LINE_PLACEMENT);\n")
      if ('halo_radius' in attrs):
        fout.write("textSymb.set_halo_radius("+attrs['halo_radius']+");\n")
      if ('wrap_width' in attrs):
        fout.write("textSymb.set_wrap_width ("+attrs['wrap_width']+");\n")
      if ('dy' in attrs):#no dx in OSM
        fout.write("textSymb.set_displacement(0.0,"+attrs['dy']+");\n")
      fout.write("rule.append(textSymb);\n")




def end_element(name):
    print 'End element:', name
    global writeNextCharData
    global strokeColor
    global strokeWidth
    global strokeOpacity
    global strokeLinejoin
    global strokeLinecap
    global strokeDasharray
    global currentStyleName
    if (name == 'Style'):
      fout.write("mapnikMap.insert_style(\""+currentStyleName+"\",style);\n}\n")
      currentStyleName = ""
    if (name == 'Rule'):
      fout.write("style.add_rule(rule);\n}\n")
    if (name == 'Filter'):
      fout.write("\"));\n")
      writeNextCharData=0
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
      if (strokeLinejoin == "round"):#no other case appears in OSM
        fout.write("stroke.set_line_join(mapnik::ROUND_JOIN);\n")
      if (strokeLinecap == "round"):#no other case appears in OSM
        fout.write("stroke.set_line_cap(mapnik::ROUND_CAP);\n")
      if (strokeDasharray != ""):
        fout.write("stroke.add_dash("+strokeDasharray+");\n")
      fout.write("geom.set_stroke(stroke);\n")
      fout.write("rule.append(geom);\n}\n")
      strokeWidth, strokeOpacity, strokeColor, strokeLinejoin, strokeLinecap = "", "", "", "", ""
    if (name == 'CssParameter'):
      global isStroke
      global isStrokeWidth
      global isStrokeOpacity
      global isStrokeLinejoin
      global isStrokeLinecap
      global isStrokeDasharray
      if (isStroke):
        isStroke=0
      if (isStrokeWidth):
        isStrokeWidth=0
      if (isStrokeOpacity):
        isStrokeOpacity=0
      if (isStrokeLinejoin):
        isStrokeLinejoin=0
      if (isStrokeLinecap):
        isStrokeLinecap=0
      if (isStrokeDasharray):
        isStrokeDasharray=0


def char_data(data):
    global strokeColor
    global strokeWidth
    global strokeOpacity
    global strokeLinejoin
    global strokeLinecap
    global strokeDasharray
    print 'Character data:', repr(data)
    if(writeNextCharData):
      fout.write(data)
    if (isStroke):
      strokeColor=data
    if (isStrokeWidth):
      strokeWidth=data
    if (isStrokeOpacity):
      strokeOpacity=data
    if (isStrokeLinejoin):
      strokeLinejoin=data
    if (isStrokeLinecap):
      strokeLinecap=data
    if (isStrokeDasharray):
      strokeDasharray=data

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

