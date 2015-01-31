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

#include "PluginDefinition.h"
#include "menuCmdID.h"

// The plugin data that Notepad++ needs
//
FuncItem funcItem[nbFunc];

// Notepad++ data
//
NppData nppData;

// Notepad++ handle instance
//
HANDLE _hModule;

// Plugin data initialization (called when loading)
//
void pluginInit(HANDLE hModule)
{
    _hModule = hModule;
}

// Clean up, save the parameters (if any) for the next session
//
void pluginCleanUp()
{
}

// Initialization of plugin commands
//
void commandMenuInit()
{
    // setCommand(int index,                      // zero based number to indicate the order of command
    //            TCHAR *commandName,             // the command name that you want to see in plugin menu
    //            PFUNCPLUGINCMD functionPointer, // the symbol of function (function pointer) associated with this command. The body should be defined below. See Step 4.
    //            ShortcutKey *shortcut,          // optional. Define a shortcut to trigger this command
    //            bool check0nInit                // optional. Make this menu item be checked visually
    //            );
    setCommand(0, TEXT("Selection"), reverseSelection, NULL, false);
    setCommand(1, TEXT("Current Line"), reverseCurrentLine, NULL, false);
    setCommand(2, TEXT("---"), NULL, NULL, false);
    setCommand(3, TEXT("Each Line"), reverseEachLine, NULL, false);
    setCommand(4, TEXT("---"), NULL, NULL, false);
    setCommand(5, TEXT("About"), about, NULL, false);
}

// Clean up (especially for the shortcut)
//
void commandMenuCleanUp()
{
	// Don't forget to deallocate your shortcut here
}


// Function to initialize plugin commands
//
bool setCommand(size_t index, TCHAR const *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk, bool check0nInit)
{
    if (index >= nbFunc)
        return false;

    if (!pFunc)
        return false;

    lstrcpy(funcItem[index]._itemName, cmdName);
    funcItem[index]._pFunc = pFunc;
    funcItem[index]._init2Check = check0nInit;
    funcItem[index]._pShKey = sk;

    return true;
}

