#include <cassert>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

// Definition of a matrix of integers of a given size
class Matrix {
private:
  unsigned rows_;
  unsigned cols_;
  shared_ptr<int[]> data;

  // Class used to enable accessing elements from a Matrix using a M[r][c]
  // form, only defines the [] operator. Is constructed from the beginning of
  // the row in the matrix, and the number of columns (for dynamic bounds
  // checking)
  class Row {
    private:
      int* array;
      const unsigned cols;
    public:
      Row(int* data, unsigned cols_) : array(data), cols(cols_) {}
      int& operator[](int col) const {
        assert(0 <= col && col < cols && "Matrix access out of range");
        return array[col];
      }
  };
public:
  Matrix(unsigned rs, unsigned cs) : rows_(rs), cols_(cs),
                                     data(new int[rs * cs])
  {
    for (int i = 0; i < rows_ * cols_; i++) data[i] = 0;
  }

  // Accessors
  unsigned rows() const { return rows_; }
  unsigned cols() const { return cols_; }

  // Fills a matrix by calling a given generator function at each location in
  // the matrix
  void fill(std::function<int(int,int)> generator) {
    for (int r = 0; r < rows_; r++) {
      for (int c = 0; c < cols_; c++) {
        (*this)[r][c] = generator(r, c);
      }
    }
  }

  friend ostream& operator<<(ostream& os, const Matrix& m) {
    os << "Rows " << m.rows_ << ", Cols " << m.cols_ << "\n";
    for(int r = 0; r < m.rows_; r++){
      os << "[ ";
      for(int c = 0; c < m.cols_; c++){
        os << m[r][c] <<" ";
      }
      os << "] " << "\n";
    }
    return os;
  }

  // Operator overloading to access a row of the matrix
  Row operator[](int row) const {
    assert(0 <= row && row < rows_ && "Matrix access out of range");
    return Row(&(data[row * cols_]), cols_);
  }

  bool operator==(Matrix& other) {
    if (rows_ != other.rows_ || cols_ != other.cols_) return false;

    for (int r = 0; r < rows_; r++) {
      for (int c = 0; c < cols_; c++) {
        if ((*this)[r][c] != other[r][c]) return false;
      }
    }

    return true;
  }
};

int min(int a, int b){
  return (a < b) ? a : b;
}

// ============================================================================
// Kernel 1: Matrix transpose.
// ============================================================================
Matrix Transpose(Matrix& A) {
  const unsigned aRows = A.rows();
  const unsigned aCols = A.cols();

  Matrix output(aCols, aRows);

  for (int i = 0; i < aRows; i++) {
    for (int j = 0; j < aCols; j++) {
      output[j][i] = A[i][j];
    }
  }

  return output;
}

// ============================================================================
// Kernel 2: Matrix convolution.
// ============================================================================
Matrix Convolution(Matrix& input, Matrix& kernel) {
  const unsigned inRows = input.rows();
  const unsigned inCols = input.cols();

  const unsigned kRows = kernel.rows();
  const unsigned kCols = kernel.cols();

  const unsigned outRows = inRows - kRows + 1;
  const unsigned outCols = inCols - kCols + 1;

  Matrix output(outRows, outCols);

  for (int i = 0; i < outRows; i++) {
    for (int j = 0; j < outCols; j++) {
      for (int ki = i; ki < i + kRows; ki++) {
        for (int kj = j; kj < j + kCols; kj++) {
          int inVal = input[ki][kj];
          int kVal  = kernel[ki-i][kj-j];
          output[i][j] += inVal * kVal;
        }
      }
    }
  }

  return output;
}

// ============================================================================
// Kernel 3: Matrix multiplication.
// ============================================================================
Matrix MatrixMultiply(Matrix& A, Matrix& B) {
  const unsigned aRows = A.rows();
  const unsigned aCols = A.cols();

  const unsigned bRows = B.rows();
  const unsigned bCols = B.cols();

  assert(aCols == bRows && "Incompatible matrix multiplication dimensions");

  Matrix output(aRows, bCols);

  for (int i = 0; i < aRows; i++) {
    for (int j = 0; j < aCols; j++) {
      for (int k = 0; k < bCols; k++) {
        output[i][k] += A[i][j] * B[j][k];
      }
    }
  }

  return output;
}

// Helper function for Kernel 4: Computes the multiplicative inverse of a
// modulo m
int inverse(int a, int m);

