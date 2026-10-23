
FROM gcc:14

WORKDIR /app

COPY . .

RUN g++ -std=c++17 server.cpp -o server

EXPOSE 10000

CMD ["./server"]
