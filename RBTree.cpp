#include <iostream>      // Для ввода и вывода данных
#include <fstream>       // Для работы с файлами
#include <vector>        // Для использования вектора
#include <chrono>        // Для измерения времени выполнения операций
#include <iomanip>       // Для форматирования вывода
#include <string>        // Для работы со строками

// Структура узла красно-черного дерева
struct Node {
    int key;        // Значение ключа
    bool isRed;     // Цвет узла: true - красный, false - черный
    Node* left;     // Указатель на левый дочерний узел
    Node* right;    // Указатель на правый дочерний узел

    // Конструктор узла, по умолчанию новый узел является красным
    Node(int k) : key(k), isRed(true), left(nullptr), right(nullptr) {}
};

// Функция для проверки, является ли узел красным
bool isRed(Node* x) {
    return x != nullptr && x->isRed;
}

// Функция для выполнения левого поворота вокруг узла h
Node* rotateLeft(Node* h) {
    Node* x = h->right;     // Узел x становится новым корнем поддерева
    h->right = x->left;     // Левое поддерево x становится правым поддеревом h
    x->left = h;            // h становится левым дочерним узлом x
    x->isRed = h->isRed;    // x получает цвет h
    h->isRed = true;        // h становится красным
    return x;               // Возвращаем новый корень поддерева
}

// Функция для выполнения правого поворота вокруг узла h
Node* rotateRight(Node* h) {
    Node* x = h->left;      // Узел x становится новым корнем поддерева
    h->left = x->right;     // Правое поддерево x становится левым поддеревом h
    x->right = h;           // h становится правым дочерним узлом x
    x->isRed = h->isRed;    // x получает цвет h
    h->isRed = true;        // h становится красным
    return x;               // Возвращаем новый корень поддерева
}

// Функция для изменения цветов узла и его дочерних узлов
void flipColors(Node* h) {
    h->isRed = !h->isRed;               // Инвертируем цвет h
    if (h->left) h->left->isRed = !h->left->isRed;   // Инвертируем цвет левого дочернего узла
    if (h->right) h->right->isRed = !h->right->isRed; // Инвертируем цвет правого дочернего узла
}

// Функция вставки узла с ключом key в красно-черное дерево
Node* insert(Node* h, int key) {
    if (h == nullptr) return new Node(key); // Если дерево пустое, создаем новый красный узел

    if (key < h->key) {
        h->left = insert(h->left, key);     // Рекурсивно вставляем в левое поддерево
    } else if (key > h->key) {
        h->right = insert(h->right, key);   // Рекурсивно вставляем в правое поддерево
    } else {
        // Ключ уже существует, ничего не делаем
    }

    // Исправление возможных нарушений красно-черного дерева
    if (isRed(h->right) && !isRed(h->left))
        h = rotateLeft(h);                  // Левый поворот, если правый дочерний узел красный, а левый черный
    if (isRed(h->left) && isRed(h->left->left))
        h = rotateRight(h);                 // Правый поворот, если левый дочерний и его левый дочерний узлы красные
    if (isRed(h->left) && isRed(h->right))
        flipColors(h);                      // Смена цветов, если оба дочерних узла красные

    return h;                               // Возвращаем корень поддерева
}

// Функция для перемещения красного узла влево
Node* moveRedLeft(Node* h) {
    flipColors(h); // Инвертируем цвета h и его дочерних узлов
    if (isRed(h->right->left)) { // Если левый дочерний узел правого поддерева красный
        h->right = rotateRight(h->right); // Правый поворот на правом поддереве
        h = rotateLeft(h); // Левый поворот на текущем узле
        flipColors(h); // Инвертируем цвета вновь полученного узла
    }
    return h; // Возвращаем обновленный узел
}

// Функция для перемещения красного узла вправо
Node* moveRedRight(Node* h) {
    flipColors(h); // Инвертируем цвета h и его дочерних узлов
    if (isRed(h->left->left)) { // Если левый дочерний узел левого поддерева красный
        h = rotateRight(h); // Правый поворот на текущем узле
        flipColors(h); // Инвертируем цвета вновь полученного узла
    }
    return h; // Возвращаем обновленный узел
}

// Функция для исправления структуры после удаления
Node* fixUp(Node* h) {
    if (isRed(h->right))
        h = rotateLeft(h); // Левый поворот, если правый дочерний узел красный
    if (isRed(h->left) && isRed(h->left->left))
        h = rotateRight(h); // Правый поворот, если левый дочерний и его левый дочерний узлы красные
    if (isRed(h->left) && isRed(h->right))
        flipColors(h); // Смена цветов, если оба дочерних узла красные

    return h; // Возвращаем обновленный узел
}

// Функция для поиска минимального узла в дереве
Node* min(Node* h) {
    while (h->left != nullptr)
        h = h->left; // Идем в самое левое поддерево
    return h; // Возвращаем минимальный узел
}

