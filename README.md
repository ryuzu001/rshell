# rshell

# Description
This is our rshell design. It's a basic shell that works with most commands. Currently supports the use of && || and ; connectors.

# Known bugs: 
    does not work with cd commands.
    does not work with commands more than 1000 characters (use of -pedantic flag must mean the array size is known at compile time)
    