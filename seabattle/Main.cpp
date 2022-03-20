#include <iostream>
//#include <Windows.h>
#include <time.h>
#include <string>
#include "Console.h"

// Экзаменационное задание № 1 курса "Разработчик ПО"  ПВ113 Кулеев В.А.      2022-03-10 ---------------------------------------|
//                                                                                                                              |
// ---------------------------------------- П Р О Г Р А М М А    М О Р С К О Й    Б О Й ----------------------------------------|
// --------------------- Р А З Р А Б О Т А Л   В Л А Д И М И Р   А Л Е К С А Н Д Р О В И Ч   К У Л Е Е В -----------------------|
// --------------------- Учебное заведение  Академия ШАГ г. Новосибирск группа ПВ113  vakuleev@gmail.com -----------------------|
//   Данная работа выполнена на основе советов и консультации меня моими сыновьями Артемом и Арсением (морской бой - тактика)   |

using namespace MCons;
//Глобальные переменные параметров и координат консоли, карт, кораблей.
//Размер максимального корабля определяет размер плотности установки на карту флотилии !
//ВНИМАНИЕ! ИСТИННЫЕ ПАРАМЕТРЫ ОПРЕДЕЛЯЮСЯ В main() -----------------------------------------------------------------------------
bool gameUser = false;                              //Пользователь игры человек true компьютер false//                          |
double boatMin = 1.0, boatMax = 5.0;                //Размер минимального и максимального количества палуб кораблей//           |
int AllFieldsBoat = 0;        //Общее количество полей занятых кораблями в функции main будет расчет для инициализации счетчика |
                                                    //                                 \\|                                      |
  //Размер координат карты X = (от 0 до XMax-1),  Y = (от 0 до YMax-1)//                 |        ДИЗАЙН АРТЕМ КУЛЕЕВ           |
int YMax = (2.0 + boatMax * 0.02) * (boatMax - boatMin) + 2, XMax = 2*YMax;//           /##\==   ТАКТИКА АРСЕНИЙ КУЛЕЕВ         |
                                                    //                                ||||||||                                  |
int highMap = YMax, withMap = XMax + 1;             //                    --/#\   ///////////\\\\\\\\\\\    /#\==               |
                                                    //                    \\\\\\\\\\\\\\\\\\\//////////////////////             |
// Координаты карты Left, Right                     //                      \\\\\\\\\\\\\\\\\////////////////////               |
int leftRow = 8, rightRow = leftRow;                //                                     ==                                   |
int leftCol = 5, rightCol = leftCol + boatMax * 4.0 + 33; //107//                        //##\\==   /#\--           ###         |
                                                    //                             \\\\\\\\\\\///////////           # #         |
int iRowMax = 2 * leftRow + highMap;                //                               \\\\\\\\\/////////         #    #    #     |
int iColMax = 2 * rightCol;                         //                                                         ###   #   ###    |
int rowConsoleMax = 4.6 * iRowMax;                  //Размер срок консоли  vConsole mapCons;//                  ##  ###  ##     |
int colConsoleMax = 1.7 * iColMax;                  //                                                            #######       |
                                                    //                                                               #          |
int rowAzimut = 2;                                  //Вывод состояний координат при попадании в тупик rowAzimut++,              |
int colAzimut = rightCol + withMap;                 // + 20//                                         colAzimut                 |
                                                    //                                                                          |
int rowStatusSeaBatalia = iRowMax - 1;              //Строка результатов попаданий и уничтожений кораблей флотилий//            |
int rowListdamage       = iRowMax + 4;              //Строка листинга вывода попаданий и уничтожений кораблей флотилий//        |
int visListdamage       = 0;                        //                                                                          |
int visLostVectorOrient = 0;                        //                                                                          |
int idLostVectorOrient  = 0;                        //Счетчик тупиков//                                                         |
                                                    //                                                                          |
int rowgetCoordFromUSER = rowStatusSeaBatalia;      //                                                                          |
int colgetCoordFromUSER = rightCol;                 //                                                                          |
                                                    //                                                                          |
int animatePause = 5;                               //Для задержки при анимации попадания по кораблю или карте                  |
bool visibleL = true;                               //Видимость флота L                                                         |
bool visibleR = true;                               //Видимость флота R                                                         |
//ВНИМАНИЕ! ИСТИННЫЕ ПАРАМЕТРЫ ОПРЕДЕЛЯЮСЯ В main() -----------------------------------------------------------------------------

int cntNegativOrientBoat = 0, cntNegativSearchNextPartShip = 0; //ОТЛАДКА !

//Если игрок - компьютер то на него влияют параметры интеллекта:
//   0 - случайный выбора координат стрельбы
//   1 - случайный выбора координат стрельбы, но исключаются предыдущие пораженные поля и корабли
//   2 - дополнительно обнаружение и добивание корабля
//   3 - дополнительно ИНТЕЛЕКТ стрельбы с пропуском ближайших координат уничтоженного корабля
int onIntelligenceL = 3, onIntelligenceR = 2;

vConsole mapCons;

// Цвета состояния областей с кораблем и без
int cMapL                   = COLOR_GREEN;
int cMapR                   = COLOR_LIGHTGRAY;
int cSpace                  = COLOR_WHITE;
int cSpaceBackGround        = COLOR_BLACK;
int cSpaceDestroy           = COLOR_CYAN;
int cSpaceDestroyBackGround = COLOR_BLACK;
int cBoat                   = COLOR_BLUE;
int cBoatBackGround         = COLOR_WHITE;
int cBoatDestroy            = COLOR_LIGHTMAGENTA;
int cBoatDestroyBackGround  = COLOR_CYAN;

//Символы обозначения корабля и состояния полей
char space        = ' ';          //Пустая область char(32);
char spaceDestroy = 'o';          //Попадание в пустую область
char boatF        = '#';          //Корабль
char boatFDestroy = '*';          //Попадание в корабль

struct mField {
    int yMapCons = 0, xMapCons = 0;       //Координаты xMapCons, yMapCons начала карты
    int coordY   = 0, coordX   = 0;       //Координата головной части корабля относительно начала координат карты
    int yField   = 0, xField   = 0;       //Координата смещения относительно начала координат карты
    int boatOnField       = 0;    //0 - пусто   1, 2, 3, 4 - корабль (1 - однопалубный, 2 - двухпалубный, 3 - трёхпалубный, 4 - четырёхпалубный) 
//---------------------------------------------------- BOAT ----------------------------------------------------------------                         
    int idBoat      = 0;
    int countPalub  = 0;   // Количество палуб корабля
    bool orientBoat = true;// orientBoat = true - горизонтальное расположение корабля, false - вертикальное расположение корабля
    bool statusBoat = true;// true - корабль в строю,  false - корабль уничтожен огнем противника (поражены все палубы_)
//---------------------------------------------------- BOAT ----------------------------------------------------------------                         
    bool statusPalubaBoat = true; //false - палуба корабля уничтожена, true - палуба корабля в строю
    bool statusField      = true; //false - поле поражено снарядом,    true - поле не поражено снарядом

    bool isVisibleBoat    = false;//false - поле скрыто, true - поле открыто и видно статус (причина - попадание снаряда, принудительное включение для режима ЧИТЕР или отладки программы)
    char onField          = space;

