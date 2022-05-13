Barbershop problem project, a task to teach multithreading in C++.

The main thread contains the original code, which consists of the barber class, the customer class, and the manager class. The manager class is initiated in the main, and the constructor handles all the creation and joining of the threads. The system can handle multiple barbers, each of which has a seperate barber thread, in addition to the customer thread, which handles adding customers to the queue and waking up the barbers if necessary. In the main thread, the customer objects are created and stored in a queue held in the manager class.

In the PassingObjects branch, the system has been modified so that the customer objects are passed from the queue to the barber that is cutting their hair, rather than remaining on the queue.

The GUI branch has the text output replaced with a rudimentary GUI output, with boxes for the queue and barbers' seats, with the number of the customer in the seat they are currently occupying.