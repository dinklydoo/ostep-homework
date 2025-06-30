lottery = [783799,303313,476597,583382,908113,504687,281838,755804,618369,
           250506,909747,982786,810218,902166,310147,729832,898839]

jobs = [8, 4, 5]
tickets = [75, 25, 40]

for n in lottery:
    total = 140
    ran = 0
    for i in range(0,3):
        if (jobs[i] == 0): total -= tickets[i]

    for i in range(0,3):
        if (jobs[i] == 0): continue

        if (n % total <= ran + tickets[i]):
            jobs[i] -= 1
            print(f"Run Job: {i}, Time remaining: {jobs[i]}")
            break
        else:
            ran += tickets[i]