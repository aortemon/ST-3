# ST-3 UNIT-тестирование с использованием Googletest (C++) (3)


![GitHub pull requests](https://img.shields.io/github/issues-pr/UNN-CS/ST-3)
![GitHub closed pull requests](https://img.shields.io/github/issues-pr-closed/UNN-CS/ST-3)

Срок выполнения задания:

**по 30.03.2025** ![Relative date](https://img.shields.io/date/1743368400)


## Задание

Имеется холодильник с "умной" дверью, которая снабжена таймером и устройством, издающим сигнал через определенное время после открытия. 

В классе `include/TimedDoor.h` даны интерфейсы для основных классов системы:

```c++
class TimerClient {
 public:
  virtual void Timeout() = 0;
};

class Door {
 public:
  virtual void lock() = 0;
  virtual void unlock() = 0;
  virtual bool isDoorOpened() = 0;
};
```

Так же приводятся сами классы-наследники, реализующие интерфейсы:

```c++
class DoorTimerAdapter : public TimerClient {
 private:
  TimedDoor& door;
 public:
  explicit DoorTimerAdapter(TimedDoor&);
  void Timeout();
};

class TimedDoor : public Door {
 private:
  DoorTimerAdapter * adapter;
  int iTimeout;
  bool isOpened;
 public:
  explicit TimedDoor(int);
  bool isDoorOpened();
  void unlock();
  void lock();
  int  getTimeOut();
  void throwState();
};

class Timer {
  TimerClient *client;
  void sleep(int);
 public:
  void tregister(int, TimerClient*);
};
``` 

**Реализация**

Необходимо разработать простейшую реализацию методов классов `DoorTimerAdapter`, `TimedDoor` и `Timer` для поддержки простого алгоритма работы:

- создание объекта двери с адаптером
- перевод двери в состояние "закрыто"
- открытие двери и активизация метода `Timeout()` адаптера, который активизирует таймер на заданное время и по истечение этого времени, если состояние двери не изменилось, выбрасывает исключение

Исходный код должен располагаться в `src/TimedDoor.cpp`


**Тестирование**

Разработать тесты проверки работы двери, адаптера и таймера с использованием:

- **SetUp** и **TearDown()** для настройки объектов
- **MOCK_METHOD** - для тестирования интерфейсов (проверки факта вызова методов внутри других)

Кроме того, необходимо протестировать факт выброса/не выброса указанного исключения в ситуации с открытой/закрытой дверью, по истечении определенного времени.

Минимальное количество тестов - 10.

*Замечание*

1. Расширять набор классов для тестирования интерфейсов нужно внутри файла **tests.cpp**.
2. Возможно, потребуются небольшие правки конфигураций **txt** и **yml** для успешного построения проекта и запуска тестов.


**Состав проекта**

- **include/TimedDoor.h** - описание классов задачи
- **src/TimedDoor.cpp** - реализация методов классов
- **src/main.cpp** - пример использования системы
- **test/tests.cpp** - модульные тесты 




 


