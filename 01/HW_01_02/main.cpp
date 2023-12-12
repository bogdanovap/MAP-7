#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <iomanip>

void sum(std::vector<int>& result, const std::vector<int>& v1, const std::vector<int>& v2, int start, int end) {
    for (int i = start; i < end; i++) {
        result[i] = v1[i] + v2[i];
    }
}

int main() {
    std::cout << std::setfill(' ');
    std::cout << "Всего ядер: " << std::thread::hardware_concurrency() << "\n";

    std::vector<int> nb_threads = {1, 2, 4, 8, 16};
    std::vector<int> sizes = {1000, 10000, 100000, 1000000};

    int std_width = 12;
    // шапка
    std::cout << std::setw(10) << "";
    for (auto& s : sizes) {
        std::cout << std::setw(std_width+1) << s;
    }
    std::cout<<std::endl;

    // в строках - количество потоков
    for (auto& threads : nb_threads) {
        std::cout << std::setw(2) << threads << " потоков";
        // в столбцах - размер векторов
        for (int size : sizes) {
            std::vector<int> v1(size, 5), v2(size, 3), result(size);
            std::vector<std::thread> all_threads;
            all_threads.reserve(threads);
            // каждый поток будет складывать часть массива
            // рассчитаем часть массива исходя из размера массива и количества потоков
            int step = size / threads;

            auto start = std::chrono::steady_clock::now();

            // ставим задачи в очередь
            for (int k = 0; k < threads; k++) {
                all_threads.emplace_back(sum, std::ref(result), std::ref(v1), std::ref(v2), k * step, (k + 1) * step);
            }
            // запускаем очередь
            for (auto &t: all_threads) {
                if (t.joinable()) t.join();
            }

            auto stop = std::chrono::steady_clock::now();
            std::chrono::duration<double> elapsed = stop - start;
            // выводим время на расчет
            std::cout << std::setw(std_width) << std::fixed << std::setprecision(8) << elapsed.count() << "s";
        }
        std::cout << std::endl;
    }

    return 0;
}