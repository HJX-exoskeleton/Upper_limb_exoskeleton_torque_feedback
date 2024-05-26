# Upper_limb_exoskeleton_torque_feedback
ULETF: elbow torque feedback -- dual motor direct drive

给出相关的实验数据、三维模型、硬件部分与代码：

Data：

EMG:

Naive data:

OFF-EXO: 

https://github.com/HJX-exoskeleton/Upper_limb_exoskeleton_torque_feedback/blob/master/data/EMG/OFF-EXO/off-exo_naive.txt

ON-EXO: 

https://github.com/HJX-exoskeleton/Upper_limb_exoskeleton_torque_feedback/blob/master/data/EMG/ON-EXO/on-exo_naive.txt

The processed data:

OFF-EXO: 

https://github.com/HJX-exoskeleton/Upper_limb_exoskeleton_torque_feedback/blob/master/data/EMG/OFF-EXO/off-exo_conversion.xlsx

ON-EXO:

https://github.com/HJX-exoskeleton/Upper_limb_exoskeleton_torque_feedback/blob/master/data/EMG/ON-EXO/on-exo_conversion.xlsx

OFF-EXO emg envelope:
![image](https://github.com/HJX-exoskeleton/Upper_limb_exoskeleton_torque_feedback/assets/156507453/91ae1c8d-03a1-436c-a62a-810d3eb88c9f)
![image](https://github.com/HJX-exoskeleton/Upper_limb_exoskeleton_torque_feedback/assets/156507453/87ea9b5a-7dc5-4420-a9a8-7f9e23d02596)

ON-EXO emg envelope:
![image](https://github.com/HJX-exoskeleton/Upper_limb_exoskeleton_torque_feedback/assets/156507453/cb4f20ce-6844-4856-b420-1d592d8c0d0c)
![image](https://github.com/HJX-exoskeleton/Upper_limb_exoskeleton_torque_feedback/assets/156507453/4c82a184-8adf-4e67-9c09-507bdae24c66)

IMU:
https://github.com/HJX-exoskeleton/Upper_limb_exoskeleton_torque_feedback/blob/master/data/IMU/data__imu.csv

Model:
Modeling was done using UG software
![image](https://github.com/HJX-exoskeleton/Upper_limb_exoskeleton_torque_feedback/assets/156507453/f5f0fbc8-141a-4946-9469-4f6b310c477b)

Hardware part:
SimpleFOC控制板(Simple103)/5008无刷电机/2804云台电机
![image](https://github.com/HJX-exoskeleton/Upper_limb_exoskeleton_torque_feedback/assets/156507453/b49bbb99-9c64-474e-b544-16193065b632)

Code:

control: 
https://github.com/HJX-exoskeleton/Upper_limb_exoskeleton_torque_feedback/tree/master/code/control/HapticControl_5008%2B2804

emg: 
https://github.com/HJX-exoskeleton/Upper_limb_exoskeleton_torque_feedback/tree/master/code/emg

imu: 
https://github.com/HJX-exoskeleton/Upper_limb_exoskeleton_torque_feedback/tree/master/code/imu

Figure(experiment):
![1](https://github.com/HJX-exoskeleton/Upper_limb_exoskeleton_torque_feedback/assets/156507453/16e6d501-ab6b-4ddd-b9a5-30ffdb062608)
![image](https://github.com/HJX-exoskeleton/Upper_limb_exoskeleton_torque_feedback/assets/156507453/3e232c8c-4416-4768-b22c-74958313cea4)

