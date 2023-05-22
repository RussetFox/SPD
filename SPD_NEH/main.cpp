#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <utility>
#include <chrono>
using namespace std;

int cMax(int N, int M, int P1[503][503], vector<int> Q){
    int T[1000][100];
    for (int n = 0; n < N; n++){
        for (int m = 0; m <= M; m++){
            T[n][m] = 0;
        }
    }

    for (int n = 1; n <= N; n++){
        for (int m = 1; m <= M; m++) {
            T[n][m] = max(T[n-1][m], T[n][m-1]) + P1[Q[n-1]][m-1];
        }
    }

    return T[N][M];
}

//funkcja zwracajaca ustawienie tablicy Q, w celu
int bestCmax(int N, int M, int P1[503][503], vector<int> Q){
    vector<int> help1, help2;
    for (int i = 0; i < N; i++){
        help1.push_back(Q[i]);
        help2 = help1;
        for(int z = 0; z < help1.size()-1; z++){
            swap(help1[help1.size()-z-2], help1[help1.size()-z-1]);
            if (cMax(i+1, M, P1, help1) <= cMax(i+1, M, P1, help2)) help2 = help1;
        }
    help1 = help2;
    }
    Q = help1;
    return cMax(N,M,P1,Q);
}

int main() {
    std::chrono::duration<double, std::milli> suma_czasow(0);
    ifstream f("dane.txt");
    string s;
    int N, M, neh, result;
    int P1[503][503];
    vector<int> Q(10000);
    vector<pair<int,int>> P2(503);
    int help;
    for (int i = 0; i < 121; i++){
        string str;
        if(i < 10) str = "00"+ to_string(i);
        if (10 <= i && i <100) str = "0"+ to_string(i);
        if (i >= 100) str = to_string(i);
        while (s != string("data.")+str+":") f >> s;
        f >> N >> M;
        for (int i = 0; i < N; i++){
            for(int j = 0; j < M; j++)
            {
                f >> help;
                P1[i][j] = help;
            }
        }
        while (s != string("neh:")) f >> s;
        f >> neh; //czytanie wartości neh dla danego zestawu danych w celu weryfikacji poprawnego wyniku
        for(int i = 0; i < N; i++){
            P2[i].second = 0;
        }

        for (int i = 0; i < N; i++)
        {
            P2[i].first = i;
            for (int j = 0; j < M; j++){
                P2[i].second += P1[i][j];
            }
        }
        sort(P2.begin(), P2.begin()+N, [](const auto& a, const auto& b) {
        if (a.second > b.second) return true;
        if (a.second ==  b.second && a.first < b.first) return true;
        return false;

        });
        for (int i = 0; i < N; i++){
        Q[i] = P2[i].first;
        }
        auto start = std::chrono::high_resolution_clock::now();
        result = bestCmax(N,M,P1,Q);
        auto end = std::chrono::high_resolution_clock::now();
        suma_czasow += end - start;
        //sprawdzenie zgodnosci wynikow
        if (result == neh) cout << "data." + str << ": zgodne" <<endl;
        else cout << "data."+str << ": niezgodne"<<endl;
    }
    std::cout << "Suma czasow: " << suma_czasow.count() / 1000.00 << " sekund" << std::endl;
}





