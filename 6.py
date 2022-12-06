if __name__ == '__main__':
    inputFile = open("D:\\LocalStuff\\Dev\\AdventOfCode\\Python-2022\\6\\input.txt", "r")

    inputList = list(inputFile.readline())

    # Part 1
    preambleSize = 4

    for i in range(len(inputList)):

        comparison = inputList[i:i + preambleSize]

        if len(comparison) == len(set(comparison)):
            # No dupes
            print(i+preambleSize)
            break

    # Part 2
    preambleSize = 14
    for i in range(len(inputList)):

        comparison = inputList[i:i + preambleSize]

        if len(comparison) == len(set(comparison)):
            # No dupes
            print(i + preambleSize)
            break
