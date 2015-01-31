//This file is part of Reverse Text plugin for notepad++
//Copyright (C)2014 Themistokle Benetatos <info@mrt-prodz.com>
//
//Based on the C++ plugin template by Don HO
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#ifndef PLUGINDEFINITION_H
#define PLUGINDEFINITION_H

#define VERSION_VALUE "0.1"
#define VERSION_DIGITALVALUE 0, 1, 0, 0

#define IDC_STATIC  -1

// All definitions of plugin interface
//
#include "Npp/PluginInterface.h"

// Plugin name
//
const TCHAR NPP_PLUGIN_NAME[] = TEXT("Reverse Text");

// Number of your plugin commands
//
const int nbFunc = 6;

// Called while plugin loading
//
void pluginInit(HANDLE hModule);

// Called while plugin unloading
//
void pluginCleanUp();

// Initialization of plugin commands
//
void commandMenuInit();

// Clean up commands allocation
//
void commandMenuCleanUp();

// Function which sets command
//
bool setCommand(size_t index, TCHAR const *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk = NULL, bool check0nInit = false);

// Function to return current scintilla
//
HWND getCurScintilla();

// Function to send message to scintilla
//
inline LRESULT sendScintilla(HWND &hCurrScintilla, UINT Msg, WPARAM wParam = 0, LPARAM lParam = 0);

// ============================================================================
//
// Plugin command functions
//
// ============================================================================

// Reverse the text provided by selectedText and save it in reversedText
//
void reverseText(size_t &bufLength, char * selectedText, char * reversedText);

// Reverse the text selection
//
void reverseSelection();

// Reverse the text content on line specified by curLine and restore caret position from curPos
//
void reverseLine(HWND &hCurrScintilla, size_t &curPos, size_t &curLine);

// Reverse the text content on current line
//
void reverseCurrentLine();

// Reverse each single line in the document, keeps line ordering
//
void reverseEachLine();

// Dialog About
//
void about();

#endif //PLUGINDEFINITION_H
