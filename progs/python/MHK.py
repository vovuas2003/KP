import matplotlib.pyplot as plt
#https://pythonbaba.com/online-python-code-editor-and-ide-for-data-science/

x = [1, 1.8, 3.3]
y = [5.1, 10.8, 14.7]

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
linex=[x[0], x[-1]]
liney=[a+b*x for x in linex]
plt.scatter(x,y)
plt.plot(linex, liney)
plt.show()
