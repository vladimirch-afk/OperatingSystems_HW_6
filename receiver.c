#include <stdio.h>
#include <signal.h>
#include <unistd.h>

// Глобальная переменная для хранения принятого числа
volatile int received_number = 0;
// ID потока-отправителя
int sender_pid;
// Счетчик битов
volatile int bit_counter = 0;

// Обработчик сигнала SIGUSR1
void sigusr1_handler(int signum) {
    // Устанавливаем бит в принятом числе
    received_number |= (1 << bit_counter);
    bit_counter++;
    kill(sender_pid, SIGUSR1);
}

// Обработчик сигнала SIGUSR2
void sigusr2_handler(int signum) {
    // Просто увеличиваем счетчик битов
    bit_counter++;
     kill(sender_pid, SIGUSR1);
}

int main() {
    // Устанавливаем обработчики сигналов
    signal(SIGUSR1, sigusr1_handler);
    signal(SIGUSR2, sigusr2_handler);

    // Выводим свой PID
    printf("Мой PID: %d\n", getpid());
    
    // Получаем PID отправителя
    printf("Введите PID программы-отправителя: ");
    scanf("%d", &sender_pid);
    pause();

    // Бесконечный цикл ожидания сигналов
    while (bit_counter < sizeof(int) * 8) {
        usleep(1000);
    }

    // Выводим принятое число
    printf("Принятое число: %d\n", received_number);

    return 0;
}

