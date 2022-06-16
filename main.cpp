#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <fstream>

using namespace std;

//структура для хранения данных о автобусе
struct DataBus {
    int busNum{}; // номер автобуса
    string driverName; // имя водителя
    int routeNum{}; // номер маршрута
    // конструктор
    DataBus(int busNum, string driverName, int routeNum) {
        this->busNum = busNum;
        this->driverName = move(driverName);
        this->routeNum = routeNum;
    }

    // перегрузка оператора сравнения для удобства сравнивать разные объекты данной структуры
    bool operator==(const DataBus &dataBus) const {
        return (this->busNum == dataBus.busNum)
               && (this->routeNum == dataBus.routeNum)
               && (this->driverName == dataBus.driverName);
    }

    // просто пустой конструктор
    DataBus() = default;
};

// структура для хранения автобуса как элемента списка
struct Bus {
    DataBus data; // информация об автобусе
    struct Bus *next{}, *prev{}; // ссылки на следующий и предыдущий автобус соответственно

    Bus() = default;// пустой конструктор

    // конструктор на основе databus
    Bus(DataBus dataBus) {
        this->data = move(dataBus);
    }

    // функция для удобства вывода автобусов в консоль
    string toStr() const {
        return "Номер автобуса: " + to_string(this->data.busNum)
               + " ФИО водителя: " + this->data.driverName
               + " Номер маршрута: " + to_string(this->data.routeNum) + "\n";
    }
};

// сам двусвязный список
struct List {
    Bus *head, *tail; // хранит ссылки на начало и конец списка
public:
    List() : head(nullptr),
             tail(nullptr) {}; // начальный конструктор, ставит ссылки в nullptr - то бишь "отсутствие" автобусов

    void addToEnd(const Bus &bus) { // добавление в конец списка
        Bus *tmp = new Bus; // выделение места в памяти для нового элемента
        tmp->next = tmp->prev = nullptr; // дальше переопределение связующих указателей
        tmp->data = bus.data;
        if (head != nullptr) {
            tmp->prev = tail;
            tail->next = tmp;
            tail = tmp;
        } else {
            tmp->prev = nullptr;
            head = tail = tmp;
        }
    };

    void addToTop(const Bus &bus) { // добавление в начало списка
        Bus *tmp = new Bus;
        tmp->next = tmp->prev = nullptr;
        tmp->data = bus.data;
        if (head != nullptr) {
            tmp->next = head;
            head->prev = tmp;
            head = tmp;
        } else {
            tmp->next = nullptr;
            head = tail = tmp;
        }
    };

    void addAfterBus(const DataBus &dataBus, const DataBus &ins) { // добавление в список после определённого автобуса
        for (Bus *i = head; i != nullptr; i = i->next) {
            if (i->data == dataBus) {
                if (tail == i) {
                    this->addToEnd(Bus(ins));
                } else {
                    Bus *tmp = new Bus;
                    tmp->data = ins;
                    tmp->prev = i;
                    tmp->next = i->next;
                    i->next->prev = tmp;
                    i->next = tmp;
                }
                break;
            }
        }
    }

    void addBeforeBus(const DataBus &dataBus, const DataBus &ins) { // добавление автобуса перед автобусом
        for (Bus *i = head; i != nullptr; i = i->next) {
            if (i->data == dataBus) {
                if (head == i) {
                    this->addToTop(Bus(ins));
                } else {
                    Bus *tmp = new Bus;
                    tmp->data = ins;
                    tmp->prev = i->prev;
                    tmp->next = i;
                    i->prev->next = tmp;
                    i->prev = tmp;
                }
                break;
            }
        }
    }

    DataBus removeBus(int number) { // удаление автобуса по номеру. возвращает информацию об удалённом автобусе
        DataBus data = DataBus(0, "NANI", 0);
        for (Bus *i = head; i != nullptr; i = i->next) {
            if (i->data.busNum == number) {
                data = i->data;
                if (i->next != nullptr) i->next->prev = i->prev;
                if (i->prev != nullptr) i->prev->next = i->next;
                if (i == head) head = i->next;
                if (i == tail) tail = i->prev;
                delete (i);
                break;
            }
        }
        return data;
    }

