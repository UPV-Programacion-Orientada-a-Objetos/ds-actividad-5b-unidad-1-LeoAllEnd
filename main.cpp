#include <iostream>
using namespace std;

template <typename T>
class MatrizBase {
protected:
    int _filas;
    int _columnas;

public:
    MatrizBase(int f, int c) {
        _filas = f;
        _columnas = c;
    }

    virtual ~MatrizBase() {}

    int getFilas() const { return _filas; }
    int getColumnas() const { return _columnas; }

    virtual void cargarValores() = 0;
    virtual MatrizBase<T>* sumar(const MatrizBase<T>& otra) const = 0;
    virtual void imprimir() const = 0;
    virtual T getValor(int i, int j) const = 0;
};

template <typename T>
class MatrizDinamica : public MatrizBase<T> {
private:
    T **_datos;

public:
    MatrizDinamica(int f, int c) : MatrizBase<T>(f, c) {
        _datos = new T*[f];
        for(int i = 0; i < f; i++) {
            _datos[i] = new T[c];
            for(int j = 0; j < c; j++) {
                _datos[i][j] = 0;
            }
        }
    }

    MatrizDinamica(const MatrizDinamica<T>& otra) : MatrizBase<T>(otra._filas, otra._columnas) {
        _datos = new T*[this->_filas];
        for(int i = 0; i < this->_filas; i++) {
            _datos[i] = new T[this->_columnas];
            for(int j = 0; j < this->_columnas; j++) {
                _datos[i][j] = otra._datos[i][j];
            }
        }
    }

    MatrizDinamica<T>& operator=(const MatrizDinamica<T>& otra) {
        if(this != &otra) {
            for(int i = 0; i < this->_filas; i++) {
                delete[] _datos[i];
            }
            delete[] _datos;

            this->_filas = otra._filas;
            this->_columnas = otra._columnas;

            _datos = new T*[this->_filas];
            for(int i = 0; i < this->_filas; i++) {
                _datos[i] = new T[this->_columnas];
                for(int j = 0; j < this->_columnas; j++) {
                    _datos[i][j] = otra._datos[i][j];
                }
            }
        }
        return *this;
    }

    ~MatrizDinamica() {
        for(int i = 0; i < this->_filas; i++) {
            delete[] _datos[i];
        }
        delete[] _datos;
    }

    void cargarValores() {
        for(int i = 0; i < this->_filas; i++) {
            for(int j = 0; j < this->_columnas; j++) {
                cin >> _datos[i][j];
            }
        }
    }

    void setValor(int i, int j, T valor) {
        _datos[i][j] = valor;
    }

    T getValor(int i, int j) const {
        return _datos[i][j];
    }

    MatrizBase<T>* sumar(const MatrizBase<T>& otra) const {
        if(this->_filas != otra.getFilas() || this->_columnas != otra.getColumnas()) {
            return nullptr;
        }

        MatrizDinamica<T>* resultado = new MatrizDinamica<T>(this->_filas, this->_columnas);

        const MatrizDinamica<T>* otraDinamica = dynamic_cast<const MatrizDinamica<T>*>(&otra);

        if(otraDinamica) {
            for(int i = 0; i < this->_filas; i++) {
                for(int j = 0; j < this->_columnas; j++) {
                    resultado->_datos[i][j] = this->_datos[i][j] + otraDinamica->_datos[i][j];
                }
            }
        } else {
            for(int i = 0; i < this->_filas; i++) {
                for(int j = 0; j < this->_columnas; j++) {
                    resultado->_datos[i][j] = this->_datos[i][j] + otra.getValor(i, j);
                }
            }
        }

        return resultado;
    }

    void imprimir() const {
        for(int i = 0; i < this->_filas; i++) {
            cout << "| ";
            for(int j = 0; j < this->_columnas; j++) {
                cout << _datos[i][j];
                if(j < this->_columnas - 1) {
                    cout << " | ";
                }
            }
            cout << " |" << endl;
        }
    }
};

template <typename T, int M, int N>
class MatrizEstatica : public MatrizBase<T> {
private:
    T _datos[M][N];

public:
    MatrizEstatica() : MatrizBase<T>(M, N) {
        for(int i = 0; i < M; i++) {
            for(int j = 0; j < N; j++) {
                _datos[i][j] = 0;
            }
        }
    }

