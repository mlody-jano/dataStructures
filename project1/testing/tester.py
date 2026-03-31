import subprocess
import os
import sys
import numpy as np
import matplotlib.pyplot as plt

# Parametry testu
SEED = int(sys.argv[1])
executable_path = "./project1.exe"                                                                          # Hook to C++ executable
data_file = f"testing/data/data_{SEED}.txt"                                                                 # Data file
sizes_to_test = [20000, 40000, 60000, 80000, 100000, 120000, 140000, 160000, 180000, 200000]                # Sizes to test
data_structures = ['DynamicTable', 'SinglyLinkedList', 'DoublyLinkedList']                                  # Data structures to test
repetitions = 4                                                                                           # Number of repetitions for averaging

# Lists for saving results
sizes_plot = np.array(sizes_to_test)
time_push_front = np.zeros_like(sizes_plot, dtype=float)
time_push_back = np.zeros_like(sizes_plot, dtype=float)
time_insert_random = np.zeros_like(sizes_plot, dtype=float)
time_search = np.zeros_like(sizes_plot, dtype=float)

for i, size in enumerate(sizes_to_test):
    avgResultsAdditionFirst = np.zeros((repetitions, 3), dtype=float)  # To store cumulative times for each operation
    avgResultsDeletionFirst = np.zeros((repetitions, 3), dtype=float)  # To store cumulative times for each operation

    avgResultsAdditionLast = np.zeros((repetitions, 3), dtype=float)  # To store cumulative times for each operation
    avgResultsDeletionLast = np.zeros((repetitions, 3), dtype=float)  # To store cumulative times for each operation

    avgResultsAdditionRandom = np.zeros((repetitions, 3), dtype=float)  # To store cumulative times for each operation
    avgResultsDeletionRandom = np.zeros((repetitions, 3), dtype=float)  # To store cumulative times for each operation

    avgResultsSearchRandom = np.zeros((repetitions, 3), dtype=float)    # To store cumulative times for each operation
    for data_structure in data_structures:
        for rep in range(repetitions):
            print(f"Testing {data_structure} with size {size} (Repetition {rep + 1}/{repetitions})...")
            result = subprocess.run([executable_path, str(data_structure), str(size), data_file], capture_output=True, text=True)
            output = result.stdout.strip().splitlines()

            # Parsing output
            for line in output:
                if line.startswith("Addition at front:"):
                    times = list(map(float, line.split(":")[1].strip().split()))
                    avgResultsAdditionFirst[rep] += times
                elif line.startswith("Deletion at front:"):
                    times = list(map(float, line.split(":")[1].strip().split()))
                    avgResultsDeletionFirst[rep] += times
                elif line.startswith("Addition at back:"):
                    times = list(map(float, line.split(":")[1].strip().split()))
                    avgResultsAdditionLast[rep] += times
                elif line.startswith("Deletion at back:"):
                    times = list(map(float, line.split(":")[1].strip().split()))
                    avgResultsDeletionLast[rep] += times
                elif line.startswith("Addition at random position:"):
                    times = list(map(float, line.split(":")[1].strip().split()))
                    avgResultsAdditionRandom[rep] += times
                elif line.startswith("Deletion at random position:"):
                    times = list(map(float, line.split(":")[1].strip().split()))
                    avgResultsDeletionRandom[rep] += times
                elif line.startswith("Search for random element:"):
                    times = list(map(float, line.split(":")[1].strip().split()))
                    avgResultsSearchRandom[rep] += times

    # Averaging results for each operation and data structure
    # Each avgResults* is (repetitions, 3) for 3 data structures
    mean_add_first = np.mean(avgResultsAdditionFirst, axis=0)    # (3,)
    mean_add_last = np.mean(avgResultsAdditionLast, axis=0)
    mean_add_random = np.mean(avgResultsAdditionRandom, axis=0)

    mean_del_first = np.mean(avgResultsDeletionFirst, axis=0)
    mean_del_last = np.mean(avgResultsDeletionLast, axis=0)
    mean_del_random = np.mean(avgResultsDeletionRandom, axis=0)

    mean_search = np.mean(avgResultsSearchRandom, axis=0)        # (1,)

    # Store results for plotting: shape (len(sizes_to_test), 3) for each op
    if i == 0:
        add_front = np.zeros((len(sizes_to_test), 3))
        add_back = np.zeros((len(sizes_to_test), 3))
        add_random = np.zeros((len(sizes_to_test), 3))
        del_front = np.zeros((len(sizes_to_test), 3))
        del_back = np.zeros((len(sizes_to_test), 3))
        del_random = np.zeros((len(sizes_to_test), 3))
        search = np.zeros((len(sizes_to_test), 3))

    add_front[i] = mean_add_first
    add_back[i] = mean_add_last
    add_random[i] = mean_add_random
    del_front[i] = mean_del_first
    del_back[i] = mean_del_last
    del_random[i] = mean_del_random
    # mean_search is (3,) for 3 structures
    search[i] = mean_search

