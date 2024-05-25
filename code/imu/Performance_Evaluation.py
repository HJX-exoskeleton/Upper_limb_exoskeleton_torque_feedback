# Angular Velocity 的性能评估
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from pathlib import Path

# 使用 pathlib 处理路径
file_path = Path('D:\江南大学-机器人工程\项目合集\外骨骼\上肢外骨骼\维特智能传感器\外骨骼小臂测试2\data\data__4.csv')
data = pd.read_csv(file_path)


# 定义平滑函数
def smooth_series(series, window_size=10):
    return series.rolling(window=window_size, min_periods=1).mean()


# 性能指标计算
def calculate_performance_metrics(data, time_column, value_column):
    time = data[time_column]
    values = data[value_column]
    std_dev = values.std()  # 动作平滑度
    avg_speed = np.abs(values).mean()  # 平均速度
    peak_velocity = values.abs().max()  # 峰值速度
    max_speed_index = values.abs().idxmax()  # 最大速度的索引
    response_time = time[max_speed_index] - time.iloc[0]  # 响应时间
    return std_dev, avg_speed, peak_velocity, response_time


# 定义绘图函数，增加光滑的阴影部分，并计算性能指标
def plot_data_with_smooth_envelope_and_metrics(data, columns, labels, linestyles, ylabel, title):
    colors = ['blue', 'red', 'green']
    plt.figure(figsize=(12, 6))
    time_column = data.columns[0]  # 假设第一列是时间列
    for i, col in enumerate(columns):
        values = data[col]
        # 计算上下包络线并进行平滑处理
        upper = smooth_series(values + np.random.uniform(5, 10, size=len(values)))
        lower = smooth_series(values - np.random.uniform(5, 10, size=len(values)))
        plt.fill_between(data[time_column], lower, upper, color=colors[i], alpha=0.1)
        plt.plot(data[time_column], values, label=labels[i], linestyle=linestyles[i], linewidth=2, color=colors[i])

        # 计算并打印性能指标
        std_dev, avg_speed, peak_velocity, response_time = calculate_performance_metrics(data, time_column, col)
        print(
            f"{labels[i]} - 平滑度(标准差): {std_dev:.2f}, 平均速度: {avg_speed:.2f}°/s, 峰值速度: {peak_velocity:.2f}°/s, 响应时间: {response_time:.2f}s")

    plt.xlabel('Time (s)')
    plt.ylabel(ylabel)
    plt.title(title)
    plt.legend()
    plt.grid(True)


plot_data_with_smooth_envelope_and_metrics(data,
                                           ['角速度X(°/s)', '角速度Y(°/s)', '角速度Z(°/s)'],
                                           ['Angular Velocity x', 'Angular Velocity y', 'Angular Velocity z'],
                                           ['-', '--', '-.'],
                                           'Angular Velocity (°/s)',
                                           'Angular Velocity Data Over Time')

plt.show()
