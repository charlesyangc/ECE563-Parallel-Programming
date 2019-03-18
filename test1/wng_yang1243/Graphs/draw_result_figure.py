import matplotlib.pyplot as plt
import numpy as np

# For task 1
# For a 1200 * 1200 matrix for task 1, it takes 10.963044 to finish sequentially.
T1_task1 = 10.963044
# using four procs in one node, it take 2.012816 elapsed time
Tp_task1 = 2.012816

# For task 1
# For a 1600 * 1600 matrix 
# running sequentially.
T1_task1 = 44.198925
# using 2 procs
T2_task1 = 22.402673
# using 4 procs
T4_task1 = 10.382256
# using 8 procs
T8_task1 = 4.165496
# using 16 procs
T16_task1 = 2.567748

num_procs_task1  = np.array([1, 2, 4, 8, 16])
time_task1       = np.array([T1_task1, T2_task1, T4_task1, T8_task1, T16_task1])
speedup_task1    = T1_task1 / time_task1
efficiency_task1 = speedup_task1 / num_procs_task1
KarpFlatte_task1 = (num_procs_task1[1:5] / speedup_task1[1:5] - 1) / (num_procs_task1[1:5] - 1)



# For task 2
# For a 1600 * 1600 matrix
# running sequentially.
T1_task2 = 44.903316
# using 4 procs
T4_task2 = 8.395329
# using 16 procs
T16_task2 = 2.337392


num_procs_task2  = np.array([1, 4, 16])
time_task2       = np.array([T1_task2, T4_task2, T16_task2])
speedup_task2    = T1_task2 / time_task2
efficiency_task2 = speedup_task2 / num_procs_task2
KarpFlatte_task2 = (num_procs_task2[1:3] / speedup_task2[1:3] - 1) / (num_procs_task2[1:3] - 1)



# For task 3
# For a 1600 * 1600 matrix, it takes 1.340378 secs to finish on one core.
# running sequentially:
T1_task3 = 1.340378
# using 2 procs
T2_task3 = 1.350893
# using 4 procs
T4_task3 = 1.351812
# using 8 procs
T8_task3 = 1.357724
# using 16 procs
T16_task3 = 1.430724

num_procs_task3  = np.array([1, 2, 4, 8, 16])
time_task3       = np.array([T1_task3, T2_task3, T4_task3, T8_task3, T16_task3])



# plot time 
plt.figure()
plt.scatter(num_procs_task1, time_task1)
plt.plot(num_procs_task1, time_task1, label='task1_time')
plt.scatter(num_procs_task2, time_task2)
plt.plot(num_procs_task2, time_task2, label='task2_time')
plt.scatter(num_procs_task3, time_task3)
plt.plot(num_procs_task3, time_task3, label='task3_time')
plt.legend()
plt.title('compare of time of matrix matrix multiplication')
plt.show()



# plot speed up
plt.figure()
plt.scatter(num_procs_task1, speedup_task1 )
plt.plot(num_procs_task1, speedup_task1 , label='speedup_task1 ')
plt.scatter(num_procs_task2, speedup_task2 )
plt.plot(num_procs_task2, speedup_task2 , label='speedup_task2 ')
plt.legend()
plt.title('compare of speed up of matrix matrix multiplication')
plt.show()


# plot efficiency
plt.figure()
plt.scatter(num_procs_task1, efficiency_task1)
plt.plot(num_procs_task1, efficiency_task1, label='efficiency_task1')
plt.scatter(num_procs_task2, efficiency_task2)
plt.plot(num_procs_task2, efficiency_task2, label='efficiency_task2')
plt.legend()
plt.title('compare of efficiency of matrix matrix multiplication')
plt.show()


# plot Karp Flatt e
plt.figure()
plt.scatter(num_procs_task1[1:5], KarpFlatte_task1)
plt.plot(num_procs_task1[1:5], KarpFlatte_task1, label='KarpFlatte_task1')
plt.scatter(num_procs_task2[1:3], KarpFlatte_task2)
plt.plot(num_procs_task2[1:3], KarpFlatte_task2, label='KarpFlatte_task2')
plt.legend()
plt.title('compare of Karp Flatt e of matrix matrix multiplication')
plt.show()
