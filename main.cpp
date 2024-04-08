#include "Matrix.hpp"

int main(){
   
   // ввод и вывод матриц с консоли
   
//    unsigned int number_of_rows, number_of_columns;
//    cout << "Number of rows (matrix1): "; cin >> number_of_rows; cout << '\n';
//    cout << "Number of columns (matrix1): "; cin >> number_of_columns; cout << '\n';
//    Matrix<double> matrix1(number_of_rows, number_of_columns);

//    cout << "Number of rows (matrix2): "; cin >> number_of_rows; cout << '\n';
//    cout << "Number of columns (matrix2): "; cin >> number_of_columns; cout << '\n';
//    Matrix<double> matrix2(number_of_rows, number_of_columns);
//    //cout << matrix1;
//    matrix1.PrintMatrix(); matrix2.PrintMatrix();
   
//    Matrix<double> matrix3 = matrix1 + matrix2;
   //cout << matrix3.matrix[1][1] << '\n';
   //cout << "Сумма: " << matrix3; //matrix3.PrintMatrix();
   //cout << "Произведение: " << matrix1 * matrix2 << '\n';
   
  // ввод и вывод матрицы в файл
  
   Matrix<double> matrix1("input.txt");                           //файл откуда считывается матрица, пример ввода: 3 3 2 5 7 6 3 4 5 -2 -3
   matrix1.PrintMatrix();
   Matrix<double> matrix2 = matrix1.Inverse(); 
   matrix2.PrintMatrix();                                          //вывод матрицы с фиксированной длинной ячейки 
   //cout << matrix1.Determinant() << '\n';
   //cout << ( (matrix1 * !matrix1 == matrix1 * matrix2) ?  "equation is true" : "equation is false" ) << '\n';
   Matrix<double> result =  matrix1 * !matrix1 * matrix1.Determinant();   //любое выражение с матрицами
   result.Round();
   result.PrintMatrix();
   ofstream file("output.txt");                                   //файл куда записывается матрица
   //file << result;
   file.close();
   
   return 0;
} 