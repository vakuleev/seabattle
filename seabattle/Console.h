#pragma once

#include<iostream>
#include<windows.h>

namespace MCons {
    //�����
#define COLOR_BLACK 0           //������
#define COLOR_BLUE 1            //�����
#define COLOR_GREEN 2           //�������
#define COLOR_CYAN 3            //�������
#define COLOR_RED 4             //�������
#define COLOR_MAGENTA 5         //����������
#define COLOR_BROWN 6           //����������
#define COLOR_LIGHTGRAY 7       //������-�����
#define COLOR_DARKGRAY 8        //�����-�����
#define COLOR_LIGHTBLUE 9       //������-�����
#define COLOR_LIGHTGREEN 10     //������-�������
#define COLOR_LIGHTRED 12       //������-�������
#define COLOR_LIGHTMAGENTA 13   //������-����������
#define COLOR_YELLOW 14         //������
#define COLOR_WHITE 15          //�����

//    MConsole;
HANDLE InputHandle;                                         //���������� ������ �����
HANDLE OutputHandle;                                        //���������� ������ ������
CONSOLE_SCREEN_BUFFER_INFO ScreenBufInfo;                   //���������� �� �������� ������ �������

                                                            //��������� �������
struct vConsole {
        
        //������� ������� ������
        void ClearScreen() {
            system("cls");
        }
        
        //������� ����������� ������� �� ������� � �������� � ������� ��������� � 1 !!!
        void GotoRC(int Row, int Col) {
            GotoXY(Col-1, Row-1);
        }

        //������� ����������� ������� �� ���������� X � Y � ������� ��������� � 0 !!!
        void GotoXY(int X, int Y) {
            ScreenBufInfo.dwCursorPosition.X = X; // Col    
            ScreenBufInfo.dwCursorPosition.Y = Y; // Row
            SetConsoleCursorPosition(OutputHandle, ScreenBufInfo.dwCursorPosition);
        }

        //������� ������� ����� ����
        void TextBackground(BYTE Color) {
            GetConsoleScreenBufferInfo(OutputHandle, &ScreenBufInfo);
            WORD TextAttr = ScreenBufInfo.wAttributes;
            TextAttr = (Color << 4) | (TextAttr & 0x0F);
            SetConsoleTextAttribute(OutputHandle, TextAttr);
        }

        //������� ������� ����� ������
        void TextColor(BYTE Color) {
            GetConsoleScreenBufferInfo(OutputHandle, &ScreenBufInfo);
            WORD TextAttr = ScreenBufInfo.wAttributes;
            TextAttr = (Color & 0x0F) | (TextAttr & 0xF0);
            SetConsoleTextAttribute(OutputHandle, TextAttr);
        }
        //������� ������� �������� ���� �� ���������� ����� � �������� � ������� ��������� � 1 !!!
        void ConsoleWindowRC(int Row, int Col) {
            ConsoleWindowXY(Col-1, Row-1);
        }

        //������� ������� �������� ���� �� ������� X � Y � ������� ��������� � 0 !!!
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