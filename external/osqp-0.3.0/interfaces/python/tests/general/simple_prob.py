import osqp
import osqppurepy
import scipy.sparse as sparse
import scipy as sp
import numpy as np
import mathprogbasepy as mpbpy
sp.random.seed(2)

n = 500
m = 1000
A = sparse.random(m, n, density=0.5,
                  data_rvs=np.random.randn,
                  format='csc')
l = -1. - np.random.rand(m)
u = 1 + np.random.rand(m)

#  l *= 0.01
#  u *= 0.01

# A = sparse.eye(n).tocsc()
# l = -1 * np.ones(n)
# u = 1 * np.ones(n)

# l += 10
# u += 10

# l *= 1000
# u *= 1000
# A *= 1000

# Make problem infeasible
# A_temp = A[5, :]
# A[6, :] = A_temp
# l[6] = l[5] + 2.
# u[6] = l[6] + 3.

P = sparse.random(n, n, density=0.5,
                  data_rvs=np.random.randn,
                  format='csc')
P = P.dot(P.T)
q = sp.randn(n)
#  q *= 1000

# Test
rho = 0.1

osqp_opts = {'rho': rho,
             'adaptive_rho': True,
             'adaptive_rho_interval': 25,
             'sigma': 1e-06,
             'scaled_termination': False,
             'check_termination': 1,
             'polish': False,
             'verbose': True,
             'linsys_solver': 'suitesparse ldl'
             }

qp = mpbpy.QuadprogProblem(P, q, A, l, u)
res_gurobi = qp.solve(solver=mpbpy.GUROBI, verbose=True)

model = osqppurepy.OSQP()
model.setup(P=P, q=q, A=A, l=l, u=u, **osqp_opts)
res_osqppurepy = model.solve()

# Solve with SuiteSparse LDL
model = osqp.OSQP()
model.setup(P=P, q=q, A=A, l=l, u=u, **osqp_opts)
res_osqp = model.solve()


# Check difference with gurobi
if res_gurobi.status == 'optimal':
    print("Difference OSQP vs Gurobi")
    print("  - primal = %.4f" %
          (np.linalg.norm(res_gurobi.x - res_osqp.x) /
           np.linalg.norm(res_gurobi.x)))
    print("  - dual = %.4f" %
          (np.linalg.norm(res_gurobi.y - res_osqp.y) /
           np.linalg.norm(res_gurobi.y)))
