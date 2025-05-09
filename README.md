# My Dispute - Discord-like Terminal Interface

This is a terminal-based Discord-like interface implemented in C using ncurses. It provides a UI for channel-based messaging, private messaging, user roles, and moderation features.

## Features

- Multi-user chat with channels
- Private messaging
- User authentication and registration
- Roles-based permission system
- Moderation tools (mute users)
- Channel management

## Client-Server Architecture

The application follows a client-server architecture:

### Server
- Manages user authentication and registration
- Stores and relays messages to clients
- Maintains user status (online/offline)
- Handles channel creation and deletion
- Enforces permissions based on user roles

### Client
- Provides a terminal UI using ncurses
- Connects to the server via TCP sockets
- Displays channels, users, and messages
- Sends user actions to the server
- Updates the UI based on server responses

## Building the Application

To build both the client and server:

```
make all
```

To build just the client:

```
make my_dispute
```

To build just the server:

```
make my_dispute_server
```

## Running the Application

First, start the server:

```
./my_dispute_server
```

Then, in another terminal, start the client:

```
./my_dispute
```

Multiple clients can connect to the same server.

## Default Channels and Users

The server creates the following default channels:
- general
- random
- help

It also creates an admin user:
- Username: admin
- Password: admin123

## Client Usage

- Arrow keys to navigate
- Enter to select
- ESC to go back
- /command syntax for special commands

## Server Commands

The server supports the following commands:
- /create [channel] - Create a new channel (admin only)
- /delete [channel] - Delete a channel (admin only)
- /mute [user] [minutes] - Mute a user (admin/mod only)
- /role [user] [1-3] - Set user role (admin only)

## Roles
1. User
2. Moderator
3. Admin

## License

MIT