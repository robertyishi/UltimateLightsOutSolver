import numpy as np

def construct_matrix(m: int, n: int):
    mat = np.zeros((m * n, m * n), dtype = int)
    for k in range(m * n):
        i = k // n
        j = k % n
        mat[k][k] = 1
        # toggle left cell if possible
        if j > 0:
            mat[k-1][k] = 1
        # toggle right cell if possible
        if j < n - 1:
            mat[k+1][k] = 1
        # toggle cell above if possible
        if i > 0:
            mat[k-n][k] = 1
        # toggle cell below if possible
        if i < m - 1:
            mat[k+n][k] = 1
    return mat

mat = construct_matrix(1, 1)
print(mat)
print(np.linalg.det(mat))
# res = np.array([0,1,0,1,1,0,0,1,1])
# print(res)
# ans = np.linalg.inv(mat)@res.T
# print(ans)
