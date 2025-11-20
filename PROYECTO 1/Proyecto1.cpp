#include <bits/stdc++.h>
using namespace std;

struct InstructorRec {
    int codigo{};
    string nombre;
    double n1{}, n2{}, n3{};
    double nf{}; // nota final
};
struct EmpleadoRec {
    int codigo{};
    string nombre;
    int cursos_antes{}; // > 0
};

static inline string trim(string s) {
    auto notspace = [](int ch){ return !isspace(ch); };
    s.erase(s.begin(), find_if(s.begin(), s.end(), notspace));
    s.erase(find_if(s.rbegin(), s.rend(), notspace).base(), s.end());
    return s;
}

vector<string> split_semicolon(const string& line) {
    vector<string> parts;
    string cur;
    for (char c : line) {
        if (c == ';') {
            parts.push_back(trim(cur));
            cur.clear();
        } else {
            cur.push_back(c);
        }
    }
    parts.push_back(trim(cur));
    return parts;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    if (!(cin >> N)) {
        cerr << "Error: no se pudo leer N\n";
        return 1;
    }
    string dummy; getline(cin, dummy); // consumir fin de línea

    vector<InstructorRec> instrs;
    instrs.reserve(N);

    for (int i = 0; i < N; ++i) {
        string line; getline(cin, line);
        if (line.empty()) { --i; continue; }
        auto p = split_semicolon(line);
        if (p.size() != 5) {
            cerr << "Linea instructor invalida: se esperan 5 campos -> " << line << "\n";
            return 1;
        }
        InstructorRec r;
        try {
            r.codigo = stoi(p[0]);
            r.nombre = p[1];
            r.n1 = stod(p[2]);
            r.n2 = stod(p[3]);
            r.n3 = stod(p[4]);
        } catch (...) {
            cerr << "Error de formato en linea de instructor: " << line << "\n";
            return 1;
        }
        r.nf = (r.n1 + r.n2 + r.n3) / 3.0;
        instrs.push_back(r);
    }

    int M;
    if (!(cin >> M)) {
        cerr << "Error: no se pudo leer M\n";
        return 1;
    }
    getline(cin, dummy); // fin de línea

    vector<EmpleadoRec> emps;
    emps.reserve(M);

    for (int i = 0; i < M; ++i) {
        string line; getline(cin, line);
        if (line.empty()) { --i; continue; }
        auto p = split_semicolon(line);
        if (p.size() != 3) {
            cerr << "Linea empleado invalida: se esperan 3 campos -> " << line << "\n";
            return 1;
        }
        EmpleadoRec e;
        try {
            e.codigo = stoi(p[0]);
            e.nombre = p[1];
            e.cursos_antes = stoi(p[2]); // el enunciado dice > 0 (confiamos o validamos si quieres)
        } catch (...) {
            cerr << "Error de formato en linea de empleado: " << line << "\n";
            return 1;
        }
        emps.push_back(e);
    }

    // Índices por código para cruces rápidos
    unordered_map<int, EmpleadoRec> empByCode;
    empByCode.reserve(emps.size() * 2);
    for (const auto& e : emps) empByCode[e.codigo] = e;

    // Comparator por nombre ascendente (estable, sensible a acentos estándar)
    auto byNombre = [](const auto& a, const auto& b){
        return a.nombre < b.nombre;
    };

    cout.setf(std::ios::fixed); cout << setprecision(2);

    // (a) Reporte: codigo, nombre, nota final, cursos aprobados (incl. este si nf>=11)
    // Se entiende que requiere datos de AMBOS archivos -> intersección por código.
    struct ARow { int codigo; string nombre; double nf; int cursos_tot; };
    vector<ARow> repA;
    repA.reserve(instrs.size());

    for (const auto& r : instrs) {
        auto it = empByCode.find(r.codigo);
        if (it != empByCode.end()) {
            int total = it->second.cursos_antes + ((r.nf >= 11.0) ? 1 : 0);
            repA.push_back({r.codigo, r.nombre, r.nf, total});
        }
    }
    sort(repA.begin(), repA.end(), [&](const ARow& x, const ARow& y){
        return x.nombre < y.nombre;
    });

    cout << "==============================\n";
    cout << "a) REPORTE GENERAL (interseccion por codigo)\n";
    cout << "==============================\n";
    cout << left << setw(10) << "Codigo"
         << left << setw(30) << "Nombre"
         << right << setw(10) << "Final"
         << right << setw(12) << "CursosTot\n";
    cout << string(62,'-') << "\n";
    for (const auto& row : repA) {
        cout << left << setw(10) << row.codigo
             << left << setw(30) << row.nombre.substr(0,29)
             << right << setw(10) << row.nf
             << right << setw(12) << row.cursos_tot << "\n";
    }
    cout << "\n";

    // (b) Reporte: alumnos con nota final < 11
    vector<InstructorRec> repB;
    for (const auto& r : instrs) if (r.nf < 11.0) repB.push_back(r);
    sort(repB.begin(), repB.end(), byNombre);

    cout << "==============================\n";
    cout << "b) DESAPROBADOS (final < 11)\n";
    cout << "==============================\n";
    cout << left << setw(10) << "Codigo"
         << left << setw(30) << "Nombre"
         << right << setw(10) << "Final\n";
    cout << string(50,'-') << "\n";
    for (const auto& r : repB) {
        cout << left << setw(10) << r.codigo
             << left << setw(30) << r.nombre.substr(0,29)
             << right << setw(10) << r.nf << "\n";
    }
    cout << "\n";

    // (c) En instructor pero NO en empleado
    vector<InstructorRec> repC;
    for (const auto& r : instrs) {
        if (!empByCode.count(r.codigo)) repC.push_back(r);
    }
    sort(repC.begin(), repC.end(), byNombre);

    cout << "==============================\n";
    cout << "c) EN INSTRUCTOR PERO NO EN EMPLEADO\n";
    cout << "==============================\n";
    cout << left << setw(10) << "Codigo"
         << left << setw(30) << "Nombre" << "\n";
    cout << string(40,'-') << "\n";
    for (const auto& r : repC) {
        cout << left << setw(10) << r.codigo
             << left << setw(30) << r.nombre.substr(0,29) << "\n";
    }
    cout << "\n";

    // (d) Tabla para el instructor con promedios por examen y promedio final
    // Orden por nombre
    vector<InstructorRec> repD = instrs;
    sort(repD.begin(), repD.end(), byNombre);

    double sum1=0, sum2=0, sum3=0, sumf=0;
    for (const auto& r : repD) {
        sum1 += r.n1; sum2 += r.n2; sum3 += r.n3; sumf += r.nf;
    }
    int C = (int)repD.size();

    cout << "==============================\n";
    cout << "d) TABLA PARA EL INSTRUCTOR (con promedios al final)\n";
    cout << "==============================\n";
    cout << left << setw(10) << "Codigo"
         << left << setw(30) << "Nombre"
         << right << setw(8)  << "Ex1"
         << right << setw(8)  << "Ex2"
         << right << setw(8)  << "Ex3"
         << right << setw(10) << "Final\n";
    cout << string(74,'-') << "\n";
    for (const auto& r : repD) {
        cout << left << setw(10) << r.codigo
             << left << setw(30) << r.nombre.substr(0,29)
             << right << setw(8)  << r.n1
             << right << setw(8)  << r.n2
             << right << setw(8)  << r.n3
             << right << setw(10) << r.nf << "\n";
    }
    cout << string(74,'-') << "\n";
    if (C > 0) {
        cout << left << setw(40) << "Promedios"
             << right << setw(8)  << (sum1 / C)
             << right << setw(8)  << (sum2 / C)
             << right << setw(8)  << (sum3 / C)
             << right << setw(10) << (sumf / C) << "\n";
    } else {
        cout << "(Sin registros de instructor)\n";
    }

    return 0;
}

