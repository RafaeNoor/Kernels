#include <iostream>
#include <cstdlib>

using namespace std;

#define TILE_I 4
#define TILE_J 4
#define TILE_K 2

struct Tensor_t {
    float* data;
    unsigned rows;
    unsigned cols;

    Tensor_t(unsigned rows_, unsigned cols_ ) 
        : rows(rows_), cols(cols_){
            data = new float[rows_ * cols_];
            for(int k = 0; k < rows_ * cols_; k++) data[k] = 0.0;
        }

    void randomize(){
        for(int k = 0; k < rows * cols; k++) 
            data[k]  = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/5.0));
    }

    void print(){
        cout << "Rows:\t"<< rows <<"\n";
        cout << "Cols:\t"<< cols <<"\n";

        for(int i = 0; i < rows; i++){
            cout << "[ ";
            for(int j = 0; j < cols; j++){
                cout << data[i*cols + j] <<" ";

            }
            cout << "] " << "\n";
        }

    }


};

int min(int a , int b){
  return (a < b)? a : b;

}

Tensor_t Convolution(Tensor_t Input, Tensor_t Kernel){
    unsigned Output_Rows = Input.rows - Kernel.rows + 1;
    unsigned Output_Cols = Input.cols - Kernel.cols + 1;

    Tensor_t Output(Output_Rows, Output_Cols);

    for(int i = 0; i < Output_Rows; i++){
        for(int j = 0; j < Output_Cols; j++){
            for(int ki = i; ki < i + Kernel.rows; ki++){
                for(int kj = j; kj < j + Kernel.cols; kj++){
                    float Input_Value = Input.data[ki* Input.cols + kj]; 
                    float Kernel_Value = Kernel.data[(ki-i)*Kernel.cols + (kj-j)];
                    Output.data[i*Output_Cols + j] += Input_Value * Kernel_Value;

                }
            }
        }
    }


    return Output;

}

Tensor_t MatrixMultiply(Tensor_t A, Tensor_t B){

    Tensor_t Output(A.rows, B.cols);

    for(int i = 0; i < A.rows; i+= TILE_I){
        int imax = min(i+TILE_I,A.rows);
        for(int j =0; j < B.cols; j+= TILE_J){
            int jmax = min(j+TILE_J,B.cols);
            for(int k = 0; k < A.cols; k+=TILE_K){
                int kmax = min(k+TILE_K, A.cols);
                for(int ii = i; ii < imax ; ii++){
                    for(int jj =j; jj < jmax; jj++){
                        for(int kk = k; kk < kmax; kk++){
                            Output.data[ii*B.cols + jj] += A.data[ii*A.cols + kk] * B.data[kk * B.cols + jj];
                        }
                    }
                }
            }
        }
    }

    return Output;


}

Tensor_t Transpose(Tensor_t A){
    Tensor_t Output(A.cols, A.rows);

    for(int i = 0; i < A.rows; i++){
        for(int j = 0; j < A.cols; j++){
            Output.data[j* A.rows + i] = A.data[i*A.cols + j];
        }
    }

    return Output;
}

// Output[i,j] = A[i,j] + Output[i-1,j] + Output[i,j-1]
Tensor_t PrefixSum2D(Tensor_t A) {
    Tensor_t Sum(A.rows, A.cols);

    Sum.data[0] = A.data[0];
    // Sum first row
    for (int i = 1; i < A.cols; i++)
      Sum.data[i] = A.data[i] + Sum.data[i - 1];

    // Sum first column
    for (int i = 1; i < A.rows; i++)
      Sum.data[i*A.cols] = A.data[i*A.cols] + Sum.data[(i - 1)*A.cols];

    for(int i = 1; i < A.rows; i++){
        for(int j = 1; j < A.cols; j++){
            Sum.data[i*A.cols + j] = A.data[i*A.cols + j];
            Sum.data[i*A.cols + j] += Sum.data[i*A.cols + j - 1];
            Sum.data[i*A.cols + j] += Sum.data[(i - 1)*A.cols + j];
        }
    }

    return Sum;
}

int main(int argc, char** argv){


    Tensor_t Input(4,4);
    Tensor_t Kernel(3,3);

    Input.randomize();
    Kernel.randomize();

    Tensor_t Output = Convolution(Input, Kernel);

    cout << "Input: "<<"\n";
    Input.print();



    cout << "Kernel: "<<"\n";
    Kernel.print();



    cout << "Output: "<<"\n";
    Output.print();


    Tensor_t A(4,2);
    Tensor_t B(2,6);

    A.randomize();
    B.randomize();

    Tensor_t AxB = MatrixMultiply(A,B);

    A.print();
    B.print();

    AxB.print();

    Tensor_t AxBt = Transpose(AxB);
    AxBt.print();

    Tensor_t ASum = PrefixSum2D(A);
    ASum.print();

    return 0;
}
