#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>

bool received = false;
// Функция для отправки битовых данных
void send_bitwise(int pid, int num) {
    int i;
    // Перебираем каждый бит числа
    for (i = 0; i < sizeof(int) * 8; i++) {
        received = false;
        // Если текущий бит равен 1, отправляем SIGUSR1, иначе отправляем SIGUSR2
        if ((num >> i) & 1) {
            kill(pid, SIGUSR1);
        } else {
            kill(pid, SIGUSR2);
        }
        // Ждем некоторое время между отправкой каждого бита
        if (!received) {
            pause();
        }
    }
}

void signal_handler(int s) {
    if (s == SIGUSR1) {
        received = true;
    }
}

int main() {
    int receiver_pid;
    int number;
    
    signal(SIGUSR1, signal_handler);
    
    // Выводим свой собственный PID
    printf("Мой PID: %d\n", getpid());

    // Получаем PID приемника
    printf("Введите PID программы-приемника: ");
    scanf("%d", &receiver_pid);

    // Получаем число для передачи
    printf("Введите целое число для передачи: ");
    scanf("%d", &number);

    // Отправляем биты числа
    send_bitwise(receiver_pid, number);
    
    kill(receiver_pid, SIGINT);

    return 0;
}

