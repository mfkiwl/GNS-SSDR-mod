#! /usr/bin/python3

from sys import argv
import random

if len(argv) != 5 and len(argv) != 3 :
    print("wrong args")

else:

    fileObj = open(argv[1])
    lines = fileObj.readlines()
    fileObj.close()

    #skip header
    lines.pop[0]

    random.shuffle(lines)

    outDir = argv[2]

    validSize = int(0.15 * len(lines))
    testSize = int(0.15 * len(lines))

    if len(argv) == 5:
        validSize = int(float(argv[3]) * len(lines))
        testSize = int(float(argv[4]) * len(lines))

    trainSize = len(lines) - validSize - testSize;

    # create validation set

    validFile = open(outDir + argv[1].split('/')[-1].split('.')[0] + "_valid.csv", 'w')

    for i in range(validSize):

        idx = random.randint(0, len(lines) -1)
        l = lines.pop(idx)
        validFile.write(l)

    validFile.close()

    # create test set

    testFile = open(outDir + argv[1].split('/')[-1].split('.')[0]  + "_test.csv", 'w')

    for i in range(testSize):

        idx = random.randint(0, len(lines) -1)
        l = lines.pop(idx)
        testFile.write(l)

    testFile.close()

    # create training set

    trainFile = open(outDir + argv[1].split('/')[-1].split('.')[0]  + "_train.csv", 'w')

    for i in range(trainSize):

        idx = random.randint(0, len(lines) -1)
        l = lines.pop(idx)
        trainFile.write(l)

    trainFile.close()















