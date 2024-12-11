import pandas as pd
import matplotlib.pyplot as plt

path='data/threads-parallel-dfs-bfs.csv'
# 讀取 CSV 檔案
data = pd.read_csv(path)

# 檢查數據
print("原始數據：")
print(data)

# 提取數據
thread_counts = data['Thread_Count']

# 獲取 Serial 方法在 Thread_Count=1 時的基準時間
serial_baseline = data.loc[data['Thread_Count'] == 1, 'Serial_Time'].values[0]

# 確保 Serial_Time 在 Thread_Count=1 是基準
if serial_baseline <= 0:
    raise ValueError("Serial_Time at Thread_Count=1 must be greater than 0.")

# 計算加速倍數
speedup_backtracking_pthread = serial_baseline / data['backtracking-dfs-pthread']
speedup_backtracking_openmp = serial_baseline / data['backtracking-dfs-openmp']
speedup_bruteforce_pthread = serial_baseline / data['bruteforce-bfs-pthread']
speedup_bruteforce_openmp = serial_baseline / data['bruteforce-bfs-openmp']

# 創建圖表
plt.figure(figsize=(12, 8))

# 繪製 Serial Method 曲線（加速倍數為1）
plt.plot(thread_counts, [1]*len(thread_counts), marker='o', linestyle='-', color='blue', label='Serial Method')

# 繪製 Backtracking-DFS-Pthread Method 曲線
plt.plot(thread_counts, speedup_backtracking_pthread, marker='s', linestyle='--', color='green', label='Backtracking-DFS-Pthread')

# 繪製 Backtracking-DFS-OpenMP Method 曲線
plt.plot(thread_counts, speedup_backtracking_openmp, marker='^', linestyle='-.', color='orange', label='Backtracking-DFS-OpenMP')

# 繪製 Bruteforce-BFS-Pthread Method 曲線
plt.plot(thread_counts, speedup_bruteforce_pthread, marker='D', linestyle=':', color='red', label='Bruteforce-BFS-Pthread')

# 繪製 Bruteforce-BFS-OpenMP Method 曲線
plt.plot(thread_counts, speedup_bruteforce_openmp, marker='*', linestyle='-', color='purple', label='Bruteforce-BFS-OpenMP')

# 設置標題和軸標籤
plt.title('Performance Speedup Comparison: Serial, Backtracking-DFS-Pthread, Backtracking-DFS-OpenMP, Bruteforce-BFS-Pthread, Bruteforce-BFS-OpenMP Methods')
plt.xlabel('Thread Count')
plt.ylabel('Speedup (Relative to Serial)')

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
    plt.annotate(f'{speedup_bruteforce_pthread[i]:.2f}', 
                 (thread_counts[i], speedup_bruteforce_pthread[i]),
                 textcoords="offset points", xytext=(0,10), ha='center', color='red')
    plt.annotate(f'{speedup_bruteforce_openmp[i]:.2f}', 
                 (thread_counts[i], speedup_bruteforce_openmp[i]),
                 textcoords="offset points", xytext=(0,-15), ha='center', color='purple')

# 儲存圖表（可選）
plt.savefig('performance_speedup_comparison.png', dpi=300, bbox_inches='tight')

# 顯示圖表
# plt.show()
