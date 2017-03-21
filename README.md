# CS100 rshell

# Description
This is our rshell design. It's a basic shell that works with most commands. Currently supports the use of && || and ; connectors. Support for precedence. After much hard work by ONE PARTICULAR MEMBER, now supports input, output redirection and piping up to six commands together. 
Use parenthesis for precedence and input/output currently supports <, >, >>. There are no plans to support 2>. Piping is accomplished with the use of the sheffer stroke. 
Also supports the use of both kinds of test commands.

# How to install
run the commands:
    git clone https://github.com/ryuzu001/rshell.git
    cd rshell
    git checkout hw4
    make
    bin/rshell

# Known bugs: 
Does not work with cd commands.
Does not work with commands more than 1000 characters (use of -pedantic flag must mean the array size is known at compile time)
Arrow keys act as a character, and does not control the cursor.
Uses a simple "$" instead of getting login and enviornment details

# How to use features
Use ";" to link together commands
Use "&&" as a logical "and" operator
Use "||" as a logical "or" operator
Use parenthesis in addition with the above features to add precedence to commmands
Use "<" to redirect input
Use ">" to redirect output
Use ">>" to redirect output (append)
Use "|" to pipe together commands.
    