// Функция для удаления минимального узла
Node* removeMin(Node* h) {
    if (h->left == nullptr) {
        delete h; // Удаляем узел, если нет левого дочернего
        return nullptr; // Возвращаем пустое поддерево
    }
    if (!isRed(h->left) && !isRed(h->left->left))
        h = moveRedLeft(h); // Перемещаем красный узел влево для балансировки

    h->left = removeMin(h->left); // Рекурсивно удаляем минимальный узел
    return fixUp(h); // Исправляем структуру дерева после удаления
}

// Функция удаления узла с ключом key из красно-черного дерева
Node* remove(Node* h, int key) {
    if (key < h->key) { // Если ключ меньше текущего узла
        if (h->left == nullptr) {
            // Ключ не найден, возвращаем текущий узел
            return h;
        }
        if (!isRed(h->left) && !isRed(h->left->left))
            h = moveRedLeft(h); // Перемещаем красный узел влево для балансировки
        h->left = remove(h->left, key); // Рекурсивно удаляем ключ из левого поддерева
    } else {
        if (isRed(h->left))
            h = rotateRight(h); // Правый поворот, если левый дочерний узел красный
        if (key == h->key && (h->right == nullptr)) {
            delete h; // Удаляем узел, если он является искомым и нет правого поддерева
            return nullptr; // Возвращаем пустое поддерево
        }
        if (h->right == nullptr) {
            // Ключ не найден, возвращаем текущий узел
            return h;
        }
        if (!isRed(h->right) && !isRed(h->right->left))
            h = moveRedRight(h); // Перемещаем красный узел вправо для балансировки
        if (key == h->key) {
            Node* x = min(h->right); // Находим минимальный узел в правом поддереве
            h->key = x->key; // Заменяем ключ текущего узла ключом найденного минимального узла
            h->right = removeMin(h->right); // Удаляем минимальный узел из правого поддерева
        } else {
            h->right = remove(h->right, key); // Рекурсивно удаляем ключ из правого поддерева
        }
    }
    return fixUp(h); // Исправляем структуру дерева после удаления
}

// Функция поиска узла с ключом key в дереве
Node* find(Node* p, int key) {
    while (p != nullptr) { // Пока не достигли конца дерева
        if (key < p->key)
            p = p->left; // Идем в левое поддерево
        else if (key > p->key)
            p = p->right; // Идем в правое поддерево
        else
            return p; // Ключ найден, возвращаем узел
    }
    return nullptr; // Ключ не найден
}

// Функция для получения k наименьших элементов в дереве (инфиксный обход)
void getMinElements(Node* p, std::vector<int>& result, int k) {
    if (!p || result.size() >= k) return; // Если узел пустой или уже собрали k элементов, прекращаем
    getMinElements(p->left, result, k);   // Рекурсивно собираем элементы из левого поддерева
    if (result.size() < k)
        result.push_back(p->key);         // Добавляем текущий ключ, если еще не достигли k
    getMinElements(p->right, result, k);  // Рекурсивно собираем элементы из правого поддерева
}

// Функция для графического вывода дерева в консоль с ограничением глубины
void printTree(const Node* node, const std::string& prefix = "", bool isLeft = true, int currentDepth = 0, int maxDepth = -1) {
    if (!node) return; // Если узел пустой, выходим

    // Если достигли максимальной глубины и maxDepth не равен -1, выводим узел и прекращаем рекурсию
    if (maxDepth != -1 && currentDepth > maxDepth) {
        std::cout << prefix;
        std::cout << (isLeft ? "├── " : "└── ");
        std::cout << node->key << " ..." << std::endl; // "..." указывает на продолжение поддерева
        return;
    }

    // Выводим текущий узел с префиксом и символом для визуального представления
    std::cout << prefix;
    std::cout << (isLeft ? "├── " : "└── ");
    // Выводим цвет узла: (R) для красного, (B) для черного
    std::cout << node->key << (node->isRed ? " (R)" : " (B)") << std::endl;

    // Рекурсивно выводим левое и правое поддеревья с увеличением текущей глубины
    printTree(node->left, prefix + (isLeft ? "│   " : "    "), true, currentDepth + 1, maxDepth);
    printTree(node->right, prefix + (isLeft ? "│   " : "    "), false, currentDepth + 1, maxDepth);
}

// Функция для удаления всего дерева и освобождения памяти
void deleteTree(Node* node) {
    if (node) { // Если узел не пустой
        deleteTree(node->left);   // Рекурсивно удаляем левое поддерево
        deleteTree(node->right);  // Рекурсивно удаляем правое поддерево
        delete node;               // Удаляем текущий узел
    }
}

