import csv, os, math

def standard_deviation(values):
    if len(values) < 2:
        return 0
    average = sum(values) / len(values)
    variation = sum((x - average) ** 2 for x in values) / (len(values) - 1)
    return int(math.sqrt(variation))

def read_values_from_csv(file_path, column_index=1):
    values = []
    with open(file_path, newline='\n', encoding='utf-8') as csvfile:
        reader = csv.reader(csvfile)
        for row in reader:
            if len(row) > column_index:
                try:
                    values.append(int(row[column_index]))
                except ValueError:
                    pass  # ignora values non convertibili
    return values

def print_statistics(values, device_name="", test=""):
    if not values:
        print("No valid values find")
        return
    maximum = max(values)
    minimum = min(values)
    average = int(sum(values) / len(values))
    dev_std = standard_deviation(values)

    print(f"Max value: {maximum}")
    print(f"Min value: {minimum}")
    print(f"Standard deviation: {dev_std}")
    print(f"Average: {average}")
    
    with open(os.path.join(os.getcwd(), 'src', 'statistics', f'statistics.csv'), 'a') as f:
        f.write(f"{device_name},{test},{average},{minimum},{maximum}\n")

if __name__ == "__main__":
    device_name = "Stonkam" 
    # file_csv = os.path.join(os.getcwd(), 'src', 'files', f'{device_name}_videomode_latency.csv')
    file_csv = os.path.join(os.getcwd(), 'src', 'realTime_files', f'{device_name}_latency2_27062025.csv')
    values = read_values_from_csv(file_csv)
    print_statistics(values, device_name, "Test4")