    void insertPalubaBoat(int i, int j, int palubaBoat, bool visibleBoat) {
        if (boatOnField == 0) { // && palubaBoat >= boatMin && palubaBoat <= boatMax)
            yField = i; xField = j;
            boatOnField   = palubaBoat;
            statusField   = true;
            isVisibleBoat = visibleBoat;
            onField       = boatF;
        }
    }

    //Функция определения статуса палубы корабля
    bool statusBoatOnField() {
        return (boatOnField > 0 && statusPalubaBoat);
    }
};

//Функция установки основных параметров игры
template <typename T>
void setParamGame(int row, int col, int colSetParametr, T& setParametr, int minParametr, int maxParametr, std::string setInfo);

//Функция определения статуса поля true - не поражено снарядом  false - поражено снарядом
bool statusFieldNoDestroy(mField** mapFields, int i, int j);

//Функция рачета количества палуб кораблей флотилии
int countAllFieldsBoat();

//Функция вывода результатов сражения флотов (вывод в со строки rowStatusSeaBatalia -3..+3 + highMap + AllFieldsBoat  - глобальные переменные )
void statusSeaBatalia(int i, int j, int col, int countFire, int countDamage, bool targetDamaged, int onIntelligence);

//Функция определения новых координат i, j  по обнаруженным координатам попадания iDiscovered, jDiscovered 
//                                          (ВАЖНО: maxCoordI, maxCoordJ - максимальное значение индексов !)
void searchPartShip(int& i, int& j, int iFirstDiscovered, int jFirstDiscovered, int iDiscovered, int jDiscovered, int& orientBoat, mField** pMapFields, int maxCoordI, int maxCoordJ, bool& searchNextPartShip);

//Функция проверки корректного внесенния координат нанесения удара игроком
bool validateCoord(int& i, int& j, int maxCoordI, int maxCoordJ, std::string getCoord);

//Функция внесения координат удара игроком
void getCoordFromUSER(int& i, int& j, int maxCoordI, int maxCoordJ, int col);

//Поиск координат OnIntelligence == 1, 2 путем простой проверки статуса поля - не разрушено выстрелом !
//                OnIntelligence == 3 на основе разведданных координат уничтоженных кораблей  
bool searchTargetOnIntelligence(int i, int j, mField** pMapFields, int maxCoordI, int maxCoordJ, int onIntelligence);

//Основная функция "атака карты флота кораблей" по карте противника расчет координат и нанесения ударов с последующим ходом
// - если промах то выход и запоминание координат недобитого корабля для последующего хода после противника
void shipsFlotMapAttack(int& iFirstDiscovered, int& jFirstDiscovered, int& iDiscovered, int& jDiscovered, int& orientBoat, mField** pMapFields, int row, int col, int& iFire, int& iCountDamage, bool& searchNextPartShip, bool gameUser, int onIntelligence);

//Функция сражения флотов путем вызова поочерёдно функции attackBattleField с проверкой на нанесенные поражения
// и вывода результатов сражения akjnjd
void viewSeaBatalia(mField** pMapFieldsL, mField** pMapFieldsR, int& rowListDamage, int lCol, int rCol);

//Функция проверки координат головной части корабля для размещения его на карте
//проверяется область занимаемая предполагаемым местоположением корабля и область векторов вокруг на наличие занятых координат
bool newCoordInsertBoat(int i, int j, int yMax, int xMax, mField** pMapFields, int sizeBoat, bool orient);

//Функция анимирования попадания в карту или корабль
void viewAnimateDamageFireField(mField** mapFields, int i, int j, int colorStatus, int colorStatusBackGround);

//Функция отображения поля с координатами i, j из массива карты mapFields
void viewFieldMap(mField** mapFields, int i, int j);

//Выстрел по полю возвращает true если попадание в действующую палубу корабля
bool damageBoatFireToField(mField** mapFields, int i, int j);

//Инициализация координат для полей карт флотов
void initMap(mField** mapFields, int xMapCons, int yMapCons, int sizeY, int sizeX);

//Функция отображения корабля
void viewBoat(int i, int j, mField** mapFields, bool visibleBoat);

//Функция активации (установки) корабля на карте
void activateBoat(int indIDBoat, int i, int j, mField** mapFields, int setCountPalub, bool setOrientBoat, bool visibleBoat);

//Функция проверки поля занятого уничтоженым кораблем. - ДЛЯ ИСКЛЮЧЕНИЯ УДАРОВ ПО ПРИЛЕГАЮЩИМ КООРДИНАТАМ !
bool isCoordBoatFullDamage(int i, int j, mField** mapFields);

//Функция проверки уничтожен корабль или в строю и возврат количества целых палуб countP.
bool isBoatOnFlot(int i, int j, mField** mapFields, int& countP);

//Функция активации флота на карте расстановка компьютером по случайным координатам и случайной ориентации
void activateFlot(mField** mapFields, bool visibleBoat);

