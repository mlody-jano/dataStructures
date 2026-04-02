import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

def plot_per_structure_fixed(file_path):
    try:
        df = pd.read_csv(file_path, sep=';', header=None, names=['Structure', 'Operation', 'Size', 'Time_ns'])
        
        # Konwersja na liczby i sortowanie
        df['Size'] = pd.to_numeric(df['Size'], errors='coerce')
        df['Time_ns'] = pd.to_numeric(df['Time_ns'], errors='coerce')
        df = df.dropna(subset=['Size', 'Time_ns']).sort_values(by=['Structure', 'Operation', 'Size'])
        
        structures = df['Structure'].unique()
        
        for struct_name in structures:
            struct_data = df[df['Structure'] == struct_name]
            plt.figure(figsize=(12, 7))
            
            # Tworzymy listę unikalnych operacji dla tej struktury
            ops = struct_data['Operation'].unique()
            
            # Rysujemy każdą operację osobno, aby precyzyjnie kontrolować styl
            # Używamy pętli zamiast jednego wywołania lineplot dla lepszej kontroli nakładania
            for i, op_name in enumerate(ops):
                data_op = struct_data[struct_data['Operation'] == op_name]
                
                # Definiujemy style linii dla lepszej widoczności przy pokrywaniu
                line_styles = ['-', '--', ':', '-.', (0, (3, 5, 1, 5)), (0, (5, 10))]
                style = line_styles[i % len(line_styles)]
                
                plt.plot(
                    data_op['Size'], 
                    data_op['Time_ns'], 
                    label=op_name,
                    marker='o',
                    linestyle=style,
                    linewidth=4 - (i * 0.4), # Każda kolejna linia jest ciut cieńsza, by widzieć te pod spodem
                    alpha=0.8,               # Lekka przezroczystość
                    markersize=6
                )
            
            plt.title(f'Analiza operacji dla: {struct_name}', fontsize=14, fontweight='bold')
            plt.xlabel('Rozmiar struktury ($n$)', fontsize=11)
            plt.ylabel('Czas wykonania ($ns$)', fontsize=11)
            
            # Włączamy siatkę i legendę
            plt.grid(True, which="both", ls="-", alpha=0.3)
            plt.legend(title='Typ operacji', bbox_to_anchor=(1.05, 1), loc='upper left')
            
            plt.tight_layout()
            plt.savefig(f"wykres_{struct_name}.png", dpi=300)
            plt.show()
            plt.close()

    except Exception as e:
        print(f"Błąd: {e}")

import pandas as pd
import matplotlib.pyplot as plt
import itertools

def plot_per_operation(file_path):

    df = pd.read_csv(file_path, sep=";")

    base_operations = {
        'Dodawanie': ['AddAtBeginning', 'AddAtEnd', 'AddAtPosition'],
        'Usuwanie': ['DeleteAtBeginning', 'DeleteAtEnd', 'DeleteAtPosition'],
        'Wyszukiwanie': ['Search']
    }

    line_styles = ['-', '--', '-.', ':']
    markers = ['o', 's', '^', 'D', 'v', 'p', '*']

    for base_name, ops in base_operations.items():
        plt.figure(figsize=(12, 7))
        
        style_cycler = itertools.cycle(line_styles)
        marker_cycler = itertools.cycle(markers)
        
        for op in ops:
            for ds in df['DataStructure'].unique():
                subset = df[(df['Operation'] == op) & (df['DataStructure'] == ds)]
                
                if not subset.empty:
                    subset = subset.sort_values(by='Size')
                    
                    if base_name == 'Wyszukiwanie':
                        label = f"{ds}"
                    else:
                        label = f"{ds} - {op}"
                    
                    current_style = next(style_cycler)
                    current_marker = next(marker_cycler)
                    
                    plt.plot(
                        subset['Size'], 
                        subset['AverageTime_ns'], 
                        marker=current_marker, 
                        linestyle=current_style, 
                        linewidth=2,       
                        alpha=0.75,        
                        label=label
                    )
        
        plt.title(f"Złożoność czasowa: {base_name}")
        plt.xlabel("Rozmiar struktury (Size)")
        plt.ylabel("Średni czas wykonania [ns]")
        
        plt.legend(bbox_to_anchor=(1.05, 1), loc='upper left')
        plt.grid(True, linestyle=':', alpha=0.6)
        
        plt.tight_layout()
        
        plt.savefig(f"Wykres_{base_name}.png")
        plt.close()

    print("Generowanie wykresów zakończone pomyślnie! Linie i znaczniki są teraz zróżnicowane.")

plot_per_operation('data/wyniki.csv')