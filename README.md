# LameGameTrainer

LameGameTrainer is a tool for Windows that allows you to set hot keys for cheat codes, which makes using cheat codes easier with only one click or two.

## Requirements:

You'll only need GCC (the GNU compiler collection, Mingw32 or Cygwin) to compile LameGameTrainer, just run `make.bat` and you're all set.

## Usage:

You can start adding hot keys by just running the result binary.

![LameGameTrainer start](http://i.imgur.com/4s7SQ61.jpg)

Write `endpls` to stop, and when you want the hot keys to get registered just run it again, and you're ready to use them.

You can also run a custom configuration file from the command line, example:

![LameGameTrainer CLI](http://i.imgur.com/6BaPmnL.jpg)

The syntax of the configuration file is simply `HOTKEY|CHEATCODE` with `|` as the delimeter, example of a configuration file for GTA Vice City cheat codes:

```
{F1}|THUGSTOOLS
{F2}|PROFESSIONALTOOLS
{F3}|NUTTERTOOLS
{F4}|PRECIOUSPROTECTION
{F5}|ASPIRINE
{F6}|ALOVELYDAY
{F7}|ABITDRIEG
{F8}|CATSANDDOGS
{F9}|CANTSEEATHING
{F10}|PANZER
{F11}|LIFEISPASSINGMEBY
{F12}|BIGBANG
{DEL}|STILLLIKEDRESSINGUP
{PAGEUP}|FIGHTFIGHTFIGHT
```

The hot key must be either of the following: one alphanumeric character, one "special" key (e.g RSHIFT/LSHIFT, F10, LBUTTON, ..) or one "hold key" (SHIFT, CTRL, WIN or ALT) + one alphanumeric character or one "special key".

Unfortunately, LameGameTrainer only allows alphanumeric cheat codes and spaces currently.

## Examples of accepted and not accepted hot keys:

| Right    | Wrong |
| -------- | ----- |
| {SHIFT}t | {DEL}t      |
| {DEL}    |  {tttt}    |
| {RSHIFT} |   {SHIFT}    |
| {CTRL}{LCTRL} | {LCTRL}k  |
| K  |      KKKKK        |

## List of special keys:

| Special keys |
| -----------  |
|LBUTTON|
|RBUTTON|
|CANCEL|
|MBUTTON|
|XBUTTON1|
|XBUTTON2|
|BACKSPACE|
|TAB|
|CLEAR|
|ENTER|
|PAUSE|
|CAPSLOCK|
|ESC|
|SPACE|
|PAGEUP|
|PAGEDOWN|
|END|
|HOME|
|LEFT|
|UP|
|RIGHT|
|DOWN|
|SELECT|
|PRINT|
|EXECUTE|
|PRINTSCR|
|INS|
|DEL|
|HELP|
|LWIN|
|RWIN|
|NUMPAD0|
|NUMPAD1|
|NUMPAD2|
|NUMPAD3|
|NUMPAD4|
|NUMPAD5|
|NUMPAD6|
|NUMPAD7|
|NUMPAD8|
|NUMPAD9|
|MULTIPLY|
|ADD|
|SEPARATOR|
|SUBTRACT|
|DECIMAL|
|DIVIDE|
|F1|
|F2|
|F3|
|F4|
|F5|
|F6|
|F7|
|F8|
|F9|
|F10|
|F11|
|F12|
|F13|
|F14|
|F15|
|F16|
|F17|
|F18|
|F19|
|F20|
|F21|
|F22|
|F23|
|F24|
|NUMLOCK|
|SCROLLLOCK|
|LSHIFT|
|RSHIFT|
|LCTRL|
|RCTRL|
|ALT|

## Problems:

Don't ever use a configuration like the following:
```
B|TEST
T|BEBT
```
As it will end up causing an infinite loop of sending characters.