void main() {
    int playerСhoice = -1; //Инициализация для выбора играющего
    int maxParametr;       //Для ограничения ввода размеров кораблей в случае игры пользователем, так как затруднен ввод горизонтальных координат
    //Инициализации функционала структуры консоли для позиционирования курсора ввода вывода-------------------------
    InputHandle = GetStdHandle(STD_INPUT_HANDLE);            //                            |////////////#\\\\\\\\\| 
    OutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);          //                            |///////////###\\\\\\\\\|
    GetConsoleScreenBufferInfo(OutputHandle, &ScreenBufInfo);//                            |\\\\              /////|
    mapCons.ConsoleWindowRC(rowConsoleMax, colConsoleMax);   //                            |///////////////////////|
    //Инициализации функционала структуры консоли для позиционирования курсора ввода выода -------------------------

    while (1) {
        srand(time(NULL));
        SetConsoleOutputCP(1251);
        SetConsoleCP(1251);
        system("cls");
        mapCons.GotoRC(1, 1);
        std::cout << "Приветствуем ВАС !\n";
        std::cout << "Представляем вам игру МОРСКОЙ БОЙ версия 1.1\n";
        playerСhoice = -1; 
        setParamGame(3, 1, 53, playerСhoice, 0, 2, "Игроки: (ВЫ и CPU - 1, CPU и CPU - 2) ЗАВЕРШИТЬ 0 ->          ");
        if (playerСhoice == 2) gameUser = false;          //Пользователь игры компьютер false
        else if (playerСhoice == 1) {
            gameUser = true;                              //Пользователь игры человек   true
            visibleL = true;
            visibleR = false;
        }
        else if (playerСhoice == 0) break;//------------Завершение игры путем выхода из основной функции----------

        //Установка основных параметров игры пользователем ------------------------------------------------------------------
        if (gameUser)       //Ограничение на размер корабля если играет пользовател до 6 в связи с тем что горизонтальные координаты ограничены алфавитом и 
            maxParametr = 6;//другими символами ASCII, при размере корабля 7-20 будет затруднительно вводить горизонтальную координату удара
        else                //поэтому размер 7 - 20 для оценки игры компьютер с компьютером при разных значениях интеллекта  
            maxParametr = 20;
        boatMax = 0;
        setParamGame(4, 1, 42, boatMax, 2, maxParametr, "Размер самого большого корабля (2 - " + std::to_string(maxParametr) + ")->          ");
        animatePause = -1;
        setParamGame(5, 1, 42, animatePause, 0, 1000, "Время анимации удара (0 - 1000) мл.сек ->          ");

        visListdamage       = 0; // Сдвигание листинга для просмотра ударов
        visLostVectorOrient = 0; // Сдвигание листинга состояний координат при попадании в тупик rowAzimut++, colAzimut
        idLostVectorOrient  = 0; //Счетчик тупиков

        //Размер координат карты X = (от 0 до XMax-1),  Y = (от 0 до YMax-1)
        YMax = (2.0 + boatMax * 0.02) * (boatMax - boatMin) + 2, XMax = 2.0 * YMax;  //Размер максимального корабля определяет размер плотности установки на карту флотилии
        highMap = YMax, withMap = XMax + 1;

        // Координаты карты Left, Right
        leftRow = 8, rightRow = leftRow;
        leftCol = 5, rightCol = leftCol + boatMax * 4.0 + 33; //107

        iRowMax = 2 * leftRow + highMap;
        iColMax = 2 * rightCol;
        rowConsoleMax = 4.6 * iRowMax;                  //Размер срок консоли
        colConsoleMax = 1.7 * iColMax;

        rowAzimut = 2;                                  //Вывод состояний координат при попадании в тупик rowAzimut++, colAzimut
        if (withMap < 25) {                             //Корректировка для незатирания сток с информацией
            colAzimut = rightCol + 2 * withMap;
        }
        else if (withMap < 40) {
            colAzimut = rightCol + withMap;    
        }
        else colAzimut = rightCol;
        
        onIntelligenceR = -1;
        setParamGame(5, rightCol, rightCol + 28, onIntelligenceR, 0, 3, "Интеллект флота R (0 - 3) ->          ");

        rowStatusSeaBatalia = iRowMax - 1;              //Строка результатов попаданий и уничтожений кораблей флотилий
        rowListdamage = iRowMax + 4;                    //Строка листинга вывода попаданий и уничтожений кораблей флотилий

        rowgetCoordFromUSER = rowStatusSeaBatalia;
        colgetCoordFromUSER = rightCol;

        //Общее количество полей занятых кораблями в функции main будет расчет для инициализации счетчика
        AllFieldsBoat = countAllFieldsBoat();
        mapCons.ConsoleWindowRC(rowConsoleMax, colConsoleMax);
        //Отрисовка карт и сетки полей координат флотов ------------/////////////////////////////////////////////////////////|
        mapCons.ConsoleWindowRC(rowConsoleMax, colConsoleMax);      /////////////////////////##|/////////////////////////////|
        mapCons.TextColor(cMapL);                                   //////////////////////////  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\|
        mapCons.TextBackground(COLOR_BLACK);                        /////////////////////////    \\//  \\\\\\\\\\\\\\\\\\\\\\|
        mapCons.boxMap(leftRow, leftCol, highMap, withMap, '#');    //\\\\\\\\\\\\                          /////////////////|
        mapCons.TextColor(cMapR);                                   //\\\\\\\\\\\\\                        //////////////////|
        mapCons.boxMap(rightRow, rightCol, highMap, withMap, '#');  /////////////////////////////////////////////////////////|
        //Отрисовка карт и сетки полей координат флотов ----------------------------------------------------------------------

        //Создание карт флотов
        //------------------------------------------------------- Выделение памяти --------------------------------------------
        mField** pMapFieldsL = new mField * [YMax];
        mField** pMapFieldsR = new mField * [YMax];
        for (int i = 0; i < YMax; i++) {
            pMapFieldsL[i] = new mField[XMax];
            pMapFieldsR[i] = new mField[XMax];
        }
        //------------------------------------------------------- Выделение памяти --------------------------------------------

        //Инициализация координат для полей карт флотов
        initMap(pMapFieldsL, leftCol, leftRow, YMax, XMax);
        initMap(pMapFieldsR, rightCol, rightRow, YMax, XMax);

        //Создание флотов
        //Инициализация флота кораблей на картах флотов
        activateFlot(pMapFieldsL, visibleL);
        activateFlot(pMapFieldsR, visibleR);

        SetConsoleOutputCP(1251);

        viewSeaBatalia(pMapFieldsL, pMapFieldsR, rowListdamage, leftCol, rightCol);

        //    viewRndFireOnMap(pMapFieldsL, iRowMax, leftCol);  // Использовалось для отладки
        //    viewRndFireOnMap(pMapFieldsR, iRowMax, rightCol); // Использовалось для отладки
        //    mapCons.GotoRC(rowListdamage, 1);
        //--------------------------------------------------- Освобождение памяти ---------------------------------------------
        for (int i = 0; i < YMax; i++) {    //////////////////////////////////////////////////////////////////////////////////|
            delete[] pMapFieldsL[i];        //////          ////          ////          ////          /////////// \###////////|
            delete[] pMapFieldsR[i];        //////   ///////////  /////// ////  ////////////  //////////////////   \//////////|
        }                                   //////       ///////          ////      ////////      /////////\               ///|
        delete[] pMapFieldsL;               //////  ////////////  ///  ///////  ////////////  //////////////\             ////|
        delete[] pMapFieldsR;               //////  ////////////  ////  //////         /////         /////////////////////////|
        //--------------------------------------------------- Освобождение памяти ---------------------------------------------
        system("pause");
    }
}

//Функция установки основных параметров игры
template <typename T>
void setParamGame(int row, int col, int colSetParametr , T& setParametr, int minParametr, int maxParametr, std::string setInfo) {
    while ((setParametr < minParametr) || (setParametr > maxParametr)) {
        mapCons.GotoRC(row, col);
        std::cout << setInfo;
        mapCons.GotoRC(row, colSetParametr);
        std::cin >> setParametr;
    }
}

//Главная функция обмена ударами флотов
void viewSeaBatalia(mField** pMapFieldsL, mField** pMapFieldsR, int& rowListDamage, int lCol, int rCol) {
    //глобальная переменная для поиска координат корабля 
    //bool searchFirstDiscoveredL = false, searchFirstDiscoveredR = false;
    //глобальная переменная первого попадания в корабль для поиска координат корабля
    int iFirstDiscoveredL = -1, jFirstDiscoveredL = -1, iFirstDiscoveredR = -1, jFirstDiscoveredR = -1;

    int iFireL = 0, iFireR = 0;
    int iL = -1, jL = -1, iR = -1, jR = -1, orientBoatL   = -1, orientBoatR  = -1;
    int countDamageL = 0, countDamageR = 0, iLPalubBoatON = 0, iRPalubBoatON = 0;
    bool searchNextPartShipL = false, searchNextPartShipR = false;
    while (countDamageL < AllFieldsBoat && countDamageR < AllFieldsBoat) { //Пока все корабли не уничтожены
        //Колонка вывода результатов попадания под картой противника 

        //почередный обмен ударами контролируется внутри attackBattleField: если одна флотилия раньше уничтожит противника цикл прерывается  ! 
        //Команду флотилии L атакует компьютер на основе определенного уровня интеллекта onIntelligenceR (0 - 3)
        shipsFlotMapAttack(iFirstDiscoveredL, jFirstDiscoveredL, iL, jL, orientBoatL, pMapFieldsL, rowListDamage, rCol, iFireL, countDamageL, searchNextPartShipL, false, onIntelligenceR);
        if (countDamageL == AllFieldsBoat || countDamageR == AllFieldsBoat) break;
        //если флотилия L раньше уничтожит противника а цикл не прерывается - ответный удар может привести к ничьей ! 
        
        //Команду флотилии R атакует интелектуальный выбор координат или gameUser, добивание обнаруженного активно!
        shipsFlotMapAttack(iFirstDiscoveredR, jFirstDiscoveredR, iR, jR, orientBoatR, pMapFieldsR, rowListDamage, lCol, iFireR, countDamageR, searchNextPartShipR, gameUser, onIntelligenceL);
    }
    //        mapCons.TextColor(cSpace);
    mapCons.TextBackground(cSpaceBackGround);
    if (countDamageL < countDamageR) {
        mapCons.TextColor(COLOR_YELLOW);
        mapCons.GotoRC(rowStatusSeaBatalia - 5, lCol);
        std::cout << "L ФЛОТ ПОБЕДА  !!!";
        mapCons.TextColor(COLOR_WHITE);
        mapCons.GotoRC(rowStatusSeaBatalia - 5, rCol);
        std::cout << "R ФЛОТ УНИЧТОЖЕН !";
    }
    else if (countDamageR < countDamageL) {
        mapCons.TextColor(COLOR_YELLOW);
        mapCons.GotoRC(rowStatusSeaBatalia - 5, rCol);
        std::cout << "R ФЛОТ ПОБЕДА  !!!";
        mapCons.TextColor(COLOR_WHITE);
        mapCons.GotoRC(rowStatusSeaBatalia - 5, lCol);
        std::cout << "L ФЛОТ УНИЧТОЖЕН !";
    }
    if (visListdamage == 0)                         //Установка курсора на конец вывода чтобы не затереть результаты
        mapCons.GotoRC(rowListDamage + 1, lCol);
    else if (countDamageL > countDamageR) {
        rowListDamage += countDamageL;
        mapCons.GotoRC(rowListDamage, lCol);
    }
    else {
        rowListDamage += countDamageR;
        mapCons.GotoRC(rowListDamage, lCol);
    }
}

