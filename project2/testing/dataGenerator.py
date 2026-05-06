import random
import sys
import csv
import os

def generate_test_data(filename, count, seed_value=0):
    random.seed(seed_value)

    min_val  = -2147483648
    max_val  =  2147483647
    min_prio = 0
    max_prio = 2000000

    print(f"Generowanie {count} rekordów do pliku '{filename}'...")
    print(f"Użyte ziarno (seed): {seed_value}")

    try:
        os.makedirs(os.path.dirname(filename), exist_ok=True)
        with open(filename, 'w', newline='') as f:
            writer = csv.writer(f)                          
            writer.writerow(['value', 'priority'])          
            for _ in range(count):
                writer.writerow([
                    random.randint(min_val, max_val),
                    random.randint(min_prio, max_prio)
                ])
        print("Plik wygenerowany pomyślnie!")
    except IOError as e:
        print(f"Błąd zapisu do pliku: {e}")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Użycie: python generate.py <seed>")
        sys.exit(1)

    SEED         = int(sys.argv[1])
    FILE_NAME    = f"data/data_{SEED}.csv"
    RECORD_COUNT = 200000

    generate_test_data(FILE_NAME, RECORD_COUNT, SEED)