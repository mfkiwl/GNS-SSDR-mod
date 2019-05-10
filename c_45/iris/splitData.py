#! /usr/bin/python3

from sys import argv
import random

if len(argv) != 4 :
    print("wrong args")

else:

    fileObj = open(argv[1])
    lines = fileObj.readlines()
    fileObj.close()
    random.shuffle(lines)

    validSize = int(float(argv[2]) * len(lines))
    testSize = int(float(argv[3]) * len(lines))
    trainSize = len(lines) - validSize - testSize;

    # create validation set

    validFile = open(argv[1] + "_valid.txt", 'w')

    for i in range(validSize):

        idx = random.randint(0, len(lines) -1)
        l = lines.pop(idx)
        validFile.write(l)

    validFile.close()

    # create test set

    testFile = open(argv[1] + "_test.txt", 'w')

    for i in range(testSize):

        idx = random.randint(0, len(lines) -1)
        l = lines.pop(idx)
        testFile.write(l)

    testFile.close()

    # create training set

    trainFile = open(argv[1] + "_train.txt", 'w')

    for i in range(trainSize):

        idx = random.randint(0, len(lines) -1)
        l = lines.pop(idx)
        trainFile.write(l)

    trainFile.close()















