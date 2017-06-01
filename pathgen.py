from math import sqrt
import matplotlib.pyplot as plt

# path inside garage
points = [(0.0,0.0), (9.0,0.0), (9.0,6.0), (0.0,6.0), (0.0,0.0)]
loop_close = [(0.0,0.0), (5.0,0.0)]
# points = [(0,0), (9,0), (9,6), (0,6), (0,0), (5,0)]

# path for Gazebo simulation
points = [(0.0,0.0), (7.9248,0.0), (7.9248,-4.572), (0.0,-4.572), (0.0,0.0)]
loop_close = [(0.0,0.0), (4.5,0.0)]

# path for Honeywell parking lot first try. DO NOT USE
points = [
        (0.0, 0.0),
        (21.3151, 0.0),
        (22.9451, 45.8042),
        (14.0973, 48.659),
        (-2.34517, 9.75557),
        (0.0, 0.0)
        ]

# path for Honeywell parking lot demo
points = [
    (0.000000, 0.000000),
    (23.168110, 0.000000),
    (29.654981, 45.371788),
    (21.160500, 49.150449),
    (0.678128, 12.213946),
    (0.000000, 0.000000)
]

# path for Honeywell parking lot big car demo
points = [
    (0.000000, 0.000000),
    (26.968110, 0.000000),
    (29.654981, 49.371788),
    (19.660500, 52.150449),
    (1.678128, 12.213946),
    # (0.000000, 0.000000)
]

loop_close = [(0.0,0.0), (9.0,0.0)]

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

    x_sign = 1 if vy >= 0 else -1
    y_sign = 1 if vx >= 0 else -1
    if zigzag == 'zag':
        x_sign *= -1
        y_sign *= -1

    eps = 0.0001
    if abs(vx) > eps:
        k = vy / vx
        yoff = y_sign * sqrt(scale ** 2 / (1 + k ** 2)) / 4
        xoff = x_sign * yoff * k
    else:
        ik = vx / vy
        xoff = x_sign * sqrt(scale ** 2 / (1 + ik ** 2)) / 4
        yoff = y_sign * xoff * ik

    for i in range(n):
        mx = p1[0] + i * scale_x
        my = p1[1] + i * scale_y
        res.append((mx, my))
        mx += scale_x / 4
        my += scale_y / 4
        res.append((mx - xoff, my + yoff))

        mx += scale_x / 4
        my += scale_y / 4
        res.append((mx, my))
        mx += scale_x / 4
        my += scale_y / 4
        res.append((mx + xoff, my - yoff))

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
    res.extend(points)
    # res.extend(path_zigzag(points, step=step))
    # res.extend(path_zigzag(points, 'zag', step=step))
    # res.extend(loop_close)
    return res

path = pathgen(points, 7)
x, y = zip(*path)
plt.scatter(x, y)
plt.show()

# import csv
#
# with open('tmp.csv', 'w') as tmpfile:
#     # bug DO NOT USE
#     wr = csv.writer(tmpfile, quoting=csv.QUOTE_ALL)
#     for p in path:
#         wr.writerow(str(p))
#     tmpfile.close()

import numpy as np
np.savetxt('tmp.csv', path, delimiter=",", fmt='%s')

from subprocess import call

call('./pathgen tmp.csv > driving_lane.csv.honeywell.bigcar', shell=True)
