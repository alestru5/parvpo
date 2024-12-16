FROM alpine:latest

RUN apk add --no-cache g++ git

RUN git clone <URL вашего репозитория> /src

WORKDIR /src

RUN g++ -o main main.cpp

CMD ["./main", ">", "/output/result.txt"]
