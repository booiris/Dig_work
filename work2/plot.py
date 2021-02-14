import matplotlib.pyplot as plt

for i in range(10):
    file_name = "plot_file/" + str(i) + '-1.txt'
    with open(file_name, "r") as f:
        x = []
        for line in f:
            x.append(int(line))
    plt.plot(x)

    file_name = "plot_file/" + str(i) + '.txt'
    with open(file_name, "r") as f:
        x = []
        for line in f:
            x.append(int(line))
    plt.plot(x)
    plt.figure(dpi=500)
    plt.show()