    void cargarValores() {
        for(int i = 0; i < M; i++) {
            for(int j = 0; j < N; j++) {
                cin >> _datos[i][j];
            }
        }
    }

    void setValor(int i, int j, T valor) {
        _datos[i][j] = valor;
    }

    T getValor(int i, int j) const {
        return _datos[i][j];
    }

    MatrizBase<T>* sumar(const MatrizBase<T>& otra) const {
        if(this->_filas != otra.getFilas() || this->_columnas != otra.getColumnas()) {
            return nullptr;
        }

        MatrizDinamica<T>* resultado = new MatrizDinamica<T>(M, N);

        const MatrizEstatica<T, M, N>* otraEstatica = dynamic_cast<const MatrizEstatica<T, M, N>*>(&otra);

        for(int i = 0; i < M; i++) {
            for(int j = 0; j < N; j++) {
                T suma = this->_datos[i][j] + otraEstatica->_datos[i][j];
                resultado->setValor(i, j, suma);
            }
        }

        return resultado;
    }

    void imprimir() const {
        for(int i = 0; i < M; i++) {
            cout << "| ";
            for(int j = 0; j < N; j++) {
                cout << _datos[i][j];
                if(j < N - 1) {
                    cout << " | ";
                }
            }
            cout << " |" << endl;
        }
    }
};

template <typename T>
MatrizBase<T>* operator+(const MatrizBase<T>& m1, const MatrizBase<T>& m2) {
    return m1.sumar(m2);
}

int main() {
    cout << "--- Sistema gerico de Algebra Lineal ---" << endl;
    cout << endl;

    cout << ">> Demostracion de Genericidad (Tipo FLOAT) <<" << endl;
    cout << endl;

    cout << "// 1. Creacion de Matriz Dinamica (a traves del puntero base)" << endl;
    cout << "Creando Matriz Dinamica A (3x2)..." << endl;

    MatrizBase<float>* A = new MatrizDinamica<float>(3, 2);
    MatrizDinamica<float>* ptrA = dynamic_cast<MatrizDinamica<float>*>(A);

    ptrA->setValor(0, 0, 1.5);
    ptrA->setValor(0, 1, 2.0);
    ptrA->setValor(1, 0, 0.0);
    ptrA->setValor(1, 1, 1.0);
    ptrA->setValor(2, 0, 4.5);
    ptrA->setValor(2, 1, 3.0);

    cout << "A =" << endl;
    A->imprimir();
    cout << endl;

    cout << "// 2. Creacion de Matriz Estatica (a traves del puntero base)" << endl;
    cout << "Creando Matriz Estatica B (3x2)..." << endl;

    MatrizBase<float>* B = new MatrizEstatica<float, 3, 2>();
    MatrizEstatica<float, 3, 2>* ptrB = dynamic_cast<MatrizEstatica<float, 3, 2>*>(B);

    ptrB->setValor(0, 0, 0.5);
    ptrB->setValor(0, 1, 1.0);
    ptrB->setValor(1, 0, 2.0);
    ptrB->setValor(1, 1, 3.0);
    ptrB->setValor(2, 0, 1.0);
    ptrB->setValor(2, 1, 1.0);

    cout << "B =" << endl;
    B->imprimir();
    cout << endl;

    cout << "// 3. Operacion Polimorfica (Suma)" << endl;
    cout << "SUMANDO: Matriz C = A + B ..." << endl;
    cout << "(La suma es manejada por el metodo virtual de MatrizDinamica)" << endl;
    cout << endl;

    MatrizBase<float>* C = *A + *B;

    cout << "Matriz Resultado C (3x2, Tipo FLOAT):" << endl;
    C->imprimir();
    cout << endl;

    cout << ">> Demostracion de Limpieza de Memoria <<" << endl;
    cout << "Llamando al destructor de C..." << endl;
    cout << "Liberando memoria de C (Matriz Dinamica)..." << endl;
    delete C;

    cout << "Llamando al destructor de A..." << endl;
    cout << "Liberando memoria de A (Matriz Dinamica)..." << endl;
    delete A;

    cout << "Sistema cerrado." << endl;

    return 0;
}
