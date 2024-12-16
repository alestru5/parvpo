FROM alpine:latest

RUN apk add --no-cache g++ git

RUN git clone https://github.com/alestru5/parvpo /src

WORKDIR /src

RUN g++ -o main lab1/main.cpp

CMD ["./main"]
