# Blind math

- we need to be able to traverse equations
  - goto first term/last term?
  - next/previous term?
  - left/right
  - forward/backward
  - nesting levels?
  - vertical levels?
  - super/subscripts?
  - division lines
- we need language to describe braces and order-of operations
$$
\sin(A+B) \newline  % sine of A plus B 
\sin(A) + B \newline
(A+B)^c \newline
A + B^c \newline
e^{ix} \newline
e^ax \newline
A + \frac{B}{C} \newline
A + \frac{B}{\frac{C}{\frac{D}{E}}}
$$

There appear to be bugs in MathJAX rendering.
Is this true in LaTeX too?
$$
\sqrt(\log_b(a) + k\log_\phi(b)) \newline
\sqrt{\log_b(a) + k\log_\phi(b)} \newline
\Sigma_{i=a}^b i^2 \newline
\int_a^b ax^3+bx^2+c dx \newline

a_{b_c+d} \newline
a_{b_c}+d \newline
a_{b_{c+d}} \newline

% 3x4 matrix with parentheses
\begin{pmatrix}
a_{11} & a_{12} & a_{13} & a_{14} \\
a_{21} & a_{22} & a_{23} & a_{24} \\
a_{31} & a_{32} & a_{33} & a_{34}
\end{pmatrix}

% 2x3 matrix with square brackets
\begin{bmatrix}
b_{11} & b_{12} & b_{13} \\
b_{21} & b_{22} & b_{23}
\end{bmatrix}

\newline

$$



