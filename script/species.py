# test
import matplotlib.pyplot as plt

def u(n):
    if n == 0:
        return -3 # valeur de u(0)
    
    return u(n-1) + 2.5

x = [ n for n in range(10) ] 
y = [ u(n) for n in range(10) ]
# display "Hello world!"
print("Hello world!")

plt.scatter(x,y)

plt.title('Nuage de points : suite arithmétique')
plt.xlabel('x')
plt.ylabel('y')
plt.savefig('nuage-points-suites-arithmétique-python.jpg')

plt.show()