import subprocess
import os
import sys
import numpy as np
import matplotlib.pyplot as plt

# Parametry testu
SEED = int(sys.argv[1])
executable_path = "./project1DoubleList.exe"                                                                # Hook to C++ executable
data_file = f"testing/data/data_{SEED}.txt"                                                                 # Data file
sizes_to_test = [20000, 40000, 60000, 80000, 100000, 120000, 140000, 160000, 180000, 200000]                # Sizes to test
data_structures = ['DynamicTable', 'SinglyLinkedList', 'DoublyLinkedList']                                  # Data structures to test
repetitions = 100                                                                                           # Number of repetitions for averaging

# Lists for saving results
sizes_plot = np.array(sizes_to_test)
time_push_front = np.zeros_like(sizes_plot, dtype=float)
time_push_back = np.zeros_like(sizes_plot, dtype=float)
time_insert_random = np.zeros_like(sizes_plot, dtype=float)
time_search = np.zeros_like(sizes_plot, dtype=float)

for i, size in enumerate(sizes_to_test):
    avg_results = np.zeros((4, repetitions), dtype=float)  # To store cumulative times for each operation
    for rep in range(repetitions):
        result = subprocess.run([executable_path, str(data_structures[2]), str(size), data_file], capture_output=True, text=True)           # API call to C++ executable 

        if result.returncode == 0:
            output = result.stdout.strip().split(',')
            avg_results[0, rep] = float(output[0])
            avg_results[1, rep] = float(output[1])
            avg_results[2, rep] = float(output[2])
            avg_results[3, rep] = float(output[3])
        else:
            print(f"Error for size {size}: {result.stderr}")
            break

    # Averaging
    avg_results = np.mean(avg_results, axis=0)                                                              # Average times for each operation

    # Saving results for plotting
    sizes_plot[i] = size
    time_push_front[i] = avg_results[0]
    time_push_back[i] = avg_results[1]
    time_insert_random[i] = avg_results[2]
    time_search[i] = avg_results[3]

# --- Generating plot ---
print("\nGenerating plot...")

plt.figure(figsize=(10, 6))

# Drawing lines for each operation
plt.plot(sizes_plot, time_push_front, label='Push Front', marker='o', linewidth=2)
plt.plot(sizes_plot, time_push_back, label='Push Back', marker='s', linewidth=2)
plt.plot(sizes_plot, time_insert_random, label='Insert Random', marker='^', linewidth=2)
plt.plot(sizes_plot, time_search, label='Search', marker='d', linewidth=2)

# Configuring plot
plt.title('Time Complexity of Data Structure Operations', fontsize=14)
plt.xlabel('Structure Size (N)', fontsize=12)
plt.ylabel('Execution Time [nanoseconds]', fontsize=12)
plt.grid(True, linestyle='--', alpha=0.7)
plt.legend(fontsize=11)
plt.tight_layout()

# Saving to file and displaying
plt.savefig('complexity_results.png', dpi=300)
print("Plot saved as: 'complexity_results.png'.")
