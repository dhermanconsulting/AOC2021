if __name__ == '__main__':

    totalCalories = []

    inputFile = open("D:\\LocalStuff\\Dev\\AdventOfCode\\Python-2022\\1\\input.txt", "r")

    lines = inputFile.readlines()

    currentCalories = 0

    for line in lines:
        if line == "\n":
            totalCalories.append(currentCalories)
            currentCalories = 0
            continue

        currentCalories = currentCalories + int(line)

    totalCalories.sort(reverse=True)

    print(F"Most calories: {totalCalories[0]}")

    addTop = 3
    topxCals = 0

    for i in range(addTop):
        topxCals = topxCals + totalCalories[i]

    print(F"Top {addTop} Total: {topxCals}")
