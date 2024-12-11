import pandas as pd
import matplotlib.pyplot as plt

# 讀取 CSV 檔案
path = 'data/threads-parallel-dfs-bfs.csv'
data = pd.read_csv(path)

# 檢查數據
print("原始數據：")
print(data)

# 提取線程數量
thread_counts = data['Thread_Count']

# --- Backtracking Methods ---
# 獲取 Backtracking Serial 方法在 Thread_Count=1 時的基準時間
serial_baseline_backtracking = data.loc[data['Thread_Count'] == 1, 'backtracking-serial'].values[0]

# 確保 Serial_Time 在 Thread_Count=1 是基準
if serial_baseline_backtracking <= 0:
    raise ValueError("backtracking-serial Time at Thread_Count=1 must be greater than 0.")

# 計算加速倍數
speedup_backtracking_pthread = serial_baseline_backtracking / data['backtracking-dfs-pthread']
speedup_backtracking_openmp = serial_baseline_backtracking / data['backtracking-dfs-openmp']

# --- Bruteforce Methods ---
# 獲取 Bruteforce Serial 方法在 Thread_Count=1 時的基準時間
serial_baseline_bruteforce = data.loc[data['Thread_Count'] == 1, 'bruteforce-serial'].values[0]

# 確保 Serial_Time 在 Thread_Count=1 是基準
if serial_baseline_bruteforce <= 0:
    raise ValueError("bruteforce-serial Time at Thread_Count=1 must be greater than 0.")

# 計算加速倍數
speedup_bruteforce_pthread = serial_baseline_bruteforce / data['bruteforce-bfs-pthread']
speedup_bruteforce_openmp = serial_baseline_bruteforce / data['bruteforce-bfs-openmp']

# --- 繪製 Backtracking 加速倍數圖表 ---
plt.figure(figsize=(12, 6))
plt.plot(thread_counts, [1]*len(thread_counts), marker='o', linestyle='-', color='blue', label='Backtracking-Serial')
plt.plot(thread_counts, speedup_backtracking_pthread, marker='s', linestyle='-', color='green', label='Backtracking-DFS-Pthread')
plt.plot(thread_counts, speedup_backtracking_openmp, marker='^', linestyle='-', color='orange', label='Backtracking-DFS-OpenMP')

# 設置標題和軸標籤
plt.title('Backtracking Methods Speedup Comparison')
plt.xlabel('Thread Count')
plt.ylabel('Speedup (Relative to Backtracking-Serial)')

# 顯示圖例
plt.legend()

# 添加網格
plt.grid(True)

# 添加數據標籤（可選）
for i in range(len(thread_counts)):
    plt.annotate(f'{speedup_backtracking_pthread[i]:.2f}', 
                 (thread_counts[i], speedup_backtracking_pthread[i]),
                 textcoords="offset points", xytext=(0,10), ha='center', color='green')
    plt.annotate(f'{speedup_backtracking_openmp[i]:.2f}', 
                 (thread_counts[i], speedup_backtracking_openmp[i]),
                 textcoords="offset points", xytext=(0,-15), ha='center', color='orange')

# 儲存圖表（可選）
plt.savefig('backtracking_speedup_comparison.png', dpi=300, bbox_inches='tight')

# 顯示圖表
plt.show()

# --- 繪製 Bruteforce 加速倍數圖表 ---
plt.figure(figsize=(12, 6))
plt.plot(thread_counts, [1]*len(thread_counts), marker='o', linestyle='-', color='blue', label='Bruteforce-Serial')
plt.plot(thread_counts, speedup_bruteforce_pthread, marker='D', linestyle='-', color='red', label='Bruteforce-BFS-Pthread')
plt.plot(thread_counts, speedup_bruteforce_openmp, marker='*', linestyle='-', color='purple', label='Bruteforce-BFS-OpenMP')

# 設置標題和軸標籤
plt.title('Bruteforce Methods Speedup Comparison')
plt.xlabel('Thread Count')
plt.ylabel('Speedup (Relative to Bruteforce-Serial)')

# 顯示圖例
plt.legend()

# 添加網格
plt.grid(True)

# 添加數據標籤（可選）
for i in range(len(thread_counts)):
    plt.annotate(f'{speedup_bruteforce_pthread[i]:.2f}', 
                 (thread_counts[i], speedup_bruteforce_pthread[i]),
                 textcoords="offset points", xytext=(0,10), ha='center', color='red')
    plt.annotate(f'{speedup_bruteforce_openmp[i]:.2f}', 
                 (thread_counts[i], speedup_bruteforce_openmp[i]),
                 textcoords="offset points", xytext=(0,-15), ha='center', color='purple')

# 儲存圖表（可選）
plt.savefig('bruteforce_speedup_comparison.png', dpi=300, bbox_inches='tight')

# 顯示圖表
# plt.show()