    void show() const { // выводим все автобусы в консоль
        for (Bus *i = head; i != nullptr; i = i->next) {
            cout << i->toStr();
        }
    };

    string outPrint() const { // преобразуем автобусы в формат для сохранения в файле
        string outData;
        for (Bus *i = head; i != nullptr; i = i->next) {
            outData.append(
                    to_string(i->data.busNum) + "\n" +
                    i->data.driverName + "\n" +
                    to_string(i->data.routeNum) + "\n");
        }
        return outData;
    }

    bool checkIn(const DataBus& dataBus) const {
        for(Bus *i = head; i != nullptr; i = i->next) {
            if(i->data == dataBus) return true;
        }
        return false;
    }
};

bool menu(); // прототип функции меню

void loadTxt();// прототип функции загрузки из текстового файла

List park = List(); // создаём экземпляр списка, ответственного за парк
List route = List(); // // создаём экземпляр списка, ответственного за маршрут

//baseBuses

int main() {
    setlocale(LC_ALL, "Russian");
    cout << "Добро пожаловать!\n";
    loadTxt(); // подгружаем из txt файла (см. в другой функции)
    while (true) {
        string nut;
        cout << "\n-------Нажмите Enter для продолжения----------\n";
        getline(cin, nut);
        getline(cin, nut);
        if (menu()) break; // меню обрабатывается пока мы не выйдем из него (см. в другой функции)

    }
    cout
            << "Сохранение изменённых данных в файл \"baseBuses.txt\"...\n"; // после выхода из меню сохраняем данные в файл
    ofstream out("baseBuses.txt");
    out.clear();
    out << "Base of Buses\n";// сохранение идёт в формате, который мы приняли. нельзя менять
    out << park.outPrint();
    out << "END PARK\n";
    out << route.outPrint();
    out << "END ROUTE\n";
    out.close();
    return 0;
}

