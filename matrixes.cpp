#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

class Matrix
{
private:
   unsigned int number_of_rows, number_of_columns;
   double **matrix;

   void Delete(){
      for(unsigned int row = 0; row < number_of_rows; row++)
         delete[] matrix[row];
      delete[] matrix;
   // cout << "Matrix was deleted" << '\n'; //очистили выдленную память
   }
   void Create(){ //выделили память под матрицу number_of_rows х number_of_columns
      matrix = new double* [number_of_rows];
      for(unsigned int row = 0; row < number_of_rows; row++)
         matrix[row] = new double [number_of_columns];
      //cout << "Matrix was declared" << '\n';
   }
   void Assign(istream& from){ //присваиваем значение из потока
      for(unsigned int row = 0; row < number_of_rows; row++){
         for(unsigned int column = 0; column < number_of_columns; column++){
            // cout << "Element [" << row << "," << column << "]: ";
            from >> matrix[row][column];
         }   
      }
      //cout << "Matrix was assigned with values" << '\n';
   }
   void Assign(double value){ //присваиваем дефолтное значение 
      for(unsigned int row = 0; row < number_of_rows; row++){
         for(unsigned int column = 0; column < number_of_columns; column++){
            // cout << "Element [" << row << "," << column << "]: ";
            matrix[row][column] = value;
         }   
      }
      //cout << "Matrix was assigned with values" << '\n';
   }

public:
   // конструктор для задания матрицы из консоли
   Matrix(unsigned int _number_of_rows_, unsigned int _number_of_columns_){
      if(_number_of_rows_ and _number_of_columns_){
      number_of_rows = _number_of_rows_;
      number_of_columns = _number_of_columns_;
      Create();
      Assign(cin);
    }else
      cerr << "ERROR: MATRIX IS EMPTY" << '\n';
   }
   // конструктор для задания матрицы со значением по умолчанию
   Matrix(unsigned int _number_of_rows_, unsigned int _number_of_columns_, double default_value){
      number_of_rows = _number_of_rows_;
      number_of_columns = _number_of_columns_;
      Create();
      Assign(default_value);
   }
   // конструктор для задания матрицы из файла
   Matrix(const char *path_to_file){
      ifstream file(path_to_file);
      file >> number_of_rows >> number_of_columns;
      Create();
      Assign(file);
      file.close();
   }
   // деструктор
   ~Matrix(){
      Delete();
   }
   bool IsSquare() const{
      return this->number_of_rows == this->number_of_columns;
   }
   void PrintMatrix() const{
      for(unsigned int row = 0; row < number_of_rows; row++){
         for(unsigned int column = 0; column < number_of_columns; column++)
            cout << setw(7) << setprecision(5) << matrix[row][column] << ' ';
         cout << '\n';
      }cout << '\n';
   }
   void Round(unsigned int num = 7){ //округляем до n чисел после запятой
      for(unsigned int row = 0; row < number_of_rows; row++){
         for(unsigned int column = 0; column < number_of_columns; column++)
            matrix[row][column] = round(matrix[row][column]*(10^num))/(10^num);
      }
   }
   // элементарное преобразование 1
   void elemental_1(unsigned int row1, unsigned int row2){
      if(row1 > this->number_of_rows or row2 >this->number_of_rows)
         cerr << "ERROR: number of rows is out of range" << '\n';
      else{
         row1--; row2--;
         for(unsigned int column = 0; column < this->number_of_columns; column++)
            std::swap(this->matrix[row1][column], this->matrix[row2][column]);
      }
   }
   // элементарное преобразование 2
   void elemental_2(unsigned int row, double num){
      if(row > this->number_of_rows)
         cerr << "ERROR: number of rows is out of range" << '\n';
      else{
         row--;
         for(unsigned int column = 0; column < number_of_columns; column++)
            this->matrix[row][column] *= num;
      }
   }
   // элементарное преобразование 3
   void elemental_3(unsigned int row1, unsigned int row2, double num){
      if(row1 > this->number_of_rows or row2 >this->number_of_rows)
         cerr << "ERROR: number of rows is out of range" << '\n';
      else{
         row1--;row2--;
         for(unsigned int column = 0; column < this->number_of_columns; column++)
            this->matrix[row2][column] += this->matrix[row1][column] * num;
      }
   }
   // вычисляем детерминант
   double Determinant() const{
      if(!(this->IsSquare())){
         cerr << "ERROR: non-square matrix has no determinant" << '\n';
      }if(this->number_of_columns == 1)
         return this->matrix[0][0];
      double result = 0;
      for(unsigned int column = 0; column < this->number_of_columns; column++){
         result += ( (column % 2 == 0) ? 1 : -1) * this->matrix[0][column] * this->Minor(0, column).Determinant();
      }return result;
   }
   // вычисляем минор 
   Matrix Minor(unsigned int row_current, unsigned int column_current) const{
      Matrix result(this->number_of_rows - 1, this->number_of_columns - 1, 0);
      for(unsigned int row = 0; row < this->number_of_rows; row++){
            if(row == row_current) continue;
            for(unsigned int column = 0; column < this->number_of_columns; column++){
                if(column == column_current) continue;
                result.matrix[(row > row_current) ? (row - 1) : row][(column > column_current) ? (column - 1) : column] = this->matrix[row][column];
            }
      }return result;
   }
   // транспонируем
   Matrix Transponed() const{
      if(!(this->IsSquare()))
         cerr << "ERROR: Cannot transpone a non-square matrix" << '\n';
      else{
         Matrix result(this->number_of_rows, this->number_of_columns, 0);
         for(unsigned int row = 0; row < this->number_of_rows; row++){
            for(unsigned int column = 0; column < this->number_of_columns; column++)
               result.matrix[row][column] = this->matrix[column][row];
         }return result;
      }
   }
   Matrix Linear() const{
      Matrix result(this->number_of_rows, this->number_of_columns, 0);
      for(unsigned int row = 0; row < this->number_of_rows; row++){
         for(unsigned int column = 0; column < this->number_of_columns; column++)
            result.matrix[row][column] = ((row + column) % 2 == 0 ? 1 : -1) * this->Minor(row, column).Determinant();
      }return result;
   }
   // считаем обратную матрицу
   Matrix Inverse() const{
      if(!(this->IsSquare()))
         cerr << "ERROR: non-square matrix has no inverse matrix" << '\n';
      else{
         return ( this->Linear().Transponed() * (1 / this->Determinant()) );
      }
   }

