# epoll-server
`epoll-server` is a network server written in C/C++. It uses Linux's `epoll` API to handle TCP and UDP connections on the same port.

## Installation (Debian/Ubuntu)

This program can be installed using the provided `.deb` package.

```bash
sudo dpkg -i epoll-server.deb
```

## Building

1.  Clone the repository:
    ```bash
    git clone https://github.com/RDaniil1/epoll-server.git
    cd epoll-server
    ```

2.  Run `make` to compile the project:
    ```bash
    make
    ```
    This will create the executable at `build/epoll-server`.

## Usage

Run the server directly after building:

```bash
./build/epoll-server
```

The server will start and listen on port `8080` for both TCP and UDP traffic.