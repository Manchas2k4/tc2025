# PARALLELISM AND PERFORMANCE
## SPEEDUP
The speed of a program is the time it takes the program to execute. This could be measured in any increment of time. Speedup is defined as the time it takes a program to execute sequentially (with one processor) divided by the time it takes to execute in parallel (with many processors). The formula for speedup is:

Sp = T1 / Tp

Where:

Sp: The speedup obtained from using p processors.

T1: The time it takes the program to be executed sequentially.

Tp: The time it takes the program to be executed in parallel using p processors.

## LINEAR SPEEDUP
**Linear speedup** or **ideal speedup** is obtained when Sp = _p_. When running an algorithm with linear speedup, doubling the number of processors doubles the speed. As this is ideal, it is considered very good scalability.

## AMDAHL’S LAW
**Amdahl's law** states that the speedup of a program using multiple processors in parallel computing is limited by the time needed for the sequential fraction of the program.

As a formula:

Sp = 1 / (F + (1 - F) / _p_)

Where:

_p_: The number of processors.

Sp: The speedup obtained from using p processors.

F: The fraction of the program that must be executed sequentially. 0 ≤ F ≤ 1.

If _p_ tends to ∞, the maximum speedup tends to 1 / F.

**Reference:** [AKHTER] pp. 13-15.