   Matrix operator * (const Matrix&) const;
   Matrix operator * (double) const;

   Matrix operator + (const Matrix&) const;
   Matrix operator - (const Matrix&) const;

   bool operator == (const Matrix&) const;
   bool operator != (const Matrix&) const;
   bool operator == (double) const;
   bool operator != (double) const;

   friend ostream& operator<<(ostream &, const Matrix&);

   Matrix operator ! () const;
};
// матрица * матрица
Matrix Matrix::operator * (const Matrix& other) const
{
   if(this->number_of_columns != other.number_of_rows){ //A condition for Matrix to be compatible
      cerr << "ERROR: Matrix are not compatible (cannot multiply)" << '\n';
   }else{
      Matrix result(this->number_of_rows, other.number_of_columns, 0);
      for(unsigned int row = 0; row < result.number_of_rows; row++){
         for(unsigned int column = 0; column < result.number_of_columns; column++){
            double sum = 0;
            for(int count = 0; count < this->number_of_columns; count++)
               sum += this->matrix[row][count] * other.matrix[count][column];
            result.matrix[row][column] = sum;
         }
      }return result;
   }
}
// матрица * скаляр
Matrix Matrix::operator * (double number) const
{
   Matrix result(this->number_of_rows, this->number_of_columns, 0);
   for(unsigned int row = 0; row < result.number_of_rows; row++){
      for(unsigned int column = 0; column < result.number_of_columns; column++)
         result.matrix[row][column] = this->matrix[row][column] * number;
   }return result;
}
// матрица + матрица
Matrix Matrix::operator + (const Matrix& other) const
{
   if(this->number_of_rows != other.number_of_rows or this->number_of_columns != other.number_of_columns){
      cerr << "ERROR: Matrix are not compatible (cannot add)" << '\n';
   }else{
      Matrix result(this->number_of_rows, this->number_of_columns, 0);
      for(unsigned int row = 0; row < result.number_of_rows; row++){
         for(unsigned int column = 0; column < result.number_of_columns; column++)
         result.matrix[row][column] = this->matrix[row][column] + other.matrix[row][column];
      }return result;
   }
}
// матрица - матрица
Matrix Matrix::operator - (const Matrix& other) const
{
   if(this->number_of_rows != other.number_of_rows or this->number_of_columns != other.number_of_columns){
      cerr << "ERROR: Matrix are not compatible (cannot subtract)" << '\n';
   }else{
      Matrix result(this->number_of_rows, this->number_of_columns, 0);
      for(unsigned int row = 0; row < result.number_of_rows; row++){
         for(unsigned int column = 0; column < result.number_of_columns; column++)
            result.matrix[row][column] = this->matrix[row][column] - other.matrix[row][column];
      }return result;
   }
}
// матрица == матрица
bool Matrix::operator == (const Matrix& other) const
{
   if(this->number_of_rows == other.number_of_rows and this->number_of_columns == other.number_of_columns){
      for(unsigned int row = 0; row < this->number_of_rows; row++){
         for(unsigned int column = 0; column < this->number_of_columns; column++){
            if(this->matrix[row][column] != other.matrix[row][column])
               return false;
         }
      }return true;
   }return false;
}
// матрица != матрица
bool Matrix::operator != (const Matrix& other) const
{
   if(*this == other){
      return false;
   }return true;
}
// матрица == скаляр
bool Matrix::operator == (double number) const{
   if(!(this->IsSquare()))
      return false;
   else{
      for(unsigned int row = 0; row < this->number_of_rows; row++){
         for(unsigned int column = 0; column < this->number_of_columns; column++){
            if(row == column and this->matrix[row][column] != number)
               return false;
            else if(row != column and this->matrix[row][column] != 0)
               return false;
         }
      }return true;
   }
}
// матрица != скаляр
bool Matrix::operator != (double number) const{
   if(!(this->IsSquare()))
      return true;
   else{
      for(unsigned int row = 0; row < this->number_of_rows; row++){
         for(unsigned int column = 0; column < this->number_of_columns; column++){
            if(row == column and this->matrix[row][column] != number)
               return true;
            else if(row != column and this->matrix[row][column] != 0)
               return true;
         }
      }return false;
   }
}
// поток вывода << матрица
ostream& operator << (ostream &out, const Matrix& result){
  for(unsigned int row = 0; row < result.number_of_rows; row++){
         for(unsigned int column = 0; column < result.number_of_columns; column++)
            out << result.matrix[row][column] << ' ';
         out << '\n';
      }out << '\n';
   return out;
}
// !матрица
Matrix Matrix::operator ! () const{
   return this->Inverse();
}

