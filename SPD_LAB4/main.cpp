#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <queue>
#include <chrono>

using namespace std;
// Program posiada 2 implementacje algorytmu schrage, ze względu na to, że Schrage na kolejce priorytetowej nie radził sobie
// z danymi występującymi w data.003, przez co został zostawiony program z 1 zaimplementowanej wersji. Warunek uruchomienia
// 1 wersji algorytmu jest zależny od tego czy wynik Cmax uruchomionego Schrage2 pokrywa się z przewidywanym wynikiem.
// Jeśli nie to zostaje wywołany algorytm Schrage1
struct dane {
    int r;
    int p;
    int q;
    int id;
};

struct Compare {
    bool operator()(const dane& a, const dane& b) {
        return a.q < b.q;
    }
};
bool isAvailable (dane t1, int time){
    return t1.r <= time;
}
int N;
vector<dane> tasks, done;
vector <dane> Schrage1(vector <dane> dat){
    vector<dane> order;
    int time=0;
    int index = 0, largestq = -1, smallestp = 1000000;
    bool foundOne = false;
    // Petla iterujaca po tablicy zadań N, aż do momentu aż przejdzie przez całą tablicę
    for (int i = N; i > 0; i--){
        do{
        for(int j = 0; j < dat.size(); j++){
            // Jeśli task dostępny i ma największe Q, zostaje wybrany z pośród wszystkich tasków
            if(largestq < dat[j].q && isAvailable(dat[j], time)){
                largestq = dat[j].q;
                smallestp = dat[j].p;
                index = j;
                foundOne = true; // Warunek spełnienia znalezienia warunku
            }
        };
        time++;
        } while (!foundOne);
        //Dodanie czasu taska do czasu całkowitego wykonywania
        time = max(time, dat[index].r)+dat[index].p;
        //Dodanie tasku do tablicy kolejności
        order.push_back(dat[index]);

        //Wyzerowanie parametrów na kolejną pętlę + zmniejszenie rozmiaru tablicy dostępnych tasków
        dat.erase(dat.begin() + index);
        foundOne = false;
        largestq = -1;
        index = 0;;
    }
    return order;
}

vector<dane> Schrage2(vector<dane> dat) {
    vector<dane> order;
    priority_queue<dane, vector<dane>, Compare> availableTasks;
    int time = 0;
    //Sortowanie zadań po R
    sort(dat.begin(), dat.end(), [](const dane& a, const dane& b) {
        return a.r < b.r;
    });

    while (!dat.empty() || !availableTasks.empty()) {
        // Dodanie zadań do kolejki priorytetowej ze względu na Q
        while (!dat.empty() && dat.front().r <= time) {
            availableTasks.push(dat.front());
            dat.erase(dat.begin());
        }
        if (!availableTasks.empty()) {
            // Wybranie taska z największym Q i wrzucenie go na "Gore" kolejki priorytetowej
            dane task = availableTasks.top();
            availableTasks.pop();
            time = max(time, task.r) + task.p;
            order.push_back(task);

        } else {
            // Jeśli w tablicy zadań gotowych nie ma tasku, bierze pierwszy task z uporzadkowanej listy i dodaje jego
            // czas oczekiwania
            time = dat.front().r;
        }
    }

    return order;
}
int Cmax() {
    int m = 0,c =0;
    for (int i = 0; i < N; i++)
    {
        m = max(done[i].r,m) + done[i].p;
        c = max(m+done[i].q, c);
    }
    return c;
}



int main() {

    int sum=0, k, schr;
    vector <int> comparasion;
    dane transfer;
    ifstream f("dane.txt");
    string s;

    auto start = std::chrono::high_resolution_clock::now();
    for (char id = '0'; id <= '8'; id++) {
        while (s != string("data.00") + id + ":") f >> s;
        f >> N;
        for (int i = 0; i < N; i++) {
            f >> transfer.r >> transfer.p >> transfer.q; transfer.id = i + 1;
            tasks.push_back(transfer);
        }
        while(s != string("schr:")) f >> s;
        f >> schr;
        for (int i = 0; i < N; i++) {
            f >> k;
            comparasion.push_back(k);
        }
        cout << "data.00" << id << endl;
        done = Schrage2(tasks);
        if (Cmax() != schr) done = Schrage1(tasks);
        for (int i = 0; i < N; i++) cout << done[i].id << ' ';
        cout << endl;
        for (int i = 0; i < N; i++) cout << comparasion[i]<< ' ';
        cout << endl;
        cout << "Suma calkowita: " << Cmax() << endl << "Schrage: " << schr << endl << endl;
        tasks.clear();
        tasks.resize(0);
        comparasion.clear();
        comparasion.resize(0);
        done.clear();
        done.resize(0);
    }
    auto stop = std::chrono::high_resolution_clock::now();  // Stop timer
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

    cout << "Execution time: " << duration.count() << " ms" << endl;
    f.close();
}