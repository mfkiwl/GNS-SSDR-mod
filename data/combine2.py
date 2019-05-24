#! /usr/bin/python3

from sys import argv

if len(argv) != 4:
    print("expected args: 2 input and1 output file names")

else:

    fileObj1 = open(argv[1])
    lines1 = fileObj1.readlines()
    fileObj1.close()

    fileObj2 = open(argv[2])
    lines2 = fileObj2.readlines()
    fileObj2.close()

    header = lines1.pop(0)
    lines2.pop(0)

    outFile = open(argv[3], 'w')

    outFile.write(header)

    for line in lines1:
        outFile.write(line)

    for line in lines2:
        outFile.write(line)

    outFile.close()

    print("done")


