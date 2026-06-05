import csv
import random
from pathlib import Path


ROWS_PER_FILE = 1_000_000
SEEDS = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]

OUTPUT_DIR = Path("data")
OUTPUT_DIR.mkdir(exist_ok=True)

INT_MIN = -2_147_483_648
INT_MAX = 2_147_483_647


def generate_csv(seed: int, rows: int = ROWS_PER_FILE) -> None:
    rng = random.Random(seed)

    output_file = OUTPUT_DIR / f"s{seed}.csv"

    with output_file.open("w", newline="", encoding="utf-8") as file:
        writer = csv.writer(file)
        writer.writerow(["key", "value"])

        for _ in range(rows):
            key = rng.randint(1, INT_MAX)
            value = rng.randint(INT_MIN, INT_MAX)
            writer.writerow([key, value])

    print(f"Generated: {output_file}")


def main() -> None:
    for seed in SEEDS:
        generate_csv(seed)


if __name__ == "__main__":
    main()