#https://pythonbaba.com/online-python-code-editor-and-ide-for-data-science/
import matplotlib.pyplot as plt
import math
flag = False
d = 0.03

teta = [i * 10 for i in range(12)]
x = [1 - math.cos(i * math.pi / 180) for i in teta]
N = [624, 627, 604, 492, 450, 388, 327, 290, 257, 230, 207, 186]
y = [1 / i for i in N]
dN = [7, 6, 7, 8, 10, 35, 20, 13, 11, 12, 9, 6]
dteta = [0.5 for i in range(len(teta))]

n=len(x)
if n!=len(y):
    print('!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n\n\n\n\n')
def sr(a):
    global n
    s=0
    for x in a:
        s=s+x
    return s/n
def srp(a,b):
    global n
    s=0
    for i in range(n):
        s=s+a[i]*b[i]
    return s/n
b=(srp(x,y)-sr(x)*sr(y))/(srp(x,x)-(sr(x))**2)
a=sr(y)-b*sr(x)
gb=((srp(y,y)-(sr(y))**2)/(srp(x,x)-(sr(x))**2)-b**2)**0.5
gb=gb/(n**0.5)
ga=gb*((srp(x,x)-(sr(x))**2)**0.5)
print('y = a + b * x')
print()
print('a = ', a, '\nабсолютная погрешность a = ', ga, '\nотносительная (в процентах) = ', (ga * 100)/a, sep = '')
print()
print('b = ', b, '\nабсолютная погрешность b = ', gb, '\nотносительная (в процентах) = ', (gb * 100)/b, sep = '')
print()
'''
linex=[x[0]-d, x[-1]+d]
liney=[a+b*x for x in linex]
plt.scatter(x,y)
plt.plot(linex, liney)
if flag:
    for i in range(n):
        plt.plot([x[i], x[i]], [1 / (N[i] + dN[i]), 1 / (N[i] - dN[i])], "r")
        plt.plot([1 - math.cos((teta[i] + dteta[i]) * math.pi / 180), 1 - math.cos((teta[i] - dteta[i]) * math.pi / 180)], [y[i], y[i]], "r")
plt.show()
'''
def NN(nu):
    global a
    global b
    global x
    return 1 / (a + b * x[nu])
print(NN(0), NN(9))
print()
print(662 * NN(9) / (NN(0) - NN(9)))
print()
E = 662
dn0 = ga / (y[0]**2)
dn90 = (ga + gb + b * math.pi * 0.5 / 180) / (y[9]**2)
#dn0 = dN[0]
#dn90 = dN[9]
print(E * (((NN(9) * dn0)**2 + (NN(0) * dn90)**2)**0.5) / ((NN(0) - NN(9))**2))
