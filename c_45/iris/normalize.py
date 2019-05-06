#! /usr/bin/python3

from sys import argv

f_obj = open(argv[1], 'r')
lines = f_obj.readlines()
f_obj.close()

nLines = len(lines)
nFields = len(lines[0].split(',')) -1

maxima = [-float("inf")] * nFields
minima = [float("inf")] * nFields

def normalize(x, x1, x2):

    d = x2 - x1
    x0 = x - x1
    return x0 / d

for i in range(nLines):

    fields = lines[i].split(',')

    for j in range(nFields):

        value = float(fields[j])
            
        if value >= maxima[j]:
            maxima[j] = value

        if value <= minima[j]:
            minima[j] = value;

outFile = open(argv[2], 'w')

for i in range(nLines):

    fields = lines[i].split(',')
    s = ""

    for j in range(nFields):   
        s += str(normalize(float(fields[j]), minima[j], maxima[j])) + ','

    s += fields[j+1]

    outFile.write(s)

outFile.close()








