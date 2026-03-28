import subprocess
import os
import sys
import matplotlib.pyplot as plt

# Parametry testu
SEED = int(sys.argv[1])
executable_path = "./project1"                                                                              # Path to compiled C++ executable
data_file = f"testing/data/data_{SEED}.txt"                                                                 # Data file
sizes_to_test = [20000, 40000, 60000, 80000, 100000, 120000, 140000, 160000, 180000, 200000]                # Sizes to test
repetitions = 100                                                                                           # Number of repetitions for averaging

print(f"{'Size':>10} | {'PushFront':>10} | {'PushBack':>10} | {'InsertRand':>10} | {'Search':>10}")
print("-" * 60)

# Lists for saving results
sizes_plot = []
time_push_front = []
time_push_back = []
time_insert_random = []
time_search = []

for size in sizes_to_test:
    avg_results = [0.0, 0.0, 0.0, 0.0]
    
    for _ in range(repetitions):
        result = subprocess.run([executable_path, str(size), data_file], capture_output=True, text=True)
        
        if result.returncode == 0:
            output = result.stdout.strip().split(',')
            avg_results[0] += float(output[1])
            avg_results[1] += float(output[2])
            avg_results[2] += float(output[3])
            avg_results[3] += float(output[4])
        else:
            print(f"Error for size {size}: {result.stderr}")
            break

    # Averaging
    avg_results = [res / repetitions for res in avg_results]

    # Saving results to console
    print(f"{size:10} | {avg_results[0]:10.0f} | {avg_results[1]:10.0f} | {avg_results[2]:10.0f} | {avg_results[3]:10.0f}")
    
    # Saving results for plotting
    sizes_plot.append(size)
    time_push_front.append(avg_results[0])
    time_push_back.append(avg_results[1])
    time_insert_random.append(avg_results[2])
    time_search.append(avg_results[3])

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
