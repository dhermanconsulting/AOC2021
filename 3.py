def GetLetterScore(letter):
    asciiCode = ord(letter[0])
    if asciiCode > 96:
        return asciiCode - 96
    else:
        return asciiCode - 65 + 27
    return 0


if __name__ == '__main__':

    inputFile = open("D:\\LocalStuff\\Dev\\AdventOfCode\\Python-2022\\3\\input.txt", "r")

    totalScore = 0

    for line in inputFile:
        lineArr = list(line.rstrip())

        totalScore += GetLetterScore(
            set(lineArr[0: int(len(lineArr) / 2)]).intersection(lineArr[int(len(lineArr) / 2): len(lineArr)]).pop())


    print(totalScore)
