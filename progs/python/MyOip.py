#https://pythonbaba.com/online-python-code-editor-and-ide-for-data-science/

import matplotlib.pyplot as plt
import numpy as np

mydata = [2, 4, 6, 7, 10, 12, 14, 15, 17, 19, 21, 23, 25, 27, 29, 31, 31, 35, 36, 37, 39, 41, 43, 43, 47, 47, 49, 51, 53, 54, 56, 57, 59, 60, 63, 63, 66, 67, 68, 70, 71, 72, 75, 75, 77, 78, 79, 81, 82, 83, 85, 85, 87, 87, 90, 91, 93, 94, 95, 96, 98, 99, 99, 102, 103, 103, 105, 106, 107, 108, 110, 111, 111, 112, 113, 115, 116, 117, 118, 119, 120, 122, 122, 123, 124, 125, 126, 127, 129, 130, 131, 131, 133, 133, 135, 135, 137, 138, 138, 139, 140, 141, 141, 143, 143, 144, 145, 146, 147, 148, 149, 149, 150, 151, 151, 153, 154, 155, 155, 156, 156, 157, 158, 159, 159, 159, 161, 161, 163, 163, 164, 165, 165, 165, 166, 167, 168, 169, 169, 170, 170, 171, 171, 172, 173, 173, 175, 174, 175, 176, 176, 177, 178, 178, 179, 179, 180, 180, 182, 181, 183, 183, 184, 184, 184, 185, 186, 185, 186, 187, 187, 188, 189, 189, 189, 190, 190, 191, 191, 192, 193, 193, 193, 194, 194, 195, 195, 197, 195, 196, 197, 197, 198, 198, 199, 199, 199, 200, 200, 201, 201, 202, 202, 202, 202, 203, 203, 203, 204, 205, 204, 206, 205, 206, 207, 207, 207, 207, 207, 208, 208, 209, 208, 209, 209, 210, 210, 211, 210, 211, 212, 212, 212, 212, 213, 213, 213, 213, 214, 214, 214, 214, 215, 215, 216, 216, 217, 216, 216, 217, 217, 217, 218, 218, 218, 218, 219, 220, 220, 219, 220, 220, 221, 220, 221, 221, 221, 221, 222, 222, 222, 223, 223, 223, 224, 223, 223, 223, 224, 225, 225, 224, 225, 225, 225, 225, 225, 226, 226, 226, 226, 226, 227, 227, 227, 228, 227, 228, 228, 228, 228, 229, 229, 229, 229, 230, 230, 230, 230, 230, 230, 230, 231, 230, 230, 231, 231, 231, 231, 231, 232, 232, 232, 232, 232, 232, 232, 233, 233, 233, 233, 233, 233, 233, 233, 234, 234, 234, 235, 234, 234, 234, 235, 235, 235, 235, 235, 235, 235, 235, 236, 235, 236, 236, 236, 236, 236, 237, 237, 236, 236, 237, 237, 237, 237, 238, 238, 238, 237, 238, 238, 238, 239, 238, 239, 238, 239, 239, 239, 239, 239, 239, 239, 239, 239, 239, 240, 239, 240, 239, 240, 240, 240, 239, 240, 241, 241, 240, 241, 241, 240, 241, 241, 241, 241, 241, 241, 241, 241, 241, 241, 241, 241, 242, 241, 242, 242, 242, 242, 241, 243, 242, 242, 242, 243, 242, 243, 243, 243, 243, 243, 243, 243, 243, 243, 243, 244, 243, 243, 243, 244, 243, 244, 243, 243, 244, 244, 244, 243, 244, 244, 244, 244, 244, 244, 244, 244, 245, 242, 240, 240, 236, 235, 232, 230, 229, 228, 225, 224, 222, 220, 218, 216, 216, 213, 212, 211, 209, 207, 206, 204, 202, 200, 198, 198, 196, 194, 193, 192, 190, 189, 186, 186, 184, 182, 180, 179, 178, 177, 176, 174, 173, 171, 170, 169, 168, 166, 164, 163, 162, 161, 160, 158, 157, 156, 155, 153, 152, 151, 150, 148, 148, 146, 146, 144, 144, 142, 141, 140, 139, 138, 137, 136, 135, 134, 133, 132, 131, 130, 129, 128, 128, 125, 124, 123, 122, 122, 120, 120, 119, 118, 116, 115, 115, 114, 113, 112, 111, 110, 110, 109, 108, 107, 107, 105, 105, 104, 103, 102, 102, 101, 100, 100, 98, 97, 98, 96, 95, 95, 94, 92, 92, 91, 92, 90, 90, 89, 88, 88, 87, 86, 85, 85, 84, 84, 84, 82, 82, 81, 80, 80, 80, 78, 78, 77, 77, 77, 76, 75, 75, 74, 73, 73, 72, 72, 72, 71, 70, 69, 69, 69, 68, 67, 67, 66, 66, 65, 65, 65, 64, 64, 63, 62, 62, 61, 61, 61, 60, 60, 59, 60, 58, 58, 57, 57, 57, 56, 56, 55, 55, 54, 54, 54, 54, 53, 52, 52, 52, 51, 51, 50, 50, 49, 49, 49, 49, 48, 48, 48, 47, 47, 47, 46, 45, 45, 45, 44, 45, 44, 44, 43, 43, 42, 42, 42, 41, 41, 41, 41, 42, 40, 40, 39, 39, 39, 39, 39, 38, 38, 37, 37, 37, 37, 36, 36, 38, 36, 35, 35, 34, 34, 34, 35, 34, 33, 33, 32, 33, 32, 32, 32, 31, 31, 32, 32, 31, 31, 31, 30, 30, 30, 30, 29, 29, 29, 29, 28, 28, 28, 27, 27, 28, 27, 27, 26, 26, 26, 27, 26, 26, 26, 25, 25, 25, 24, 25, 24, 25, 23, 23, 24, 23, 23, 23, 23, 22, 23, 22, 22, 22, 22, 22, 21, 21, 22, 21, 21, 20, 20, 20, 20, 20, 20, 20, 19, 19, 19, 19, 19, 18, 19, 19, 19, 18, 18, 18, 18, 18, 18, 18, 17, 18, 17, 17, 17, 16, 16, 16, 17, 16, 16, 15, 16, 15, 16, 15, 15, 15, 16, 15, 15, 15, 15, 15, 14, 14, 14, 14, 14, 14, 13, 14, 14, 13, 13, 13, 14, 13, 14, 14, 13, 13, 13, 12, 13, 12, 12, 12, 12, 12, 11, 12, 12, 11, 12, 12, 11, 11, 11, 12, 12, 11, 10, 10, 11, 11, 10, 10, 11, 10, 11, 10, 10, 10, 10, 10, 10, 9, 9, 10, 9, 10, 9, 9, 9, 10, 9, 9, 9, 9, 8, 8, 9, 9, 9, 9, 8, 8, 9, 9, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 7, 8, 8, 8, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 6, 7, 7, 6, 6, 7, 6, 6, 7, 7, 7, 6, 6, 7, 6, 6, 7, 6, 6, 6, 6, 6, 5]
my_n = len(mydata)
my_t = 12.268362939475795

coef = 0.012890625
mydata = [(coef * x) for x in mydata]

volt = np.array(mydata)

charge = 457/my_t
finish = my_n/my_t


time = np.linspace (0, finish, len(volt))

x_axes = np.linspace (0, finish, 10)
y_axes = np.linspace(0, volt.max(), 10)

plt.ylim([0, volt.max() * 1.1])
plt.xlim([0, finish])

plt.grid()



plt.plot(time, volt, c='blue', linewidth=1, label = 'V(t)')
plt.axvline(charge, linestyle=':', c='black', linewidth=1, label = "start discharging")
    
plt.text(charge + 5, volt.max() - 0.5, "charging time: {:.5f} с".format(charge))
plt.text(charge + 5, volt.max() - 0.6, "discharge time:  {:.5f} с".format(finish - charge))
plt.xlabel(r'$Time$, $c$',    wrap=True)
plt.ylabel(r'Voltage, $V$', wrap=True)
plt.scatter (charge, volt.max(), c="red")

plt.legend()
plt.title('Charging and discharging process in RC circuit', wrap=True)

plt.show()