//Функция определения статуса поля true - не поражено снарядом  false - поражено снарядом
bool statusFieldNoDestroy(mField** mapFields, int i, int j) {
    return (mapFields[i][j].statusField);
}

//Функция рачета количества палуб кораблей флотилии
int countAllFieldsBoat() {
    int res = 0;
    for (int countPalubBoat = boatMax; countPalubBoat >= boatMin; countPalubBoat--) {
        for (int j = boatMin; j <= (boatMax - countPalubBoat + 1); j++) {
            res += countPalubBoat;
        }
    }
    return res;
}

//Функция вывода результатов сражения флотов (вывод в со строки rowStatusSeaBatalia -3..+3 + highMap + AllFieldsBoat  - глобальные переменные )
void statusSeaBatalia(int i, int j, int col, int countFire, int countDamage, bool targetDamaged, int onIntelligence) {
    mapCons.TextColor(cSpace); 
    mapCons.TextBackground(cSpaceBackGround);
    if (onIntelligence == 0) {
        mapCons.GotoRC(rowStatusSeaBatalia - 3, col);
        std::cout << "ВЫСТРЕЛЫ ПО RAND    0";
    }
    if (onIntelligence > 0) {
        mapCons.GotoRC(rowStatusSeaBatalia - 3, col);
        std::cout << "ВЫСТРЕЛЫ без повтор 1";
    }
    if (onIntelligence > 1) {
        mapCons.GotoRC(rowStatusSeaBatalia - 2, col);
        std::cout << "ДОБИВАНИЕ  КОРАБЛЯ  2";
    }
    if (onIntelligence > 2) {
        mapCons.GotoRC(rowStatusSeaBatalia - 1, col);
        std::cout << "ИНТЕЛЛЕКТ СТРЕЛЬБЫ  3";
    }
    mapCons.GotoRC(rowStatusSeaBatalia, col);
    std::cout << "Выстрел по координатам  [" << char(65 + j) << ":";
    std::cout.width(2); std::cout << i+1 << "]";
    if (targetDamaged)
        std::cout << " попадание";
    else 
        std::cout << " промах   ";
    mapCons.GotoRC(rowStatusSeaBatalia + 1, col);
    std::cout << "Попаданий в корабли  = " << countDamage;
    mapCons.GotoRC(rowStatusSeaBatalia + 2, col);
    std::cout << "Всего по противнику  = " << countFire << " выстрелов  ";
    mapCons.GotoRC(rowStatusSeaBatalia + 3, col);
    std::cout << "Противник:палуб кораб= " << AllFieldsBoat << " осталось " << (AllFieldsBoat - countDamage) << "  ";
}

