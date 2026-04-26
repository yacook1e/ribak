#include <iostream>
#include <cmath>
#include <iomanip>
#include <windows.h>

using namespace std;


//f1(x,y) = y - x^2
double f1(double x, double y) {
    return y - x * x;
}

//f2(x,y) = x - exp(-y)
double f2(double x, double y) {
    return x - exp(-y);
}

//Phi(x,y) = f1^2 + f2^2
double Phi(double x, double y) {
    double F1 = f1(x, y);
    double F2 = f2(x, y);
    return F1 * F1 + F2 * F2;
}

//  d = 2 * W' * F  W' — транспонированная матрица Якоби
// ( матрица симметрична, поэтому W' = W)
void compute_d(double x, double y, double& d1, double& d2) {
    double F1 = f1(x, y);
    double F2 = f2(x, y);
    double e = exp(-y);                     // e^{-y} 

    // Элементы матрицы
    // J11 = df1/dx = -2x,  J12 = df1/dy = 1
    // J21 = df2/dx = 1,    J22 = df2/dy = e^{-y}
    // Вычисляем d = W' * F
    d1 = 2.0 * ( - 2.0 * x * F1 + 1.0 * F2);          // = J11*F1 + J21*F2
    d2 = 2.0 * (1.0 * F1 + e * F2);                 // = J12*F1 + J22*F2
}

int main() {
    setlocale(LC_ALL, "Russian");

    double x = 0.5;
    double y = 0.5;

    const double tol = 1e-4;
    const int max_iter = 100;
    int iter = 0;

    double x_prev, y_prev;  //для проверки сходимости

    cout << fixed << setprecision(6);
    cout << "Iter" << setw(16) << "x" << setw(16) << "y"
        << setw(16) << "f1" << setw(16) << "f2"
        << setw(16) << "Phi" << endl;

    while (iter < max_iter) {
        x_prev = x;
        y_prev = y;

        double F1 = f1(x, y);
        double F2 = f2(x, y);
        double phi = F1 * F1 + F2 * F2;

        cout << setw(4) << iter << setw(16) << x << setw(16) << y
            << setw(16) << F1 << setw(16) << F2
            << setw(16) << phi << endl;

        // Считаем направление спуска
        double d1, d2;
        compute_d(x, y, d1, d2);

        // Квадрат нормы вектора
        double norm_d2 = d1 * d1 + d2 * d2;
        if (norm_d2 < 1e-30) {
            cout << "Градиент слишком мал, процесс остановлен." << endl;
            break;
        }

        // Вычисление шага лямбда
        double lambda = phi / norm_d2;

        // Обновление приближения
        x -= lambda * d1;
        y -= lambda * d2;

        // проверка сходимости
        double norm_diff = abs(x - x_prev) + abs(y - y_prev);  // ||X^(k+1) - X^(k)||
        double norm_curr = abs(x) + abs(y);                     // ||X^(k)||

        if (norm_curr < 1e-10) norm_curr = 1e-10;

        double relative_error = norm_diff / norm_curr;

        if (relative_error < tol) {
            cout << "Решение найдено с заданной точностью." << endl;
            break;
        }

        ++iter;
    }

    if (iter == max_iter) {
        cout << "\nДостигнуто максимальное число итераций." << endl;
    }

    return 0;
}
