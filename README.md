# Event-Listener
Implement threads that track specific events of interest to the application.

It is common that an application monitors and reacts to external events. There are various types of events such as package arrival over the network, kernel events, user input or other process message. In this project, thread just focuses on any arrival of UDP messages as an example. The main thread has the launched threads use passed callback function when they get a UPD message. The test script to send UDP messages to each thread is also included.
