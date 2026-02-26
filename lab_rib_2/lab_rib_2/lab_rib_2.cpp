#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

// Левая часть первого уравнения: f1(x,y) = y - x^2
double f1(double x, double y) {
    return y - x * x;
}

// Левая часть второго уравнения: f2(x,y) = x - exp(-y)
double f2(double x, double y) {
    return x - exp(-y);
}

// Функция Phi(x,y) = f1^2 + f2^2
double Phi(double x, double y) {
    double F1 = f1(x, y);
    double F2 = f2(x, y);
    return F1 * F1 + F2 * F2;
}

// Вычисление вектора d = W' * F, где W' — транспонированная матрица Якоби
// (в данной задаче матрица симметрична, поэтому W' = W)
void compute_d(double x, double y, double& d1, double& d2) {
    double F1 = f1(x, y);
    double F2 = f2(x, y);
    double e = exp(-y);                     // e^{-y}

    // Элементы матрицы Якоби:
    // J11 = df1/dx = -2x,  J12 = df1/dy = 1
    // J21 = df2/dx = 1,    J22 = df2/dy = e^{-y}
    // Вычисляем d = W' * F (здесь W' совпадает с W)
    d1 = -2.0 * x * F1 + 1.0 * F2;          // = J11*F1 + J21*F2
    d2 = 1.0 * F1 + e * F2;                 // = J12*F1 + J22*F2
}

int main() {
    // Начальное приближение (выбираем произвольно, например (0.5, 0.5))
    double x = 0.5;
    double y = 0.5;

    const double tol = 1e-14;                // желаемая точность по Phi
    const int max_iter = 100;                 // максимальное число итераций
    int iter = 0;

    // Форматированный вывод заголовка таблицы
    cout << fixed << setprecision(12);
    cout << "Iter" << setw(16) << "x" << setw(16) << "y"
        << setw(16) << "f1" << setw(16) << "f2"
        << setw(16) << "Phi" << endl;

    while (iter < max_iter) {
        double F1 = f1(x, y);
        double F2 = f2(x, y);
        double phi = F1 * F1 + F2 * F2;

        // Вывод текущей итерации
        cout << setw(4) << iter << setw(16) << x << setw(16) << y
            << setw(16) << F1 << setw(16) << F2
            << setw(16) << phi << endl;

        // Проверка достижения точности
        if (phi < tol) {
            cout << "\nРешение найдено с заданной точностью." << endl;
            break;
        }

        // Вычисление направления спуска d = W' * F
        double d1, d2;
        compute_d(x, y, d1, d2);

        // Квадрат нормы вектора d (знаменатель для лямбды)
        double norm_d2 = d1 * d1 + d2 * d2;
        if (norm_d2 < 1e-30) {                // защита от деления на ноль
            cout << "Градиент слишком мал, процесс остановлен." << endl;
            break;
        }

        // Вычисление шага λ по формуле варианта А
        double lambda = phi / norm_d2;

        // Обновление приближения: X_{k+1} = X_k - λ * d
        x -= lambda * d1;
        y -= lambda * d2;

        ++iter;
    }

    if (iter == max_iter) {
        cout << "\nДостигнуто максимальное число итераций." << endl;
    }

    return 0;
}