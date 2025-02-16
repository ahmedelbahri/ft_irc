# ft_irc

**ft_irc** is a project that simulates an IRC (Internet Relay Chat) server and client system. The goal of the project is to create a basic implementation of a chat system that mimics the functionality of real IRC services, allowing multiple users to join channels, send messages, and interact in real-time. The project also includes a **bonus** feature, which enhances the functionality further.

---

## About

The **ft_irc** project implements the core functionality of an IRC system. The server listens for client connections, allows users to join channels, send messages, and supports basic commands like `/join`, `/msg`, and `/nick`. The bonus feature introduces the addition of **channels with different user permissions**, allowing for a more dynamic user experience.

### Core Requirements:
- Implement a simple IRC server and client using sockets.
- Support basic IRC commands such as `/join`, `/msg`, `/nick`, and `/part`.
- Ensure that multiple clients can interact in real-time.
- Implement user registration and authentication (optional).
  
---

## Installation

To install **ft_irc**, follow these steps:

1. Clone the repository:
    ```bash
    git clone https://github.com/ahmedelbahri/ft_irc.git
    cd ft_irc
    ```

2. Compile the project:
    ```bash
    make
    ```

3. Optionally, to remove compiled objects:
    ```bash
    make clean
    ```

4. To remove the executable:
    ```bash
    make fclean
    ```

5. To recompile the project after cleaning:
    ```bash
    make re
    ```

---

## Usage

To run the IRC server, simply execute the server program:

```bash
./server
```

## To connect to the server as a client, execute the client program:

```bash
./client <server_ip> <port>
```

## Example:

```bash
./client 127.0.0.1 6667
```

## Bonus
In the bonus section, the following features are implemented:

- Channel Permissions: Each channel can have different user roles, such as operators, who can kick or ban users from channels.
- Private Messages: Users can send private messages to others who are connected to the server.
- Channel Modes: Support for channel modes such as password-protected channels and private channels.
- Server Authentication: Users can register and log in with a username and password, allowing for more secure communication.
- To test the bonus features, connect as a client and try out commands such as /join <channel> <password> for password-protected channels or use /msg to send private messages.
