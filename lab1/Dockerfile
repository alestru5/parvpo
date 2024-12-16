FROM alpine:latest

RUN apk add --no-cache g++ git

RUN git clone https://github.com/alestru5/parvpo /lab1

WORKDIR /lab1

RUN g++ -o main ./lab1/main.cpp

CMD ["./main", ">", "/output/lab1/result.txt"]
