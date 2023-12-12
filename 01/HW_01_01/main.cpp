#include <iostream>
#include <thread>
#include <chrono>

int client_counter = 0;
const int maxClients = 10;

void client_thread() {
    while (true) {
        if (client_counter < maxClients) {
            client_counter++;
            std::cout << "Пришел клиент. Всего клиентов в очереди: " << client_counter << "\n";
            std::cout.flush();
        }
        if (client_counter >= maxClients) {
            std::cout << "Очередь наполнена\n";
            std::cout.flush();
            break;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void operator_thread() {
    while (true) {
        // сразу проверяем есть ли клиент в очереди
        if (client_counter <= 0) {
            std::cout << "Очередь пустая\n";
            std::cout.flush();
            break;
        }
        // если в очереди есть клиент, то берем 2 секунды на его обслуживание
        std::this_thread::sleep_for(std::chrono::seconds(2));
        // после обслуживания - уменьшаем очередь
        client_counter--;
        std::cout << "Клиент обслужен. Осталось клиентов в очереди: " << client_counter << "\n";
        std::cout.flush();
    }
}

int main() {
    std::thread client(client_thread);
    std::thread thread(operator_thread);

    if (client.joinable() && thread.joinable()) {
        client.join();
        thread.join();
    }

    return 0;
}