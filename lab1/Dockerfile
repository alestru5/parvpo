FROM alpine:latest

RUN apk add --no-cache g++ git

RUN git clone https://github.com/alestru5/parvpo /src

WORKDIR /src/lab1

RUN g++ -o main main.cpp

RUN mkdir -p ./output

CMD ["./main"]
