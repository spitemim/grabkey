# grabkey

grabkey is a simple tool for grabbing X keys and reporting keypresses.

## Usage

You can invoke grabkey with any number of keybind names as arguments. Keybind
names should conform to the following syntax:

* Any number of 'Control', 'Alt', 'Win', or 'Shift', delimited by '+'

* A valid X key name (ex: 'a', 'F1', 'Return')

grabkey will print the name of the key that was pressed to stdout. It will not
print the modifiers the key was pressed with.

The `-x` argument will immediately stop the program when any grabbed key is
pressed.

The `-p` argument will cause the program to only print the key name when
grabbed keys are pressed.

Examples: 

```
$ grabkey Control+Shift+w
Key 'w' pressed

$ grabkey -p Control+Shift+w
w
```

## Installation

Clone the repo, run `make`, and put the resulting executable somewhere in your $PATH.

```
git clone https://github.com/spitemim/grabkey
cd grabkey
make
cp grabkey ~/.local/bin
```

## License

This project is licensed under the GNU GPLv3.
