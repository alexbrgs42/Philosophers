# philosopher

The philosopher problem is a really common exercice handeling multi-threading.
Basically, we consider for example 8 philosophers around a round table eating spaghettis. Actually we don't know if the eat these with bolognese or cream... Wathever :)
There is a fork between each of them, in total 8 forks are on the table. Philosophers need 2 forks to eat so the one on theire left and the one on their right. The goal here is to avoid philosophers from dying by feeding them in the smallest gap of time. We have a time constraint for the interval of time in which they should eat to avoid death.

| | |
| -- | --|
| Arguments | number_of_philosophers, time_to_die, time_to_eat, time_to_sleep, number_of_times_each_philosopher_must_eat (last one optional) |
| Output | each change of state of a philosopher with its number and the timestamp in milliseconds |

| | * _timestamp_in_ms X has taken a fork_
    * _timestamp_in_ms X is eating_
    * _timestamp_in_ms X is sleeping_
    * _timestamp_in_ms X is thinking_
    * _timestamp_in_ms X died_

