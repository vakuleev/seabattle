#pragma once

#include<iostream>
#include<windows.h>

namespace MCons {
    //цвета
#define COLOR_BLACK 0           //черный
#define COLOR_BLUE 1            //синий
#define COLOR_GREEN 2           //зеленый
#define COLOR_CYAN 3            //голубой
#define COLOR_RED 4             //красный
#define COLOR_MAGENTA 5         //фиолетовый
#define COLOR_BROWN 6           //коричневый
#define COLOR_LIGHTGRAY 7       //светло-серый
#define COLOR_DARKGRAY 8        //темно-серый
#define COLOR_LIGHTBLUE 9       //светло-синий
#define COLOR_LIGHTGREEN 10     //светло-зеленый
#define COLOR_LIGHTRED 12       //светло-красный
#define COLOR_LIGHTMAGENTA 13   //светло-фиолетовый
#define COLOR_YELLOW 14         //желтый
#define COLOR_WHITE 15          //белый

//    MConsole;
HANDLE InputHandle;                                         //дескриптор буфера ввода
HANDLE OutputHandle;                                        //дескриптор буфера вывода
CONSOLE_SCREEN_BUFFER_INFO ScreenBufInfo;                   //информация об экранном буфере консоли

                                                            //структура консоли
struct vConsole {
        
        //функция очистки экрана
        void ClearScreen() {
            system("cls");
        }
        
        //функция перемещения курсора по строкам и колонкам в консоли нумерация с 1 !!!
        void GotoRC(int Row, int Col) {
            GotoXY(Col-1, Row-1);
        }

        //функция перемещения курсора по координате X и Y в консоли нумерация с 0 !!!
        void GotoXY(int X, int Y) {
            ScreenBufInfo.dwCursorPosition.X = X; // Col    
            ScreenBufInfo.dwCursorPosition.Y = Y; // Row
            SetConsoleCursorPosition(OutputHandle, ScreenBufInfo.dwCursorPosition);
        }

        //функция задания цвета фона
        void TextBackground(BYTE Color) {
            GetConsoleScreenBufferInfo(OutputHandle, &ScreenBufInfo);
            WORD TextAttr = ScreenBufInfo.wAttributes;
            TextAttr = (Color << 4) | (TextAttr & 0x0F);
            SetConsoleTextAttribute(OutputHandle, TextAttr);
        }

        //функция задания цвета текста
        void TextColor(BYTE Color) {
            GetConsoleScreenBufferInfo(OutputHandle, &ScreenBufInfo);
            WORD TextAttr = ScreenBufInfo.wAttributes;
            TextAttr = (Color & 0x0F) | (TextAttr & 0xF0);
            SetConsoleTextAttribute(OutputHandle, TextAttr);
        }
        //функция задания размеров окна по количеству строк и столбцов в консоли нумерация с 1 !!!
        void ConsoleWindowRC(int Row, int Col) {
            ConsoleWindowXY(Col-1, Row-1);
        }

        //функция задания размеров окна по размеру X и Y в консоли нумерация с 0 !!!
        void ConsoleWindowXY(int X, int Y) {
            SMALL_RECT ConsoleRect;
            ConsoleRect.Top = 0;
            ConsoleRect.Left = 0;
            ConsoleRect.Right = X - 1;
            ConsoleRect.Bottom = Y - 1;
            COORD BufSize;
            BufSize.X = X;
            BufSize.Y = Y;
            SetConsoleScreenBufferSize(OutputHandle, BufSize);
            SetConsoleWindowInfo(OutputHandle, TRUE, &ConsoleRect);
            GotoXY(0, 0);
        }

        void box(int Row, int Col, int high, int with, const char field) {
            int i = 0;
            rowBox(Row + i, Col, char(218), char(191), with, char(196), true);
            for (i = 0; i < high; i++) {
                rowBox(Row + i + 1, Col, char(179), char(179), with, field, false);
            }
            rowBox(Row + i + 1, Col, char(192), char(217), with, char(196), true);
        }

        void rowBox(int Row, int Col, const char start, const char end, int with, const char field, bool fields) {
            SetConsoleOutputCP(855);
            GotoRC(Row, Col);
            std::cout << start;
            if (fields) {
                for (int j = 0; j < with; j++) {
                    std::cout << field;
                }
            }
            GotoRC(Row, Col + with);
            std::cout << end; // << std::endl;
        }

        void rowBoxMap(int Row, int Col, const char start, const char end, int with, const char field, bool fields, int num) {
            SetConsoleOutputCP(855);
            GotoRC(Row, Col);
            std::cout.width(2);
            if (num >= 0) std::cout << num; else std::cout << "  ";

            std::cout << start;
            if (fields) {
                for (int j = 0; j < with; j++) {
                    std::cout << field;
                }
            }
            GotoRC(Row, Col + with);
            std::cout << end; // << std::endl;
        }

        void mapCoordY(int Row, int Col, int high, int with) {
            for (int i = 1; i <= high; i++) {
                GotoRC(Row + i, Col-2);
                std::cout.width(2);
                std::cout << i;
            }
        }

        void mapCoordX(int Row, int Col, int high, int with) {
            GotoRC(Row - 1, Col + 1);
            for (int i = 0; i < with - 1; i++) {
                std::cout << char(65 + i);
            }
        }

        void boxMap(int Row, int Col, int high, int with, const char field) {
            box(Row, Col, high, with, field);
            mapCoordY(Row, Col, high, with);
            mapCoordX(Row, Col, high, with);
        }
    };

}