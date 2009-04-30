import xml.parsers.expat

fout= open("osm-style-in-c.c", 'w')
writeNextCharData = 0;
currentStyleName = ""#continue here...

# 3 handler functions
def start_element(name, attrs):
    print 'Start element:', name, attrs
    global writeNextCharData
    if (name == 'Rule'):
      fout.write("{\n")
      fout.write("mapnik::feature_type_style style;\n")
      fout.write("mapnik::rule_type rule;\n")
    if (name == 'MaxScaleDenominator'):
      fout.write("rule.set_max_scale(");
      writeNextCharData=1
    if (name == 'MinScaleDenominator'):
      fout.write("rule.set_min_scale(");
      writeNextCharData=1


def end_element(name):
    print 'End element:', name
    global writeNextCharData
    if (name == 'Rule'):
      fout.write("}\n")
    if (name == 'MaxScaleDenominator'):
      fout.write("LLU);\n");
      writeNextCharData=0
    if (name == 'MinScaleDenominator'):
      fout.write("LLU);\n");
      writeNextCharData=0

def char_data(data):
    print 'Character data:', repr(data)
    if(writeNextCharData):
      fout.write(data)

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

