### Prerequisites ###

Copy Android FCM key and iOS push certificate at <git root>/push-server/cert

### How to build docker image ###

`docker build -t br .`

### How to run a new container ###

`docker run -d -p 19856:19856 -p 10288:10288/udp -e "BATTLE_HOST=<public IP>" br`
