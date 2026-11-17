## Concept

- The PC reads a text file, encodes its contents as Base64, and sends the encoded data to a ZYBO board over UART.
- The board receives and decodes the text, then processes the same decoded input using two word-counting implementations:
  - A software MapReduce implementation running on the Zynq XC7Z010 processor.
  - A hardware-accelerated MapReduce implementation running on a synthesized IP core.
- Each implementation counts the words and determines the most frequent word and its count.
- The board measures the MapReduce processing time for each implementation, excluding UART transfer and Base64 decoding so the two computation times can be compared fairly.
- The board sends each result to the PC over UART, including the implementation name, most frequent word, its count, and elapsed processing time.
- The processing follows a pipe-and-filter structure: receive data, decode it, perform MapReduce, select the most frequent word, format the result, and transmit it. The MapReduce stage has software and hardware alternatives.
- The project will compare the implementations' performance.
