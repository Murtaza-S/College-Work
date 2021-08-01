The command for running client is:
./client <port_no>

The command for running server is:
./server <port_no>

The interaction from client side is menu based. 
This is for ease thus requires no error handling in name of commands.

The error handling of file names being present or not is done at server side and required message is sent to the client.

The following funtions work :
LIST
DELE
QUIT

RETR function had issues with implementation only part of writing on buffer is remaining as connection error could not be resolved.
STOR not implemented.

All connection is mostly done via single socket. The RETR is being implemented with 2 sockets but isn't fully done.

Rest program has all the required comments.
