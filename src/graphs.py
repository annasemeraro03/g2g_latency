import os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns

# === Folders ===
FILES_FOLDER = os.path.join(os.getcwd(), 'src', 'files')
STATS_FOLDER = os.path.join(os.getcwd(), 'src', 'statistics')

# === Set True the type of graph you want to visualize ===
SHOW_SCATTER = False
SHOW_HISTOGRAM = False
SHOW_CANDLESTICK = False
SHOW_VIOLIN = True 

# === === ===
# === SCATTER PLOT ===
# === === ===

if SHOW_SCATTER:
    device = "SamsungGalaxyJ6"
    files_to_plot = [
        f"{device}_videomode_latency.csv",
        f"{device}_photomode_latency.csv"
    ]

    plt.figure(figsize=(12, 6))

    for filename in files_to_plot:
        filepath = os.path.join(FILES_FOLDER, filename)
        if not os.path.isfile(filepath):
            print(f"File non trovato: {filename}")
            continue

        df = pd.read_csv(filepath, header=None)
        x = np.arange(len(df))
        y = df.iloc[:, -1].to_numpy() / 1000  # Da us a ms

        plt.scatter(
            x, y,
            label=filename.replace(f"{device}_", "").replace('_', ' ').replace('.csv', ''),
            s=10
        )

    plt.xlabel('Sample Index')
    plt.ylabel('Latency (ms)')
    plt.title(f'Latency Scatter Plot - {device}')
    plt.legend(fontsize='small', loc='upper right')
    plt.tight_layout()
    plt.show()

# === === ===
# === HISTOGRAM ===
# === === ===

elif SHOW_HISTOGRAM:
    filename = 'photoStatistics.csv'
    filepath = os.path.join(STATS_FOLDER, filename)

    if not os.path.isfile(filepath):
        print(f"File non trovato: {filename}")
    else:
        df = pd.read_csv(filepath, header=None)
        devices = df.iloc[:, 0]
        data = df.iloc[:, 1] / 1000  # Da us a ms

        plt.figure(figsize=(10, 6))
        plt.bar(devices, data, color='skyblue')
        plt.xlabel('Device')
        plt.ylabel('Latency (ms)')
        plt.title('Average Latency per Device - Photomode')
        plt.xticks(rotation=45, ha='right')
        plt.tight_layout()
        plt.show()

# === === ===
# === CANDLESTICK ===
# === === ===

elif SHOW_CANDLESTICK:
    filename = 'photoStatistics.csv'
    filepath = os.path.join(STATS_FOLDER, filename)

    if not os.path.isfile(filepath):
        print(f"File non trovato: {filename}")
    else:
        df = pd.read_csv(filepath, header=None)
        df.columns = ['Device', 'Average', 'Min', 'Max']

        devices = df['Device']
        means = df['Average'] / 1000
        mins = df['Min'] / 1000
        maxs = df['Max'] / 1000

        fig, ax = plt.subplots(figsize=(10, 6))

        x = np.arange(len(devices))
        cmap = plt.get_cmap('tab10')
        colors = [cmap(i % cmap.N) for i in range(len(devices))]

        for i in range(len(devices)):
            ax.vlines(x[i], mins[i], maxs[i], color=colors[i], linewidth=2)

        ax.scatter(x, means, color='black', marker='s', s=80, label='Average')

        ax.set_xticks(x)
        ax.set_xticklabels(devices, rotation=45, ha='right')
        ax.set_ylabel('Latency (ms)')
        ax.set_title('Min / Max / Average Latency - Photomode')

        ax.legend()
        plt.tight_layout()
        plt.show()

# === === ===
# === VIOLIN PLOT ===
# === === ===

elif SHOW_VIOLIN:
    devices = [
        "iPhone13",
        "iPod",
        "SamsungGalaxyA21s",
        "SamsungGalaxyJ5",
        "SamsungGalaxyJ6",
        "HuaweiY5",
        "SonyPSVita"
    ]

    mode = "photomode"
    data = []
    labels = []

    for device in devices:
        filename = f"{device}_{mode}_latency.csv"
        filepath = os.path.join(FILES_FOLDER, filename)
        if not os.path.isfile(filepath):
            print(f"File non trovato: {filename}")
            continue

        df = pd.read_csv(filepath, header=None)
        latencies = df.iloc[:, -1].to_numpy() / 1000  # from us to ms

        data.extend(latencies)
        labels.extend([device] * len(latencies))

    plt.figure(figsize=(14, 7))
    palette = sns.color_palette("tab10", len(devices))

    ax = sns.violinplot(
        x=labels, y=data,
        inner=None, scale="width",
        palette=palette
    )

    
    unique_devices = sorted(set(labels), key=labels.index)
    for i, device in enumerate(unique_devices):
        latencies = [d for d, l in zip(data, labels) if l == device]
        mean = np.mean(latencies)
        ax.scatter(i, mean, color='black', marker='o', s=40, zorder=10, label='Average' if i == 0 else "")

    handles, legend_labels = ax.get_legend_handles_labels()
    if 'Average' not in legend_labels:
        handles.append(
            plt.Line2D([0], [0], marker='o', color='w', markerfacecolor='black', markersize=8, label='Average')
        )
        legend_labels.append('Average')
    ax.legend(handles, legend_labels)

    ax.set_xlabel("Device")
    ax.set_ylabel("Latency (ms)")
    ax.set_title(f"Latency Distribution per Device - {mode.capitalize()}")
    plt.xticks(rotation=45, ha='right')
    plt.tight_layout()
    plt.show()
