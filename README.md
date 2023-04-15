# ClientServerExercise
 Sending signals between a server and a client exercise in linux.
 Instructions:
 1. Run the server in the background: "./server.exe &".
 2. Ping the server's process ID with the client, giving him a task (3 strings after the ping) e.g: "./client.exe #SERVERID# 1 1 1".
the first and the third strings will be integer and the second string represents the action: 1 is plus, 2 is minus, 3 to multiply and 4 to divide.
thus, the answer for (line 5) is: 1 + 1 = 2.
3. The output will be the answer to the task, creating a text file with the client's process id as the file's name.
