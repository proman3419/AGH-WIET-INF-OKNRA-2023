import csv
import os
import matplotlib.pyplot as plt
from matplotlib.pyplot import figure
import re


OUTPUT_FILES_DIR = "."
MAX_GFLOPS = 44.8


def get_legend_str(f_name):
    return f_name.replace("output_", "").replace(".csv", "")


def get_labels_order(labels):
    A = [(int(labels[i].replace("MMult", "").split("_")[-1]), i) for i in range(len(labels))]
    return [e[1] for e in sorted(A)]


def plot(f_name_pattern_str, save_f_name):
    print(f"plotting {f_name_pattern_str}")
    figure(figsize=(9, 6), dpi=80)
    f_name_pattern = re.compile(f_name_pattern_str)
    for entry in os.scandir(OUTPUT_FILES_DIR):
        if entry.is_file() and \
        f_name_pattern.match(entry.name):
            with open(entry.path) as f:  
                line_reader = csv.reader(f, delimiter=',')
                xlabel, ylabel = tuple(v.strip() for v in next(line_reader))
                # next(line_reader)
                x = []
                y = []
                for row in line_reader:
                    x.append(int(row[0]))
                    y.append(float(row[1]))
                plt.plot(x, y, label=get_legend_str(entry.name), marker="o")
    ax = plt.gca()
    ax.set_xlim([40, 800])
    ax.set_ylim([0, MAX_GFLOPS])            
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)

    handles, labels = ax.get_legend_handles_labels()
    order = get_labels_order(labels)
    plt.legend([handles[idx] for idx in order], [labels[idx] for idx in order], bbox_to_anchor=(1.04, 0.5), loc="center left")
    plt.subplots_adjust(right=0.75)


    plt.savefig(save_f_name)
    print(f"saved as {save_f_name}")


plot("^output_MMult[0-9]+.csv$", "plot_initial.png")
plot("^output_MMult_1x4_[0-9]+.csv$", "plot_1x4.png")
plot("^output_MMult_4x4_[0-9]+.csv$", "plot_4x4.png")
plot("^output_MMult(_[0-9]+x[0-9]+_)?[0-9]+.csv$", "plot_all.png")
