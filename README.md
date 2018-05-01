# Notepad++ Plugin: Reverse Text
 
#### Reverse the text content of a line, selection or complete document.

Reverse Text for Notepad++ is a very simple plugin made to help you reverse text content.

## Features:

So far this plugin will:

* reverse the selected text content
* reverse the text content on the current line
* reverse all lines in your document while keeping order


## Usage:

##### Reverse Selection
Select some text in your document and click on Plugins -> Reverse Text -> Selection

##### Reverse Current Line
Position the caret on a line you want to reverse and click on Plugins -> Reverse Text -> Current Line

##### Reverse Each Line while keeping order
You don't need to make any selection or positioning for the caret, just click on Plugins -> Reverse Text -> Each Line


## Screenshot:
![Notepad++ Plugin: Reverse Text](https://raw.githubusercontent.com/mrt-prodz/NPP-Reverse-Text/master/screenshot.png)


## Development

This plugin has been developed using Code::Blocks v13.12 and the GNU GCC Compiler. Selecting the target "Debug" will output the DLL in the Debug folder of the project while "Release" will copy the built DLL into the Notepad++ plugin directory.

There is a Makefile provided as well compatible with MinGW/MSYS, the dll will be compiled into the "Release" folder.

## Reference:
[Scintilla API](http://www.scintilla.org/ScintillaDoc.html)

[Notepad++ Plugin Template](http://notepad-plus-plus.org/contribute/plugin-howto.html)
