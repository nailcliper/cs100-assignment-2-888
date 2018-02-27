# CS100: Basic Command Shell
# Main Features
This basic command shell will present the user with a command prompt ($) from which they can run the standard bash commands found in the /bin folder. The shell can:

 - Accept multiple commands on one line
 - Run commands sequentially using ; or with the logic operators && and ||
 - Manage strings declared with " and ', including nested strings (e.g. "a 'bc' d")
 - Ignore text after a # is declared as the start of a substring (e.g. echo a # or echo a;#)
 - Continuously present the command prompt until an exit command (--q) is entered
 - Integrate the exit command with the logic operators
 - Test if a file/directory exists with the "test" command or [ ] brackets with optional -e, -f, -d flags
 - Precedence operators ( ) can control the order of executed commands.


## Known bugs

- Escape characters not implemented (e.g. echo " a \\" b" will print a \\" b)
- The shell scripts found in /test encounter a localized segmentation fault if their associated .txt files do not contain the exit command