//Функция определения новых координат i, j  по обнаруженным координатам попадания iDiscovered, jDiscovered 
//                                          (ВАЖНО: maxCoordI, maxCoordJ - максимальное значение индексов !)
void searchPartShip(int& i, int& j, int iFirstDiscovered, int jFirstDiscovered, int iDiscovered, int jDiscovered, int& orientBoat, mField** pMapFields, int maxCoordI, int maxCoordJ, bool &searchNextPartShip) {
    //Ориентация по глобусу северного полушария :)
    bool north = false, south = false, west = false, east = false, isNoFreeCoord = true; 
    int countOrient = 0, orient = 0, noFreeCoord = 0, y, x; // cntNegativOrientBoat = 0, cntNegativSearchNextPartShip = 0; - GLOBAL
    std::string azimut = "";
    i = iDiscovered; j = jDiscovered;
    //---------------------- При первом ударе определение следующих координат вокруг первого попадания в корабль ----------------------
    do {
        countOrient = 0;
        azimut      = "";
        if (orientBoat < 0) { // ориентация корабля неизвестна поэтому запоминаем все свободные направления в azimut для нанесения удара 
            if (i > 0) {
                north = statusFieldNoDestroy(pMapFields, i - 1, j);
                if (north) {
                    countOrient++;
                    azimut += "N";
                    //std::cout << "север  N" << countOrient; 
                }
            }
            if (i < maxCoordI) {
                south = statusFieldNoDestroy(pMapFields, i + 1, j);
                if (south) {
                    countOrient++;
                    azimut += "S";
                    //std::cout << "юг     S" << countOrient; 
                }
            }
            if (j > 0) {
                west = statusFieldNoDestroy(pMapFields, i, j - 1);
                if (west) {
                    countOrient++;
                    azimut += "W";
                    //std::cout << "запад   W" << countOrient; 
                }
            }
            if (j < maxCoordJ) {
                east = statusFieldNoDestroy(pMapFields, i, j + 1);
                if (east) {
                    countOrient++;
                    azimut += "E";
                    //std::cout << "восток E" << countOrient; 
                }
            }
            if (countOrient > 0) {
                // Генерируем выбор направления orient и из запомненного в azimut[orient-1] создаем координату для последующего удара
                orient = rand() % countOrient + 1;
                if ((char)azimut[orient - 1] == 'N') i -= 1;        // ориентация вектора корабля вертикальная   на N(СЕВЕР) 
                else if ((char)azimut[orient - 1] == 'S') i += 1;   // ориентация вектора корабля вертикальная   на S(ЮГ)
                else if ((char)azimut[orient - 1] == 'W') j -= 1;   // ориентация вектора корабля горизонтальная на W(ЗАПАД) 
                else if ((char)azimut[orient - 1] == 'E') j += 1;   // ориентация вектора корабля горизонтальная на E(ВОСТОК)
                isNoFreeCoord = false;
            }
        }
        else if (orientBoat == 0) { // ориентация корабля вертикальная
            if (i > 0) {
                north = statusFieldNoDestroy(pMapFields, i - 1, j);
                if (north) {
                    countOrient++;
                    azimut += "N";
                }
            }
            if (i < maxCoordI) {
                south = statusFieldNoDestroy(pMapFields, i + 1, j);
                if (south) {
                    countOrient++;
                    azimut += "S";
                }
            }
            if (countOrient > 0) {  // ориентация корабля вертикальная N(СЕВЕР) S(ЮГ)
                // Генерируем выбор направления orient и из запомненного в azimut[orient-1] создаем координату для последующего удара
                orient = rand() % countOrient + 1;
                if ((char)azimut[orient - 1] == 'N') i -= 1;
                else if ((char)azimut[orient - 1] == 'S') i += 1;
                isNoFreeCoord = false;
            }
        }
        else if (orientBoat == 1) { // ориентация корабля горизонтальная
            if (j > 0) {
                west = statusFieldNoDestroy(pMapFields, i, j - 1);
                if (west) {
                    countOrient++;
                    azimut += "W";
                }
            }
            if (j < maxCoordJ) {
                east = statusFieldNoDestroy(pMapFields, i, j + 1);
                if (east) {
                    countOrient++;
                    azimut += "E";
                }
            }
            if (countOrient > 0) {  // ориентация корабля горизонтальная W(ЗАПАД) E(ВОСТОК)
                // Генерируем выбор направления orient и из запомненного в azimut[orient-1] создаем координату для последующего удара
                orient = rand() % countOrient + 1;
                if ((char)azimut[orient - 1] == 'W') j -= 1;
                else if ((char)azimut[orient - 1] == 'E') j += 1;
                isNoFreeCoord = false;
            }
        }
        if (isNoFreeCoord) {
            //--------------------------------------отладка-------------------------------------------------------------
            idLostVectorOrient++;
            noFreeCoord++;
            isNoFreeCoord = true;
            //Визуализация листинга попаданий в корабли ----------------------------------------------------------------
            if (visLostVectorOrient > 0) rowAzimut += visLostVectorOrient;      ///////////////////////////////////////|
            //Визуализация листинга попаданий в корабли ----------------------------------------------------------------
            mapCons.GotoRC(rowAzimut, colAzimut);
            std::cout << idLostVectorOrient << " !азим=" << countOrient << " вектор=" << orientBoat;
            std::cout << " обнаруж[" << char(65 + jDiscovered) << ":" << iDiscovered + 1 << "]";
            std::cout << " перв.обн[" << char(65 + jFirstDiscovered) << ":" << iFirstDiscovered + 1 << "]";
            //Возникла ситуация когда последующая итерация уперлась в конец корабля и вокруг нет свободных полей
            // - в результате чего возвращаемся на место первого попадания в корабль и ищем следующую часть корабля

            i = iFirstDiscovered; j = jFirstDiscovered;
            //----------------------             ?    //---------------------------------------------------------------
            if (noFreeCoord > 1) { 
                orientBoat = -1;  
                cntNegativOrientBoat++;
            }
            if (noFreeCoord > 2) { 
                searchNextPartShip = false; 
                cntNegativSearchNextPartShip++;
            }
            //iDiscovered = -1; jDiscovered = -1;
            //                               i
            y = pMapFields[iFirstDiscovered][jFirstDiscovered].coordY;
            x = pMapFields[iFirstDiscovered][jFirstDiscovered].coordX;
            //std::cout << " корабль [" << y << ":" << x << "]";
            std::cout << " палуб[" << char(65 + x) << ":" << y + 1 << "]" << pMapFields[y][x].countPalub;
            std::cout << " OB = " << cntNegativOrientBoat << " SNPS = " << cntNegativSearchNextPartShip;
            //system("pause");
            //--------------------------------------отладка-------------------------------------------------------------
        }   //noFreeCoord - счетчик  попадания в тупик - смена направления нанесения ударов по большому кораблю на второй итерации
    } while (noFreeCoord < 4 && isNoFreeCoord);
}

//Функция проверки корректного внесенния координат нанесения удара игроком
bool validateCoord(int& i, int& j, int maxCoordI, int maxCoordJ, std::string getCoord) {
    bool coordOK = true;
    int sizeCoordI = getCoord.size();
    int icoord = 0, s = 0;
    if (sizeCoordI < 2) return false;
    else {
        j = int(getCoord[0]) - 65;
        if (j < 0 || j > maxCoordJ) return false;
        for (int k = 1; k < sizeCoordI; k++) {
            s = int(getCoord[k]) - 48;
            icoord = icoord * 10 + s;
        }
        icoord--;
        if (icoord < 0 || icoord > maxCoordI) return false;
        i = icoord;
    }
    return coordOK;
}

//Функция внесения координат удара игроком
void getCoordFromUSER(int& i, int& j, int maxCoordI, int maxCoordJ, int col) {
    std::string getCoord;
    bool validCoord = false;
    mapCons.TextColor(cSpace);
    mapCons.TextBackground(cSpaceBackGround);
    while (!validCoord) {
        mapCons.GotoRC(rowgetCoordFromUSER, col);
        std::cout << "Введите координ. удара->[    ]"; //Делаю первый вывод для очистки позиции ввода координат пользователем
        mapCons.GotoRC(rowgetCoordFromUSER, col);
        std::cout << "Введите координ. удара->[";
        std::cin >> getCoord;
        validCoord = validateCoord(i, j, maxCoordI, maxCoordJ, getCoord);
    }
}

//Поиск координат OnIntelligence == 1, 2 путем простой проверки статуса поля - не разрушено выстрелом !
//                OnIntelligence == 3 на основе разведданных координат уничтоженных кораблей  
bool searchTargetOnIntelligence(int i, int j, mField** pMapFields, int maxCoordI, int maxCoordJ, int onIntelligence) {
    if (onIntelligence == 0) return true;
    if (onIntelligence == 1 || onIntelligence == 2) return statusFieldNoDestroy(pMapFields, i, j);
    else if (onIntelligence == 3) {
        //Исключение координат на основе разведданных координат уничтоженных кораблей OnIntelligence == 3
        if (statusFieldNoDestroy(pMapFields, i, j)) {   //      сначала проверяем поле удара - если целое, то
            if (i > 0) {                                //      -  проверяем нет ли рядом пораженного корабля
                if (isCoordBoatFullDamage(i - 1, j, pMapFields)) return false;
                //    azimut += "N";
            }
            if (i < maxCoordI) {
                if (isCoordBoatFullDamage(i + 1, j, pMapFields)) return false;
                //    azimut += "S";
            }
            if (j > 0) {
                if (isCoordBoatFullDamage(i, j - 1, pMapFields)) return false;
                //    azimut += "W";
            }
            if (j < maxCoordJ) {
                if (isCoordBoatFullDamage(i, j + 1, pMapFields)) return false;
                //    azimut += "E";
            }
            return true;                                //      Если поле удара целое и нет примыканий то возвращаем true 
        }                                               //      для нанесения удара по полю с координатами i, j
        else return false;
    }
}

