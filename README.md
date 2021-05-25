#### Use Qt5 and C++17 to build the project
***
Для корректной работы сначала необходимо запустить сервер, далее подключать клиентов

При подключении клиент получит сообщение об успешном соединении, на сервере выведется адрес клиента в сети

Отправка произвольного текста без специализированных команд уходит только на сервер

#### Команды клиентов:

/send N TEXT - отправить клиенту с id N сообщение с текстом TEXT;

/list - вывести список всех подключенных клиентов с пометкой "(you)" для текущего пользователя.
