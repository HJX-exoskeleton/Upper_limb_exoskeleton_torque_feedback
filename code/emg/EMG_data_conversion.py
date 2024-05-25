import pandas as pd
import os


# 修改此函数以处理用空格分隔的数据
def read_txt_and_process(file_path):
    data = []  # 初始化空列表存储处理后的数据
    with open(file_path, 'r', encoding='utf-8') as file:
        for line in file:
            # 检查行是否为空或仅包含空格
            if line.strip():
                # 使用split()在空白处分割，适用于多空格分隔的情况
                split_line = line.strip().split()
                data.append(split_line)
    return data


# 修改此函数以将数据转换为DataFrame然后保存到Excel文件
def save_to_excel(data, output_file_path):
    # 确保输出文件的目录存在
    os.makedirs(os.path.dirname(output_file_path), exist_ok=True)

    # 创建DataFrame，假设data是一个二维列表，每个子列表是一行
    df = pd.DataFrame(data)
    # 保存到Excel
    df.to_excel(output_file_path, index=False, header=False)


# 主函数：将TXT转换为Excel并处理数据
def convert_txt_to_excel(txt_file_path, excel_file_path):
    data = read_txt_and_process(txt_file_path)
    save_to_excel(data, excel_file_path)
    print(f"File '{txt_file_path}' has been converted to Excel file '{excel_file_path}' successfully.")


# 使用示例
txt_file_path = 'D:/江南大学-机器人工程/项目合集/外骨骼/上肢外骨骼/EMG肌电信号/MATLAB程序/上肢肌电信号处理/原始数据/OFF-EXO/naive/4.txt'  # 更换为你的TXT文件路径
excel_file_path = 'D:/江南大学-机器人工程/项目合集/外骨骼/上肢外骨骼/EMG肌电信号/MATLAB程序/上肢肌电信号处理/原始数据/OFF-EXO/naive/4.xlsx'  # 更换为你想要的Excel文件路径

convert_txt_to_excel(txt_file_path, excel_file_path)