//Основная функция "атака карты флота кораблей" по карте противника расчет координат и нанесения ударов с последующим ходом
// - если промах то выход и запоминание координат недобитого корабля для последующего хода после противника
void shipsFlotMapAttack(int& iFirstDiscovered, int& jFirstDiscovered, int& iDiscovered, int& jDiscovered, int& orientBoat, mField** pMapFields, int row, int col, int& iFire, int& iCountDamage, bool& searchNextPartShip, bool gameUser, int onIntelligence) {
    //глобальная переменная первого попадания в корабль для поиска координат корабля
    //int iFirstDiscoveredL = -1, jFirstDiscoveredL = -1, iFirstDiscoveredR = -1, jFirstDiscoveredR = -1;
    int countRND = 0, countRND1 = 0;
    int i = -1, j = -1, iPalubBoatON = 0, rowList = 0;
    bool targetDamaged, fieldNoDestroy;
                                  // orientBoat = 1 - горизонтальная,  = 0 вертикальная,  -1 неизвестная ориентация корабля
    do {                          //Цикл атаки на противника пока (targetDestruction = true) попадание в корабль успешно 
        //Если заявлен низкий onIntelligence = 0 или onIntelligence = 1 то отключаем обнаруженные координаты для добивания корабля
        if (onIntelligence < 2) { 
            iDiscovered = -1; jDiscovered = -1;
        }
        if (!gameUser) {
            if (onIntelligence == 0) {
                i = rand() % YMax; //  Интеллек == 0 - огонь по любым координатам даже повторно !
                j = rand() % XMax;
                fieldNoDestroy = true;
            }
            else if (iDiscovered < 0) {    // Если координаты iDiscovered && jDiscovered < 0 это :
                srand(time(NULL));  
                countRND = 0;
                do {                  //   1)начало сражения, 2)был промах, 3)уничтожен корабль и 
                    i = rand() % YMax;//       производится поиск новых координат нанесения удара
                    j = rand() % XMax;
                    fieldNoDestroy = searchTargetOnIntelligence(i, j, pMapFields, YMax - 1, XMax - 1, onIntelligence);

                    //-----------------------------------------------------------------------------------------КОНТРОЛЬ ОТЛАДКИ
                    if (countRND++ > 2 * YMax * XMax) {                                      ////////////////////////////////////|
                        //std::cout << "countRND  > YMax * 4096 *XMax"; system("pause");     // непонятное попадание в           |
                        break;                                                               // бесконечный цикл ... ???         |
                    }                                                                        ////////////////////////////////////|
                    //-----------------------------------------------------------------------------------------КОНТРОЛЬ ОТЛАДКИ
                } while (!fieldNoDestroy);
            }                               
            else {      //Если координаты iDiscovered jDiscovered >= 0 был обнаружен корабль и поврежден, 
                        // - требуется дальнейший поиск и уничтожение неповрежденных частей корабля.  
                        //   поиск и расчет координат обнаруженного корабля
                searchPartShip(i, j, iFirstDiscovered, jFirstDiscovered, iDiscovered, jDiscovered, orientBoat, pMapFields, YMax - 1, XMax - 1, searchNextPartShip);
            }
        }
        else {
            getCoordFromUSER(i, j, YMax - 1, XMax - 1, col);
        }
        iFire++;
        targetDamaged = damageBoatFireToField(pMapFields, i, j);
        //--------------------------------------отладка-------------------------------------------------------------
        //mapCons.GotoRC(rotl, 1);
        //system("pause");
        //--------------------------------------отладка-------------------------------------------------------------
        if (targetDamaged) {
            mapCons.TextColor(cSpace);
            mapCons.TextBackground(cSpaceBackGround);
            iCountDamage++; visListdamage;
            //Визуализация листинга попаданий в корабли -------------------------------------------------------------
            if (visListdamage > 0) rowList = row + iCountDamage;    ////////////////////////////////////////////////|
            else rowList = row;                                     ////////////////////////////////////////////////|
            //Визуализация листинга попаданий в корабли -------------------------------------------------------------
            mapCons.GotoRC(rowList, col);
            //                std::cout << "Попадание по координатам y = " << i << " x = " << j;
            //                std::cout << " в корабль № " << pMapFieldsR[i][j].idBoat << " палуб = " << pMapFieldsR[i][j].countPalub;
            if (orientBoat < 0 && !searchNextPartShip) { // Первое попадание в корабль
                iDiscovered = i; jDiscovered = j;
                iFirstDiscovered = i; jFirstDiscovered = j; //Сохраняем координаты первого попадания в корабль для поиска неповрежденных палуб
                //searchNextPartShip = true;
            }
            else if (orientBoat < 0 && searchNextPartShip) { // Последующее попадание и определение ориентации вектора корабля
                //searchNextPartShip = true;
                if (iDiscovered == i)
                    orientBoat = 1;
                else if (jDiscovered == j)
                    orientBoat = 0;
                iDiscovered = i; jDiscovered = j;
            }
            if (!isBoatOnFlot(i, j, pMapFields, iPalubBoatON)) { // Проверка на жив корабль или повреждена палуба
                mapCons.TextColor(COLOR_YELLOW);
                std::cout << "[" << char(65 + j) << ":";
                std::cout.width(2); std::cout << i + 1 << "]";
                std::cout << " Уничтож.кор.№ " << pMapFields[i][j].idBoat << " разм. " << pMapFields[i][j].countPalub << "        ";
                searchNextPartShip = false; //После поражения корабля очищаем обнаруженные координаты и ориентацию
                //if (onIntelligence > 1) {
                    orientBoat = -1;
                    iDiscovered = -1; jDiscovered = -1;
                    iFirstDiscovered = -1; jFirstDiscovered = -1;
                //}
            }
            else {
                std::cout << "[" << char(65 + j) << ":";
                std::cout.width(2); std::cout << i + 1 << "]";
                std::cout << " Попад.в кор.№ " << pMapFields[i][j].idBoat << " разм. " << pMapFields[i][j].countPalub;
                std::cout << " ост. " << iPalubBoatON;
                searchNextPartShip = true;
                if (orientBoat < 0) {       //Определяем вектор корабля горизонтальный 1 или вертикальный 0
                    if (iDiscovered == i)
                        orientBoat = 1;
                    else if (jDiscovered == j)
                        orientBoat = 0;
                }
                iDiscovered = i; jDiscovered = j;
            }
        }
        else {   // В результате промаха и корабль ещё живой возвращаемся на начальные координаты первого попадания и после промаха соперника начинаем добивать 
            if (searchNextPartShip) {
                //нельзя убирать ориентацию - так как после попадания в центр и выбивания в одну сторону 
                // - добивание после промаха более быстрое по вектору координат
                //orientBoat = -1; 
                //Требуется проверка на корректность
                iDiscovered = iFirstDiscovered;
                jDiscovered = jFirstDiscovered;
                searchNextPartShip = true;
            }
        }
        statusSeaBatalia(i, j, col, iFire, iCountDamage, targetDamaged, onIntelligence);
        //--------------------------------------отладка-------------------------------------------------------------
        //mapCons.GotoRC(AllFieldsBoat + rotlL, 1);
        //system("pause");
        //--------------------------------------отладка-------------------------------------------------------------
        
    } while (targetDamaged && iCountDamage < AllFieldsBoat);//если одна флотилия раньше уничтожит противника цикл прерывается  !
}

