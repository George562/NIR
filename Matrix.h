#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>
using namespace std;

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

template <typename _T>
class Matrix {
public:
    vector<vector<_T>> data;
    pair<size_t, size_t> dim;

    Matrix(size_t, size_t=0); // create matrix nxm
    Matrix(pair<size_t, size_t> p) : Matrix(p.first, p.second) {}
    Matrix() : Matrix((int)0) {} // defolt constructor
    Matrix(Matrix<_T>&); // copy matryx
    Matrix(char*); // load matrix from file
    Matrix(vector<_T>, size_t=0, size_t=0); // load matrix from vector
    vector<_T>& operator[](size_t); // get line
    void printM(); // print matrix 
    void recordM(char*); // record matrix to file
    void sumL(size_t, size_t, _T, size_t=0, size_t=0); // adds the first row to the second row with a coefficient
    void subL(size_t i, size_t j, _T coef, size_t from=0, size_t to=0) { this->sumL(i, j, -coef, from, to); }
    void sumC(size_t, size_t, _T, size_t=0, size_t=0); // adds the first row to the second row with a coefficient
    void subC(size_t i, size_t j, _T coef, size_t from=0, size_t to=0) { this->sumC(i, j, -coef, from, to); }
    void swapL(size_t, size_t); // swap lines
    void swapC(size_t, size_t); // swap colomns
    pair<size_t, size_t> max(size_t, size_t, size_t, size_t); // max in rect 
    size_t maxC(size_t, size_t=0, size_t=0); // max in one colomn
    size_t maxL(size_t, size_t=0, size_t=0); // max in one line
    Matrix<_T> T();
    double norm2();
    double normC();
    void resize(size_t, size_t);
};

////////////////////////////////////////////////////////////
// Operators
////////////////////////////////////////////////////////////

template <typename _T>
Matrix<_T> operator*(Matrix<_T> left, Matrix<_T> right) {
    if (left.dim.second != right.dim.first) {
        cerr << "Dimension error: can't multiply matrix " << left.dim.first << "x" <<
                left.dim.second << " with matrix " << right.dim.first << "x" << right.dim.second << "\n";
        exit(1);
    }
    Matrix<_T> res(left.dim.first, right.dim.second);
    for (size_t i = 0; i < left.dim.first; i++)
        for (size_t j = 0; j < right.dim.second; j++)
            for (size_t k = 0; k < right.dim.first; k++)
                res[i][j] += left[i][k] * right[k][j];
    return res;
}

template <typename _T>
Matrix<_T> operator-(Matrix<_T> m) {
    Matrix<_T> res(m.dim);
    for (size_t i = 0; i < res.dim.first; i++)
        for (size_t j = 0; j < res.dim.second; j++)
            res[i][j] = -m[i][j];
    return res;
}

template <typename _T>
Matrix<_T> operator+(Matrix<_T> left, Matrix<_T> right) {
    if (left.dim.first != right.dim.first && left.dim.second != right.dim.second) {
        cerr << "Dimension error: can't sum matrix " << left.dim.first << "x" <<
                left.dim.second << " with matrix " << right.dim.first << "x" << right.dim.second << "\n";
        exit(1);
    }
    Matrix<_T> res(left);
    for (size_t i = 0; i < left.dim.first; i++)
        for (size_t j = 0; j < right.dim.second; j++)
            res[i][j] = res[i][j] + right[i][j];
    return res;
}
template <typename _T>
Matrix<_T> operator-(Matrix<_T> left, Matrix<_T> right) { return left + (-right); }

template <typename _T>
Matrix<_T> operator*(Matrix<_T> left, _T right) {
    Matrix<_T> res(left);
    for (size_t i = 0; i < left.dim.first; i++)
        for (size_t j = 0; j < left.dim.second; j++)
            res[i][j] = left[i][j] * right;
    return res;
}
template <typename _T>
Matrix<_T> operator*(_T left, Matrix<_T> right) { return right * left; }

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

template <typename _T>
Matrix<_T>::Matrix(size_t n, size_t m) {
    m = (m == 0) ? n : m;
    this->dim = {n, m};
    this->data.resize(n, vector<_T>(m));
}

template <typename _T>
Matrix<_T>::Matrix(Matrix<_T>& m) {
    this->dim = m.dim;
    this->data = m.data;
}

template <typename _T>
Matrix<_T>::Matrix(char* s) {
    ifstream input(s);
    input >> this->dim.first >> this->dim.second;
    this->data.resize(this->dim.first, vector<_T>(this->dim.second));
    for (size_t i = 0; i < this->dim.first; i++)
        for (size_t j = 0; j < this->dim.second; j++)
            input >> this->data[i][j];
}

