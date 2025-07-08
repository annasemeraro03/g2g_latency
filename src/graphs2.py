import os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from matplotlib.lines import Line2D

# === Folders ===
FILES_FOLDER = os.path.join(os.getcwd(), 'src', 'realTime_files')
STATS_FOLDER = os.path.join(os.getcwd(), 'src', 'statistics')

# === Set True the type of graph you want to visualize ===
SHOW_SCATTER = False
SHOW_HISTOGRAM = False
SHOW_CANDLESTICK = True
SHOW_VIOLIN = False 

# === === ===
# === SCATTER PLOT ===
# === === ===

if SHOW_SCATTER:
    device = "cameraRaspberry"
    files_to_plot = [
        f"{device}_latency_24062025.csv",
        f"{device}_latency2_24062025.csv",
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
    filename = 'statistics.csv'
    filepath = os.path.join(STATS_FOLDER, filename)

    if not os.path.isfile(filepath):
        print(f"File non trovato: {filename}")
    else:
        df = pd.read_csv(filepath, header=None)
        df.columns = ['Device', 'Test', 'Average', 'Min', 'Max']

        # Crea etichette combinate per x: "device test N"
        x_labels = df['Device'].astype(str) + ' ' + df['Test'].astype(str)
        data = df['Average'] / 1000  # Da us a ms

        plt.figure(figsize=(10, 6))
        plt.bar(x_labels, data, color='skyblue')
        plt.xlabel('Device and Test')
        plt.ylabel('Latency (ms)')
        plt.title('Average Latency per Device and Test')
        plt.xticks(rotation=45, ha='right')
        plt.tight_layout()
        plt.show()

# === === ===
# === CANDLESTICK ===
# === === ===

elif SHOW_CANDLESTICK:
    filename = 'statistics.csv'
    filepath = os.path.join(STATS_FOLDER, filename)

    if not os.path.isfile(filepath):
        print(f"File non trovato: {filename}")
    else:
        df = pd.read_csv(filepath, header=None)
        df.columns = ['Device', 'Test', 'Average', 'Min', 'Max']

        # Crea etichette combinate per x: "device test N"
        device_counts = {}
        x_labels = []
        for device in df['Device']:
            device_counts.setdefault(device, 0)
            device_counts[device] += 1
            x_labels.append(f"{device} Test {device_counts[device]}")

        means = df['Average'] / 1000
        mins = df['Min'] / 1000
        maxs = df['Max'] / 1000

        fig, ax = plt.subplots(figsize=(12, 6))

        x = np.arange(len(x_labels))
        cmap = plt.get_cmap('tab10')
        colors = [cmap(i % cmap.N) for i in range(len(x_labels))]

        for i in range(len(x_labels)):
            ax.vlines(x[i], mins[i], maxs[i], color=colors[i], linewidth=2)

        ax.scatter(x, means, color='black', marker='s', s=80, label='Average')

        ax.set_xticks(x)
        ax.set_xticklabels(x_labels, rotation=45, ha='right')
        ax.set_ylabel('Latency (ms)')
        ax.set_title('Min / Max / Average Latency for Device and Test')

        ax.legend()
        plt.xlabel('Device and Test')
        plt.tight_layout()
        plt.show()

# === === ===
# === VIOLIN PLOT ===
# === === ===

elif SHOW_VIOLIN:
    files = [
        'cameraRaspberry_latency_24062025.csv',
        'cameraRaspberry_latency2_24062025.csv',
        'Stonkam_latency_24062025.csv',
        'Stonkam_latency2_24062025.csv',
        'Stonkam_latency_27062025.csv',
        'Stonkam_latency2_27062025.csv',
    ]
    data = []
    devices = []
    sources = []

    device_counts = {}

    for file in files:
        filepath = os.path.join(FILES_FOLDER, file)
        if not os.path.isfile(filepath):
            print(f"File non trovato: {file}")
            continue

        df = pd.read_csv(filepath, header=None)
        latencies = df.iloc[:, -1].to_numpy() / 1000  # from us to ms

        device = file.split('_')[0]
        device_counts.setdefault(device, 0)
        device_counts[device] += 1
        device_label = f"{device} test {device_counts[device]}"

        source = file.replace('.csv', '')

        data.extend(latencies)
        devices.extend([device_label] * len(latencies))
        sources.extend([source] * len(latencies))

    plot_df = pd.DataFrame({
        'Latency (ms)': data,
        'Device': devices,
        'Source': sources
    })

    plt.figure(figsize=(16, 8))
    ax = sns.violinplot(
        x='Device', y='Latency (ms)', hue='Source',
        data=plot_df, split=False, inner=None, scale="width",
        palette="tab10"
    )

    xticks = ax.get_xticks()
    device_order = [t.get_text() for t in ax.get_xticklabels()]

    # Per la legenda custom
    legend_elements = [Line2D([0], [0], marker='o', color='w', label='Average',
                              markerfacecolor='black', markersize=8)]

    for i, device in enumerate(device_order):
        for j, source in enumerate(plot_df[plot_df['Device'] == device]['Source'].unique()):
            idx = (plot_df['Device'] == device) & (plot_df['Source'] == source)
            latencies = plot_df[idx]['Latency (ms)']
            mean = latencies.mean()
            ax.scatter(xticks[i], mean, color='black', marker='o', s=40, zorder=10)

    ax.set_xlabel("Device")
    ax.set_ylabel("Latency (ms)")
    ax.set_title("Latency Distribution per Device")
    plt.xticks(rotation=45, ha='right')
    # Solo la legenda custom in alto a destra, dentro il grafico
    ax.legend(handles=legend_elements, loc='upper right', title=None)
    plt.tight_layout()
    plt.show()
