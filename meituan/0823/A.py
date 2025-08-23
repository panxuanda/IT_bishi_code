'''小美有 n 个长方形，第之个长方形的两条边长分别为 xi,yi;小美拥有一个仅包含第一象限的平面直角坐标系

小美希望将这 n, 个长方形按顺序(可以旋转)放置在 x轴上，不允许重叠，并且每个长方形放置后的高度不超过m，保证max(min(xi,yi))≤m;

请问，在满足上述条件的前提下，小美最少需要占用 x轴的长度是多少?

输入描述第一行输入两个整数n,m(1≤n≤2*10^5;1≦m≤10^5)分别表示长方形的个数和允许的最大高度;接下来n 行，每行输入两个整数 i,yi(1 ≤ xi,yi≤ 10^9),分别表示第;个长方形的两条边长。

输出描述

输出一个整数，表示在每个长方形高度不超过 m 的情况下，所需占用的最短 x 轴长度。

示例 1

3 4

1 2

2 5

4 2

输出8'''

import sys

def solve():
    """
    """
    try:
        n, m = map(int, sys.stdin.readline().split())

        total_length = 0

        for _ in range(n):
            x, y = map(int, sys.stdin.readline().split())

            shorter_side = min(x, y)
            longer_side = max(x, y)
            if longer_side > m:
                total_length += longer_side
            else:
                total_length += shorter_side
        
        print(total_length)

    except (IOError, ValueError) as e:
        pass

solve()
