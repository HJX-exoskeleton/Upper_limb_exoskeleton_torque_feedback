import pandas as pd
import numpy as np
from scipy.signal import butter, filtfilt
import matplotlib.pyplot as plt

# 定义带通滤波器函数
def butter_bandpass_filter(data, lowcut, highcut, fs, order=5):
    nyq = 0.5 * fs
    low = lowcut / nyq
    high = highcut / nyq
    b, a = butter(order, [low, high], btype='band')
    y = filtfilt(b, a, data)
    return y

# 定义低通滤波器函数，用于包络线处理
def butter_lowpass_filter(data, cutoff, fs, order=5):
    nyq = 0.5 * fs
    normal_cutoff = cutoff / nyq
    b, a = butter(order, normal_cutoff, btype='low')
    y = filtfilt(b, a, data)
    return y

# 定义计算性能指标的函数
def calculate_metrics(signal):
    metrics = {
        'Max Value': np.max(signal),
        'Min Value': np.min(signal),
        'RMS': np.sqrt(np.mean(signal**2)),
        'Mean Absolute Value': np.mean(np.abs(signal))
    }
    return metrics

# 读取Excel文件
file_path = 'D:/江南大学-机器人工程/项目合集/外骨骼/上肢外骨骼/EMG肌电信号/MATLAB程序/上肢肌电信号处理/原始数据/ON-EXO/naive/4.xlsx'  # Update this path to your actual file path
data = pd.read_excel(file_path)

# 假设的采样频率和滤波器参数
fs = 1000  # 采样频率为1000Hz
lowcut = 20  # 带通滤波低频截止为20Hz
highcut = 450  # 带通滤波高频截止为450Hz
lowpass_cutoff = 10  # 包络线低通滤波截止频率为10Hz，用于平滑

# 对通道2的采集值进行带通滤波处理
filtered_signal = butter_bandpass_filter(data['通道2采集值'], lowcut, highcut, fs, order=6)

# 对通道2的功率值进行带通滤波处理
filtered_power_signal = butter_bandpass_filter(data['通道2功率值'], lowcut, highcut, fs, order=6)

# 计算并打印性能指标
print("通道2采集值的性能指标:")
print(calculate_metrics(filtered_signal))
print("\n通道2功率值的性能指标:")
print(calculate_metrics(filtered_power_signal))

# 绘制滤波后的采集值信号与包络线
plt.figure(figsize=(15, 6))
plt.plot(filtered_signal, label='The collected value after filtering', color='skyblue', linewidth=1)
envelope_signal = butter_lowpass_filter(np.abs(filtered_signal), lowpass_cutoff, fs, order=4)
plt.plot(envelope_signal, label='Upper envelope of the collected value', color='r', linestyle='-', linewidth=2)
plt.plot(-envelope_signal, label='Lower envelope of the collected value', color='g', linestyle='-', linewidth=2)
plt.xlabel('Time/ms')
plt.ylabel('Myoelectric strength')
plt.legend()
plt.tight_layout()
plt.show()

# 绘制滤波后的功率值信号与包络线
plt.figure(figsize=(15, 6))
plt.plot(filtered_power_signal, label='The power value after filtering', color='darksalmon', linewidth=1)
envelope_power_signal = butter_lowpass_filter(np.abs(filtered_power_signal), lowpass_cutoff, fs, order=4)
plt.plot(envelope_power_signal, label='Upper envelope of the power value', color='r', linestyle='-', linewidth=2)
plt.plot(-envelope_power_signal, label='Lower envelope of the power value', color='g', linestyle='-', linewidth=2)
plt.xlabel('Time/ms')
plt.ylabel('Myoelectric strength')
plt.legend()
plt.tight_layout()
plt.show()


# 图片拼接
plt.figure(figsize=(15, 12))  # 增加画布大小以适应两个图

# 绘制滤波后的采集值信号与包络线
plt.subplot(2, 1, 1)  # 两行一列的第一个图
plt.plot(filtered_signal, label='The collected value after filtering', color='skyblue', linewidth=1)
envelope_signal = butter_lowpass_filter(np.abs(filtered_signal), lowpass_cutoff, fs, order=4)
plt.plot(envelope_signal, label='Upper envelope of the collected value', color='red', linestyle='-', linewidth=2)
plt.plot(-envelope_signal, label='Lower envelope of the collected value', color='green', linestyle='-', linewidth=2)
plt.xlabel('Time (ms)', fontsize=10)
plt.ylabel('Myoelectric Strength', fontsize=10)
plt.title('Collected Value and Its Envelopes', fontsize=10)
plt.legend(loc='upper right', fontsize=10)
plt.grid(True, which='both', linestyle='--', linewidth=0.5, alpha=0.5)

# 绘制滤波后的功率值信号与包络线
plt.subplot(2, 1, 2)  # 两行一列的第二个图
plt.plot(filtered_power_signal, label='The power value after filtering', color='darksalmon', linewidth=1)
envelope_power_signal = butter_lowpass_filter(np.abs(filtered_power_signal), lowpass_cutoff, fs, order=4)
plt.plot(envelope_power_signal, label='Upper envelope of the power value', color='red', linestyle='-', linewidth=2)
plt.plot(-envelope_power_signal, label='Lower envelope of the power value', color='green', linestyle='-', linewidth=2)
plt.xlabel('Time (ms)', fontsize=10)
plt.ylabel('Myoelectric Strength', fontsize=10)
plt.title('Power Value and Its Envelopes', fontsize=10)
plt.legend(loc='upper right', fontsize=10)
plt.grid(True, which='both', linestyle='--', linewidth=0.5, alpha=0.5)

plt.tight_layout()
plt.show()
