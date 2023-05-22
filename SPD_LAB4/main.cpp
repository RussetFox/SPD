#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

struct dane {
    int r;
    int p;
    int q;
    int id;
    bool checked;
};

int N;
dane temp[100], final[100];

bool compare_r(dane a, dane b) {
    return a.r < b.r;
}

bool compare_p(dane a, dane b) {
    return a.p > b.p;
}

bool compare_q(dane a, dane b) {
    return a.q > b.q;
}

int Cmax() {
    int m = 0, c = 0;
    for (int i = 0; i < N; i++)
    {
        m = max(final[i].r, m) + final[i].p;
        c = max(m + final[i].q, c);
    }
    return c;
}
void select_order() {
    int help = 0, runtime = 0;
    do {
        for (int i = 0; i < N; i++) {
            if (!temp[i].checked && temp[i].r <= runtime) {
                final[help] = temp[i];
                temp[i].checked = true;
                help++;
                runtime += temp[i].p;
            }
        }
        runtime++;
    } while (help < N);
}


int main() {
    int sum=0;
    vector<vector<int>>(10)
    ifstream f("dane.txt");
    string s;

    for (char id = '1'; id <= '4'; id++) {
        while (s != string("data.") + id) f >> s;
        f >> N;
        for (int i = 0; i < N; i++) {
            f >> temp[i].r >> temp[i].p >> temp[i].q; temp[i].checked = false; temp[i].id = i + 1;
        }
        cout << "data." << id << endl;
        cout << "Kolejnosc: ";
        for (int i = 0; i < N; i++) cout << final[i].id << ' ';
        cout << endl << "Suma: " << Cmax() << endl;
        cout << endl;
        sum += Cmax();
    }
    cout << "Suma calkowita: " << sum << endl;
    f.close();
}