bool menu() {
    cout << "Меню:\n"
            "0 - выйти из меню и сохранить файл\n"
            "31/32 - добавить автобус в начало списка в парк/маршрут\n"
            "41/42 - добавить автобус в конец списка в парк/маршрут\n"
            "51/52 - добавить автобус после другого в парк/маршрут\n"
            "61/62 - добавить автобус перед другим в парк/маршрут\n"
            "71/72 - Вывести автобусы в парке/маршруте\n"
            "8 - Выезд автобуса на маршрут из парка\n"
            "9 - Вьезд автобуса в парк с маршрута\n";
    int point;
    cin >> point;
    if (point == 0)
        return true; // Обработка команд через меню. просто пользуемся созданными ранее функциями в структуре List
    if (point == 71) park.show();
    else if (point == 72) route.show();
    else if (point == 31 || point == 32 || point == 41 || point == 42) {
        cout << "Введите данные об автобусе: номер(число), "
                "ФИО водителя(строка из трёх слов через пробел) и номер маршрута(число) через Enter.\n";

        //Инициализация информации об автобусе
        int busNumber, routeNumber;
        string name1, name2, name3;
        cin >> busNumber;
        cin >> name1 >> name2 >> name3;
        cin >> routeNumber;
        string name = name1 + " " + name2 + " " + name3;
        DataBus dataBus = DataBus(busNumber, name, routeNumber);

        //Проверяем, что автобус не был добавлен ранее
        if(park.checkIn(dataBus) || route.checkIn(dataBus)) {
            cout << "Данный автобус уже добавлен в парк или на маршрут!\n";
            return false;
        }
        if (point == 31) {
            park.addToTop(Bus(dataBus));
        } else if (point == 32) {
            route.addToTop(Bus(dataBus));
        } else if (point == 41) {
            park.addToEnd(Bus(dataBus));
        } else if (point == 42) {
            route.addToEnd(Bus(dataBus));
        }
    } else if (point == 51 || point == 52 || point == 61 || point == 62) {
        cout << "Введите данные об автобусе, от которого отталкиваемся: "
                "номер(число), ФИО водителя(строка из трёх слов через пробел) и номер маршрута(число) через Enter.\n";

        //Инициализация информации об автобусе 1
        int busNumber, routeNumber;
        string name1, name2, name3;
        cin >> busNumber;
        cin >> name1 >> name2 >> name3;
        cin >> routeNumber;
        string name = name1 + " " + name2 + " " + name3;
        DataBus tmp1 = DataBus(busNumber, name, routeNumber);
        //Проверяем, что такой автобус есть в парке или на маршруте в зависимости от того, что мы хотим сделать
        if((point == 51 || point == 61) && !park.checkIn(tmp1)) {
            cout << "Автобуса с такими данными нет в парке!\n";
            return false;
        }
        if((point == 52 || point == 62) && !route.checkIn(tmp1)) {
            cout << "Автобуса с такими данными нет на маршруте!\n";
            return false;
        }
        cout << "Введите данные об автобусе, который ставим: "
                "номер(число), ФИО водителя(строка из трёх слов через пробел) и номер маршрута(число) через Enter.\n";

        //Инициализация информации об автобусе 2
        cin >> busNumber;
        cin >> name1 >> name2 >> name3;
        cin >> routeNumber;
        name = name1 + " " + name2 + " " + name3;
        DataBus tmp2 = DataBus(busNumber, name, routeNumber);

        //Проверяем, что автобус не был добавлен ранее
        if(park.checkIn(tmp2) || route.checkIn(tmp2)) {
            cout << "Данный автобус уже добавлен в парк или на маршрут!\n";
            return false;
        }

        if (point == 51) park.addAfterBus(tmp1, tmp2);
        else if (point == 52) route.addAfterBus(tmp1, tmp2);
        else if (point == 61) park.addBeforeBus(tmp1, tmp2);
        else route.addBeforeBus(tmp1, tmp2);
    } else if (point == 8) {
        cout << "Введите номер выездающего автобуса.\n";
        int num;
        cin >> num;
        DataBus tmp = park.removeBus(num);
        if(tmp.driverName == "NANI" && tmp.busNum == 0 && tmp.routeNum == 0) {
            cout << "Данный автобус не найден.";
            return false;
        }
        route.addToEnd(tmp);
    } else if (point == 9) {
        cout << "Введите номер въезжающего автобуса.\n";
        int num;
        cin >> num;
        DataBus tmp = route.removeBus(num);
        if(tmp.driverName == "NANI" && tmp.busNum == 0 && tmp.routeNum == 0) {
            cout << "Данный автобус не найден.";
            return false;
        }
        park.addToEnd(tmp);
    } else cout << "Некорректный вызов.\n";
    return false;
}

void loadTxt() {
    string tmp;
    while (true) {
        cout << "Вы хотите загрузить данные из текстового документа? [Y/n] "; // просим ввести y или n - yes или no
        cin >> tmp;
        if (_tolower(tmp[0]) == 'y') {
            cout
                    << "Загрузите в директорию с этим исполняющим файлом текстовый документ и введите его название без расширения.\n";
            cin >> tmp;
            ifstream in(tmp + ".txt");
            getline(in, tmp);
            if (tmp != "Base of Buses") {
                cout << "Некорректный файл или файл не найден. Повторите попытку.\n\n";
                continue;
            } else {
                cout << "Загрузка данных...\n";
            }
            string busNum;
            string driverName;
            string routeNum;
            while (true) { // подгружаем из файла в прогу автобусы из парка
                getline(in, busNum);
                if (busNum == "END PARK") break;
                getline(in, driverName); //приложения для открытия doc файлов linux
                getline(in, routeNum);
                park.addToEnd(Bus(DataBus(stoi(busNum), driverName, stoi(routeNum))));
            }
            while (true) { // подгружаем из файла в прогу автобусы на маршруте
                getline(in, busNum);
                if (busNum == "END ROUTE") break;
                getline(in, driverName);
                getline(in, routeNum);
                route.addToEnd(Bus(DataBus(stoi(busNum), driverName, stoi(routeNum))));
            }
            in.close();
            cout << "Загрузка завершена!\n";
            return;
        } else if (_tolower(tmp[0]) == 'n') {
            return;
        } else {
            cout << "Неккоректный ввод. Повторите попытку!\n\n";
        }
    }
}


