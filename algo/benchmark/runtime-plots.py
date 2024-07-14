import csv
import matplotlib.pyplot as plt

# Load data from CSV
data = []
with open('sparse-results.csv', newline='') as csvfile:
    reader = csv.DictReader(csvfile)
    for row in reader:
        if float(row['runtime']) != 0.0:
            data.append(row)

# Organize data into dictionaries with (node_count, edge_count) as keys for each algorithm
schmidt_data = {}
tarjan_data = {}
for entry in data:
    key = (int(entry['node_count']), int(entry['edge_count']))
    if entry['algorithm'] == 'schmidt-flags':
        if key not in schmidt_data:
            schmidt_data[key] = []
        schmidt_data[key].append(float(entry['runtime']))
    elif entry['algorithm'] == 'tarjan-flags':
        if key not in tarjan_data:
            tarjan_data[key] = []
        tarjan_data[key].append(float(entry['runtime']))

# Sort the keys based on (node_count, edge_count)
sorted_keys = sorted(set(schmidt_data.keys()) | set(tarjan_data.keys()))

# Plotting
x_axis_labels = [f"{key}" for key in sorted_keys]
x = range(len(sorted_keys))
schmidt_y = [sum(schmidt_data.get(key, [0])) / len(schmidt_data.get(key, [0])) for key in sorted_keys]
tarjan_y = [sum(tarjan_data.get(key, [0])) / len(tarjan_data.get(key, [0])) for key in sorted_keys]

plt.figure(figsize=(10, 6))
plt.plot(x, schmidt_y, label='schmidt-flags')
plt.plot(x, tarjan_y, label='tarjan-flags')
# plt.yscale('log')  # Set y-axis scale to logarithmic
plt.xticks(x, x_axis_labels, rotation='vertical')
plt.ylabel('Runtime (log scale)')
plt.xlabel('(node_count, edge_count)')
plt.title('Sparse Graphs - Average Runtime for (node_count, edge_count) Pairs')
plt.legend()
plt.tight_layout()
plt.show()
