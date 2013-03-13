#!/usr/bin/env Python

import sys

if len(sys.argv)<4 or len(sys.argv)>5:
    print "Usage: visualize.py file1 file2 threshold [output filename]"
    exit()

fn_1 = sys.argv[1]
fn_2 = sys.argv[2]
threshold = sys.argv[3]
if len(sys.argv)==5:
    out_vn = sys.argv[4]+'.html'
else:
    out_fn = "comparison.html"

file1 = open(fn_1,'r')
file2 = open(fn_2,'r')

text_size = 15
file1_lines = file1.readlines()
file2_lines = file2.readlines()

# remove all of a given char from a word
def remove_all(word, chars):
    for c in chars:
        word = filter(lambda let:let != c, word)
    return word

# remove all \n, \r, and " from input files which canvas cannot display
file1_lines = [remove_all(line, ['"','\n','\r']) for line in file1_lines]
file2_lines = [remove_all(line, ['"','\n','\r']) for line in file2_lines]

canvas_length = max(len(file1_lines),len(file2_lines))*text_size

output = ("<canvas id=\"lines\" width=\"2000\" height=\"%d\">" % canvas_length+
          "No HTML5 support.</canvas>\n")
output += ("<script type=\"text/javascript\">\n")
output += ("var line_canvas = document.getElementById('lines');\n"+
           "var line = line_canvas.getContext('2d');\n"+
           "line.fillStyle = 'black';\n")

output += ("line.font = \"%dpx Times\";\n" % text_size +
           "line.textAlign = \"left\";\n" +
           "line.textBaseline = \"top\";\n" +
           "line.strokeStyle = '#ff0000';\n" +
           "line.lineWidth = .7;")

#print the input files to two output columns
for i in range(len(file1_lines)):
    output += "line.fillText(\"%s\",0,%d);\n" % (file1_lines[i], i*text_size)

for i in range(len(file2_lines)):
    output += "line.fillText(\"%s\",800,%d);\n" % (file2_lines[i], i*text_size)

#draw line underlining each given line and connect them
def connectLines(f1_line, f2_line):
    x1 = 400
    x2 = 800
    x3 = 1200
    y1 = (f1_line+1)*text_size
    y2 = (f2_line+1)*text_size
    out = ("line.beginPath();\n" +
           "line.moveTo(0,%d);\n" % y1 +
           "line.lineTo(%d,%d);\n" % (x1,y1) +
           "line.lineTo(%d,%d);\n" % (x2,y2) +
           "line.lineTo(%d,%d);\n" % (x3,y2) +
           "line.stroke();\n")
    return out

def compareLines(f1_line, f2_line):
    i = 0
    j = 0
    same = 0
    max_i = len(f1_line)
    max_j = len(f2_line)
    total = max(max_i,max_j)
    while i < max_i and j < max_j:
        if f1_line[i] in ignore_chars:
            i += 1
            continue
        elif f2_line[j] in ignore_chars:
            j += 1
            continue
        elif f1_line[i] == f2_line[j]:
            same += 1
            i += 1
            j += 1
        else:
            if i < j:
                i += 1
            else:
                j += 1
        
    return float(same)/float(total)

ignore_chars = [' ', '}', '{', '\t']
#remove unnecessary chars before comparing
file1_lines = [remove_all(line, ignore_chars) for line in file1_lines]
file2_lines = [remove_all(line, ignore_chars) for line in file2_lines]

for i in range(len(file1_lines)):
    for j in range(len(file2_lines)):
        if len(file1_lines[i]) < 1:
            break
        if len(file2_lines[j]) < 1:
            continue
        if compareLines(file1_lines[i], file2_lines[j])>=float(threshold):
            output += connectLines(i,j)
            file1_lines[i] = ""
            file2_lines[j] = ""
            break

output += ("</script>")

out = open(out_fn, 'w')

out.write(output)

out.close()
