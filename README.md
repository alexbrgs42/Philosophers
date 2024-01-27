# Philosophers

The philosopher problem is a really common exercice handeling multi-threading.
Basically, we consider for example 8 philosophers around a round table eating spaghettis. Actually we don't know if they eat these with bolognese or cream... Wathever :)
There is a fork between each of them, in total 8 forks are on the table. Philosophers need 2 forks to eat : the one on their left and the one on their right. The goal here is to avoid philosophers from dying by feeding them in the smallest gap of time. We have a time constraint for the interval of time in which they should eat to avoid death.

| | |
| -- | --|
| Arguments | number_of_philosophers, time_to_die, time_to_eat, time_to_sleep, number_of_times_each_philosopher_must_eat (last one optional) |
| Output | each change of state of a philosopher with its number and the timestamp in milliseconds |
| | - _timestamp_in_ms X has taken a fork_<br> - _timestamp_in_ms X is eating_ <br> - _timestamp_in_ms X is sleeping_ <br> - _timestamp_in_ms X is thinking_ <br> - _timestamp_in_ms X died_

## Mandatory part

I chose to seperate even philosophers from odd philosophers by starting to feed even philos to avoid each philo from taking a single fork which would has resulted in death. After this start, I then had a mutex to protect the state of each fork (available or busy). If the two forks next to a philo are available, I directly block them (also in a mutex to avoid stealing forks) and then the philo can start eating. In the case of an odd number of philosophers, I have a loop in which I accord a priority to the philosopher that is waiting since a long time and that has a single fork available next to him to ensure that he will eat in the next "eating round".

In this project each philosopher is a thread which means that they are executing "simultaniously".  In practice, as we can use a big number of philosophers, the execution of each thread will not be simultanious. What will happen is that a small portion of each will be executed one after the other, but in real life the user won't notice it because the transitions between the thread's execution will be really fast. Based on the number of cores of your CPU (Central Prossecing Unit), some thread will indeed be executed at the same time but each core will pass hand from one thread to an other.