//Функция проверки координат головной части корабля для размещения его на карте
//проверяется область занимаемая предполагаемым местоположением корабля и область векторов вокруг на наличие занятых координат
bool newCoordInsertBoat(int i, int j, int yMax, int xMax, mField** pMapFields, int sizeBoat, bool orient) {
    //на вход передаются координаты головной части корабля 
    //если ориентация горизонтальна (orient = true ) то начало корабля левая палуба   координата 
    //если ориентация вертикальна   (orient = false) то начало корабля верхняя палуба координата
    //Необходимо выполнить внешний вызов 3 раза :
    //                                           1)проверяется область занимаемая предполагаемым местоположением корабля 
    //                                           2), 3) и область векторов вокруг на наличие занятых координат
    bool coordInsertBoat = true;
    //                                 
    if (orient && (j + sizeBoat > xMax)) {
        return false;
    }
    else if (!orient && (i + sizeBoat > yMax)) {
        return false;
    }                                 //<  было так ------------------------------------------------ ТРЕБУЕТСЯ ПРОВЕРКА < ??? <= 
    for (int iPalubBoat = 0; iPalubBoat <= sizeBoat; iPalubBoat++) {               // <= повлияло на НЕ слипание последних 1-палубных
        if ((pMapFields[i][j].boatOnField > 0)) {
            coordInsertBoat = false;
            break;
        }
        if (orient) j++; else i++;
    }
    return coordInsertBoat;
}

//Функция анимирования попадания в карту или корабль
void viewAnimateDamageFireField(mField** mapFields, int i, int j, int colorStatus, int colorStatusBackGround) {
    int y = mapFields[i][j].yMapCons + mapFields[i][j].yField;
    int x = mapFields[i][j].xMapCons + mapFields[i][j].xField;
    mapCons.TextColor(COLOR_WHITE);
    mapCons.TextBackground(COLOR_RED);
    mapCons.GotoXY(x, y); //Позиционируем относительно координат xMapCons, yMapCons начала карты 
    std::cout << mapFields[i][j].onField;
    Sleep(animatePause);
    mapCons.TextColor(COLOR_RED);
    mapCons.TextBackground(COLOR_WHITE);
    mapCons.GotoXY(x, y); //Позиционируем относительно координат xMapCons, yMapCons начала карты 
    std::cout << mapFields[i][j].onField;
    Sleep(animatePause);
    mapCons.TextColor(colorStatus);
    mapCons.TextBackground(colorStatusBackGround);
    mapCons.GotoXY(x, y); //Позиционируем относительно координат xMapCons, yMapCons начала карты 
    std::cout << mapFields[i][j].onField;
}

//Функция отображения поля с координатами i, j из массива карты mapFields
void viewFieldMap(mField** mapFields, int i, int j) {
    bool damageBoat = false;
    int y = mapFields[i][j].yMapCons + mapFields[i][j].yField;
    int x = mapFields[i][j].xMapCons + mapFields[i][j].xField;
    int colorStatus, colorStatusBackGround;

    if (mapFields[i][j].boatOnField > 0) {
        if (mapFields[i][j].statusPalubaBoat) {
            colorStatus           = cBoat;
            colorStatusBackGround = cBoatBackGround;
        }
        else {
            colorStatus           = cBoatDestroy;
            colorStatusBackGround = cBoatDestroyBackGround;
            viewAnimateDamageFireField(mapFields, i, j, colorStatus, colorStatusBackGround);
        }
    }
    else {
        if (mapFields[i][j].statusField) {
            colorStatus           = cSpace;
            colorStatusBackGround = cSpaceBackGround;
        }
        else {
            colorStatus           = cSpaceDestroy;
            colorStatusBackGround = cSpaceDestroyBackGround;
        }
    }
    //Если режим карта открыта (читер) то показываем на карте по позиции i,j палубы корабля
    mapCons.TextColor(colorStatus);
    mapCons.TextBackground(colorStatusBackGround);
    mapCons.GotoXY(x, y); //Позиционируем относительно координат xMapCons, yMapCons начала карты 

    if (mapFields[i][j].isVisibleBoat) {
        std::cout << mapFields[i][j].onField;
    }
    //Если поле карты поражено попаданием то показываем на карте по позиции i,j поврежденную палуб корабля либо воронку промаха
    else if (!mapFields[i][j].statusField) {
        viewAnimateDamageFireField(mapFields, i, j, colorStatus, colorStatusBackGround);
    }
}

//Выстрел по полю возвращает true если попадание в действующую палубу корабля
bool damageBoatFireToField(mField** mapFields, int i, int j) {
    if (!mapFields[i][j].statusField) return false;
    mapFields[i][j].statusField = false;
    if (mapFields[i][j].boatOnField > 0) {
        mapFields[i][j].statusPalubaBoat = false;
        mapFields[i][j].onField = boatFDestroy;
        viewFieldMap(mapFields, i, j);
        return true;
    }
    mapFields[i][j].onField = spaceDestroy;
    viewFieldMap(mapFields, i, j);
    return false;
}

//Инициализация координат для полей карт флотов
void initMap(mField** mapFields, int xMapCons, int yMapCons, int sizeY, int sizeX) {
    for (int i = 0; i < sizeY; i++) {
        for (int j = 0; j < sizeX; j++) {
            mapFields[i][j].xMapCons = xMapCons; //Координаты xMapCons, yMapCons начала карты
            mapFields[i][j].yMapCons = yMapCons;
            //---------------------------------------------------- BOAT ----------------------------------------------------------------
            mapFields[i][j].boatOnField = 0; //0 - пусто   1, 2, 3, 4 - корабль (1 - однопалубный, 2 - двухпалубный, 3 - трёхпалубный, 4 - четырёхпалубный)
            mapFields[i][j].yField = i;//Координата y корабля относительно начала координат карты
            mapFields[i][j].xField = j;//Координата x корабля относительно начала координат карты
            mapFields[i][j].idBoat = 0;
            mapFields[i][j].countPalub = 0;   // Количество палуб корабля
            mapFields[i][j].orientBoat = true;// setOrientBoat = true - горизонтальное расположение корабля, false - вертикальное расположение корабля
            mapFields[i][j].statusBoat = true;// true - корабль в строю,  false - корабль уничтожен огнем противника (поражены все палубы_)
            mapFields[i][j].statusPalubaBoat = true; //false - палуба корабля уничтожена, true - палуба корабля в строю
            //---------------------------------------------------- BOAT ----------------------------------------------------------------
            mapFields[i][j].statusField = true; //false - поле поражено снарядом,    true - поле не поражено снарядом
            mapFields[i][j].isVisibleBoat = false;//false - поле скрыто, true - поле открыто и видно статус (причина - попадание снаряда, принудительное включение для режима ЧИТЕР или отладки программы)
        }
    }
}

//Функция отображения корабля
void viewBoat(int i, int j, mField** mapFields, bool visibleBoat) {
    //смещение координаты палубы относительно начала корабля горизонтальное по x, вертикальное по y
    int y = i, x = j;
    int iMaxPalub = mapFields[y][x].countPalub;
    for (int iPalub = 0; iPalub < iMaxPalub; iPalub++) {
        mapFields[i][j].isVisibleBoat = visibleBoat;
        viewFieldMap(mapFields, i, j);
        if (mapFields[y][x].orientBoat) j++; else i++;
    }
}

