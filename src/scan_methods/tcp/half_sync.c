/*
A half-open SYN scan, also known as a stealth scan,
is a type of port scan where you send a SYN packet,
as if you are going to open a full TCP connection,
but you change your mind after the server responds acknowledging the communication.

Instead of using SOCK_RAW when creating the socket, use SOCK_STREAM. This will create a TCP socket.

Instead of using the connect function to establish a connection,
you'll need to manually send a SYN packet to the target. You can do this using the sendto function.
The SYN packet can be created using a tcphdr structure, with the syn field set to 1 and the ack field set to 0.

After sending the SYN packet, use the recvfrom function to receive a packet from the target.
This will be the server's response.

Check the syn and ack fields of the received packet.
If syn is 1 and ack is 1, the port is open. If syn is 0 and ack is 1, the port is closed.

Instead of sending an ACK packet back to the server to complete the three-way handshake and establish a connection,
just move on to the next port.

*/
