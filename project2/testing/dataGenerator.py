import random
import sys

# ZMIENIC NA GENERATOR DO FORMATU CSV -> value, priority

def generate_test_data(filename, count, seed_value = 0):

    random.seed(seed_value)                                             # Setting the seed for reproducibility
    
    min_val = -2147483648                                               # Minimum value for 4 byte signed integer
    max_val = 2147483647                                                # Maximum value for 4 byte signed integer

    print(f"Generowanie {count} rekordów do pliku '{filename}'...")
    print(f"Użyte ziarno (seed): {seed_value}")
    
    try:
        with open(filename, 'w') as f:
            for i in range(count):
                f.write(f"{random.randint(min_val, max_val)}\n")        # Generating random integers from specified range and saving to file
        print("Plik wygenerowany pomyślnie!")
    except IOError as e:
        print(f"Błąd zapisu do pliku: {e}")

if __name__ == "__main__":
    SEED = int(sys.argv[1])                                             # Catching seed value from command line arguments
    FILE_NAME = f"data/data_{SEED}.csv"                                 # Specyfing data file
    RECORD_COUNT = 200000                                               # Specyfing number of records to generate
    
    generate_test_data(FILE_NAME, RECORD_COUNT, SEED)