# --- Generating plots ---
print("\nGenerating plots...")

markers = ['o', 's', '^']
colors = ['tab:blue', 'tab:orange', 'tab:green']

# 1. Addition plot
plt.figure(figsize=(10, 6))
for ds_idx, ds_name in enumerate(data_structures):
    plt.plot(sizes_to_test, add_front[:, ds_idx], marker=markers[0], color=colors[ds_idx], linestyle='-', label=f'{ds_name} - Front')
    plt.plot(sizes_to_test, add_back[:, ds_idx], marker=markers[1], color=colors[ds_idx], linestyle='--', label=f'{ds_name} - Back')
    plt.plot(sizes_to_test, add_random[:, ds_idx], marker=markers[2], color=colors[ds_idx], linestyle=':', label=f'{ds_name} - Random')
plt.title('Addition Time Complexity')
plt.xlabel('Structure Size (N)')
plt.ylabel('Execution Time [ns]')
plt.grid(True, linestyle='--', alpha=0.7)
plt.legend(fontsize=9)
plt.tight_layout()
plt.savefig('addition_results.png', dpi=300)
print("Addition plot saved as: 'addition_results.png'.")

# 2. Deletion plot
plt.figure(figsize=(10, 6))
for ds_idx, ds_name in enumerate(data_structures):
    plt.plot(sizes_to_test, del_front[:, ds_idx], marker=markers[0], color=colors[ds_idx], linestyle='-', label=f'{ds_name} - Front')
    plt.plot(sizes_to_test, del_back[:, ds_idx], marker=markers[1], color=colors[ds_idx], linestyle='--', label=f'{ds_name} - Back')
    plt.plot(sizes_to_test, del_random[:, ds_idx], marker=markers[2], color=colors[ds_idx], linestyle=':', label=f'{ds_name} - Random')
plt.title('Deletion Time Complexity')
plt.xlabel('Structure Size (N)')
plt.ylabel('Execution Time [ns]')
plt.grid(True, linestyle='--', alpha=0.7)
plt.legend(fontsize=9)
plt.tight_layout()
plt.savefig('deletion_results.png', dpi=300)
print("Deletion plot saved as: 'deletion_results.png'.")

# 3. Search plot
plt.figure(figsize=(10, 6))
for ds_idx, ds_name in enumerate(data_structures):
    plt.plot(sizes_to_test, search[:, ds_idx], marker=markers[ds_idx], color=colors[ds_idx], linestyle='-', label=f'{ds_name}')
plt.title('Search Time Complexity')
plt.xlabel('Structure Size (N)')
plt.ylabel('Execution Time [ns]')
plt.grid(True, linestyle='--', alpha=0.7)
plt.legend(fontsize=11)
plt.tight_layout()
plt.savefig('search_results.png', dpi=300)
print("Search plot saved as: 'search_results.png'.")
