#! /usr/bin/python3

from sys import argv

f_obj = open(argv[1], 'r')
lines = f_obj.readlines()
f_obj.close()

nLines = len(lines)
nFields = len(lines[0].split(',')) -1

maxima = [-float("inf")] * nFields
minima = [float("inf")] * nFields

labels = []

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

    if fields[j+1].rstrip() == "Iris-setosa":
        print(fields[j+1].rstrip() + " == Iris-setosa")
        labels.append(0)

    elif fields[j+1].rstrip() == "Iris-versicolor":
        print(fields[j+1].rstrip() + " == Iris-versicolor")
        labels.append(1)

    elif fields[j+1].rstrip() == "Iris-virginica":
        print(fields[j+1].rstrip() + " == Iris-virginica")
        labels.append(2)



outFile = open(argv[2], 'w')

for i in range(nLines):

    fields = lines[i].split(',')
    s = ""

    for j in range(nFields):   
        s += str(normalize(float(fields[j]), minima[j], maxima[j])) + ','

    s += str(labels[i]) + '\n'

    outFile.write(s)

outFile.close()








