# Program Parallelization
In this assignment, we ask you to parallelize some matrix kernels that we've
defined in [Kernels.cc](./Kernels.cc). There are five kernels:

1. A [matrix transpose](https://en.wikipedia.org/wiki/Transpose)
2. A [convolution](https://en.wikipedia.org/wiki/Kernel_(image_processing))
3. A [matrix multiplication](https://en.wikipedia.org/wiki/Matrix_multiplication)
4. A [matrix inverse](https://en.wikipedia.org/wiki/Invertible_matrix) [modulo m](https://en.wikipedia.org/wiki/Modular_arithmetic)
5. A product of a list of matrices

For this assignment, we are asking you to parallelize each of these benchmarks
(if possible) using OpenMP.
A relatively simple introduction to OpenMP can be found [here](https://medium.com/swlh/introduction-to-the-openmp-with-c-and-some-integrals-approximation-a7f03e9ebb65)
and there are many other tutorials and sources of documentation available
online.
For these kernels you can primarily focus on parallel for loops in OpenMP,
though you may find tasks useful in certain places.

For each of these kernels, add the appropriate `#pragma`s to the code to
parallelize the kernel as much as possible.
Feel free to make changes to the code (such as reorganizing code, loops, etc.)
if you find that these changes can allow you to expose additional parallelism.
For each kernel, document (using comments) your decisions and the changes you
make to the code; explain why you made the changes you did, and especially when
you add parallelism explain why it is safe to do so.
Also, try to explain why some parts of the program can't be parallelized.

Note that not all kernels have the same types of parallelism, and we anticipate
the difficulty of parallelizing the kernels will vary.
Additionally, not all kernels may be amenable to parallelization, in these
cases try to explain why.

Ultimately, while we want to see your ability to parallelize these kernels, we
are most interested in seeing you explain your thought process and how you
approached these problems.
Demonstrating effort and thought is as important as performing the
parallelization.
Note that we will not be evaluating you based on the performance of your code,
we are just interested in you identifying the parallelism in these kernels, so
do not worry about optimizing the kernels or your parallelism.
The code you submit should compile and run and produce the same results as the
sequential versions we provide, and your versions should not introduce any
memory errors or leaks (note that OpenMP does not always properly exit its
threads which can lead to valgrind reporting memory leaks, you do not have to
worry about these leaks).
OpenMP is available in modern versions of both GCC and Clang for you to test
your code, please document at the top of your submission the compiler you used
and its version.

Again, make sure to document your thought process; explain how and why a kernel
can be parallelized, even if you're not able to implement the parallelism we
want to see evidence that you've thought about it.

**This assignment is due by Midnight (in Urbana, IL) on Friday, March 11,
2022.**

Directions for submitting this assignment, and for who to contact if you have
difficulties, were provided in the email.

If you complete the above portions of this assignment and have more time and
want to do more, consider attempting the following.

**Challenge 1:** For the 5th kernel (product of a list of matrices) we note in
the source code that you may assume all matrices are square n x n matrices. If
your parallelization used this assumption, now implement a version which does
not require this assumption.

**Challenge 2:** There is some opportunity for parallelism outside of the
kernels (specifically either within `main` or our definition of the `Matrix`
class). Parallelize the rest of the program as much as possible; if you find
it useful you may perform arbitrary restructructing of the code we provide, as
long as the Kernels remain their own functions.