template <typename _T>
Matrix<_T>::Matrix(vector<_T> v, size_t n, size_t m) {
    n = (n == 0) ? 1        : n;
    m = (m == 0) ? v.size() : m;
    this->dim = {n, m};
    this->data.resize(n, vector<_T>(m));
    for (size_t i = 0; i < n; i++)
        for (size_t j = 0; j < m; j++)
            this->data[i][j] = v[i * m + j];
}

template <typename _T>
vector<_T>& Matrix<_T>::operator[](size_t i) { return this->data[i]; }

template <typename _T>
void Matrix<_T>::printM() {
    cout.precision(4);
    for (size_t i = 0; i < this->dim.first; i++) {
        for (size_t j = 0; j < this->dim.second; j++)
            cout << this->data[i][j] << '\t';
        cout << '\n';
    }
}

template <typename _T>
void Matrix<_T>::recordM(char* s) {
    ofstream output(s);
    output.precision(4);
    output << this->dim.first << ' ' << this->dim.second << '\n';
    for (size_t i = 0; i < this->dim.first; i++) {
        for (size_t j = 0; j < this->dim.second; j++)
            output << this->data[i][j] << '\t';
        output << '\n';
    }
}

template <typename _T>
void Matrix<_T>::sumL(size_t i, size_t j, _T coef, size_t from, size_t to) {
    to = (to == 0) ? this->dim.second : to;
    for (; from < to; from++)
        this->data[j][from] += coef * this->data[i][from];
}

template <typename _T>
void Matrix<_T>::sumC(size_t i, size_t j, _T coef, size_t from, size_t to) {
    to = (to == 0) ? this->dim.first : to;
    for (; from < to; from++)
        this->data[from][j] += coef * this->data[from][i];
}

template <typename _T>
void Matrix<_T>::swapL(size_t i, size_t j) {
    for (size_t k = 0; k < this->dim.second; k++) swap(this->data[j][k], this->data[i][k]);
}
template <typename _T>
void Matrix<_T>::swapC(size_t i, size_t j) {
    for (size_t k = 0; k < this->dim.first;  k++) swap(this->data[k][j], this->data[k][i]);
}

template <typename _T>
pair<size_t, size_t> Matrix<_T>::max(size_t fromC, size_t toC, size_t fromL, size_t toL) {
    pair<size_t, size_t> res {fromL, fromC}; // {line, colomn}
    for (size_t i; fromC < toC; fromC++)
        for (i = fromL; i < toL; i++)
            if (abs(this->data[res.first][res.second]) < abs(this->data[i][fromC]))
                res = {i, fromC};
    return res; // {line, colomn}
}

template <typename _T>
size_t Matrix<_T>::maxC(size_t Colomn, size_t from, size_t to) {
    to = (to == 0) ? this->dim.second : to;
    return this->max(Colomn, Colomn + 1, from, to).first;
}

template <typename _T>
size_t Matrix<_T>::maxL(size_t Line, size_t from, size_t to) {
    to = (to == 0) ? this->dim.first : to;
    return this->max(from, to, Line, Line + 1).second;
}

template <typename _T>
Matrix<_T> Matrix<_T>::T() {
    Matrix<_T> res(this->dim.second, this->dim.first);
    for (size_t i = 0; i < res.dim.first; i++)
        for (size_t j = 0; j < res.dim.second; j++)
            res[i][j] = this->data[j][i];
    return res;
}

template <typename _T>
double Matrix<_T>::norm2() {
    double res = 0;
    for (size_t i = 0; i < this->dim.first; i++)
        for (size_t j = 0; j < this->dim.second; j++)
            res += pow(this->data[i][j], 2);
    res = pow(res, 0.5d);
    return res;
}

template <typename _T>
double Matrix<_T>::normC() {
    double res = 0, cur = 0;
    for (size_t i = 0; i < this->dim.first; i++) {
        cur = 0;
        for (size_t j = 0; j < this->dim.second; j++)
            cur += abs(this->data[i][j]);
        res = (cur > res) ? cur : res;
    }
    return res;
}

template <typename _T>
Matrix<_T> E(size_t n) {
    Matrix<_T> res(n);
    for (size_t i = 0; i < n; i++) res[i][i] = 1;
    return res;
}

template <typename _T>
void Matrix<_T>::resize(size_t n, size_t m) {
    this->dim = {n, m};
    this->data.resize(n);
    for (size_t i = 0; i < n; i++) this->data[i].resize(m);
}
