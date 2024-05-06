import subprocess
import re
import csv
import os

def run_binary(binary_path, num_nodes, num_edges):
    command = [binary_path]
    process = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    output, _ = process.communicate(f"{num_nodes}\n{num_edges}\n")
    return output

binary_paths = ["../binaries/tarjan-flags", "../binaries/schmidt-flags"]
nodes = [10000, 30000, 60000, 100000]
density_increments = [0.6, 0.7, 0.8, 0.9]
num_iterations = 1

with open('results.csv', mode='a', newline='') as csvfile:
    fieldnames = ['algorithm', 'node_count', 'edge_count', 'density', 'runtime']
    writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
    writer.writeheader()
    for num_nodes in nodes:
        for density in density_increments:
            num_edges = int(density * (num_nodes * (num_nodes - 1)) / 2)
            for binary_path in binary_paths:
                total_time = 0
                binary_name = os.path.basename(binary_path)
                print("Processing: ", binary_name, num_nodes, num_edges, density)
                for iteration in range(num_iterations):
                    print("Iteration: ", iteration)
                    output = run_binary(binary_path, num_nodes, num_edges)
                    time_taken = re.search(r"Time taken : ([\d.]+) sec", output)
                    if time_taken:
                        total_time += float(time_taken.group(1))
                    else:
                        print(f"Time taken not found in output of {binary_path}")
                avg_time = total_time / num_iterations                
                print("Completed: ", binary_name, num_nodes, num_edges, density)
                writer.writerow({'algorithm': binary_name, 'node_count': num_nodes, 'edge_count': num_edges, 'density': density, 'runtime': avg_time})
