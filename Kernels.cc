#include <iostream>
#include <cstdlib>

using namespace std;

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
      data[k]  = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/10.0));
  }

  void print(){
    for(int i = 0; i < rows; i++){
      cout << "[ ";
      for(int j = 0; j < cols; j++){
        cout << data[i*cols + j] <<" ";

      }
      cout << "] " << "\n";
    }

  }

};



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


int main(int argc, char** argv){


  Tensor_t Input(8,8);
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



  return 0;
}
