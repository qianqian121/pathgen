from math import sqrt
import matplotlib.pyplot as plt

points = [(0,0), (9,0), (9,6), (0,6), (0,0)]
# points = [(0,0), (9,0), (9,6), (0,6), (0,0), (5,0)]

def line_zigzag(line, zigzag, step):
    res = []
    p1 = line[0]
    p2 = line[1]
    vx = p2[0] - p1[0]
    vy = p2[1] - p1[1]
    dist = sqrt(vx * vx + vy * vy)
    # one zig step is 4 points: start, left zig, right zag, end
    n = int(dist / step)
    scale = dist / n
    scale_x = vx / n
    scale_y = vy / n

    x_sign = 1 if vx > 0 else -1
    y_sign = 1 if vy > 0 else -1
    if zigzag == 'zag':
        x_sign *= -1
        y_sign *= -1

    eps = 0.0001
    if abs(vx) > eps:
        k = vy / vx
        xoff = x_sign * sqrt(scale ** 2 / (1 + k ** 2)) / 4
        yoff = y_sign * xoff * k
    else:
        ik = vx / vy
        yoff = y_sign * sqrt(scale ** 2 / (1 + ik ** 2)) / 4
        xoff = x_sign * yoff * ik

    for i in range(n):
        mx = p1[0] + i * scale_x
        my = p1[1] + i * scale_y
        res.append((mx, my))
        res.append((mx - xoff, my - yoff))

        mx += scale_x / 2
        my += scale_y / 2
        res.append((mx, my))
        res.append((mx + xoff, my + yoff))

    res.append(p2)
    return res

def path_zigzag(points, zigzag='zig', step=1):
    res = []
    prev = points[0]
    for i in range(1, len(points)):
        p = points[i]
        res.extend(line_zigzag((prev, p), zigzag, step))
        prev = p
    return res

def pathgen(points, step=1):
    res = []
    #res.extend(points)
    res.extend(path_zigzag(points))
    #res.extend(path_zigzag(points, 'zag'))
    return res

path = pathgen(points)
x, y = zip(*path)
plt.scatter(x, y)
plt.show()