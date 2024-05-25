import pandas as pd
import numpy as np
from scipy.signal import butter, filtfilt
import matplotlib.pyplot as plt

# 带通滤波器函数
def butter_bandpass_filter(data, lowcut, highcut, fs, order=5):
    nyq = 0.5 * fs
    low = lowcut / nyq
    high = highcut / nyq
    b, a = butter(order, [low, high], btype='band')
    y = filtfilt(b, a, data)
    return y

# 低通滤波器函数，用于包络线处理
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
file_path = 'D:/江南大学-机器人工程/项目合集/外骨骼/上肢外骨骼/EMG肌电信号/MATLAB程序/上肢肌电信号处理/原始数据/OFF-EXO/naive/4.xlsx'  # 请将此路径更新为你的实际文件路径
data = pd.read_excel(file_path)

# 设置采样频率和滤波器参数
fs = 1000  # 假设的采样频率为1000Hz
lowcut = 20  # 带通滤波低频截止为20Hz
highcut = 450  # 带通滤波高频截止为450Hz
lowpass_cutoff = 10  # 包络线低通滤波截止频率为10Hz，用于平滑

# 对通道2的采集值进行带通滤波处理
filtered_signal = butter_bandpass_filter(data['通道2采集值'], lowcut, highcut, fs, order=6)

# 对通道2的功率值进行带通滤波处理
filtered_power_signal = butter_bandpass_filter(data['通道2功率值'], lowcut, highcut, fs, order=6)

# 计算采集值的上下包络线
envelope_signal = butter_lowpass_filter(np.abs(filtered_signal), lowpass_cutoff, fs, order=4)

# 计算功率值的上下包络线
envelope_power_signal = butter_lowpass_filter(np.abs(filtered_power_signal), lowpass_cutoff, fs, order=4)

# 绘制采集值和功率值的上下包络线在同一张图上
plt.figure(figsize=(15, 6))
plt.plot(envelope_signal, label='Upper envelope of collected value', color='deepskyblue', linestyle='-', linewidth=2, alpha=0.85)
plt.plot(-envelope_signal, label='Lower envelope of collected value', color='skyblue', linestyle='--', linewidth=2, alpha=0.85)
plt.plot(envelope_power_signal, label='Upper envelope of power value', color='tomato', linestyle='-', linewidth=2, alpha=0.85)
plt.plot(-envelope_power_signal, label='Lower envelope of power value', color='darksalmon', linestyle='--', linewidth=2, alpha=0.85)
plt.xlabel('Time/ms')
plt.ylabel('Myoelectric strength')
plt.legend()
plt.tight_layout()
plt.show()


# 计算并打印性能指标
print("通道2采集值的性能指标:")
print(calculate_metrics(filtered_signal))
print("\n通道2功率值的性能指标:")
print(calculate_metrics(filtered_power_signal))