int main(int argc, char* argv[]) {
    // Проверка наличия аргумента командной строки (имя входного файла)
    if (argc < 2) {
        std::cerr << "Использование: " << argv[0] << " <имя_входного_файла>" << std::endl;
        return 1; // Возвращаем код ошибки
    }

    const char* filename = argv[1]; // Имя файла передается как первый аргумент
    std::ifstream file(filename, std::ios::binary); // Открываем файл в бинарном режиме
    if (!file) { // Проверяем, успешно ли открыт файл
        std::cerr << "Ошибка открытия файла: " << filename << std::endl;
        return 1; // Возвращаем код ошибки
    }

    Node* tree = nullptr; // Инициализируем корень дерева как пустой
    int value;            // Переменная для хранения считанного значения

    // Загрузка элементов из файла в дерево
    while (file.read(reinterpret_cast<char*>(&value), sizeof(value))) {
        tree = insert(tree, value); // Вставляем считанное значение в дерево
        tree->isRed = false;        // Обеспечиваем, что корень всегда черный
    }
    file.close(); // Закрываем файл после чтения

    // Основной цикл программы для выполнения операций над деревом
    while (true) {
        // Вывод меню выбора операции
        std::cout << "Выберите операцию:\n"
                  << "1. Поиск\n"
                  << "2. Вставка\n"
                  << "3. Удаление\n"
                  << "4. Получить 10 наименьших элементов\n"
                  << "5. Вывести дерево\n"
                  << "6. Выход\n"
                  << "Ваш выбор: ";
        int choice;
        std::cin >> choice; // Считываем выбор пользователя

        if (choice == 6) break; // Если выбрали 6, выходим из цикла

        auto start = std::chrono::high_resolution_clock::now(); // Запоминаем время начала операции

        if (choice == 1) { // Поиск элемента
            std::cout << "Введите элемент для поиска: ";
            int key;
            std::cin >> key; // Считываем ключ для поиска

            Node* result = find(tree, key); // Выполняем поиск
            auto end = std::chrono::high_resolution_clock::now(); // Запоминаем время окончания
            std::chrono::duration<double> duration = end - start; // Вычисляем длительность операции

            if (result) {
                std::cout << "Элемент найден в дереве." << std::endl;
            } else {
                std::cout << "Элемент не найден в дереве." << std::endl;
            }
            // Выводим время выполнения операции с шестью знаками после запятой
            std::cout << "Время операции: " << std::fixed << std::setprecision(6)
                      << duration.count() << " секунд" << std::endl;

        } else if (choice == 2) { // Вставка элемента
            std::cout << "Введите элемент для вставки: ";
            int key;
            std::cin >> key; // Считываем ключ для вставки

            tree = insert(tree, key); // Вставляем элемент в дерево
            tree->isRed = false;      // Обеспечиваем, что корень всегда черный
            auto end = std::chrono::high_resolution_clock::now(); // Запоминаем время окончания
            std::chrono::duration<double> duration = end - start; // Вычисляем длительность операции

            std::cout << "Элемент вставлен." << std::endl;
            // Выводим время выполнения операции
            std::cout << "Время операции: " << std::fixed << std::setprecision(6)
                      << duration.count() << " секунд" << std::endl;

        } else if (choice == 3) { // Удаление элемента
            std::cout << "Введите элемент для удаления: ";
            int key;
            std::cin >> key; // Считываем ключ для удаления

            if (tree != nullptr) { // Проверяем, что дерево не пустое
                tree = remove(tree, key); // Удаляем элемент из дерева
                if (tree != nullptr) tree->isRed = false; // Обеспечиваем, что корень всегда черный
            }
            auto end = std::chrono::high_resolution_clock::now(); // Запоминаем время окончания
            std::chrono::duration<double> duration = end - start; // Вычисляем длительность операции

            std::cout << "Элемент удален (если он существовал)." << std::endl;
            // Выводим время выполнения операции
            std::cout << "Время операции: " << std::fixed << std::setprecision(6)
                      << duration.count() << " секунд" << std::endl;

        } else if (choice == 4) { // Получение 10 наименьших элементов
            std::vector<int> min_elements; // Вектор для хранения результатов
            getMinElements(tree, min_elements, 10); // Получаем 10 наименьших элементов
            auto end = std::chrono::high_resolution_clock::now(); // Запоминаем время окончания
            std::chrono::duration<double> duration = end - start; // Вычисляем длительность операции

            std::cout << "10 наименьших элементов: ";
            for (int elem : min_elements) { // Выводим элементы
                std::cout << elem << " ";
            }
            std::cout << "\nВремя операции: " << std::fixed << std::setprecision(6)
                      << duration.count() << " секунд" << std::endl;

        } else if (choice == 5) { // Печать структуры дерева с ограничением глубины
            std::cout << "Введите максимальную глубину для вывода (-1 для без ограничения): ";
            int maxDepth;
            std::cin >> maxDepth; // Считываем максимальную глубину

            std::cout << "Структура дерева:\n";
            printTree(tree, "", true, 0, maxDepth); // Вызываем функцию для вывода дерева
        } else { // Обработка некорректного выбора
            std::cout << "Неверный выбор. Пожалуйста, попробуйте снова." << std::endl;
        }
    }

    // Освобождение памяти, занятой деревом
    deleteTree(tree);

    return 0; // Завершаем программу
}
