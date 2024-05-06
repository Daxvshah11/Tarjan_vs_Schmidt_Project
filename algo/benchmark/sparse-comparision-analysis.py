import subprocess
import re
import csv
import os

def run_binary(binary_path, num_nodes, num_edges):
    command = [binary_path]
    process = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    output, _ = process.communicate(f"{num_nodes}\n{num_edges}\n")
    return output

def sweep_edges(binary_paths, nodes_list, edge_multipliers, num_iterations):
    with open('results.csv', mode='a', newline='') as csvfile:
        fieldnames = ['algorithm', 'node_count', 'edge_count', 'runtime']
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        for num_nodes in nodes_list:
            for edge_multiplier in edge_multipliers:
                for binary_path in binary_paths:
                    num_edges = int(edge_multiplier * num_nodes)
                    total_time = 0
                    binary_name = os.path.basename(binary_path)
                    print("Processing: ", binary_name, num_nodes, num_edges)
                    for _ in range(num_iterations):
                        output = run_binary(binary_path, num_nodes, num_edges)
                        time_taken = re.search(r"Time taken : ([\d.]+) sec", output)
                        if time_taken:
                            total_time += float(time_taken.group(1))
                        else:
                            print(f"Time taken not found in output of {binary_path}")
                    avg_time = total_time / num_iterations                
                    print("Completed: ", binary_name, num_nodes, num_edges)
                    writer.writerow({'algorithm': binary_name, 'node_count': num_nodes, 'edge_count': num_edges, 'runtime': avg_time})

binary_paths = ["../binaries/tarjan-flags", "../binaries/schmidt-flags"]
nodes_list = [100000000]
num_iterations = 1
edge_multipliers = [0.001, 0.01, 0.1, 0.5, 1, 5, 10]
sweep_edges(binary_paths, nodes_list, edge_multipliers, num_iterations)