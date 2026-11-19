Д/з №2 по C++:

1) Сборка и запуск:

g++ -std=c++17 -Wall -Wextra -o fleet main.cpp
./fleet

Весь код лежит в hpp, поэтому компилируется только `main.cpp`.
Программа читает команды построчно из стандартного ввода до `quit` или конца ввода. 
Тест можно подать из файла:`./fleet < test.txt`.

2) Файлы:

Файл + Содержимое:
Inventory.hpp: абстрактные базисы InventoryItem и EnergyConsumer, operator<<, operator<;
Transport.hpp: Transport и транспортные средства: Bike, Scooter, Ebike;
Things.hpp: Thing и вещи: Helmet, Dock, Robot, Lock;
Errors.hpp: иерархия исключений FleetError;
1ServiceCenter.hpp: техосмотр, Registry.hpp, шаблон Registry<T, Cap>;
Fleet.hpp, парк: хранение и витрина; 
main.cpp: разбор команд и печать ошибок. 

3) Команды:

Команда + Смысл:
add scooter id name energy score: электросамокат( через техосмотр);  
add ebike id name energy score: электровелосипед (через техосмотр); 
add bike id name score: велосипед (через техосмотр); 
add helmet id name: шлем;  
add dock id name energy: док-станция;  
add robot id name energy capacity: робот-курьер (через техосмотр, это креативная часть);
add lock id name energy targetId: умный замок на транспорт targetId (это креативная часть); 
energy: суммарная суточная энергия парка;  
novice: техника для новичков c простотой >= 6; 
list: все позиции, отсортированные по инвентарному номеру;  
focus id: поставить позицию на витрину;  
show_focus: показ витрины или вызов expired, если позицию удалили; 
remove id: убрать позицию из парка;  
info id: полное описание позиции (describe(), это для креативной части);  
rejected: журнал отклонённых техосмотром устройств (это для креативной части);
quit: выход  

4) Техосмотр:
Если в имени есть подстрока "broken" без учёта регистра,то устройство отклоняется. 
Через техосмотр проходят транспорт и робот-курьер. 
Шлем, док-станция и замок добавляются без него.

5) Схема наследования:

         InventoryItem (абстрактный)          EnergyConsumer (абстрактный)
          /                 \                          
   Transport (абстр.)   Thing (абстр.)                 
    /            \       /             \               
 Bike  Scooter  Ebike Helmet Dock Robot Lock           
                                                  
          |───────|───────────|────|─────|────────────|
                   второй базис - EnergyConsumer


5.1) Множественное наследование: Scooter, Ebike (public Transport, public EnergyConsumer), Dock, Robot, Lock(public Thing, public EnergyConsumer).
5.2) Ромбовидного наследования нет, так как InventoryItem и EnergyConsumer не связаны общим предком, поэтому virtual public не нужен.
5.3) EnergyConsumer вынесен в отдельный базис, потому что энергию потребляют и часть транспорта, и часть вещей.
5.4) Нельзя создать объект InventoryItem, EnergyConsumer, Transport или Thing: в них остаются чисто виртуальные методы.

6) Динамическое связывание:

Парк хранит все позиции как указатели на InventoryItem, а команды работают только через виртуальные методы базиса:

6.1) list, novice, show_focus вызывают inventoryId() и name() через operator<<;
6.2) info вызывает describe();
6.3) novice вызывает isNoviceFriendly.По умолчанию метод возвращает false, в Transport сравнивает простоту с порогом;
6.4) energy вызывает asEnergyConsumer().По умолчанию метод возвращает nullptr, энергопотребители возвращают this, после чего вызывается dailyEnergy();
6.5) add lock проверяет canBeLocked(). По умолчанию false, в Transport true.

NB: dynamic_cast в проекте не используется.Каждый вопрос к объекту задаётся через виртуальный метод с ответом по умолчанию в базисе.

7) Порог новичка хранится в Fleet::noviceThreshold_ (по умолчанию 6) и передаётся в isNoviceFriendly параметром. Правило можно поменять в одном месте, не трогая классы техники.

8) Перегрузка операторов (Inventory.hpp, Registry.hpp):

8.1) operator<<(std::ostream&, const InventoryItem&) печатает id name.Через него работают list, novice, show_focus.
8.2) operator<(const InventoryItem&, const InventoryItem&) сравнивает по inventoryId(). Его использует std::sort в Fleet::printList (через функцию lessById, которая сравнивает
объекты, адреса указателей). Сравнение строковое, поэтому "S-10" идёт раньше "S-2".
8.3) operator[] и operator<< у шаблона Registry.

9) Smart-указатели: 

9.1) unique_ptr<InventoryItem>: main.cpp(make_unique) -> параметр Fleet::add, addWithInspection.
Новая единица техники до техосмотра принадлежит одной команде add.
Если техосмотр или проверка бросает исключение, unique_ptr сам удаляет объект при выходе из функции,то и парк не меняется. 
При успехе владение передаётся в парк через move.  

