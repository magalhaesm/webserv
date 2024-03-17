#!/usr/bin/env python3

import argparse
import socket
import sys
import signal


def signal_handler(sig, _):
    print("\nCtrl+C pressed. Exiting the client.")
    sys.exit(0)


def main():
    signal.signal(signal.SIGINT, signal_handler)

    parser = argparse.ArgumentParser(
        description="Simple TCP client to test server timeout behavior"
    )
    parser.add_argument("-p", "--port", type=int, default=8080, help="Server port")
    parser.add_argument("--host", default="127.0.0.1", help="Server host")
    args = parser.parse_args()

    try:
        with socket.create_connection((args.host, args.port)) as client_socket:
            print(f"Connected to {args.host}:{args.port}")

            while True:
                data = client_socket.recv(1024)
                if not data:
                    print("Server disconnected.")
                    break

    except Exception as e:
        print(f"Error connecting: {e}")

    print("Connection closed.")


if __name__ == "__main__":
    main()