// This function returns the current scintilla
//
HWND getCurScintilla()
{
    // Return the current scintilla or NULL if it fails
    int which = -1;
    sendScintilla(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
    if (which == -1)
        return NULL;
    return (which == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;
}

// This function sends message to scintilla
//
inline LRESULT sendScintilla(HWND &hCurrScintilla, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    return ::SendMessage(hCurrScintilla, Msg, wParam, lParam);
}

// ============================================================================
//
// Plugin command functions
//
// ============================================================================

// Reverse the text provided by selectedText and save it in reversedText
//
void reverseText(size_t &bufLength, char * selectedText, char * reversedText)
{
    // Reverse all characters except new line \r\n (0xD 0xA)
    for (unsigned int i=0; i<bufLength; i++)
    {
        switch(selectedText[(bufLength-1)-i])
        {
            case 0xA:
                if (i+1<bufLength && selectedText[(bufLength-1)-(i+1)] == 0xD)
                {
                    reversedText[i] = 0xD;
                    reversedText[++i] = 0xA;
                } else {
                    reversedText[i] = 0xA;
                }
                break;
            default:
                reversedText[i] = selectedText[(bufLength-1)-i];
        }
    }
}

// Reverse the text selection
//
void reverseSelection()
{
    HWND hCurrScintilla = getCurScintilla();
    if (!hCurrScintilla) return;

    // Get buffer length based on text selection
    size_t bufLength = sendScintilla(hCurrScintilla, SCI_GETSELTEXT);
    if (bufLength < 2) return;

    // Store text selection in selectedText
    char * selectedText = new char[bufLength];
    char * reversedText = new char[bufLength];
    sendScintilla(hCurrScintilla, SCI_GETSELTEXT, 0, (LPARAM)selectedText);

    // Reverse text and store result in reversedText
    bufLength = strlen(selectedText);
    reverseText(bufLength, selectedText, reversedText);

    // Replace content and restore selection
    size_t selStart = sendScintilla(hCurrScintilla, SCI_GETSELECTIONSTART);
    size_t selEnd = sendScintilla(hCurrScintilla, SCI_GETSELECTIONEND);
    if (selEnd < selStart)
    {
        size_t tmp = selStart;
        selStart = selEnd;
        selEnd = tmp;
    }
    sendScintilla(hCurrScintilla, SCI_SETTARGETSTART, selStart);
    sendScintilla(hCurrScintilla, SCI_SETTARGETEND, selEnd);
    sendScintilla(hCurrScintilla, SCI_REPLACETARGET, bufLength, (LPARAM)reversedText);
    sendScintilla(hCurrScintilla, SCI_SETSEL, selStart, selStart+bufLength);

    delete [] reversedText;
    delete [] selectedText;
}

// Reverse the text content on line specified by curLine and restore caret position from curPos
//
void reverseLine(HWND &hCurrScintilla, size_t &curPos, size_t &curLine)
{
    // Get buffer length based on current line
    size_t bufLength = sendScintilla(hCurrScintilla, SCI_LINELENGTH, curLine);
    if (bufLength < 2) return;

    // Store text selection from current line in selectedText
    char * selectedText = new char[bufLength];
    char * reversedText = new char[bufLength];
    sendScintilla(hCurrScintilla, SCI_GETLINE, curLine, (LPARAM)selectedText);

    // Remove new line if present in the text content, reverse text and store in reversedText
    if (selectedText[bufLength-1] == 0xA && selectedText[bufLength-2] == 0xD)
    {
        bufLength -= 2;
    }
    reverseText(bufLength, selectedText, reversedText);

    // Replace content and restore caret position
    size_t lineStart = sendScintilla(hCurrScintilla, SCI_POSITIONFROMLINE, curLine);
    size_t lineEnd = sendScintilla(hCurrScintilla, SCI_GETLINEENDPOSITION, curLine);
    sendScintilla(hCurrScintilla, SCI_SETTARGETSTART, lineStart);
    sendScintilla(hCurrScintilla, SCI_SETTARGETEND, lineEnd);
    sendScintilla(hCurrScintilla, SCI_REPLACETARGET, bufLength, (LPARAM)reversedText);
    sendScintilla(hCurrScintilla, SCI_SETSEL, -1, curPos);

    delete [] reversedText;
    delete [] selectedText;
}

// Reverse the text content on current line
//
void reverseCurrentLine()
{
    HWND hCurrScintilla = getCurScintilla();
    if (!hCurrScintilla) return;

    // Get current line based on current position
    size_t curPos = sendScintilla(hCurrScintilla, SCI_GETCURRENTPOS);
    size_t curLine = sendScintilla(hCurrScintilla, SCI_LINEFROMPOSITION, curPos);

    reverseLine(hCurrScintilla, curPos, curLine);
}

// Reverse each single line in the document, keeps line ordering
//
void reverseEachLine()
{
    HWND hCurrScintilla = getCurScintilla();
    if (!hCurrScintilla) return;

    // Store current position (in order to restore it after the operation
    size_t curPos = sendScintilla(hCurrScintilla, SCI_GETCURRENTPOS);
    size_t curLine = 0;
    // Get number of lines in the document (empty is 1)
    size_t lineCount = sendScintilla(hCurrScintilla, SCI_GETLINECOUNT);

    // Start undo action to avoid multiple undo to revert changes
    sendScintilla(hCurrScintilla, SCI_BEGINUNDOACTION);
    while (curLine < lineCount)
    {
        reverseLine(hCurrScintilla, curPos, curLine);
        curLine++;
    }
    // Finish undo action
    sendScintilla(hCurrScintilla, SCI_ENDUNDOACTION);
}

// Dialog About
//
void about()
{
    ::MessageBox(getCurScintilla(), (LPCWSTR)L"Author: Themistokle Benetatos\neMail: info@mrt-prodz.com\nURL: github.com/mrt-prodz/NPP-Reverse-Text", (LPCWSTR)L"About Reverse Text", MB_OK | MB_ICONINFORMATION);
}

