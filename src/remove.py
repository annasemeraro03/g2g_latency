import csv, os 

input_file = os.path.join(os.getcwd(), 'src', 'files', 'Stonkam_latency_27062025.csv')
output_file = os.path.join(os.getcwd(), 'src', 'files', 'Stonkam_latency_27062025_no3rd.csv')

with open(input_file, 'r', newline='\n', encoding='utf-8') as infile, open(output_file, 'w', newline='\n', encoding='utf-8') as outfile:
    reader = csv.reader(infile)
    writer = csv.writer(outfile)
    for row in reader:
        if len(row) >= 3:
            del row[2]
        writer.writerow(row)