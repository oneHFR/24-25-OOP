/* 2152131 Œ‚∫È»Ô º∆ø∆ */
#include <iostream>
#include <string>
using namespace std;

#define OPERATOR_IS_MENBER_FUNCTION 0

template<typename ElemType, size_t ROW, size_t COL>
class matrix {
private:
    ElemType value[ROW][COL];

public:
    matrix() {}

    template<typename ElemType_F, size_t ROW_F, size_t COL_F>
    friend istream& operator>>(istream& in, matrix<ElemType_F, ROW_F, COL_F>& x);

    template<typename ElemType_F, size_t ROW_F, size_t COL_F>
    friend ostream& operator<<(ostream& out, const matrix<ElemType_F, ROW_F, COL_F>& x);

#if OPERATOR_IS_MENBER_FUNCTION
    matrix operator+(const matrix& other) {
        matrix result;
        for (size_t i = 0; i < ROW; i++) {
            for (size_t j = 0; j < COL; j++) {
                result.value[i][j] = this->value[i][j] + other.value[i][j];
            }
        }
        return result;
    }
#else
    template<typename ElemType_F, size_t ROW_F, size_t COL_F>
    friend matrix<ElemType_F, ROW_F, COL_F> operator+(
        const matrix<ElemType_F, ROW_F, COL_F>& x,
        const matrix<ElemType_F, ROW_F, COL_F>& y);
#endif
};

template<typename ElemType, size_t ROW, size_t COL>
istream& operator>>(istream& in, matrix<ElemType, ROW, COL>& x) {
    for (size_t i = 0; i < ROW; i++) {
        for (size_t j = 0; j < COL; j++) {
            in >> x.value[i][j];
        }
    }
    return in;
}

template<typename ElemType, size_t ROW, size_t COL>
ostream& operator<<(ostream& out, const matrix<ElemType, ROW, COL>& x) {
    for (size_t i = 0; i < ROW; i++) {
        for (size_t j = 0; j < COL; j++) {
            out << x.value[i][j] << ' ';
        }
        out << endl;
    }
    return out;
}

#if !OPERATOR_IS_MENBER_FUNCTION
template<typename ElemType, size_t ROW, size_t COL>
matrix<ElemType, ROW, COL> operator+(
    const matrix<ElemType, ROW, COL>& x,
    const matrix<ElemType, ROW, COL>& y) {
    matrix<ElemType, ROW, COL> result;
    for (size_t i = 0; i < ROW; i++) {
        for (size_t j = 0; j < COL; j++) {
            result.value[i][j] = x.value[i][j] + y.value[i][j];
        }
    }
    return result;
}
#endif