int main(){
   
   // ввод и вывод матриц с консоли
   
   // unsigned int number_of_rows, number_of_columns;
   // cout << "Number of rows (matrix1): "; cin >> number_of_rows; cout << '\n';
   // cout << "Number of columns (matrix1): "; cin >> number_of_columns; cout << '\n';
   // Matrix matrix1(number_of_rows, number_of_columns);
   // cout << "Number of rows (matrix2): "; cin >> number_of_rows; cout << '\n';
   // cout << "Number of columns (matrix2): "; cin >> number_of_columns; cout << '\n';
   // Matrix matrix2(number_of_rows, number_of_columns);
   // matrix1.PrintMatrix(); matrix2.PrintMatrix();
   // cout << "Сумма: " << matrix1 + matrix2 << '\n';
   // cout << "Произведение: " << matrix1 * matrix2 << '\n';
   
  // ввод и вывод матрицы в файл
  
   Matrix matrix1("input.txt");                                   //файл откуда считывается матрица, пример ввода: 3 3 2 5 7 6 3 4 5 -2 -3
   matrix1.PrintMatrix();
   Matrix matrix2 = matrix1.Inverse(); 
   matrix2.PrintMatrix();                                         //вывод матрицы с фиксированной длинной ячейки 
   //cout << matrix1.Determinant() << '\n';
   cout << ( (matrix1 * !matrix1 == matrix1 * matrix2) ?  "equation is true" : "equation is false" ) << '\n';
   Matrix result =  matrix1 * !matrix1 * matrix1.Determinant();   //любое выражение с матрицами
   result.Round();
   result.PrintMatrix();
   ofstream file("output.txt");                                   //файл куда записывается матрица
   file << result;
   file.close();
   
   return 0;
} 