//Функция активации (установки) корабля на карте
void activateBoat(int indIDBoat, int i, int j, mField** mapFields, int setCountPalub, bool setOrientBoat, bool visibleBoat) {
    int y = i, x = j;           //Головные координаты корабля y, x
    mapFields[i][j].yField = y; //Головная часть корабля y
    mapFields[i][j].xField = x; //Головная часть корабля x
    mapFields[i][j].orientBoat = setOrientBoat;
    for (int iPalub = 0; iPalub < setCountPalub; iPalub++) {
        mapFields[i][j].idBoat = indIDBoat;
        mapFields[i][j].coordY = y;
        mapFields[i][j].coordX = x;
        mapFields[i][j].countPalub = setCountPalub;
        mapFields[i][j].orientBoat = setOrientBoat;
        mapFields[i][j].insertPalubaBoat(i, j, setCountPalub, visibleBoat);
        //смещение координаты палубы относительно начала корабля горизонтальное по j(x), вертикальное по i(y)
        //                  orientBoat = true - горизонтальное false - вертикальное
        if (mapFields[y][x].orientBoat) j++; else i++;
    }
    //для отладки ---------------------------------------------------------------------------------------
    if (visibleBoat) viewBoat(y, x, mapFields, visibleBoat);
    //для отладки ---------------------------------------------------------------------------------------
}

//Функция проверки поля занятого уничтоженым кораблем. - ДЛЯ ИСКЛЮЧЕНИЯ УДАРОВ ПО ПРИЛЕГАЮЩИМ КООРДИНАТАМ !
bool isCoordBoatFullDamage(int i, int j, mField** mapFields) {
    int iShipCoordinates = mapFields[i][j].coordY;
    int jShipCoordinates = mapFields[i][j].coordX;
    //Если по главным координатам находится корабль то возвращаем его статус true - живой, false - уничтожен.
    if (mapFields[iShipCoordinates][jShipCoordinates].boatOnField > 0) {
        return !mapFields[iShipCoordinates][jShipCoordinates].statusBoat;
    } 
    else return false; //По указанным координатам нет корабля и можно наносить удары
}

//Функция проверки уничтожен корабль или в строю и возврат количества целых палуб countP.
bool isBoatOnFlot(int i, int j, mField** mapFields, int& countP) {
    bool flag = false;
    int y = i, iShipCoordinates;
    int x = j, jShipCoordinates;
    int countPMax = mapFields[y][x].countPalub;
    int countActivePalub = countPMax;
    i = mapFields[y][x].coordY; iShipCoordinates = i; 
    j = mapFields[y][x].coordX; jShipCoordinates = j;
    for (int iPalub = 0; iPalub < countPMax; iPalub++) {
        if (mapFields[i][j].statusBoatOnField())
            flag = true;            //Если хотя бы одна палуба цела корабль в строю !
        else
            countActivePalub--;     //Счетчик палуб не имеющих повреждения
        if (mapFields[i][j].orientBoat) j++; else i++;
    }
    if (!flag) mapFields[iShipCoordinates][jShipCoordinates].statusBoat = false; 
    countP = countActivePalub;
    return flag;
}

//Функция активации флота на карте расстановка компьютером по случайным координатам и случайной ориентации
void activateFlot(mField** mapFields, bool visibleBoat) {
    int iMax = YMax - 1, jMax = XMax; //Задание границ генерации координат согласно размеров индексов массива карты полей
    int indIDBoat = 0;
    int irnd, jrnd, rndOrient, ii, ij, ik, jk;
    bool activateOrientBoat = true;
    bool freeCoordBoat = false;
    bool freeCoordUpL = false;
    bool freeCoordDwR = false;
    //Выстраиваем корабли от максимального до минимального для оптимального встраивания в координаты карты
    //проверяется область занимаемая предполагаемым местоположением корабля и область вокруг на наличие занятых координат
    for (int iCountPaluba = boatMax; iCountPaluba >= boatMin; iCountPaluba--) {
        for (int jCountBoat = boatMin; jCountBoat <= (boatMax - iCountPaluba + 1); jCountBoat++) {
            freeCoordBoat = false;   //Область вектора корабля свободна и примыкания к нему
            freeCoordUpL = false;   //Область граничащая сверху (либо левее ) от корабля свободна
            freeCoordDwR = false;   //Область граничащая снизу  (либо правее) от корабля свободна
            //Процедура цикла генерации и проверки свободных координат для расстановки кораблей на полях координат 
            do {
                rndOrient = rand() % 10;           //Генерируем случайное число от 0 до 9 и по распределению
                if (rndOrient > 4)                 //от 0 - 4  вертикальное, от 5 - 9 горизонтальное расположение корабля 
                    activateOrientBoat = true;
                else
                    activateOrientBoat = false;
                irnd = rand() % YMax;
                jrnd = rand() % XMax;
                if (activateOrientBoat) {                   //Горизонтальное расположение корабля
                    if (jrnd > 0 && jrnd < jMax) ij = 1; else ij = 0;
                    if (jrnd + iCountPaluba == jMax) jk = 0; else jk = 1;//jk = 0 если корабль граничит с краем карты
                    //Проверка свободных позиций координат на основной линии горизонтально
                    freeCoordBoat = newCoordInsertBoat(irnd, jrnd - ij, YMax, XMax, mapFields, iCountPaluba + jk + ij, activateOrientBoat);
                    if (irnd == 0)                         //Проверка свободных позиций координат на линии соприкосновения выше
                        freeCoordUpL = true;
                    else if (irnd > 0) {
                        ii = 1;
                        freeCoordUpL = newCoordInsertBoat(irnd - ii, jrnd - ij, YMax, XMax, mapFields, iCountPaluba + jk + ij, activateOrientBoat);
                    }
                    if (irnd == iMax)                      //Проверка свободных позиций координат на линии соприкосновения ниже
                        freeCoordDwR = true;
                    else if (irnd < iMax) {
                        ii = 1;
                        freeCoordDwR = newCoordInsertBoat(irnd + ii, jrnd - ij, YMax, XMax, mapFields, iCountPaluba + jk + ij, activateOrientBoat);
                    }
                }
                else {                                      //Вертикальное расположение корабля
                    if (irnd > 0 && irnd < iMax) ii = 1; else ii = 0;
                    if (irnd + iCountPaluba == iMax) ik = 0; else ik = 1; //ik = 0 если корабль граничит с краем карты
                    //Проверка свободных позиций координат на основной линии вертикально
                    freeCoordBoat = newCoordInsertBoat(irnd - ii, jrnd, YMax, XMax, mapFields, iCountPaluba + ik + ii, activateOrientBoat);
                    if (jrnd == 0)                         //Проверка свободных позиций координат на линии соприкосновения левее
                        freeCoordUpL = true;
                    else if (jrnd > 0) {
                        ij = 1;
                        freeCoordUpL = newCoordInsertBoat(irnd - ii, jrnd - ij, YMax, XMax, mapFields, iCountPaluba + ik + ii, activateOrientBoat);
                    }
                    if (jrnd == jMax)                      //Проверка свободных позиций координат на линии соприкосновения правее
                        freeCoordDwR = true;
                    else if (jrnd < jMax) {
                        ij = 1;
                        freeCoordDwR = newCoordInsertBoat(irnd - ii, jrnd + ij, YMax, XMax, mapFields, iCountPaluba + ik + ii, activateOrientBoat);
                    }
                }
            } while (!(freeCoordBoat && freeCoordUpL && freeCoordDwR));
            //Установка корабля и его координат на полях карты, его ориентация, параметр видимости на карте
            activateBoat(indIDBoat++, irnd, jrnd, mapFields, iCountPaluba, activateOrientBoat, visibleBoat);
        }
    }
}
