#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/stat.h>

#define PORT 45600
#define BUFFER_SIZE 4096

// Структура пользователя
typedef struct {
    char username[32]; // Уязвимость: небольшой буфер для username
    char password[32]; // Уязвимость: небольшой буфер для password
    int is_admin;      // Флаг администратора
} User;

// Функция чтения файла
char *read_file(const char *filepath) {
    FILE *file = fopen(filepath, "r");
    if (!file) {
        perror("Ошибка открытия файла");
        return NULL;
    }

    struct stat file_stat;
    if (stat(filepath, &file_stat) < 0) {
        perror("Ошибка получения информации о файле");
        fclose(file);
        return NULL;
    }

    size_t filesize = file_stat.st_size;

    char *content = malloc(filesize + 1);
    if (!content) {
        perror("Ошибка выделения памяти");
        fclose(file);
        return NULL;
    }

    fread(content, 1, filesize, file);
    content[filesize] = '\0';
    fclose(file);
    return content;
}

// Функция обработки клиента
void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    int bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_received < 0) {
        perror("Ошибка чтения от клиента");
        close(client_socket);
        return;
    }

    buffer[bytes_received] = '\0';
    printf("Получено сообщение от клиента:\n%s\n", buffer);

    if (strstr(buffer, "GET /index.html")) {
        char *file_content = read_file("index.html");
        if (file_content) {
            char response[BUFFER_SIZE];
            snprintf(response, sizeof(response),
                     "HTTP/1.1 200 OK\r\n"
                     "Content-Type: text/html\r\n"
                     "Content-Length: %ld\r\n"
                     "\r\n%s",
                     strlen(file_content), file_content);
            send(client_socket, response, strlen(response), 0);
            free(file_content);
        } else {
            const char *error_response = 
                "HTTP/1.1 404 Not Found\r\n"
                "Content-Type: text/plain\r\n"
                "Content-Length: 13\r\n"
                "\r\n"
                "404 Not Found";
            send(client_socket, error_response, strlen(error_response), 0);
        }
    } else if (strncmp(buffer, "POST /", 6) == 0) {
        char *body = strstr(buffer, "\r\n\r\n");
        if (body) {
            body += 4;  // Пропускаем \r\n\r\n
            printf("Тело POST-запроса:\n%s\n", body);

            User user = {0}; // Инициализируем структуру пользователя
            user.is_admin = 0; // По умолчанию не админ

            // Уязвимость
            sscanf(body, "username=%s&password=%s", user.username, user.password);

            // чекалка
            printf("Имя пользователя: %s\n", user.username);
            printf("Пароль: %s\n", user.password);
            printf("is_admin: %d\n", user.is_admin);

            char response_body[BUFFER_SIZE];
            if (user.is_admin == 1) {
                snprintf(response_body, sizeof(response_body),
                         "<!DOCTYPE html>"
                         "<html lang=\"en\">"
                         "<head>"
                         "<meta charset=\"UTF-8\">"
                         "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
                         "<title>Admin Access</title>"
                         "<style>"
                         "body {"
                         "    font-family: Arial, sans-serif;"
                         "    text-align: center;"
                         "    margin-top: 50px;"
                         "    background-color: #f4f4f4;"
                         "}"
                         "h1 {"
                         "    color: #333;"
                         "}"
                         "</style>"
                         "</head>"
                         "<body>"
                         "<h1>Welcome, Admin!</h1>"
                         "<p>You have administrative privileges.</p>"
                         "<p>FLAG: pudge{h0w_oth3n_d0_y0u_pwn_s1t3s?}</p>"
                         "</body>"
                         "</html>");
            } else {
                snprintf(response_body, sizeof(response_body),
                         "<!DOCTYPE html>"
                         "<html lang=\"en\">"
                         "<head>"
                         "<meta charset=\"UTF-8\">"
                         "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
                         "<title>Welcome</title>"
                         "<style>"
                         "body {"
                         "    font-family: Arial, sans-serif;"
                         "    text-align: center;"
                         "    margin-top: 50px;"
                         "    background-color: #f4f4f4;"
                         "}"
                         "h1 {"
                         "    color: #333;"
                         "}"
                         "</style>"
                         "</head>"
                         "<body>"
                         "<h1>Welcome, %s!</h1>"
                         "<p>You have successfully logged in.</p>"
                         "</body>"
                         "</html>",
                         user.username);
            }

            char response[BUFFER_SIZE];
            snprintf(response, sizeof(response),
                     "HTTP/1.1 200 OK\r\n"
                     "Content-Type: text/html\r\n"
                     "Content-Length: %ld\r\n"
                     "\r\n%s",
                     strlen(response_body), response_body);

            send(client_socket, response, strlen(response), 0);
        } else {
            const char *error_response = 
                "HTTP/1.1 400 Bad Request\r\n"
                "Content-Type: text/plain\r\n"
                "Content-Length: 12\r\n"
                "\r\n"
                "Bad Request";
            send(client_socket, error_response, strlen(error_response), 0);
        }
    } else {
        const char *error_response = 
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 13\r\n"
            "\r\n"
            "404 Not Found";
        send(client_socket, error_response, strlen(error_response), 0);
    }

    close(client_socket);
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Ошибка создания сокета");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Ошибка привязки сокета");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Ошибка установки режима прослушивания");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Сервер запущен на порту %d...\n", PORT);

    while ((client_socket = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len)) >= 0) {
        handle_client(client_socket);
    }

    close(server_fd);
    return 0;
}