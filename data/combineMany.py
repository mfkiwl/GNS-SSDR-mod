#! /usr/bin/python3

# create a balanced spoofed/clean csv from one clean and several spoofed csv files

from sys import argv
import random;

def combineFiles(fileNames):

    cleanName = fileNames.pop(0)
    spoofedNames = fileNames[:-1]
    outName = fileNames[-1]

    cleanFile = open(cleanName, 'r')
    cleanLines = cleanFile.readlines()
    cleanFile.close()

    header = cleanLines.pop(0)

    spoofedLines = []

    for fName in spoofedNames:

        f = open(fName)
        fLines = f.readlines()
        fLines.pop(0)
        spoofedLines += fLines

    nClean = len(cleanLines)
    nSpoofed = len(spoofedLines)

    mixedLines = cleanLines

    for _ in range(nClean):

        idx = random.randint(0, len(spoofedLines) -1)
        mixedLines.append(spoofedLines[idx])

    random.shuffle(mixedLines)

    outFile = open(outName, 'w')

    outFile.write(header)

    for line in mixedLines:
        outFile.write(line)

    outFile.close()


if len(argv) < 4:
    print("expected args: at least 2 input and1 output file names")

else:

    combineFiles(argv[1:])

    print("done")