9.2) shared_ptr<InventoryItem>: Fleet::items_ (vector<shared_ptr<InventoryItem>>)   
Парк хранит принятые единицы, а shared_ptr нужен, чтобы на позицию можно было повесить weak_ptr: у unique_ptr нет блока управления. 

9.3) weak_ptr<InventoryItem>: Fleet::focus_(витрина)
Витрина указывает на позицию, но не владеет ею. После remove объект удаляется, lock() возвращает пустой указатель, и show_focus печатает expired.  
Дополнительно по креативной части: weak_ptr<InventoryItem>: Lock::target_. Замок указывает на транспорт, но не продлевает ему жизнь. 
После remove транспорта info замка печатает(target removed).  

10) Что не используется:

10.1) new и delete в проекте не используется;
10.2) ModelSpec из исходного ТЗ не реализован по уточнениям из чата.

11) Исключения:

FleetError наследуется от std::runtime_error (а через него от std::exception), чтобы не хранить текст сообщения вручную.

Тут вид + где используется + для чего:

InspectionRejected: ServiceCenter::inspect в имени есть broken  
InvalidArgument: конструкторы Transport, Thing, энергопотребителей, пустой id, простота вне 1..10, энергия < 0, вместимость робота ≤ 0, замок без цели;
InvalidArgument: Fleet, повторный id, нет позиции с таким id (remove, focus, info);  
InvalidArgument: main.cpp, неизвестная команда или вид техники, не хватает аргумента, не число, замок на не транспорт;  
InvalidArgument: Registry, журнал переполнен, индекс вне диапазона.

Печать error: <what()> есть только в catch (const FleetError&) в main. Доменный код только бросает исключения.

12) Состояние:

12.1) плохие данные отсекаются в конструкторе, и объект с ними не создаётся;
12.2) в Fleet::add сначала проверки (техосмотр, дубликат id), и только потом push_back;
12.3) отклонённый объект удаляет unique_ptr.

13) Шаблон:

template<typename T, int Cap> class Registry - журнал фиксированной вместимости на массиве T items_[Cap] с методами add, size, operator[] и operator<<.
Где юзаем: Fleet::rejected_ имеет тип Registry<std::string, 100>.Это журнал устройств, которые отклонил техосмотр. Он пополняется в Fleet::addWithInspection и печатается командой rejected.
Явная специализация: шаблонная функция printEntry<T> печатает запись через пробел, а специализация printEntry<std::string> печатает каждую запись с новой строки, потому что записи журнала сами содержат пробелы.
Без шаблона для журналов разных типов пришлось бы копировать один и тот же класс.

14) Креативная часть:

14.1) Робот-курьер: Robot (Thing + EnergyConsumer), поле «вместимость, кг». Он ездит, но в иерархии это вещь: его не выдают в прокат, поэтому в novice он не попадает. При этом через техосмотр он проходит. Решение «проверять или нет» принимает команда в main, а не класс.
14.2) Умный замок Lock (Thing + EnergyConsumer). Вешается на транспорт и хранит на него weak_ptr, второе применение слабой ссылки. Повесить замок на вещь нельзя (проверка canBeLocked()).
14.3) Команда info id печатает describe() любой позиции. Это прямая демонстрация динамического связывания.
14.4) Команда rejected печатает журнал отказов техосмотра, то есть использует шаблон Registry.


Пример:

Ввод:
```
add scooter S-1 ModelX 2.5 8
add bike B-1 City 4
add scooter S-2 brokenX 1.0 9
add helmet H-1 Pro
add dock D-1 Hub 10.0
list
novice
energy
quit
```
Вывод:
```
error: inspection rejected
B-1 City
D-1 Hub
H-1 Pro
S-1 ModelX
S-1 ModelX
12.5
```

Креативная часть и витрина:
Ввод:
```
add scooter S-1 ModelX 2.5 8
add robot R-1 Rover 3.0 20
add robot R-2 brokenBot 1.0 10       error: inspection rejected
add lock L-1 Smart 0.1 S-1
focus S-1
info L-1                            → lock L-1 Smart on S-1
energy                              → 5.6
remove S-1
info L-1                            → lock L-1 Smart (target removed)
show_focus                          → expired
energy                              → 3.1
rejected                            → R-2 brokenBot
```
Вывод:
```
error: inspection rejected
lock L-1 Smart on S-1
5.6
lock L-1 Smart (target removed)
expired
3.1
R-2 brokenBot
```

15) Использование ИИ

Использовал
1) Google Ai для:
объяснения теории по темам и запросам: виртуальные функции и деструкторы, const, роли unique_ptr/shared_ptr/weak_ptr, наследование, dynamic_cast + полиморфизм, git для загрузки на GitHub;
2) Claude:
Для составления промежуточных тестов и конечного ревью моего кода.
3) ccp.reference:
для теории и примеров использования шаблонов, умных указателей и is/osstream.