// ============================================================================
// Kernel 4: Matrix inverse (modulo m)
// ============================================================================
Matrix Invert(Matrix& A, int m) {
  const unsigned aRows = A.rows();
  const unsigned aCols = A.cols();

  assert(aRows == aCols && "Cannot invert non-square matrix");
  assert(m > 0 && "Invalid modulo");

  const unsigned n = aRows;

  Matrix Aug(n, n * 2);
  Aug.fill([&A, n](int r, int c) {
              if (c < n) return A[r][c];
              else return (c-n) == r ? 1 : 0; });

  for (int r = 0; r < n; r++) {
    int factor = inverse(Aug[r][r], m);
    if (factor == 0) throw "Non-invertible matrix";

    for (int c = r; c < 2 * n; c++) Aug[r][c] = (Aug[r][c] * factor) % m;
    for (int r2 = 0; r2 < n; r2++) {
      if (r2 != r) {
        if (Aug[r2][r] != 0) {
          int factor = Aug[r2][r];
          for (int c = r; c < 2 * n; c++)
            Aug[r2][c] = (Aug[r2][c] - factor * Aug[r][c]) % m;
        }
      }
    }
  }

  Matrix Inverse(n, n);
  Inverse.fill([&Aug, n](int r, int c) { return Aug[r][n + c]; });
  return Inverse;
}

// ============================================================================
// Kernel 5: Product of a list of matrices
//  Assumption: for parallelizing this kernel you may assume that all matrices
//    are square n x n matrices. For Challenge 1 we ask you to define a
//    parallel version without this assumption
// ============================================================================
Matrix matrixProduct(vector<Matrix> matrices) {
  if (matrices.size() == 0) throw "Product of 0 matrices is undefined";

  // Reminder: matrix multiplication is associative and the identity matrix
  // is the multiplicative identity (i.e. for any matrix A, I A = A)
  Matrix prod = matrices[0];
  for (int i = 1; i < matrices.size(); i++) {
    prod = MatrixMultiply(prod, matrices[i]);
  }

  return prod;
}

// ============================================================================
// Main driver, contains examples using the above kernels
// Feel free to modify this to more extensively test your parallelized kernels
// ============================================================================
int main(int argc, char** argv) {
  Matrix input(4, 4);
  Matrix kernel(3, 3);

  input.fill([](int, int) {
              static unsigned int seed = 0;
              return rand_r(&seed) / (RAND_MAX / 5.0f);
            });
  kernel.fill([](int, int) {
              static unsigned int seed = 314159;
              return rand_r(&seed) / (RAND_MAX / 5.0f);
            });

  auto output = Convolution(input, kernel);

  cout << "====================\n";
  cout << "Input: "  << input;
  cout << "Kernel: " << kernel;
  cout << "Output: " << output;

  Matrix A(4, 2);
  Matrix B(2, 6);

  A.fill([](int r, int c) { return r + c; });
  B.fill([](int, int) { static int i = 0; return i++; });

  auto prod  = MatrixMultiply(A, B);

  cout << "====================\n";
  cout << "A: "    << A;
  cout << "B: "    << B;
  cout << "A x B:" << prod;

  auto AT = Transpose(A);
  auto BT = Transpose(B);
  auto prodT = Transpose(prod);
  auto prodOT = MatrixMultiply(BT, AT);

  cout << "====================\n";
  cout << "A^T: " << AT;
  cout << "B^T: " << BT;
  cout << "(A x B)^T: " << prodT;
  cout << "B^T x A^T: " << prodOT;

  cout << "A == B? " << (A == B ? "true" : "false") << "\n";
  cout << "A == A^T? " << (A == AT ? "true" : "false") << "\n";
  cout << "(A x B)^T == B^T x A^T? " << (prodT == prodOT ? "true" : "false") << "\n";

  Matrix M(2, 2);
  M.fill([](int r, int c) { return c + 2 * r + 1; });
  auto Inv = Invert(M, 7);
  auto Id  = MatrixMultiply(M, Inv); // This is an identity matrix (mod 7)

  cout << "====================\n";
  cout << "M: "      << M;
  cout << "M^-1: "   << Inv;
  cout << "M M^-1: " << Id;

  Matrix M1(3, 3); M1.fill([](int r, int c) { return r + c; });
  Matrix M2(3, 3); M2.fill([](int r, int c) { return r * c; });
  Matrix M3(3, 3); M3.fill([](int r, int c) { return r % (c + 1); });
  Matrix M4(3, 3); M4.fill([](int r, int c) { return c % (r + 1); });

  std::vector<Matrix> matrices = {M1, M2, M3, M4};

  Matrix res = matrixProduct(matrices);
  cout << "====================\n";
  cout << "Product: " << res;
  cout << "====================\n";


  return 0;
}

// ============================================================================
// Helper function for Kernel 4
// Computes the multiplicative inverse of a modulo m
// Uses the Extended Euclidean Algorithm
// ============================================================================
int inverse(int a, int m) {
  assert(m > 0 && "Invalid modulo");

  a %= m;
  if (a < 0) a += m;

  int b = m;

  int old_r = a, r = b;
  int old_s = 1, s = 0;

  while (r != 0) {
    int q = old_r / r;
    int tmp;

    tmp = old_r - q * r;
    old_r = r; r = tmp;

    tmp = old_s - q * s;
    old_s = s; s = tmp;
  }

  return old_r == 1 ? old_s : 0;
}
